#include "color.h"

void term_color_set(enum vga_color fg, enum vga_color bg);

void term_clear();

// This function initiates the terminal by clearing it
void term_init();

// This function places a single character onto the screen
void term_putc(char c);

// This function prints an entire string onto the screen
void term_puts(const char* str);

void term_puti(int num);
