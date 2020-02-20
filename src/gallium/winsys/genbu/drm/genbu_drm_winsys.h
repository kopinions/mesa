#ifndef GENBU_DRM_WINSYS_H
#define GENBU_DRM_WINSYS_H

#include "drm-uapi/drm.h"
#include "genbu/genbu_winsys.h"

struct genbu_drm_winsys
{
   struct genbu_winsys base;

   boolean dump_cmd;
   const char *dump_raw_file;
   boolean send_cmd;

   int fd; /**< Drm file discriptor */

   size_t max_batch_size;
};
  
static inline struct genbu_drm_winsys *
genbu_drm_winsys(struct genbu_winsys *gws)
{
   return (struct genbu_drm_winsys *)gws;
}

#endif//GENBU_DRM_WINSYS_H
