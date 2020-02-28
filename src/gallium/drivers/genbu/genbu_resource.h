#ifndef GENBU_RESOURCE_H
#define GENBU_RESOURCE_H

#include "util/u_transfer.h"
#include "util/u_debug.h"
#include "genbu_winsys.h"


struct genbu_screen;
extern struct u_resource_vtbl genbu_buffer_vtbl;
extern struct u_resource_vtbl genbu_texture_vtbl;

struct genbu_buffer {
   struct u_resource b;
   uint8_t *data;
   boolean free_on_destroy;
};

struct genbu_transfer {
   /* Base class. */
   struct pipe_transfer b;
   struct pipe_resource *staging_texture;
};

#define GENBU_MAX_TEXTURE_2D_LEVELS 12  /* max 2048x2048 */
#define GENBU_MAX_TEXTURE_3D_LEVELS  9  /* max 256x256x256 */
    
struct genbu_texture {
   struct u_resource b;

   /* tiling flags */
   enum genbu_winsys_buffer_tile tiling;
   unsigned stride;
   unsigned depth_stride;          /* per-image on i945? */
   unsigned total_nblocksy;

   unsigned nr_images[GENBU_MAX_TEXTURE_2D_LEVELS];

   /* Explicitly store the offset of each image for each cube face or
    * depth value.
    *
    * Array [depth] off offsets.
    */
   struct offset_pair *image_offset[GENBU_MAX_TEXTURE_2D_LEVELS];

   /* The data is held here:
    */
   struct genbu_winsys_buffer *buffer;
};


static inline struct genbu_buffer *genbu_buffer(struct pipe_resource *resource)
{
   struct genbu_buffer *buf = (struct genbu_buffer *)resource;
   assert(buf->b.vtbl == &genbu_buffer_vtbl);
   return buf;
}

static inline struct genbu_texture *genbu_texture(struct pipe_resource *resource)
{
   struct genbu_texture *tex = (struct genbu_texture *)resource;
   assert(tex->b.vtbl == &genbu_texture_vtbl);
   return tex;
}

void genbu_init_screen_resource_functions(struct genbu_screen *gs);
  


struct pipe_resource *
genbu_buffer_create(struct pipe_screen *screen,
		   const struct pipe_resource *template);

struct pipe_resource *
genbu_texture_create(struct pipe_screen *screen,
                    const struct pipe_resource *template,
                    boolean force_untiled);

struct pipe_resource *
genbu_texture_from_handle(struct pipe_screen * screen,
			 const struct pipe_resource *template,
			 struct winsys_handle *whandle);

#endif//GENBU_RESOURCE_H
