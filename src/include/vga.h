#include "stdbool.h"

#define VGA_ROWS 25
#define VGA_COLS 80

/*** Set colors ***/
enum vga_color
{
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_BROWN = 14,
  VGA_COLOR_WHITE = 15,
};

/*** Draw text ***/

void vga_text_setc(int row, int col, char c);

void vga_color_set_fg(enum vga_color fg);

void vga_color_set_bg(enum vga_color bg);

void vga_color_set_bold(bool on);

char vga_get_char(int x, int y);

void vga_scroll_up(int times);
void vga_scroll_down(int times);
