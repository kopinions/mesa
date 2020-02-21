#ifndef GENBU_WINSYS_H
#define GENBU_WINSYS_H

struct genbu_winsys {

   unsigned pci_id; /**< PCI ID for the device */
 
   /**
    * Destroy the winsys.
    */
   void (*destroy)(struct genbu_winsys *gws);

   int (*aperture_size)(struct genbu_winsys *iws);
};

#endif//GENBU_WINSYS_H
