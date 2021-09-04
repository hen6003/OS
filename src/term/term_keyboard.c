#include <stdbool.h>

#include "term.h"

bool shift = false;
bool ctrl  = false;
bool alt   = false;

bool capslock = false;
bool numlock  = false;

char get_ascii_char(enum keycodes keycode)
{
  switch (keycode)
  {
  case KEY_LEFT_SHIFT_PRESS:
  case KEY_RIGHT_SHIFT_PRESS:
    shift = true;
    break;

  case KEY_LEFT_SHIFT_RELEASE:
  case KEY_RIGHT_SHIFT_RELEASE:
    shift = false;
    break;

  case KEY_CAPS_LOCK_PRESS:
    capslock = true;

  case KEY_CAPS_LOCK_RELEASE:
    capslock = false;

  case KEY_NUM_LOCK_PRESS:
    numlock = true;

  case KEY_NUM_LOCK_RELEASE:
    numlock = false;

  case KEY_LEFT_ALT_PRESS:
    alt = true;

  case KEY_LEFT_ALT_RELEASE:
    alt = false;

  case KEY_LEFT_CTRL_PRESS:
    ctrl = true;

  case KEY_LEFT_CTRL_RELEASE:
    ctrl = false;
    
  default:
    if (shift||capslock)
      return get_ascii_char_upper(keycode);
    else
      return get_ascii_char_lower(keycode);
  }

  return 0;
}
