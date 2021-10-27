#ifndef __DZ_ENGINE_H__
#define __DZ_ENGINE_H__

#include <dz/screen.h>

#include <dz/common.h>
#include <dz/term/input.h>

typedef void (*hook_kb_t)(void *, kb_event_t);
typedef void (*hook_mouse_t)(void *, mouse_event_t);
typedef void (*hook_loop_t)(void *, long);

typedef struct {
  screen_t *screen;

  hook_kb_t    hook_kb;
  hook_mouse_t hook_mouse;
  hook_loop_t  hook_loop;

  bool run;
  long last_draw;
} engine_t;

engine_t *engine_new(hook_kb_t hook_kb, hook_mouse_t hook_mouse, hook_loop_t hook_loop);

void engine_free(engine_t *e);
int  engine_loop(engine_t *e);

#endif /* __DZ_ENGINE_H__ */
