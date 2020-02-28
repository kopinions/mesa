#include "util/u_memory.h"
#include "util/u_inlines.h"
#include "util/format/u_format.h"

#include "genbu_resource.h"
#include "genbu_screen.h"
                                               
static void
genbu_texture_transfer_unmap(struct pipe_context *pipe,
			    struct pipe_transfer *transfer)
{

}

static void *
genbu_texture_transfer_map(struct pipe_context *pipe,
                          struct pipe_resource *resource,
                          unsigned level,
                          unsigned usage,
                          const struct pipe_box *box,
                          struct pipe_transfer **ptransfer)
{
   return (void*) NULL;
}

static bool
genbu_texture_get_handle(struct pipe_screen * screen,
                        struct pipe_resource *texture,
                        struct winsys_handle *whandle)
{
   struct genbu_screen *gs = cast2_genbu_screen(screen);
   struct genbu_texture *tex = genbu_texture(texture);
   struct genbu_winsys *gws = gs->gws;

   return gws->buffer_get_handle(gws, tex->buffer, whandle, tex->stride);
}

static void
genbu_texture_destroy(struct pipe_screen *screen,
                     struct pipe_resource *pt)
{
   struct genbu_texture *tex = genbu_texture(pt);
   struct genbu_winsys *gws = cast2_genbu_screen(screen)->gws;
   uint i;

   if (tex->buffer)
      gws->buffer_destroy(gws, tex->buffer);

   for (i = 0; i < ARRAY_SIZE(tex->image_offset); i++)
      FREE(tex->image_offset[i]);

   FREE(tex);
}


struct u_resource_vtbl genbu_texture_vtbl =
{
   genbu_texture_get_handle,	      /* get_handle */
   genbu_texture_destroy,	      /* resource_destroy */
   genbu_texture_transfer_map,	      /* transfer_map */
   u_default_transfer_flush_region,   /* transfer_flush_region */
   genbu_texture_transfer_unmap,	      /* transfer_unmap */
};

static inline unsigned
align_nblocksy(enum pipe_format format, unsigned width, unsigned align_to)
{
   return align(util_format_get_nblocksy(format, width), align_to);
}

struct pipe_resource *
genbu_texture_from_handle(struct pipe_screen * screen,
			  const struct pipe_resource *template,
			  struct winsys_handle *whandle)
{
   struct genbu_screen *gs = cast2_genbu_screen(screen);
   struct genbu_texture *tex;
   struct genbu_winsys *gws = gs->gws;
   struct genbu_winsys_buffer *buffer;
   unsigned stride;
   enum genbu_winsys_buffer_tile tiling;

   assert(screen);

   buffer = gws->buffer_from_handle(gws, whandle, template->height0, &tiling, &stride);

   /* Only supports one type */
   if ((template->target != PIPE_TEXTURE_2D &&
       template->target != PIPE_TEXTURE_RECT) ||
       template->last_level != 0 ||
       template->depth0 != 1) {
      return NULL;
   }

   tex = CALLOC_STRUCT(genbu_texture);
   if (!tex)
      return NULL;

   tex->b.b = *template;
   tex->b.vtbl = &genbu_texture_vtbl;
   pipe_reference_init(&tex->b.b.reference, 1);
   tex->b.b.screen = screen;

   tex->stride = stride;
   tex->tiling = tiling;
   tex->total_nblocksy = align_nblocksy(tex->b.b.format, tex->b.b.height0, 8);

   
   tex->buffer = buffer;

  
   return &tex->b.b;
}

static enum genbu_winsys_buffer_tile
genbu_texture_tiling(struct genbu_screen *gs, struct genbu_texture *tex)
{
   if (!gs->debug.tiling)
      return GENBU_TILE_NONE;

   if (tex->b.b.target == PIPE_TEXTURE_1D)
      return GENBU_TILE_NONE;

   if (util_format_is_s3tc(tex->b.b.format))
      return GENBU_TILE_X;

   if (gs->debug.use_blitter)
      return GENBU_TILE_X;
   else
      return GENBU_TILE_Y;
}


struct pipe_resource *
genbu_texture_create(struct pipe_screen *screen,
                    const struct pipe_resource *template,
                    boolean force_untiled)
{
   struct genbu_screen *gs = cast2_genbu_screen(screen);
   struct genbu_winsys *gws = gs->gws;
   struct genbu_texture *tex = CALLOC_STRUCT(genbu_texture);
   unsigned buf_usage = 0;

   if (!tex)
      return NULL;

   tex->b.b = *template;
   tex->b.vtbl = &genbu_texture_vtbl;
   pipe_reference_init(&tex->b.b.reference, 1);
   tex->b.b.screen = screen;

   if ( (force_untiled) || (template->usage == PIPE_USAGE_STREAM) )
      tex->tiling = GENBU_TILE_NONE;
   else
      tex->tiling = genbu_texture_tiling(gs, tex);

   /* for scanouts and cursors, cursors arn't scanouts */

   /* XXX: use a custom flag for cursors, don't rely on magically
    * guessing that this is Xorg asking for a cursor
    */
   if ((template->bind & PIPE_BIND_SCANOUT) && template->width0 != 64)
      buf_usage = GENBU_NEW_SCANOUT;
   else
      buf_usage = GENBU_NEW_TEXTURE;

   tex->buffer = gws->buffer_create_tiled(gws, &tex->stride, tex->total_nblocksy,
                                             &tex->tiling, buf_usage);
   if (!tex->buffer)
      goto fail;


   return &tex->b.b;

fail:
   FREE(tex);
   return NULL;
}
