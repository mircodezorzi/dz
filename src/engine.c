#include <dz/engine.h>

#include <dz/term.h>

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

static long
current()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return 1000000 * tv.tv_sec + tv.tv_usec;
}

static long
deltatime()
{
  static long last = 0;

  struct timeval tv;
  long           dt;
  long           current;

  gettimeofday(&tv, NULL);
  current = 1000000 * tv.tv_sec + tv.tv_usec;
  dt      = current - last;
  last    = current;
  return dt;
}

engine_t *
engine_new(hook_kb_t hook_kb, hook_mouse_t hook_mouse, hook_loop_t hook_loop)
{
  engine_t *e   = malloc(sizeof(engine_t));
  e->screen     = screen_new();
  e->hook_kb    = hook_kb;
  e->hook_mouse = hook_mouse;
  e->hook_loop  = hook_loop;
  e->run        = true;
  e->last_draw  = current();
  return e;
}

void
engine_free(engine_t *e)
{
  screen_free(e->screen);
  free(e);
}

int
engine_loop(engine_t *e)
{
  input_event_t ev;
  long          dt;

  setlocale(LC_CTYPE, "");

  init_term();
  set_mouse_mode(MOUSE_MODE_DRAG);

  while (e->run) {
    ev = parse_input();
    dt = deltatime();

    screen_update(e->screen);

    if (ev.type == EVENT_TYPE_KEYBOARD) {
      if (ev.e.kb.key != KEY_NONE) {
        (e->hook_kb)(e, ev.e.kb);
      }
    } else if (ev.type == EVENT_TYPE_MOUSE) {
      (e->hook_mouse)(e, ev.e.mouse);
    }

    if (e->run && e->hook_loop) {
      (e->hook_loop)(e, dt);
    }

    if (current() - 100000 > e->last_draw) {
      screen_render(e->screen);
      e->last_draw = current();
    }

    screen_repaint(e->screen);

    usleep(100 / dt);
  }

  set_mouse_mode(MOUSE_MODE_OFF);
  end_term();
  return 0;
}
