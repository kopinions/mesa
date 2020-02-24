#ifndef GENBU_REG_H
#define GENBU_REG_H

#define CMD_3D (0x3<<29)

#define GENBU_MAX_TEX_INDIRECT 4
#define GENBU_MAX_TEX_INSN     32
#define GENBU_MAX_ALU_INSN     64
#define GENBU_MAX_DECL_INSN    27
#define GENBU_MAX_TEMPORARY    16

#define DST_BLND_FACT(f) ((f)<<S6_CBUF_DST_BLEND_FACT_SHIFT)
#define SRC_BLND_FACT(f) ((f)<<S6_CBUF_SRC_BLEND_FACT_SHIFT)
#define DST_ABLND_FACT(f) ((f)<<IAB_DST_FACTOR_SHIFT)
#define SRC_ABLND_FACT(f) ((f)<<IAB_SRC_FACTOR_SHIFT)


/* _3DSTATE_INDEPENDENT_ALPHA_BLEND, p177 */
#define _3DSTATE_INDEPENDENT_ALPHA_BLEND_CMD	(CMD_3D|(0x0b<<24))
#define IAB_MODIFY_ENABLE	        (1<<23)
#define IAB_ENABLE       	        (1<<22)
#define IAB_MODIFY_FUNC         	(1<<21)
#define IAB_FUNC_SHIFT          	16
#define IAB_MODIFY_SRC_FACTOR   	(1<<11)
#define IAB_SRC_FACTOR_SHIFT		6
#define IAB_SRC_FACTOR_MASK		(BLENDFACT_MASK<<6)
#define IAB_MODIFY_DST_FACTOR	        (1<<5)
#define IAB_DST_FACTOR_SHIFT		0
#define IAB_DST_FACTOR_MASK		(BLENDFACT_MASK<<0)

#define BLENDFUNC_ADD			0x0
#define BLENDFUNC_SUBTRACT		0x1
#define BLENDFUNC_REVERSE_SUBTRACT	0x2
#define BLENDFUNC_MIN			0x3
#define BLENDFUNC_MAX			0x4
#define BLENDFUNC_MASK			0x7

#define LOGICOP_CLEAR			0
#define LOGICOP_NOR			0x1
#define LOGICOP_AND_INV 		0x2
#define LOGICOP_COPY_INV		0x3
#define LOGICOP_AND_RVRSE		0x4
#define LOGICOP_INV			0x5
#define LOGICOP_XOR			0x6
#define LOGICOP_NAND			0x7
#define LOGICOP_AND			0x8
#define LOGICOP_EQUIV			0x9
#define LOGICOP_NOOP			0xa
#define LOGICOP_OR_INV			0xb
#define LOGICOP_COPY			0xc
#define LOGICOP_OR_RVRSE		0xd
#define LOGICOP_OR			0xe
#define LOGICOP_SET			0xf

#define BLENDFACT_ZERO			0x01
#define BLENDFACT_ONE			0x02
#define BLENDFACT_SRC_COLR		0x03
#define BLENDFACT_INV_SRC_COLR 		0x04
#define BLENDFACT_SRC_ALPHA		0x05
#define BLENDFACT_INV_SRC_ALPHA 	0x06
#define BLENDFACT_DST_ALPHA		0x07
#define BLENDFACT_INV_DST_ALPHA 	0x08
#define BLENDFACT_DST_COLR		0x09
#define BLENDFACT_INV_DST_COLR		0x0a
#define BLENDFACT_SRC_ALPHA_SATURATE	0x0b
#define BLENDFACT_CONST_COLOR		0x0c
#define BLENDFACT_INV_CONST_COLOR	0x0d
#define BLENDFACT_CONST_ALPHA		0x0e
#define BLENDFACT_INV_CONST_ALPHA	0x0f
#define BLENDFACT_MASK          	0x0f


