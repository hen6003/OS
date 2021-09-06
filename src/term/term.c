#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "cursor.h"
#include "util.h"
#include "term.h"
#include "vga.h"

// This is the x86's VGA textmode buffer. To display text, we write data to this memory location
volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;

// We start displaying text in the top-left of the screen (column = 0, row = 0)
unsigned int term_col = 0;
unsigned int term_row = 0;
uint8_t term_color = 0x0F; // Black background, White foreground

void term_color_set(enum vga_color fg, enum vga_color bg)
{
  term_color = fg | bg << 4;
}

void term_clear()
{
  // Clear the textmode buffer
  for (unsigned int col = 0; col < VGA_COLS; col ++)
  {
    for (unsigned int row = 0; row < VGA_ROWS; row ++)
    {
      // The VGA textmode buffer has size (VGA_COLS * VGA_ROWS).
      // Given this, we find an index into the buffer for our character
      const size_t index = (VGA_COLS * row) + col;
      // Entries in the VGA buffer take the binary form BBBBFFFFCCCCCCCC, where:
      // - B is the background color
      // - F is the foreground color
      // - C is the ASCII character
      vga_buffer[index] = ((uint16_t)term_color << 8) | '\0'; // Set the character to blank
    }
  }

  term_col = 0;
  term_row = 0;

  // Reset cursor position
  term_cursor_set(0, 0);
}
 
