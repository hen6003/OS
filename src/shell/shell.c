#include "shell.h"

#include "term.h"
#include "util.h"

char input_command[1024] = "";

struct command
{
  char name[100];
  void (*func)();
};

struct command commands[100];

void clear() { term_puts("\e[2J\e[H"); }

void fetch()
{
  term_puts("\e[34;1m"
            "   \\\\\n"
            "   (o>\n"
            "\\\\_//\n"
            " \\_/_)\n"
            "  _|_\n\n"
	    "\e[22;37m"); 
  asm volatile ("int $0x3");
}

void cmds()
{
  for (int i = 0; i < 100; i++)
    if (commands[i].name[0] != '\0')
    {
      term_puts(commands[i].name);
      term_puts("\n");
    }
}

void run_command(char *command)
{
  if (command[0] == '\0')
    return;
  
  for (int i = 0; i < 100; i++)
    if (!strcmp(command, commands[i].name))
    {
      commands[i].func();
      return;
    }

  term_puts("\e[31mUnknown command: \"");
  term_puts(command);
  term_puts("\"\e[37m\n");
}

void print_prompt()
{
  term_puts("> ");
}

// Shell to handle user input
char shell()
{
  enum keycodes key;
  char ch;

  // Setup commands (temp)
  for (int i = 0; i < 100; i++)
    commands[i].name[0] = '\0';

  strcpy(commands[0].name, "clear");
  commands[0].func = clear;

  strcpy(commands[1].name, "cmds");
  commands[1].func = cmds;

  strcpy(commands[2].name, "fetch");
  commands[2].func = fetch;

  term_puts("\e[22;37mShell started...\n\n");

  print_prompt();
  while (1)
  {
    key = get_input_keycode();

    switch (get_ascii_char(key))
    {
    case '\b':
      if (term_cursor_posx() > 2)
	term_putc('\b');
      if (strlen(input_command) != 0)
	input_command[strlen(input_command)-1] = '\0';
      break;

    case '\n':
      term_putc('\n');
      run_command(input_command);
      input_command[0] = '\0';

      print_prompt();
      break;
       
    default:
      ch = get_ascii_char(key);

      input_command[strlen(input_command)+1] = '\0';
      input_command[strlen(input_command)] = ch;
      
      if (ch != 0)
	term_putc(ch);
      break;
    }

    key = 0;
    sleep(0x0004FFFF);
  }

  return 0;
}
