#ifndef GENBU_WINSYS_H
#define GENBU_WINSYS_H

struct genbu_winsys {

   unsigned pci_id; /**< PCI ID for the device */
 
   /**
    * Destroy the winsys.
    */
   void (*destroy)(struct genbu_winsys *gws);
};

#endif//GENBU_WINSYS_H
