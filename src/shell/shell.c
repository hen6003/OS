#include "shell.h"

#include "term.h"
#include "util.h"

char command[1024] = "";

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

    case KEY_ENTER:
      term_putc('\n');
      term_puts(command);
      break;
      
    default:
      ch = get_ascii_char(key);

      command[strlen(command)+1] = '\0';
      command[strlen(command)] = ch;
      
      if (ch != 0)
	term_putc(ch);
      break;
    }

    key = 0;
    sleep(0x0005FFFF);
  }

  return 0;
}
