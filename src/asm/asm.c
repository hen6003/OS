#include "asm.h"

void outb(uint16_t port, uint8_t val)
{
  asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
  /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
   * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
   * The  outb  %al, %dx  encoding is the only option for all other cases.
   * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

void outw(uint16_t port, uint16_t val)
{
  asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) );
  /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
   * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
   * The  outb  %al, %dx  encoding is the only option for all other cases.
   * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

uint8_t inb(uint16_t port)
{
  uint8_t ret;
  asm volatile ( "inb %1, %0"
                 : "=a"(ret)
                 : "Nd"(port) );
  return ret;
}

void wait(uint32_t timer_count)
{
  while (1)
  {
    asm volatile("nop");
    timer_count--;
    if (timer_count <= 0)
      break;
  }
}
