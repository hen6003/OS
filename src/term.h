#include "color.h"

// This function initiates the terminal by clearing it
void term_init();

/// Color functions

// Set current color
void term_color_set(enum vga_color fg, enum vga_color bg);

/// Text functions

// Clear screen
void term_clear();

// Prints a single character onto the screen
void term_putc(char c);

// Prints a decimal number onto the screen
void term_puti(int num);

// Prints a hex number onto the screen
void term_puth(int num);

// Prints an entire string onto the screen
void term_puts(const char* str);

/// Cursor functions

// Set current cursor position
void term_cursor_set(unsigned int x, unsigned int y);

// Move cursor relative to current position
void term_cursor_move(int x, int y);

// Get cursor position X
unsigned int term_cursor_posx();

// Get cursor position Y
unsigned int term_cursor_posy();

// Get character in position
char term_get_char(int x, int y);
