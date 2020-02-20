libgenbudrm = static_library(
  'genbudrm',
  files(
    'genbu_drm_winsys.c'
  ),
  include_directories : [
    inc_include, inc_src, inc_gallium, inc_gallium_aux, inc_gallium_drivers
  ],
  dependencies : [dep_libdrm, dep_libdrm_genbu],
)
