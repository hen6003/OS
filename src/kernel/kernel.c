#include "term.h"
#include "util.h"
#include "shell.h"

// This is our kernel's main function
void kernel_main()
{
  // We're here! Let's initiate the terminal and display a message to show we got here.
  
  // Initiate terminal
  term_init();
  
  // Display some messages
  term_puts("Kernel started...\n\n");

  term_color_set(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);

  term_puts("   \\\\\n"
	    "   (o>\n"
	    "\\\\_//\n"
	    " \\_/_)\n"
	    "  _|_\n\n");
  
  term_color_set(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

  // Test ascii

  term_puts("t\e[4At");

  // Start shell
  //char ret = 0;
  //while ((ret = shell()))
  //{
    //// Restart shell unless exit code 0
    //term_color_set(VGA_COLOR_RED, VGA_COLOR_BLACK);
    //term_puts("ERROR: shell returned: ");
    //term_puth((int) ret);
    //term_putc('\n');
  //}

  term_puts("Shutting down...");
  
  sleep(0xFFFFFFFF);

  shutdown();
}
