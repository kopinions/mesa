#ifndef GENBU_RESOURCE_H
#define GENBU_RESOURCE_H

#include "util/u_transfer.h"
#include "util/u_debug.h"
#include "genbu_winsys.h"   
   
struct genbu_transfer {
   /* Base class. */
   struct pipe_transfer b;
   struct pipe_resource *staging_texture;
};

#define GENBU_MAX_TEXTURE_2D_LEVELS 12  /* max 2048x2048 */
#define GENBU_MAX_TEXTURE_3D_LEVELS  9  /* max 256x256x256 */

#endif//GENBU_RESOURCE_H
