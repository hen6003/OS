#include <stdint.h>

void outb(uint16_t port, uint8_t val);
void outw(uint16_t port, uint16_t val);

uint8_t inb(uint16_t port);

void wait(uint32_t timer_count);
