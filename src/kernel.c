#include "term.h"
#include "asm.h"
#include "keyboard.h"

// This is our kernel's main function
void kernel_main()
{
  // We're here! Let's initiate the terminal and display a message to show we got here.
  
  // Initiate terminal
  term_init();
  
  // Display some messages
  term_puts("Hello, World!\n");
  term_puts("Welcome to thd\be kernel.\n\n");

  term_color_set(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);

  term_puts("   \\\\\n"
	    "   (o>\n"
	    "\\\\_//\n"
	    " \\_/_)\n"
	    "  _|_\n\n");
  
  term_color_set(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

  enum keycodes key;
  char ch;

  while (1)
  {
    key = get_input_keycode();

    ch = get_ascii_char_lower(key);

    if (ch != 0)
      term_putc(ch);

    key = 0;
    sleep(0x01FFFFFF);
  }
}
