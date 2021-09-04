#include <stddef.h>
#include <stdint.h>

// Counts amount of digits in number
size_t digit_count(int num, int base);

// Converts numbers to strings
void itoa(int num, char *number, int base);

// Shuts down the computer
void shutdown();

void sleep(uint32_t timer_count);
