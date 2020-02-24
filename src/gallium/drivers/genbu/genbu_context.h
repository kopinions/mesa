#ifndef GENBU_CONTEXT_H
#define GENBU_CONTEXT_H

#include "pipe/p_context.h"
#include "pipe/p_defines.h"
#include "pipe/p_state.h"

#include "draw/draw_vertex.h"

#include "tgsi/tgsi_scan.h"

#include "util/slab.h"
#include "util/u_blitter.h"

#define GENBU_TEX_UNITS 8
#define GENBU_MAX_IMMEDIATE        8
#define GENBU_MAX_DYNAMIC          14

struct genbu_blend_state {
   unsigned iab;
   unsigned modes4;
   // load immediate state 5
   unsigned LIS5;
   // load immediate state 6
   unsigned LIS6;
};

struct genbu_depth_stencil_state {
   unsigned stencil_modes4;
   unsigned bfo[2];
   unsigned stencil_LIS5;
   unsigned depth_LIS6;
};

struct genbu_sampler_state {
   struct pipe_sampler_state templ;
   unsigned state[3];
   unsigned minlod;
   unsigned maxlod;
};

struct genbu_rasterizer_state {
   struct pipe_rasterizer_state templ;
   unsigned light_twoside : 1;
   unsigned st;

   unsigned LIS4;
   unsigned LIS7;
   unsigned sc[1];

   union { float f; unsigned u; } ds[2];
};

struct genbu_context {
   struct pipe_context base;

   struct genbu_winsys *gws;

   struct draw_context *draw;

   /* The most recent drawing state as set by the driver:
    */
   const struct genbu_blend_state           *blend;
   //TODO: const struct i915_sampler_state         *fragment_sampler[PIPE_MAX_SAMPLERS];
   struct pipe_sampler_state               *vertex_samplers[PIPE_MAX_SAMPLERS];
   // TODO: const struct i915_depth_stencil_state   *depth_stencil;
   // TODO: const struct i915_rasterizer_state      *rasterizer;

   // TODO: struct i915_fragment_shader *fs;

   void *vs;

   // TODO: struct i915_velems_state *velems;
   unsigned nr_vertex_buffers;
   struct pipe_vertex_buffer vertex_buffers[PIPE_MAX_ATTRIBS];

   struct pipe_blend_color blend_color;
   struct pipe_stencil_ref stencil_ref;
   struct pipe_clip_state clip;
   struct pipe_resource *constants[PIPE_SHADER_TYPES];
   struct pipe_framebuffer_state framebuffer;
   struct pipe_poly_stipple poly_stipple;
   struct pipe_scissor_state scissor;
   struct pipe_sampler_view *fragment_sampler_views[PIPE_MAX_SAMPLERS];
   struct pipe_sampler_view *vertex_sampler_views[PIPE_MAX_SAMPLERS];
   struct pipe_viewport_state viewport;

   unsigned dirty;

   struct pipe_resource *mapped_vs_tex[PIPE_MAX_SAMPLERS];
   // TODO: struct i915_winsys_buffer* mapped_vs_tex_buffer[PIPE_MAX_SAMPLERS];

   unsigned num_samplers;
   unsigned num_fragment_sampler_views;
   unsigned num_vertex_samplers;
   unsigned num_vertex_sampler_views;

   // TODO: struct i915_winsys_batchbuffer *batch;

   /** Vertex buffer */
   // TODO: struct i915_winsys_buffer *vbo;
   size_t vbo_offset;
   unsigned vbo_flushed;

   // TODO: struct i915_state current;
   unsigned hardware_dirty;
   unsigned immediate_dirty : GENBU_MAX_IMMEDIATE;
   unsigned dynamic_dirty : GENBU_MAX_DYNAMIC;
   unsigned static_dirty : 4;
   unsigned flush_dirty : 2;

   // TODO: struct i915_winsys_buffer *validation_buffers[2 + 1 + I915_TEX_UNITS];
   int num_validation_buffers;

   struct slab_mempool transfer_pool;
   struct slab_mempool texture_transfer_pool;

   /* state for tracking flushes */
   int last_fired_vertices;
   int fired_vertices;
   int queued_vertices;

   /** blitter/hw-clear */
   struct blitter_context* blitter;
};



struct pipe_context *genbu_context_create(struct pipe_screen *screen,
					 void *priv, unsigned flags);

static inline struct genbu_context *
cast2_genbu_context( struct pipe_context *pipe )
{
   return (struct genbu_context *)pipe;
}

void genbu_clear_blitter(struct pipe_context *pipe, unsigned buffers,
                        const union pipe_color_union *color,
                        double depth, unsigned stencil);
void genbu_clear_render(struct pipe_context *pipe, unsigned buffers,
                       const union pipe_color_union *color,
                       double depth, unsigned stencil);
void genbu_clear_emit(struct pipe_context *pipe, unsigned buffers,
                     const union pipe_color_union *color,
                     double depth, unsigned stencil,
                     unsigned destx, unsigned desty, unsigned width, unsigned height);

struct draw_stage *genbu_draw_vbuf_stage(struct genbu_context *genbu);
  
struct draw_stage *genbu_draw_render_stage( struct genbu_context *genbu);
#endif//GENBU_CONTEXT_H
