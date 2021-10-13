#include <dz/term.h>

#include <dz/common.h>

#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>

/* initial terminal settings, will be restored by `end_term` */
struct termios term_settings;

void
init_term()
{
  struct termios new_settings;

  PUTS(ANSI_CLEAR_SCREEN);
  PUTS(ANSI_HIDE_CURSOR);

  tcgetattr(0, &term_settings);

  new_settings = term_settings;
  new_settings.c_lflag &= ~ICANON;
  new_settings.c_lflag &= ~ECHO;
  new_settings.c_lflag &= ~ISIG;
  new_settings.c_cc[VMIN]  = 0;
  new_settings.c_cc[VTIME] = 0;

  tcsetattr(0, TCSANOW, &new_settings);
}

void
end_term()
{
  PUTS(ANSI_CLEAR_SCREEN);
  PUTS(ANSI_SHOW_CURSOR);
  tcsetattr(0, TCSANOW, &term_settings);
}
