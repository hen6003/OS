#include <stddef.h>
#include <stdint.h>

// Returns size of string
size_t strlen(char *str);

// Counts amount of digits in number
size_t digit_count(int num, int base);

// Converts numbers to strings
void itoa(int num, char *number, int base);

int atoi(char *string);

// Shuts down the computer
void shutdown();

void sleep(uint32_t timer_count);

void append(char *s, char c);

char *strncpy(char *restrict dest, const char *src, size_t n);

char *strcpy(char *restrict dest, const char *src);

int strcmp(const char *s1, const char *s2);

void *memset(void *s, int c, size_t n);
