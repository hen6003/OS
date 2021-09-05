#include <stddef.h>
#include <stdint.h>

// Returns size of string
size_t strlen(char *str);

// Counts amount of digits in number
size_t digit_count(int num, int base);

// Converts numbers to strings
void itoa(int num, char *number, int base);

// Shuts down the computer
void shutdown();

void sleep(uint32_t timer_count);

void append(char *s, char c);
