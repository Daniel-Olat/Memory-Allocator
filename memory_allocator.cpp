#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <mutex>
#include <unordered_map>
// External Fragmentation: Free memory is chopped into small non-contiguous chunks
//Paging gives us 128mb of virtual memory, the header struct is used to divide thid block into smaller chunks, connected in a linked list.
// Internal Fragmentation : IKEA model, we have a fixed size list(8bytes , 16bytes, 32bytes, 64bytes, 128bytes) and we allocate the smallest block that can fit the request, if we request 20 bytes we will get a 32 byte block, the remaining 12 bytes are wasted. This leaves watsed memory in the block which is a trade off for speed and faster allocation and deallocation.
//TO solve multithreded problem where two or more threads try to access the same memory block at the same time , we can use mutex locks to ensure that the access operation becomes serialized, meaning that only one thread can access the memory block at a time, preventing race conditions and ensuring data integrity. However, this can lead to performance issues due to contention and increased latency, especially in high-concurrency scenarios. To mitigate this, we can use fine-grained locking or lock-free data structures to reduce contention and improve performance while still ensuring thread safety.Wrap the allocate and deallocate function in global std::mutex

//tcmalloc design
class MyAllocator{
    public:
        void* allocate(size_t size);
        void deallocate(void* ptr);
    private:
        struct BlockHeader{
            size_t size;
        };
        struct freeBlockHeader{
            freeBlockHeader *next;
        };
        struct span {
            size_t start_page_id;
            size_t num_pages;
            span* next = nullptr;
            span* prev = nullptr;
            bool is_free = true;
        };
        struct TransferCache{
            std::mutex mtx;
            freeBlockHeader* list = nullptr;
            int count = 0;
        };
        struct ThreadCache{
            freeBlockHeader* free_lists[8] = {nullptr};
            int list_lengths[8] = {0};
        };
        class PageHeap {
        public:
            span* allocateSpan(size_t num_pages);
            void deallocateSpan(span* span_ptr);
            span* lookupSpan(void* ptr);
        private:
            std::mutex mtx;
            span* free_spans[256] = {nullptr}; // Free lists for spans of 1-255 pages
            std::unordered_map<size_t, span*> page_map;
        };

        PageHeap page_heap; // Tier 3
        TransferCache transfer_caches[8]; // Tier 2

        static size_t getSizeClassIndex(size_t size);
        static size_t getClassSizeFromIndex(size_t index);

        // Moves objects from TransferCache to ThreadCache.
        void fetchFromTransferCache(size_t class_index);
        // Scavenging: Moves objects from ThreadCache back to TransferCache.
        void releaseToTransferCache(size_t class_index);
};
