#include <stdint.h>

#include "asm.h"

void disable_cursor();

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);

void update_cursor(int x, int y);
