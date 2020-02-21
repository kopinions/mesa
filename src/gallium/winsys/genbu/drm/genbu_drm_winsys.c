#include <stdio.h>
#include <sys/ioctl.h>

#include "drm-uapi/genbu_drm.h"

#include "state_tracker/drm_driver.h"

#include "genbu_drm_winsys.h"
#include "genbu_drm_public.h"
#include "util/u_memory.h"

static int
genbu_drm_aperture_size(struct genbu_winsys *gws)
{
   struct genbu_drm_winsys *gdws = genbu_drm_winsys(gws);
   size_t aper_size, mappable_size;

   // TODO: drm_intel_get_aperture_sizes(gdws->fd, &mappable_size, &aper_size);

   return aper_size >> 20;
}

    
struct genbu_winsys *
genbu_drm_winsys_create(int drmFD)
{
   struct genbu_drm_winsys *gdws;
   unsigned int deviceID;

   gdws = CALLOC_STRUCT(genbu_drm_winsys);
   if (!gdws) {
      return NULL;
   }

   gdws->base.aperture_size = genbu_drm_aperture_size;

   return &gdws->base;
}
