#include "keyboard.h"

// This function initiates the terminal by clearing it
void term_init();

/// Text functions

// Clear screen
void term_clear(int ansi_type);

// Prints a single character onto the screen
void term_putc(char c);

// Prints a decimal number onto the screen
void term_puti(int num);

// Prints a hex number onto the screen
void term_puth(int num);

// Prints an entire string onto the screen
void term_puts(const char* str);

/// Cursor functions

// Move cursor relative to current position
void term_cursor_move(int x, int y);

// Get cursor position X
unsigned int term_cursor_posx();

// Get cursor position Y
unsigned int term_cursor_posy();

// Wrapper around keyboard for auto shift/caps handling
char get_ascii_char(enum keycodes keycode);
