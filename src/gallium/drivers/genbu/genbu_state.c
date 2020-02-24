#include "genbu_reg.h"
#include "genbu_state.h"
#include "genbu_context.h"

static inline unsigned
genbu_translate_blend_func(unsigned mode)
{
   switch (mode) {
   case PIPE_BLEND_ADD:
      return BLENDFUNC_ADD;
   case PIPE_BLEND_MIN:
      return BLENDFUNC_MIN;
   case PIPE_BLEND_MAX:
      return BLENDFUNC_MAX;
   case PIPE_BLEND_SUBTRACT:
      return BLENDFUNC_SUBTRACT;
   case PIPE_BLEND_REVERSE_SUBTRACT:
      return BLENDFUNC_REVERSE_SUBTRACT;
   default:
      return 0;
   }
}

static inline unsigned
genbu_translate_blend_factor(unsigned factor)
{
   switch (factor) {
   case PIPE_BLENDFACTOR_ZERO:
      return BLENDFACT_ZERO;
   case PIPE_BLENDFACTOR_SRC_ALPHA:
      return BLENDFACT_SRC_ALPHA;
   case PIPE_BLENDFACTOR_ONE:
      return BLENDFACT_ONE;
   case PIPE_BLENDFACTOR_SRC_COLOR:
      return BLENDFACT_SRC_COLR;
   case PIPE_BLENDFACTOR_INV_SRC_COLOR:
      return BLENDFACT_INV_SRC_COLR;
   case PIPE_BLENDFACTOR_DST_COLOR:
      return BLENDFACT_DST_COLR;
   case PIPE_BLENDFACTOR_INV_DST_COLOR:
      return BLENDFACT_INV_DST_COLR;
   case PIPE_BLENDFACTOR_INV_SRC_ALPHA:
      return BLENDFACT_INV_SRC_ALPHA;
   case PIPE_BLENDFACTOR_DST_ALPHA:
      return BLENDFACT_DST_ALPHA;
   case PIPE_BLENDFACTOR_INV_DST_ALPHA:
      return BLENDFACT_INV_DST_ALPHA;
   case PIPE_BLENDFACTOR_SRC_ALPHA_SATURATE:
      return BLENDFACT_SRC_ALPHA_SATURATE;
   case PIPE_BLENDFACTOR_CONST_COLOR:
      return BLENDFACT_CONST_COLOR;
   case PIPE_BLENDFACTOR_INV_CONST_COLOR:
      return BLENDFACT_INV_CONST_COLOR;
   case PIPE_BLENDFACTOR_CONST_ALPHA:
      return BLENDFACT_CONST_ALPHA;
   case PIPE_BLENDFACTOR_INV_CONST_ALPHA:
      return BLENDFACT_INV_CONST_ALPHA;
   default:
      return BLENDFACT_ZERO;
   }
}


static inline unsigned
genbu_translate_logic_op(unsigned opcode)
{
   switch (opcode) {
   case PIPE_LOGICOP_CLEAR:
      return LOGICOP_CLEAR;
   case PIPE_LOGICOP_AND:
      return LOGICOP_AND;
   case PIPE_LOGICOP_AND_REVERSE:
      return LOGICOP_AND_RVRSE;
   case PIPE_LOGICOP_COPY:
      return LOGICOP_COPY;
   case PIPE_LOGICOP_COPY_INVERTED:
      return LOGICOP_COPY_INV;
   case PIPE_LOGICOP_AND_INVERTED:
      return LOGICOP_AND_INV;
   case PIPE_LOGICOP_NOOP:
      return LOGICOP_NOOP;
   case PIPE_LOGICOP_XOR:
      return LOGICOP_XOR;
   case PIPE_LOGICOP_OR:
      return LOGICOP_OR;
   case PIPE_LOGICOP_OR_INVERTED:
      return LOGICOP_OR_INV;
   case PIPE_LOGICOP_NOR:
      return LOGICOP_NOR;
   case PIPE_LOGICOP_EQUIV:
      return LOGICOP_EQUIV;
   case PIPE_LOGICOP_INVERT:
      return LOGICOP_INV;
   case PIPE_LOGICOP_OR_REVERSE:
      return LOGICOP_OR_RVRSE;
   case PIPE_LOGICOP_NAND:
      return LOGICOP_NAND;
   case PIPE_LOGICOP_SET:
      return LOGICOP_SET;
   default:
      return LOGICOP_SET;
   }
}


