#ifndef __DZ_TERM_H__
#define __DZ_TERM_H__

#include <termios.h>

#include <dz/term/input.h>

#define ANSI_CLEAR_SCREEN L_("\033[1;1H\033[2J")
#define ANSI_HIDE_CURSOR  L_("\033[?25l")
#define ANSI_SHOW_CURSOR  L_("\033[?25h")

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

#endif /* __DZ_TERM_H__ */
