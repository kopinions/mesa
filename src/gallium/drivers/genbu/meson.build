test_h = custom_target(
  'test.h',
  output : 'test.h',
  command : ['touch', '@OUTPUT@'],
  build_always : true,
)

inc_genbu = files(
    'genbu_public.h',
    'genbu_screen.h',
)

files_genbu = files(
  'genbu_screen.c'
)

libgenbu = static_library(
    'genbu',
    files_genbu,
    include_directories : [inc_gallium_aux, inc_gallium, inc_include, inc_src],
    c_args : [c_vis_args, c_msvc_compat_args],
)

driver_genbu = declare_dependency(
    compile_args : '-DGALLIUM_GENBU',
    link_with : libgenbu,
)
