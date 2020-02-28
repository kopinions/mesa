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

struct genbu_drm_buffer {
   unsigned magic;

   // todo: drm_intel_bo *bo;

   void *ptr;
   unsigned map_count;

   boolean flinked;
   unsigned flink;
};

static inline struct genbu_drm_buffer *
genbu_drm_buffer(struct genbu_winsys_buffer *buffer)
{
   return (struct genbu_drm_buffer *)buffer;
}


void genbu_drm_winsys_init_buffer_functions(struct genbu_drm_winsys *gdws);

    
static inline struct genbu_drm_winsys *
genbu_drm_winsys(struct genbu_winsys *gws)
{
   return (struct genbu_drm_winsys *)gws;
}

#endif//GENBU_DRM_WINSYS_H
