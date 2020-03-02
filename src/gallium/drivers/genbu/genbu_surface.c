
#include "genbu_surface.h"
#include "genbu_context.h"
#include "genbu_surface.h"

static struct pipe_surface *
genbu_create_surface_custom(struct pipe_context *ctx,
                           struct pipe_resource *pt,
                           const struct pipe_surface *surf_tmpl,
                           unsigned width0,
                           unsigned height0)
{
   struct pipe_surface *ps;

   assert(surf_tmpl->u.tex.first_layer == surf_tmpl->u.tex.last_layer);
   if (pt->target != PIPE_TEXTURE_CUBE &&
       pt->target != PIPE_TEXTURE_3D)
      assert(surf_tmpl->u.tex.first_layer == 0);

   ps = CALLOC_STRUCT(pipe_surface);
   if (ps) {
      /* could subclass pipe_surface and store offset as it used to do */
      pipe_reference_init(&ps->reference, 1);
      pipe_resource_reference(&ps->texture, pt);
      ps->format = surf_tmpl->format;
      ps->width = u_minify(width0, surf_tmpl->u.tex.level);
      ps->height = u_minify(height0, surf_tmpl->u.tex.level);
      ps->u.tex.level = surf_tmpl->u.tex.level;
      ps->u.tex.first_layer = surf_tmpl->u.tex.first_layer;
      ps->u.tex.last_layer = surf_tmpl->u.tex.last_layer;
      ps->context = ctx;
   }
   return ps;
}


static struct pipe_surface *
genbu_create_surface(struct pipe_context *ctx,
                    struct pipe_resource *pt,
                    const struct pipe_surface *surf_tmpl)
{
   return genbu_create_surface_custom(ctx, pt, surf_tmpl,
                                     pt->width0, pt->height0);
}


void
genbu_init_surface_functions(struct genbu_context *gc)
{
   /* if (i915_screen(gc->base.screen)->debug.use_blitter) { */
   /*    gc->base.resource_copy_region = genbu_surface_copy_blitter; */
   /*    gc->base.clear_render_target = genbu_clear_render_target_blitter; */
   /*    gc->base.clear_depth_stencil = genbu_clear_depth_stencil_blitter; */
   /* } else { */
   /*    gc->base.resource_copy_region = genbu_surface_copy_render; */
   /*    gc->base.clear_render_target = genbu_clear_render_target_render; */
   /*    gc->base.clear_depth_stencil = genbu_clear_depth_stencil_render; */
   /* } */
   /* gc->base.blit = genbu_blit; */
   /* gc->base.flush_resource = genbu_flush_resource; */
   gc->base.create_surface = genbu_create_surface;
   /* gc->base.surface_destroy = genbu_surface_destroy; */
}
