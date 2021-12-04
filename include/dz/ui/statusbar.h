#ifndef __DZ_UI_STATUSBAR_H__
#define __DZ_UI_STATUSBAR_H__

#include <dz/screen.h>

typedef struct {
  char    *text;
  unsigned len;
} statusbar_t;

statusbar_t *statusbar_new(const char *title);
void         statusbar_free(statusbar_t *sb);
void         statusbar_draw(statusbar_t *sb, screen_t *s);

/**
  \brief Update status bar.
  */
void statusbar_update(statusbar_t *sb, const char *text);

/**
  \brief Update status bar.
  \param fmt Format.
  \param ... Format arguments.
  */
void statusbar_fupdate(statusbar_t *sb, const char *fmt, ...);

#endif /* __DZ_UI_STATUSBAR_H__ */
