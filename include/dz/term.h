#ifndef __DZ_TERM_H__
#define __DZ_TERM_H__

#include <termios.h>
#include <unistd.h>

#include <dz/term/input.h>
#include <dz/utf8.h>

#define ANSI_CLEAR_SCREEN "\033[1;1H\033[2J"
#define ANSI_HIDE_CURSOR  "\033[?25l"
#define ANSI_SHOW_CURSOR  "\033[?25h"

/**
  \brief Setup terminal for interactive I/O.

  Changes made by the function are the following:
    - hide cursor;
    - enable raw mode;
    - disable echo;
    - enable non blocking I/O.
 */
void init_term(void);

/**
  \brief Revert terminal changes made by `init_term`.
  */
void end_term(void);

/**
  \brief Fetch terminal size.
  */
void termsize(int *w, int *h);

#endif /* __DZ_TERM_H__ */
