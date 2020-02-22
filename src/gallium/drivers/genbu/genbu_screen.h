#ifndef GENBU_SCREEN_H
#define GENBU_SCREEN_H

#include "pipe/p_screen.h"
#include "pipe/p_defines.h"
#include "os/os_thread.h"

struct genbu_winsys;

struct genbu_screen
{
   struct pipe_screen base;

   struct genbu_winsys *gws;

   struct {
      boolean tiling;
      boolean lie;
      boolean use_blitter;
   } debug;
};

static inline struct genbu_screen *
cast2_genbu_screen( struct pipe_screen *pipe )
{
   return (struct genbu_screen *)pipe;
}

bool
genbu_is_format_supported(struct pipe_screen *screen,
                         enum pipe_format format,
                         enum pipe_texture_target target,
                         unsigned sample_count,
                         unsigned storage_sample_count,
                         unsigned tex_usage);
#endif
