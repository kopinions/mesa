#include "util/u_memory.h"

#include "genbu_screen.h"
#include "genbu_public.h"

struct pipe_screen *
genbu_create_screen(struct sw_winsys *winsys)
{
   struct genbu_screen *screen;

   screen = CALLOC_STRUCT(genbu_screen);
   if (!screen)
      return NULL;


   screen->winsys = winsys;
   
   return &screen->base;
}
