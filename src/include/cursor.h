#include <stdint.h>

#include "asm.h"

void cursor_disable();

void cursor_enable(uint8_t cursor_start, uint8_t cursor_end);

void cursor_set(int x, int y);
