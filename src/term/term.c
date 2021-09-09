#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "vga.h"
#include "cursor.h"
#include "util.h"
#include "term.h"

// We start displaying text in the top-left of the screen (column = 0, row = 0)
unsigned int term_col = 0;
unsigned int term_row = 0;

void term_clear(int ansi_type)
{
  switch (ansi_type)
  {
  case 0:
    // Clear the textmode buffer
    for (unsigned int col = 0; col < VGA_COLS; col++)
    {
      for (unsigned int row = term_row; row < VGA_ROWS; row++)
      {
	if (row == term_row)
	{
	  if (col < term_col)
	    continue;
	}

	vga_text_setc(row, col, '\0');
      }
    }
    break;

  case 1:
    // Clear the textmode buffer
    for (unsigned int col = VGA_COLS; (int) col >= 0; col--)
    {
      for (unsigned int row = term_row; (int) row >= 0; row--)
      {
	if (row == term_row)
	{
	  if (col > term_col)
	    continue;
	}

	vga_text_setc(row, col, '\0');
      }
    }
    break;
    break;

  case 3:
  case 2:
    // Clear the textmode buffer
    for (unsigned int col = 0; col < VGA_COLS; col++)
    {
      for (unsigned int row = 0; row < VGA_ROWS; row++)
      {
	vga_text_setc(row, col, '\0');
      }
    }
    break;

  case 4:
    for (unsigned int col = term_col; col < VGA_COLS; col++)
    {
      vga_text_setc(term_row, col, '\0');
    }
    break;

  case 5:
    for (unsigned int col = term_col; (int) col >= 0; col--)
    {
      vga_text_setc(term_row, col, '\0');
    }
    break;

  case 6: 
    for (unsigned int col = 0; col < VGA_COLS; col++)
    {
      vga_text_setc(term_row, col, '\0');
    }
    break;
  }
}
 