#define _3DSTATE_MODES_4_CMD		(CMD_3D|(0x0d<<24))
#define ENABLE_LOGIC_OP_FUNC		(1<<23)
#define LOGIC_OP_FUNC(x)		((x)<<18)
#define LOGICOP_MASK			(0xf<<18)
#define MODE4_ENABLE_STENCIL_TEST_MASK	((1<<17)|(0xff00))
#define ENABLE_STENCIL_TEST_MASK	(1<<17)
#define STENCIL_TEST_MASK(x)		(((x)&0xff)<<8)
#define MODE4_ENABLE_STENCIL_WRITE_MASK	((1<<16)|(0x00ff))
#define ENABLE_STENCIL_WRITE_MASK	(1<<16)
#define STENCIL_WRITE_MASK(x)		((x)&0xff)

#define S5_WRITEDISABLE_ALPHA          (1<<31)
#define S5_WRITEDISABLE_RED            (1<<30)
#define S5_WRITEDISABLE_GREEN          (1<<29)
#define S5_WRITEDISABLE_BLUE           (1<<28)
#define S5_WRITEDISABLE_MASK           (0xf<<28)
#define S5_FORCE_DEFAULT_POINT_SIZE    (1<<27)
#define S5_LAST_PIXEL_ENABLE           (1<<26)
#define S5_GLOBAL_DEPTH_OFFSET_ENABLE  (1<<25)
#define S5_FOG_ENABLE                  (1<<24)
#define S5_STENCIL_REF_SHIFT           16
#define S5_STENCIL_REF_MASK            (0xff<<16)
#define S5_STENCIL_TEST_FUNC_SHIFT     13
#define S5_STENCIL_TEST_FUNC_MASK      (0x7<<13)
#define S5_STENCIL_FAIL_SHIFT          10
#define S5_STENCIL_FAIL_MASK           (0x7<<10)
#define S5_STENCIL_PASS_Z_FAIL_SHIFT   7
#define S5_STENCIL_PASS_Z_FAIL_MASK    (0x7<<7)
#define S5_STENCIL_PASS_Z_PASS_SHIFT   4
#define S5_STENCIL_PASS_Z_PASS_MASK    (0x7<<4)
#define S5_STENCIL_WRITE_ENABLE        (1<<3)
#define S5_STENCIL_TEST_ENABLE         (1<<2)
#define S5_COLOR_DITHER_ENABLE         (1<<1)
#define S5_LOGICOP_ENABLE              (1<<0)


#define S6_ALPHA_TEST_ENABLE           (1<<31)
#define S6_ALPHA_TEST_FUNC_SHIFT       28
#define S6_ALPHA_TEST_FUNC_MASK        (0x7<<28)
#define S6_ALPHA_REF_SHIFT             20
#define S6_ALPHA_REF_MASK              (0xff<<20)
#define S6_DEPTH_TEST_ENABLE           (1<<19)
#define S6_DEPTH_TEST_FUNC_SHIFT       16
#define S6_DEPTH_TEST_FUNC_MASK        (0x7<<16)
#define S6_CBUF_BLEND_ENABLE           (1<<15)
#define S6_CBUF_BLEND_FUNC_SHIFT       12
#define S6_CBUF_BLEND_FUNC_MASK        (0x7<<12)
#define S6_CBUF_SRC_BLEND_FACT_SHIFT   8
#define S6_CBUF_SRC_BLEND_FACT_MASK    (0xf<<8)
#define S6_CBUF_DST_BLEND_FACT_SHIFT   4
#define S6_CBUF_DST_BLEND_FACT_MASK    (0xf<<4)
#define S6_DEPTH_WRITE_ENABLE          (1<<3)
#define S6_COLOR_WRITE_ENABLE          (1<<2)
#define S6_TRISTRIP_PV_SHIFT           0
#define S6_TRISTRIP_PV_MASK            (0x3<<0)

#define S7_DEPTH_OFFSET_CONST_MASK     ~0

#endif
