#include "term.h"
#include "util.h"
#include "shell.h"
#include "descriptor_tables.h"

// This is our kernel's main function
void kernel_main()
{
  // Initiate descriptor tables
  init_descriptor_tables();
  
  // Initiate terminal
  term_init();
  
  // Display some messages

  term_puts("\e[34;1m"
            "   \\\\\n"
            "   (o>\n"
            "\\\\_//\n"
            " \\_/_)\n"
            "  _|_\n\n"
	    "\e[37;22m"); 

  // Start shell
  char ret = 0;
  while ((ret = shell()))
  {
    // Restart shell unless exit code 0
    term_puts("\e[31mERROR: shell returned:");
    term_puth((int) ret);
    term_puts("\e[37m\n");
  }

  term_puts("\e[31mShutting down...");
  
  sleep(0xFFFFFFFF);

  shutdown();
}