// This function initiates the terminal
void term_init()
{
  cursor_enable(14,15);
  
  vga_color_set_fg(VGA_COLOR_LIGHT_GREY);
  vga_color_set_bg(VGA_COLOR_BLACK);

  term_clear(2);

  term_col = 0;
  term_row = 0;

  cursor_set(0, 0);
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
    case '\0':
      break;

    case '\n': // Newline characters should return the column to 0, and increment the row
      term_col = 0;
      term_row++;
      break;

    case '\b':
      //vga_buffer[index-1] = ((uint16_t)vga_color << 8) | '\0'; // Set the character to blank
      vga_text_setc(term_row, term_col-1, '\0');

      /*
      // Skip blank parts of screen
      if (term_col == 0)
      { 
	//while (vga_buffer[index - tmp] == (((uint16_t)vga_color << 8) | '\0'))
	while (vga_get_char(term_row, term_col - tmp) && tmp <= VGA_COLS-term_col)
	  tmp++;
	
	term_row--;
	if (tmp < 80)
	  term_col = VGA_COLS - tmp + 1;
      }
      else*/

      if (term_col)
	term_col--;
      
      break;
      /*
    case 0x7f: // Delete key
      for (unsigned int col = 0; col < VGA_COLS-term_col; col++)
      {
	if (vga_buffer[index+col] == (((uint16_t)vga_color << 8) | '\0'))
	  break;
	
	vga_buffer[index+col] = vga_buffer[index+col+1];
      }
      
      break;
      */
    case '\e':
      ansi_state = ANSI_ESC;
      escape_code_params[0] = '\0';
      escape_code_function  = '\0';
      break;
      
    default: // Normal characters just get displayed and then increment the column
      vga_text_setc(term_row, term_col, c);
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
    string_ends[0] = -1;
    int string_endi = 1;
    int params[10];

    for (int i = 0; i < strlen(escape_code_params); i++)
      if (escape_code_params[i] == ';')
      {
	escape_code_params[i] = '\0';
	string_ends[string_endi++] = i;
      }

    for (int i = 0; i < string_endi; i++)
      params[i] = atoi((char *) &escape_code_params[string_ends[i]+1]); 

    // Parse escape code
    switch (escape_code_function)
    {
    case 'A':
      if (params[0] == 0)
	params[0] = 1;
      term_cursor_move(0, -params[0]);
      break;
    case 'B':
      if (params[0] == 0)
	params[0] = 1;
      term_cursor_move(0, params[0]);
      break;
    case 'C':
      if (params[0] == 0)
	params[0] = 1;
      term_cursor_move(params[0], 0);
      break;
    case 'D':
      if (params[0] == 0)
	params[0] = 1;
      term_cursor_move(-params[0], 0);
      break;
    case 'E':
      if (params[0] == 0)
	params[0] = 1;
      cursor_set(0, term_row+params[0]);
      break;
    case 'F':
      if (params[0] == 0)
	params[0] = 1;
      cursor_set(0, term_row-params[0]);
      break;
    case 'G':
      if (params[0] == 0)
	params[0] = 1;
      term_col = params[0]-1;
      break;
    case 'H':
    case 'f':
      if (params[0] == 0)
	params[0] = 1;
      if (params[1] == 0 || string_endi < 2)
	params[1] = 1;
      term_row = params[0]-1;
      term_col = params[1]-1;
      cursor_set(params[1]-1, params[0]-1); 
      break;
    case 'J':
      if (params[0] > 3)
	break;
      term_clear(params[0]);
      break;
    case 'K':	
      if (params[0] > 2)
	break;
      term_clear(params[0]+4);
      break;
    case 'S': 
      if (params[0] == 0)
	params[0] = 1;
      vga_scroll_up(params[0]);
      break;
    case 'T': 
      if (params[0] == 0)
	params[0] = 1;
      vga_scroll_down(params[0]);
    case 'm':
      // Loop though params
      for (int i = 0; i < string_endi; i++)
      {
	switch (params[i])
	{
	case 1:
	  vga_color_set_bold(true);
	  break;
	case 22:
	  vga_color_set_bold(false);
	  break;
	case 30:
	  vga_color_set_fg(VGA_COLOR_BLACK);
	  break;
	case 31:
	  vga_color_set_fg(VGA_COLOR_RED);
	  break;
	case 32:
	  vga_color_set_fg(VGA_COLOR_GREEN);
	  break;
	case 33:
	  vga_color_set_fg(VGA_COLOR_BROWN);
	  break;
	case 34:
	  vga_color_set_fg(VGA_COLOR_BLUE);
	  break;
	case 35:
	  vga_color_set_fg(VGA_COLOR_MAGENTA);
	  break;
	case 36:
	  vga_color_set_fg(VGA_COLOR_CYAN);
	  break;
	case 37:
	  vga_color_set_fg(VGA_COLOR_LIGHT_GREY);
	  break;
	case 40:
	  vga_color_set_bg(VGA_COLOR_BLACK);
	  break;
	case 41:
	  vga_color_set_bg(VGA_COLOR_RED);
	  break;
	case 42:
	  vga_color_set_bg(VGA_COLOR_GREEN);
	  break;
	case 43:
	  vga_color_set_bg(VGA_COLOR_BROWN);
	  break;
	case 44:
	  vga_color_set_bg(VGA_COLOR_BLUE);
	  break;
	case 45:
	  vga_color_set_bg(VGA_COLOR_MAGENTA);
	  break;
	case 46:
	  vga_color_set_bg(VGA_COLOR_CYAN);
	  break;
	case 47:
	  vga_color_set_bg(VGA_COLOR_LIGHT_GREY);
	  break;
	}
      } 
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
    vga_scroll_up(1);
  
  cursor_set(term_col, term_row);
}

// This function prints an entire string onto the screen
void term_puts(const char* str)
{
  size_t i;
  for (i = 0; str[i] != '\0'; i++) // Keep placing characters until we hit the null-terminating character ('\0')
    term_putc(str[i]);
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

void term_cursor_move(int x, int y)
{
  x += term_col;
  y += term_row;

  cursor_set(x, y);
}

unsigned int term_cursor_posx() { return term_col; }
unsigned int term_cursor_posy() { return term_row; }
