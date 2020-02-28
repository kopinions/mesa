#include "genbu_drm_winsys.h"
#include "state_tracker/drm_driver.h"
#include "genbu_drm_winsys.h"
#include "util/u_memory.h"

#include "drm-uapi/genbu_drm.h"

static struct genbu_winsys_buffer *
genbu_drm_buffer_from_handle(struct genbu_winsys *gws,
                            struct winsys_handle *whandle,
                            unsigned height,
                            enum genbu_winsys_buffer_tile *tiling,
                            unsigned *stride)
{
   struct genbu_drm_winsys *gdws = genbu_drm_winsys(gws);
   struct genbu_drm_buffer *buf;
   uint32_t tile = 0, swizzle = 0;

   if ((whandle->type != WINSYS_HANDLE_TYPE_SHARED) && (whandle->type != WINSYS_HANDLE_TYPE_FD))
      return NULL;

   if (whandle->offset != 0)
      return NULL;

   buf = CALLOC_STRUCT(genbu_drm_buffer);
   if (!buf)
      return NULL;

   buf->magic = 0xDEAD1337;


   buf->flinked = TRUE;
   buf->flink = whandle->handle;

   /* if (!buf->bo) */
   /*    goto err; */

   *stride = whandle->stride;
   *tiling = tile;

   return (struct genbu_winsys_buffer *)buf;

err:
   FREE(buf);
   return NULL;
}

static boolean
genbu_drm_buffer_get_handle(struct genbu_winsys *gws,
                            struct genbu_winsys_buffer *buffer,
                            struct winsys_handle *whandle,
                            unsigned stride)
{
   struct genbu_drm_buffer *buf = genbu_drm_buffer(buffer);

   if (whandle->type == WINSYS_HANDLE_TYPE_SHARED) {
      if (!buf->flinked) {
         /* if (drm_intel_bo_flink(buf->bo, &buf->flink)) */
         /*    return FALSE; */
         buf->flinked = TRUE;
      }

      whandle->handle = buf->flink;
   } else if (whandle->type == WINSYS_HANDLE_TYPE_KMS) {
      // TODO: whandle->handle = buf->bo->handle;
   } else if (whandle->type == WINSYS_HANDLE_TYPE_FD) {
      int fd;

      // if (drm_intel_bo_gem_export_to_prime(buf->bo, &fd))
      //   return FALSE;
      whandle->handle = fd;
   } else {
      assert(!"unknown usage");
      return FALSE;
   }

   whandle->stride = stride;
   return TRUE;
}

static
void genbu_drm_buffer_destroy(struct genbu_winsys *gws,
                         struct genbu_winsys_buffer *buffer)
{
   // todo: drm_intel_bo_unreference(intel_bo(buffer));

#ifdef DEBUG
   genbu_drm_buffer(buffer)->magic = 0;
   // TODO: genbu_drm_buffer(buffer)->bo = NULL;
#endif

   FREE(buffer);
}

static struct genbu_winsys_buffer *
genbu_drm_buffer_create_tiled(struct genbu_winsys *gws,
                             unsigned *stride, unsigned height, 
                             enum genbu_winsys_buffer_tile *tiling,
                             enum genbu_winsys_buffer_type type)
{
   struct genbu_drm_buffer *buf = CALLOC_STRUCT(genbu_drm_buffer);
   struct genbu_drm_winsys *gdws = genbu_drm_winsys(gws);
   unsigned long pitch = 0;
   uint32_t tiling_mode = *tiling;

   if (!buf)
      return NULL;

   buf->magic = 0xDEAD1337;
   buf->flinked = FALSE;
   buf->flink = 0;

   /* buf->bo = drm_intel_bo_alloc_tiled(gdws->gem_manager, */
   /*                                    i915_drm_type_to_name(type), */
   /*                                    *stride, height, 1, */
   /*                                    &tiling_mode, &pitch, 0); */

   /* if (!buf->bo) */
   /*    goto err; */

   *stride = pitch;
   *tiling = tiling_mode;
   return (struct genbu_winsys_buffer *)buf;

err:
   assert(0);
   FREE(buf);
   return NULL;
}

void
genbu_drm_winsys_init_buffer_functions(struct genbu_drm_winsys *gdws)
{
   gdws->base.buffer_from_handle = genbu_drm_buffer_from_handle;
   gdws->base.buffer_get_handle = genbu_drm_buffer_get_handle;
   gdws->base.buffer_destroy = genbu_drm_buffer_destroy;
   gdws->base.buffer_create_tiled = genbu_drm_buffer_create_tiled;
}
