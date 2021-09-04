#include "shell.h"

#include "term.h"
#include "keyboard.h"
#include "util.h"

// Shell to handle user input
int shell()
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
      term_puth(term_get_char(0,0));
      break;

    case KEY_RIGHT:
      return 100;
      break;
      
    default:
      ch = get_ascii_char_lower(key);
      
      if (ch != 0)
	term_putc(ch);
    }

    key = 0;
    sleep(0x01FFFFFF);
  }

  return 0;
}
