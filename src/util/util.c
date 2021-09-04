// This file implements some useful functions

#include "util.h"
#include "asm.h"

size_t digit_count(int num, int base)
{
  size_t count = 0;
  if(num == 0)
    return 1;
  while(num > 0)
  {
    count++;
    num = num/base;
  }
  return count;
}

void itoa(int num, char *number, int base)
{
  int dgcount = digit_count(num, base);
  int index = dgcount - 1;
  char x;

  if(num == 0 && dgcount == 1)
  {
    number[0] = '0';
    number[1] = '\0';
  }
  else
  {
    while(num != 0)
    {
      x = num % base;
      if (x <= 9)
	number[index] = x + '0';
      else // Gone off numbers into letters
	number[index] = x + 'A'-10;
      index--;
      num = num / base;
    }
    number[dgcount] = '\0';
  }
}

void shutdown()
{
  outw(0x604, 0x2000);
}

void sleep(uint32_t timer_count)
{
  wait(timer_count);
}
