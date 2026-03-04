
#include <stdio.h>
#include "leap.h"

int main() {
    int year = 2000;  // you can change this to any year you want
    if (leap_year(year)) {
        printf("%d is a leap year.\n", year);
    } else {
        printf("%d is not a leap year.\n", year);
    }
    return 0;
}
