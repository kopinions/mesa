#include <stdio.h>
#include <sys/ioctl.h>

#include "drm-uapi/genbu_drm.h"

#include "state_tracker/drm_driver.h"

#include "genbu_drm_winsys.h"
#include "genbu_drm_public.h"
#include "util/u_memory.h"

struct genbu_winsys *
genbu_drm_winsys_create(int drmFD)
{
   struct genbu_drm_winsys *gdws;
   unsigned int deviceID;

   gdws = CALLOC_STRUCT(genbu_drm_winsys);
   if (!gdws) {
      return NULL;
   }

   return &gdws->base;
}
