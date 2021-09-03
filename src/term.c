#include <stdint.h>
#include <stddef.h>

#include "vga.h"
#include "cursor.h"
#include "util.h"
#include "term.h"

// This is the x86's VGA textmode buffer. To display text, we write data to this memory location
volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;

// We start displaying text in the top-left of the screen (column = 0, row = 0)
int term_col = 0;
int term_row = 0;
uint8_t term_color = 0x0F; // Black background, White foreground

void term_color_set(enum vga_color fg, enum vga_color bg)
{
  term_color = fg | bg << 4;
}

void term_clear()
{
  // Clear the textmode buffer
  for (int col = 0; col < VGA_COLS; col ++)
  {
    for (int row = 0; row < VGA_ROWS; row ++)
    {
      // The VGA textmode buffer has size (VGA_COLS * VGA_ROWS).
      // Given this, we find an index into the buffer for our character
      const size_t index = (VGA_COLS * row) + col;
      // Entries in the VGA buffer take the binary form BBBBFFFFCCCCCCCC, where:
      // - B is the background color
      // - F is the foreground color
      // - C is the ASCII character
      vga_buffer[index] = ((uint16_t)term_color << 8) | ' '; // Set the character to blank (a space character)
    }
  }
}
 
// This function initiates the terminal
void term_init()
{
  VGA_COLS = 80;
  VGA_ROWS = 25;
  
  enable_cursor(14,15);
  
  term_color_set(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

  term_clear();
}

// This function places a single character onto the screen
void term_putc(char c)
{
  const size_t index = (VGA_COLS * term_row) + term_col; // Like before, calculate the buffer index
  int tmp = 0;

  // Remember - we don't want to display ALL characters!
  switch (c)
  {
  case '\n': // Newline characters should return the column to 0, and increment the row
    term_col = 0;
    term_row++;
    break;

  case '\b':
    vga_buffer[index-1] = ((uint16_t)term_color << 8) | ' '; // Set the character to blank (a space character)

    // Skip blank parts of screen
    if (term_col == 0)
    { 
      while (vga_buffer[index - tmp] == (((uint16_t)term_color << 8) | ' '))
	tmp++;

      term_row--;
      if (tmp < 80)
	term_col = VGA_COLS - tmp + 1;
    }
    else
      term_col--;

    break;

  default: // Normal characters just get displayed and then increment the column
    vga_buffer[index] = ((uint16_t)term_color << 8) | c;
    term_col++;
    break;
  }
  
  // What happens if we get past the last column? We need to reset the column to 0, and increment the row to get to a new line
  if (term_col >= VGA_COLS)
  {
    term_col = 0;
    term_row++;
  }
  
  // What happens if we get past the last row? We need to reset both column and row to 0 in order to loop back to the top of the screen
  if (term_row >= VGA_ROWS)
  {
    term_col = 0;
    term_row = 0;
  }

  update_cursor(term_col, term_row);
}

// This function prints an entire string onto the screen
void term_puts(const char* str)
{
  for (size_t i = 0; str[i] != '\0'; i ++) // Keep placing characters until we hit the null-terminating character ('\0')
    term_putc(str[i]);
}

void term_puti(int num)
{
  char str_num[digit_count(num) + 1];
  itoa(num, str_num);
  term_puts(str_num);
}