// This function initiates the terminal
void term_init()
{
  enable_cursor(14,15);
  
  term_color_set(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

  term_clear();
}

enum ansi_states
{
  ANSI_PRINT,  // Default state
  ANSI_ESC,    // \e Found
  ANSI_CSI,    // [ Found
  ANSI_PARAMS, // Parameters
  ANSI_INTER,  // Intermediate
  ANSI_FINAL,  // Final
};

enum ansi_states ansi_state = ANSI_PRINT;
char escape_code_params[100];
char escape_code_function;

// This function places a single character onto the screen
void term_putc(char c)
{
  int tmp = 0;

  switch (ansi_state)
  {
  case ANSI_PRINT:
  print:
  {
    const size_t index = (VGA_COLS * term_row) + term_col; // Like before, calculate the buffer index
    // Remember - we don't want to display ALL characters!
    switch (c)
    {
    case '\n': // Newline characters should return the column to 0, and increment the row
      term_col = 0;
      term_row++;
      break;
      
    case '\b':
      vga_buffer[index-1] = ((uint16_t)term_color << 8) | '\0'; // Set the character to blank
      
      // Skip blank parts of screen
      if (term_col == 0)
      { 
	while (vga_buffer[index - tmp] == (((uint16_t)term_color << 8) | '\0'))
	  tmp++;
	
	term_row--;
	if (tmp < 80)
	  term_col = VGA_COLS - tmp + 1;
      }
      else
	term_col--;
      
      break;
      
    case 0x7f: // Delete key
      for (unsigned int col = 0; col < VGA_COLS-term_col; col++)
      {
	if (vga_buffer[index+col] == (((uint16_t)term_color << 8) | '\0'))
	  break;
	
	vga_buffer[index+col] = vga_buffer[index+col+1];
      }
      
      break;
      
    case '\e':
      ansi_state = ANSI_ESC;
      escape_code_params[0] = '\0';
      escape_code_function  = '\0';
      break;
      
    default: // Normal characters just get displayed and then increment the column
      vga_buffer[index] = ((uint16_t)term_color << 8) | c;
      term_col++;
      break;
    }
    break;
  }
    
  case ANSI_ESC:
    if (c == '[')
      ansi_state = ANSI_CSI;
    else goto abort;
    break;
    
  case ANSI_CSI:
    if (c >= '0' && c <= '?')
      append(escape_code_params, c);
    else if (c >= ' ' && c <= '/')
      ansi_state = ANSI_PARAMS;
    else if (c >= '@' && c <= '~')
    {
      ansi_state = ANSI_FINAL;
      escape_code_function = c;
    }
    else goto abort;
    break;
    
  case ANSI_PARAMS:	
    if (c >= '!' && c <= '/') ;
    else if (c >= '@' && c <= '~')
    {
      ansi_state = ANSI_FINAL;
      escape_code_function = c;
    }
    else goto abort;
    break;

  case ANSI_INTER:
    if (c >= '@' && c <= '~')
    {
      ansi_state = ANSI_FINAL;
      escape_code_function = c;
    }
    else goto abort;
    break;

  case ANSI_FINAL:
    ansi_state = ANSI_PRINT;

    int string_ends[10];
    string_ends[0] = 0;
    int string_endi = 1;
    int params[10];

    for (int i = 0; i < strlen(escape_code_params); i++)
      if (escape_code_params[i] == ';')
      {
	escape_code_params[i] = '\0';
	string_ends[string_endi++] = i;
      }

    for (int i = 0; i < string_endi; i++)
      params[i] = atoi((char *) &escape_code_params[string_ends[i]]);
    
    int tmpi;

    // Parse escape code
    switch (escape_code_function)
    {
    case 'A':
      tmpi = params[0];
      if (tmpi == 0)
	tmpi = 1;
      term_cursor_move(0, -tmpi);
      break;
    case 'B':
      tmpi = atoi(escape_code_params);
      if (tmpi == 0)
	tmpi = 1;
      term_cursor_move(0, tmpi);
      break;
    case 'C':
      tmpi = atoi(escape_code_params);
      if (tmpi == 0)
	tmpi = 1;
      term_cursor_move(tmpi, 0);
      break;
    case 'D':
      tmpi = atoi(escape_code_params);
      if (tmpi == 0)
	tmpi = 1;
      term_cursor_move(-tmpi, 0);
      break;
    case 'E':
      tmpi = atoi(escape_code_params);
      if (tmpi == 0)
	tmpi = 1;
      term_cursor_set(0, term_row+tmpi);
      break;
    case 'F':
      tmpi = atoi(escape_code_params);
      if (tmpi == 0)
	tmpi = 1;
      term_cursor_set(0, term_row-tmpi);
      break;
    case 'G':
      tmpi = atoi(escape_code_params);
      if (tmpi == 0)
	tmpi = 1;
      term_col = tmpi-1;
      break;
    case 'H':
      break;
    }

    goto print; // Print character
    break;

  abort:
    ansi_state = ANSI_PRINT;
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
    scroll_up();
  
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
  char str_num[digit_count(num, 10) + 1];
  itoa(num, str_num, 10);
  term_puts(str_num);
}

void term_puth(int num)
{
  char str_num[digit_count(num, 16) + 1];
  itoa(num, str_num, 16);
  term_puts("0x");
  term_puts(str_num);
}

void term_cursor_set(unsigned int x, unsigned int y)
{
  if (x > VGA_COLS)
    x = VGA_COLS;
  if (y > VGA_ROWS)
    y = VGA_ROWS;
  
  term_col = x;
  term_row = y;
  
  update_cursor(term_col, term_row);
}

void term_cursor_move(int x, int y)
{
  x += term_col;
  y += term_row;

  term_cursor_set(x, y);
}

unsigned int term_cursor_posx() { return term_col; }
unsigned int term_cursor_posy() { return term_row; }

char term_get_char(int x, int y)
{
  const size_t index = (VGA_COLS * y) + x;

  return (char) vga_buffer[index] & (1u << 8) - 1;;
}

void scroll_up()
{
  for (unsigned int col = 0; col < VGA_COLS; col ++)
  {
    for (unsigned int row = 1; row < VGA_ROWS; row ++)
    {
      const size_t index1 = (VGA_COLS * row) + col;
      const size_t index2 = (VGA_COLS * (row-1)) + col;

      vga_buffer[index2] = vga_buffer[index1];
    }
  }
  term_cursor_move(0, -1);
}

void scroll_down()
{
  for (unsigned int col = 0; col < VGA_COLS; col++)
  {
    for (unsigned int row = VGA_ROWS; row > 0; row--)
    {
      const size_t index1 = (VGA_COLS * row) + col;
      const size_t index2 = (VGA_COLS * (row-1)) + col;

      vga_buffer[index1] = vga_buffer[index2];
    }
  }

  // Clear top row
  for (unsigned int col = 0; col < VGA_COLS; col++)
      vga_buffer[col] = ((uint16_t)term_color << 8) | '\0';

  term_cursor_move(0, 1);
}
