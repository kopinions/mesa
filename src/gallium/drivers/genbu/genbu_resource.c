#include "genbu_screen.h"
#include "genbu_resource.h"
#include "genbu_context.h"

static struct pipe_resource * genbu_resource_create(struct pipe_screen * ps,
					     const struct pipe_resource *templat) {
   if (templat->target == PIPE_BUFFER) {
      return genbu_buffer_create(ps, templat);
   } else {
      if (!(templat->bind & PIPE_BIND_LINEAR))
         return genbu_texture_create(ps, templat, FALSE);
      else
         return genbu_texture_create(ps, templat, TRUE);
   }
}


static struct pipe_resource *
genbu_resource_from_handle(struct pipe_screen * screen,
			 const struct pipe_resource *template,
			 struct winsys_handle *whandle,
                          unsigned usage)
{
   if (template->target == PIPE_BUFFER)
      return NULL;
   else
      return genbu_texture_from_handle(screen, template, whandle);
}


void
genbu_init_resource_functions(struct genbu_context *gc )
{
   gc->base.transfer_map = u_transfer_map_vtbl;
   gc->base.transfer_flush_region = u_transfer_flush_region_vtbl;
   gc->base.transfer_unmap = u_transfer_unmap_vtbl;
   gc->base.buffer_subdata = genbu_buffer_subdata;
   gc->base.texture_subdata = u_default_texture_subdata;
}


void genbu_init_screen_resource_functions(struct genbu_screen *gs) {
   gs->base.resource_create = genbu_resource_create;
   gs->base.resource_from_handle = genbu_resource_from_handle;
   gs->base.resource_get_handle = u_resource_get_handle_vtbl;
   gs->base.resource_destroy = u_resource_destroy_vtbl;
}
