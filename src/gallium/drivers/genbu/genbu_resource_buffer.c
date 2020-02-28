#include "genbu_resource.h"
#include "util/u_inlines.h"
#include "pipe/p_defines.h"

#include "genbu_context.h"
#include "genbu_resource.h"

static void *
genbu_buffer_transfer_map(struct pipe_context *pipe,
                         struct pipe_resource *resource,
                         unsigned level,
                         unsigned usage,
                         const struct pipe_box *box,
                         struct pipe_transfer **ptransfer)
{
   struct genbu_context *i915 = cast2_genbu_context(pipe);
   struct genbu_buffer *buffer = genbu_buffer(resource);
   struct pipe_transfer *transfer = slab_alloc_st(&i915->transfer_pool);

   if (!transfer)
      return NULL;

   transfer->resource = resource;
   transfer->level = level;
   transfer->usage = usage;
   transfer->box = *box;
   *ptransfer = transfer;

   return buffer->data + transfer->box.x;
}

static void
genbu_buffer_transfer_unmap(struct pipe_context *pipe,
                           struct pipe_transfer *transfer)
{
   struct genbu_context *genbu = cast2_genbu_context(pipe);
   slab_free_st(&genbu->transfer_pool, transfer);
}


static bool
genbu_buffer_get_handle(struct pipe_screen *screen,
		       struct pipe_resource *resource,
		       struct winsys_handle *handle)
{
   return FALSE;
}

static void
genbu_buffer_destroy(struct pipe_screen *screen,
		    struct pipe_resource *resource)
{
   struct genbu_buffer *buffer = genbu_buffer(resource);
   if (buffer->free_on_destroy)
      align_free(buffer->data);
   FREE(buffer);
}

struct u_resource_vtbl genbu_buffer_vtbl =
{
   genbu_buffer_get_handle,	     /* get_handle */
   genbu_buffer_destroy,	     /* resource_destroy */
   genbu_buffer_transfer_map,	     /* transfer_map */
   u_default_transfer_flush_region,  /* transfer_flush_region */
   genbu_buffer_transfer_unmap,	     /* transfer_unmap */
};


struct pipe_resource *
genbu_buffer_create(struct pipe_screen *screen,
                    const struct pipe_resource *template)
{
   struct genbu_buffer *buf = CALLOC_STRUCT(genbu_buffer);

   if (!buf)
      return NULL;

   buf->b.b = *template;
   buf->b.vtbl = &genbu_buffer_vtbl;
   pipe_reference_init(&buf->b.b.reference, 1);
   buf->b.b.screen = screen;
   buf->data = align_malloc(template->width0, 64);
   buf->free_on_destroy = TRUE;

   if (!buf->data)
      goto err;

   return &buf->b.b;

err:
   FREE(buf);
   return NULL;
}


void
genbu_buffer_subdata(struct pipe_context *rm_ctx,
                    struct pipe_resource *resource,
                    unsigned usage, unsigned offset,
                    unsigned size, const void *data)
{
   struct genbu_buffer *buffer = genbu_buffer(resource);

   memcpy(buffer->data + offset, data, size);
}
