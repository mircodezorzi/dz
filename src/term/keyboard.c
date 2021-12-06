#include <dz/term/keyboard.h>

#include <ctype.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <wchar.h>

kb_event_t
parse_key(char *data)
{
#define CTRLMASK(Chr)  ((Chr)&0x1f)
#define SHIFTMASK(Chr) ((Chr) | 0x40)

  char c = data[0];

  switch (c) {
  case 0: return (kb_event_t){.key = KEY_NONE};
  case KEY_ESCAPE:
    if (data[1] == '[') {
      switch (data[2]) {
      case 'A': return (kb_event_t){.key = KEY_UP};
      case 'B': return (kb_event_t){.key = KEY_DOWN};
      case 'C': return (kb_event_t){.key = KEY_LEFT};
      case 'D': return (kb_event_t){.key = KEY_RIGHT};
      default: return (kb_event_t){.key = KEY_NONE};
      }
    }
    return (kb_event_t){.key = KEY_NONE};
  default:
    /* clang-format off */
    return (kb_event_t){
        .key = tolower(c),
        .modifier = (c == CTRLMASK(c) ? MOD_CTRL : MOD_NONE)
                  | (c != SHIFTMASK(c) ? MOD_SHIFT : MOD_NONE),
    };
  }

#undef CTRLMASK
#undef SHIFTMASK
}
