#ifndef GENBU_PUBLIC_H
#define GENBU_PUBLIC_H

#ifdef __cplusplus
extern "C" {
#endif

struct pipe_screen;
struct genbu_winsys;

struct pipe_screen *
genbu_create_screen(struct genbu_winsys *winsys);

#ifdef __cplusplus
}
#endif

#endif
