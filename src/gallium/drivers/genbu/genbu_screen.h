#ifndef GENBU_SCREEN_H
#define GENBU_SCREEN_H

#include "pipe/p_screen.h"
#include "pipe/p_defines.h"
#include "os/os_thread.h"

struct sw_winsys;

struct genbu_screen
{
   struct pipe_screen base;

   struct sw_winsys *winsys;
};

static inline struct genbu_screen *
genbu_screen( struct pipe_screen *pipe )
{
   return (struct genbu_screen *)pipe;
}

#endif
