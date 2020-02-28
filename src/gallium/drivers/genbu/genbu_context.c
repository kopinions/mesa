#include "genbu_context.h"
#include "genbu_screen.h"
#include "genbu_resource.h"
#include "genbu_state.h"


#include "draw/draw_context.h"
#include "pipe/p_defines.h"
#include "util/u_inlines.h"
#include "util/u_memory.h"
#include "util/u_prim.h"
#include "util/u_upload_mgr.h"
#include "pipe/p_screen.h"

DEBUG_GET_ONCE_BOOL_OPTION(genbu_no_vbuf, "GENBU_NO_VBUF", FALSE)
 
static void
genbu_destroy(struct pipe_context *pipe) {

}

static void
genbu_draw_vbo(struct pipe_context *pipe, const struct pipe_draw_info *info) {

}

struct pipe_context *genbu_context_create(struct pipe_screen *screen,
                                          void *priv, unsigned flags) {
   struct genbu_context *genbu = CALLOC_STRUCT(genbu_context);
   if (!genbu)
      return NULL;

   genbu->gws = cast2_genbu_screen(screen)->gws;
   genbu->base.screen = screen;
   genbu->base.priv = priv;
   genbu->base.stream_uploader = u_upload_create_default(&genbu->base);
   genbu->base.const_uploader = genbu->base.stream_uploader;

   genbu->base.destroy = genbu_destroy;

   if (cast2_genbu_screen(screen)->debug.use_blitter)
      genbu->base.clear = genbu_clear_blitter;
   else
      genbu->base.clear = genbu_clear_render;

   genbu->base.draw_vbo = genbu_draw_vbo;

   /* init this before draw */
   slab_create(&genbu->transfer_pool, sizeof(struct pipe_transfer),
                    16);
   slab_create(&genbu->texture_transfer_pool, sizeof(struct genbu_transfer),
                    16);

   /* Batch stream debugging is a bit hacked up at the moment:
    */
   // TODO: genbu->batch = genbu->iws->batchbuffer_create(genbu->iws);

   /*
    * Create drawing context and plug our rendering stage into it.
    */
   genbu->draw = draw_create(&genbu->base);
   assert(genbu->draw);
   if (!debug_get_option_genbu_no_vbuf()) {
      draw_set_rasterize_stage(genbu->draw, genbu_draw_vbuf_stage(genbu));
   } else {
      draw_set_rasterize_stage(genbu->draw, genbu_draw_render_stage(genbu));
   }

   //TODO: genbu_init_surface_functions(genbu);
   //TODO: genbu_init_state_functions(genbu);
   genbu_init_state_functions(genbu);
      
   // TODO: genbu_init_flush_functions(genbu);
   genbu_init_resource_functions(genbu);
   // TODO: genbu_init_query_functions(genbu);

   /* Create blitter. */
   genbu->blitter = util_blitter_create(&genbu->base);
   assert(genbu->blitter);

   /* must be done before installing Draw stages */
   util_blitter_cache_all_shaders(genbu->blitter);

   

   genbu->dirty = ~0;
   genbu->hardware_dirty = ~0;
   genbu->immediate_dirty = ~0;
   genbu->dynamic_dirty = ~0;
   genbu->static_dirty = ~0;
   genbu->flush_dirty = 0;

   return &genbu->base;
}


void genbu_clear_blitter(struct pipe_context *pipe, unsigned buffers,
                        const union pipe_color_union *color,
                        double depth, unsigned stencil) {
   
}
void genbu_clear_render(struct pipe_context *pipe, unsigned buffers,
                       const union pipe_color_union *color,
                       double depth, unsigned stencil) {
   
}
void genbu_clear_emit(struct pipe_context *pipe, unsigned buffers,
                     const union pipe_color_union *color,
                     double depth, unsigned stencil,
                     unsigned destx, unsigned desty, unsigned width, unsigned height) {
   
}

struct draw_stage *genbu_draw_vbuf_stage(struct genbu_context *genbu)
{
   struct vbuf_render *render;
   struct draw_stage *stage;

   return stage;
}


struct draw_stage *genbu_draw_render_stage( struct genbu_context *genbu) {
   struct draw_stage *stage;
   return stage;
}
