inc_genbu = files(
    'genbu_public.h',
    'genbu_screen.h',
    'genbu_winsys.h',
    'genbu_context.h',
    'genbu_resource.h',
    'genbu_reg.h',
    'genbu_state.h',
    'genbu_surface.h'
)

files_genbu = files(
  'genbu_screen.c',
  'genbu_context.c',
  'genbu_state.c',
  'genbu_resource.c',
  'genbu_resource_buffer.c',
  'genbu_resource_tex.c',
  'genbu_surface.c'
)

libgenbu = static_library(
    'genbu',
    files_genbu,
    include_directories : [inc_gallium_aux, inc_gallium, inc_include, inc_src],
    c_args : [c_vis_args, c_msvc_compat_args],
)

driver_genbu = declare_dependency(
    compile_args : '-DGALLIUM_GENBU',
    link_with : [libgenbu, libgenbudrm],
)
