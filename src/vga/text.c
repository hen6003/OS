#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "vga.h"

/*** Colors ***/
enum vga_color vga_color_fg = VGA_COLOR_LIGHT_GREY;
enum vga_color vga_color_bg = VGA_COLOR_BLACK;
uint8_t vga_color = 0x0F; // Black background, White foreground
bool vga_bold = false;

void vga_color_set_fg(enum vga_color fg)
{
  if (vga_bold)
    fg += 8;

  vga_color = fg | vga_color_bg << 4;

  vga_color_fg = fg;
}

void vga_color_set_bg(enum vga_color bg)
{
  vga_color = vga_color_fg | bg << 4;

  vga_color_bg = bg;
}

void vga_color_set_bold(bool on)
{
  vga_bold = on;

  vga_color_set_fg(vga_color_fg);
}

/*** Text ***/

// This is the x86's VGA textmode buffer. To display text, we write data to this memory location
volatile uint16_t *vga_buffer = (uint16_t *)0xB8000;

void vga_text_setc(int row, int col, char c)
{
  const size_t index = (VGA_COLS * row) + col;

  vga_buffer[index] = ((uint16_t)vga_color << 8) | c;
}

void vga_scroll_up(int times)
{
  while (times--)
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
  }
}
  
void vga_scroll_down(int times)
{
  while (times--)
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
      vga_text_setc(0, col, '\0');
  }
}

char vga_get_char(int x, int y)
{
  const size_t index = (VGA_COLS * y) + x;

  return (char) vga_buffer[index] & (1u << 8) - 1;;
}
