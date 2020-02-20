
#ifndef GENBU_DRM_PUBLIC_H
#define GENBU_DRM_PUBLIC_H

struct genbu_winsys;

struct genbu_winsys * genbu_drm_winsys_create(int drmFD);

#endif
