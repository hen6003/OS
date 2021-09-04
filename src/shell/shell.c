#include "shell.h"

#include "term.h"
#include "util.h"

// Shell to handle user input
char shell()
{
  enum keycodes key;
  char ch;

  term_color_set(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  term_puts("Shell started...\n");

  while (1)
  {
    key = get_input_keycode();

    switch (key)
    {
    case KEY_LEFT:
      term_cursor_move(-1,0);
      break;

    case KEY_RIGHT:
      term_cursor_move(1,0);
      break;

    case KEY_UP:
      scroll_down();
      break;

    case KEY_DOWN:
      scroll_up();
      break;
      
    default:
      ch = get_ascii_char(key);
      
      if (ch != 0)
	term_putc(ch);
    }

    key = 0;
    sleep(0x01FFFFFF);
  }

  return 0;
}