static void *
genbu_create_blend_state(struct pipe_context *pipe,
                        const struct pipe_blend_state *blend)
{
   struct genbu_blend_state *cso_data = CALLOC_STRUCT( genbu_blend_state );

   {
      unsigned eqRGB  = blend->rt[0].rgb_func;
      unsigned srcRGB = blend->rt[0].rgb_src_factor;
      unsigned dstRGB = blend->rt[0].rgb_dst_factor;

      unsigned eqA    = blend->rt[0].alpha_func;
      unsigned srcA   = blend->rt[0].alpha_src_factor;
      unsigned dstA   = blend->rt[0].alpha_dst_factor;

      /* Special handling for MIN/MAX filter modes handled at
       * state_tracker level.
       */

      if (srcA != srcRGB ||
          dstA != dstRGB ||
          eqA != eqRGB) {

         cso_data->iab = (_3DSTATE_INDEPENDENT_ALPHA_BLEND_CMD |
                          IAB_MODIFY_ENABLE |
                          IAB_ENABLE |
                          IAB_MODIFY_FUNC |
                          IAB_MODIFY_SRC_FACTOR |
                          IAB_MODIFY_DST_FACTOR |
                          SRC_ABLND_FACT(genbu_translate_blend_factor(srcA)) |
                          DST_ABLND_FACT(genbu_translate_blend_factor(dstA)) |
                          (genbu_translate_blend_func(eqA) << IAB_FUNC_SHIFT));
      }
      else {
         cso_data->iab = (_3DSTATE_INDEPENDENT_ALPHA_BLEND_CMD |
                          IAB_MODIFY_ENABLE |
                          0);
      }
   }

   cso_data->modes4 |= (_3DSTATE_MODES_4_CMD |
                        ENABLE_LOGIC_OP_FUNC |
                        LOGIC_OP_FUNC(genbu_translate_logic_op(blend->logicop_func)));

   if (blend->logicop_enable)
      cso_data->LIS5 |= S5_LOGICOP_ENABLE;

   if (blend->dither)
      cso_data->LIS5 |= S5_COLOR_DITHER_ENABLE;

   /* We potentially do some fixup at emission for non-BGRA targets */
   if ((blend->rt[0].colormask & PIPE_MASK_R) == 0)
      cso_data->LIS5 |= S5_WRITEDISABLE_RED;

   if ((blend->rt[0].colormask & PIPE_MASK_G) == 0)
      cso_data->LIS5 |= S5_WRITEDISABLE_GREEN;

   if ((blend->rt[0].colormask & PIPE_MASK_B) == 0)
      cso_data->LIS5 |= S5_WRITEDISABLE_BLUE;

   if ((blend->rt[0].colormask & PIPE_MASK_A) == 0)
      cso_data->LIS5 |= S5_WRITEDISABLE_ALPHA;

   if (blend->rt[0].blend_enable) {
      unsigned funcRGB = blend->rt[0].rgb_func;
      unsigned srcRGB  = blend->rt[0].rgb_src_factor;
      unsigned dstRGB  = blend->rt[0].rgb_dst_factor;

      cso_data->LIS6 |= (S6_CBUF_BLEND_ENABLE |
                         SRC_BLND_FACT(genbu_translate_blend_factor(srcRGB)) |
                         DST_BLND_FACT(genbu_translate_blend_factor(dstRGB)) |
                         (genbu_translate_blend_func(funcRGB) << S6_CBUF_BLEND_FUNC_SHIFT));
   }

   return cso_data;
}

static void *
genbu_create_depth_stencil_state(struct pipe_context *pipe,
                                 const struct pipe_depth_stencil_alpha_state *depth_stencil) {
   struct genbu_depth_stencil_state *cso_data = CALLOC_STRUCT(genbu_depth_stencil_state);
   return cso_data;
}

static void *
genbu_create_sampler_state(struct pipe_context *pipe,
                                 const struct pipe_sampler_state *sampler) {
   struct genbu_sampler_state *cso_data = CALLOC_STRUCT(genbu_sampler_state);
   return cso_data;
}

static void *
genbu_create_rasterizer_state(struct pipe_context *pipe,
                                 const struct pipe_rasterizer_state *rasterizer) {
   struct genbu_rasterizer_state *cso_data = CALLOC_STRUCT(genbu_rasterizer_state);
   return cso_data;
}

void
genbu_init_state_functions( struct genbu_context *gc )
{
   gc->base.create_blend_state = genbu_create_blend_state;
   gc->base.create_depth_stencil_alpha_state = genbu_create_depth_stencil_state;
   gc->base.create_sampler_state = genbu_create_sampler_state;
   gc->base.create_rasterizer_state = genbu_create_rasterizer_state;
}
