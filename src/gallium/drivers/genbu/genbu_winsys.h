#ifndef GENBU_WINSYS_H
#define GENBU_WINSYS_H

#include "pipe/p_compiler.h"

struct genbu_winsys;
struct genbu_winsys_buffer;
struct winsys_handle;

enum genbu_winsys_buffer_type
{
   GENBU_NEW_TEXTURE,
   GENBU_NEW_SCANOUT, /**< a texture used for scanning out from */
   GENBU_NEW_VERTEX
};

enum genbu_winsys_buffer_tile
{
   GENBU_TILE_NONE,
   GENBU_TILE_X,
   GENBU_TILE_Y
};


struct genbu_winsys {

   unsigned pci_id; /**< PCI ID for the device */
 
   /**
    * Destroy the winsys.
    */
   void (*destroy)(struct genbu_winsys *gws);

   int (*aperture_size)(struct genbu_winsys *gws);

   /**
    * Creates a buffer from a handle.
    * Used to implement pipe_screen::resource_from_handle.
    * Also provides the stride information needed for the
    * texture via the stride argument.
    */
   struct genbu_winsys_buffer *
      (*buffer_from_handle)(struct genbu_winsys *gws,
                            struct winsys_handle *whandle,
                            unsigned height,
                            enum genbu_winsys_buffer_tile *tiling,
                            unsigned *stride);

   /**
    * Create a tiled buffer.
    *
    * *stride, height are in bytes. The winsys tries to allocate the buffer with
    * the tiling mode provide in *tiling. If tiling is no possible, *tiling will
    * be set to GENBU_TILE_NONE. The calculated stride (incorporateing hw/kernel
    * requirements) is always returned in *stride.
    */
   struct genbu_winsys_buffer *
      (*buffer_create_tiled)(struct genbu_winsys *iws,
                             unsigned *stride, unsigned height,
                             enum genbu_winsys_buffer_tile *tiling,
                             enum genbu_winsys_buffer_type type);



   /**
    * Used to implement pipe_screen::resource_get_handle.
    * The winsys might need the stride information.
    */
   boolean (*buffer_get_handle)(struct genbu_winsys *gws,
                                struct genbu_winsys_buffer *buffer,
                                struct winsys_handle *whandle,
                                unsigned stride);

   void (*buffer_destroy)(struct genbu_winsys *gws,
                          struct genbu_winsys_buffer *buffer);

};

#endif//GENBU_WINSYS_H
