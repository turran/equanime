#ifndef _MP2520F_REGS_H
#define _MP2520F_REGS_H

#include <stdint.h>

/* to keep compatibility between this file and the kernel header */
#define __REGW
#define MLC_START


/*============================================================================*
 *                        Multi Layer Controller (MLC)                        *
 *============================================================================*/
/* Overlay Control Register */
#define MLC_OVLAY_CNTR 				__REGW(MLC_START + 0x0)
//[15:14] R/W Reserved Must be 0 2’b0
#define MLC_OVLAY_CNTR_DISP_FLD_POL		(1 << 13) 	/* When Field Mode, Select Polarity of Field Signal from Display Sync. Generation */
#define MLC_OVLAY_CNTR_DISP_GAMM_BYPATH 	(1 << 12) 	/* Select Bypath of RGB Gamma Table */
#define MLC_OVLAY_CNTR_DISP_SWAP 		(1 << 11) 	/* Select Priority of OSD and Sub-Picture */
//[10] R/W Reserved Must be 0 0
#define MLC_OVLAY_CNTR_DISP_CURSOR 		(1 << 9) 	/* Cursor Enable / Disable */
#define MLC_OVLAY_CNTR_DISP_SUBPICTURE 		(1 << 8) 	/* Sub-Picture Enable/ Disable */
#define MLC_OVLAY_CNTR_DISP_OSD 		(1 << 7) 	/* OSD Enable/ Disable */
#define MLC_OVLAY_CNTR_DISP_SDL5EN 		(1 << 6) 	/* Region 5 Enable/ Disable */
#define MLC_OVLAY_CNTR_DISP_SDL4EN 		(1 << 5) 	/* Region 4 Enable/ Disable */
#define MLC_OVLAY_CNTR_DISP_SDL3EN 		(1 << 4) 	/* Region 3 Enable/ Disable */
#define MLC_OVLAY_CNTR_DISP_SDL2EN 		(1 << 3) 	/* Region 2 Enable/ Disable */
#define MLC_OVLAY_CNTR_DISP_SDL1EN 		(1 << 2) 	/* Region 1 Enable/ Disable */
#define MLC_OVLAY_CNTR_DISP_VLBON 		(1 << 1) 	/* Video Plane B Enable/Disable at Video(Y/Cb/Cr) Layer */
#define MLC_OVLAY_CNTR_DISP_VLAON 		(1 << 1) 	/* Video Plane A Enable/Disable at Video(Y/Cb/Cr) Layer */

/* Effect of Video Image */
#define MLC_YUV_EFECT 				__REGW(MLC_START + 0x2)
/* [15:10] Reserved */
#define MLC_YUV_EFECT_VLB_BT 			(1 << 9)
#define MLC_YUV_EFECT_VLB_TP_MR_H 		(1 << 8)
#define MLC_YUV_EFECT_VLB_TP_MR_V 		(1 << 7)
#define MLC_YUV_EFECT_VLB_BT_MR_H 		(1 << 6)
#define MLC_YUV_EFECT_VLB_BT_MR_V 		(1 << 5)
#define MLC_YUV_EFECT_VLA_BT 			(1 << 4)
#define MLC_YUV_EFECT_VLA_TP_MR_H 		(1 << 3)
#define MLC_YUV_EFECT_VLA_TP_MR_V 		(1 << 2)
#define MLC_YUV_EFECT_VLA_BT_MR_H 		(1 << 1)
#define MLC_YUV_EFECT_VLA_BT_MR_V 		(1 << 0)

/* Video Image Control Register */
#define MLC_YUV_CNTL 				__REGW(MLC_START + 0x4)
/* [15:3] Reserved */
#define MLC_YUV_CNTL_SC2DP			(1 << 12) /* Input data path for Video Region A */
#define MLC_YUV_CNTL_SC2DP_EXTMEM		(0) /* External Memory */
#define MLC_YUV_CNTL_SC2DP_SCALE		(1) /* Scaler */
#define MLC_YUV_CNTL_FDC2DP			(1 << 11) /* Input data path for Video Region A */
#define MLC_YUV_CNTL_FDC2DP_EXTMEM		(0) /* External Memory */
#define MLC_YUV_CNTL_FDC2DP_CONV		(1) /* Frame Dimensin Converter */
#define MLC_YUV_CNTL_PRIO			(1 << 10) /* Region Priority */
#define MLC_YUV_CNTL_PRIO_REGIONA		(0)
#define MLC_YUV_CNTL_PRIO_REGIONB		(0)
/* [9] Reserved */
#define MLC_YUV_CNTL_VLB_TP_STEP 		(3 << 7)
#define MLC_YUV_CNTL_VLB_BT_STEP 		(3 << 5)
#define MLC_YUV_CNTL_VLA_TP_STEP 		(3 << 2)
#define MLC_YUV_CNTL_VLA_BT_STEP 		(3 << 0)
#define MLC_YUV_CNTL_NOSKIP			(0)
#define MLC_YUV_CNTL_SKIP1			(1)
#define MLC_YUV_CNTL_SKIP2			(2)
#define MLC_YUV_CNTL_SKIP3			(3)

/* Scale Factor Register of Region A */
#define MLC_YUVA_TP_HSC 			__REGW(MLC_START + 0x6)

/* Scale Factor Register of Region A */
#define  MLC_YUVA_BT_HSC			__REGW(MLC_START + 0x8)

/* Scale Factor Register of Region A */
#define MLC_YUVA_TP_VSCL 			__REGW(MLC_START + 0xa)
#define MLC_YUVA_TP_VSCH 			__REGW(MLC_START + 0xc)

/* Scale Factor Register of Region A */
#define MLC_YUVA_BT_VSCL 			__REGW(MLC_START + 0xe)
#define MLC_YUVA_BT_VSCH 			__REGW(MLC_START + 0x10)

/* Horizontal Pixel Width Register of Region A */
#define MLC_YUVA_TP_PXW 			__REGW(MLC_START + 0x12)

/* Horizontal Pixel Width Register of Region A */
#define MLC_YUVA_BT_PXW 			__REGW(MLC_START + 0x14)

/* Coordinate Register of Region A */
#define MLC_YUVA_STX				__REGW(MLC_START + 0x16)

/* Coordinate Register of Region A */
#define MLC_YUVA_ENDX 				__REGW(MLC_START + 0x18)

/* Coordinate Register of Region A */
#define MLC_YUVA_TP_STY				__REGW(MLC_START + 0x1a)

/* Coordinate Register of Region A */
#define MLC_YUVA_TP_ENDY			__REGW(MLC_START + 0x1c)

/* Coordinate Register of Region A */
#define MLC_YUVA_BT_ENDY 			__REGW(MLC_START + 0x1e)

/* Source Address of Region A */
#define MLC_YUVA_TP_OADRL			__REGW(MLC_START + 0x20)
#define MLC_YUVA_TP_OADRH 			__REGW(MLC_START + 0x22)

/* Source Address of Region A */
#define MLC_YUVA_TP_EADRL			__REGW(MLC_START + 0x24)
#define MLC_YUVA_TP_EADRH 			__REGW(MLC_START + 0x26)

struct mp2520f_mlc_reg
{
	union
	{
		uint16_t reg;
		struct {
			uint8_t vlaon : 1;
			uint8_t vlbon : 1;
			uint8_t sdl1en : 1;
			uint8_t sdl2en : 1;
			uint8_t sdl3en : 1;
			uint8_t sdl4en : 1;
			uint8_t sdl5en : 1;
			uint8_t osd : 1;
			uint8_t subpicture : 1;
			uint8_t cursor : 1;
			uint8_t rsvd1 : 1;
			uint8_t swap : 1;
			uint8_t gamm_bypath : 1;
			uint8_t fld_pol : 1;
			uint8_t rsvd0 : 2;
		} bits;
	} ovlay_cntr;
	union
	{
		uint16_t reg;
		struct {
			uint8_t vla_bt_mr_v : 1;
			uint8_t vla_bt_mr_h : 1;
			uint8_t vla_tp_mr_v : 1;
			uint8_t vla_tp_mr_h : 1;
			uint8_t vla_bt : 1;
			uint8_t vlb_bt_mr_v : 1;
			uint8_t vlb_bt_mr_h : 1;
			uint8_t vlb_tp_mr_v : 1;
			uint8_t vlb_tp_mr_h : 1;
			uint8_t vlb_bt : 1;
			uint8_t rsvd0 : 6;
		} bits;
	} yuf_efect;
	union
	{
		uint16_t reg;
		struct {
			uint8_t vla_bt_step : 2;
			uint8_t vla_tp_step : 2;
			uint8_t rsvd0 : 1;
			uint8_t vlb_bt_step : 2;
			uint8_t vlb_tp_step : 2;
			uint8_t rsvd1 : 1;
			uint8_t prio : 1;
			uint8_t fdc2dp : 1;
			uint8_t sc2dp : 1;
			uint8_t rsv2 : 3;
		} bits;
	} yuf_cntl;
	uint16_t yuva_tp_hsc;
	uint16_t yuva_bt_hsc;

	uint16_t yuva_tp_vscl;
	uint16_t yuva_tp_vsch;

	uint16_t yuva_bt_vscl;
	uint16_t yuva_bt_vsch;

	uint16_t yuva_tp_pxw;
	uint16_t yuva_bt_pxw;

	uint16_t yuva_stx;
	uint16_t yuva_endx;

	uint16_t yuva_tp_sty;
	uint16_t yuva_tp_endy;
	uint16_t yuva_bt_endy;

	uint16_t yuva_tp_oadrl;
	uint16_t yuva_tp_oadrh;

	uint16_t yuva_tp_eadrl;
	uint16_t yuva_tp_eadrh;

	uint16_t yuva_bt_oadrl;
	uint16_t yuva_bt_oadrh;
	/* TODO add the rest */
};

/* Source Address of Region A */
#define MLC_YUVA_BT_OADRL			__REGW(MLC_START + 0x28)
#define MLC_YUVA_BT_OADRH 			__REGW(MLC_START + 0x2a)

/* Source Address of Region A */
#define MLC_YUVA_BT_EADRL			__REGW(MLC_START + 0x2c)
#define MLC_YUVA_BT_EADRH 			__REGW(MLC_START + 0x2e)

/* Scale Factor Register of Region B */
#define MLC_YUVB_TP_HSC 			__REGW(MLC_START + 0x30) 

/* Scale Factor Register of Region B */
#define MLC_YUVB_BT_HSC 			__REGW(MLC_START + 0x32)

/* Scale Factor Register of Region B */
#define MLC_YUVB_TP_VSCL 			__REGW(MLC_START + 0x34)
#define MLC_YUVB_TP_VSCH 			__REGW(MLC_START + 0x36)

/* Scale Factor Register of Region B */
#define MLC_YUVB_BT_VSCL 			__REGW(MLC_START + 0x38)
#define MLC_YUVB_BT_VSCH 			__REGW(MLC_START + 0x3a)

/* Horizontal Pixel Width Register of Region B */
#define MLC_YUVB_TP_PXW 			__REGW(MLC_START + 0x3c)

/* Horizontal Pixel Width Register of Region B */
#define MLC_YUVB_BT_PXW 			__REGW(MLC_START + 0x3e)

/* Coordinate Register of Region B */
#define MLC_YUVB_STX				__REGW(MLC_START + 0x40)

/* Coordinate Register of Region B */
#define MLC_YUVB_ENDX 				__REGW(MLC_START + 0x42)

/* Coordinate Register of Region B */
#define MLC_YUVB_TP_STY				__REGW(MLC_START + 0x44)

/* Coordinate Register of Region B */
#define MLC_YUVB_TP_ENDY			__REGW(MLC_START + 0x46)

/* Coordinate Register of Region B */
#define MLC_YUVB_BT_ENDY 			__REGW(MLC_START + 0x48)

/* Source Address of Region B */
#define MLC_YUVB_TP_OADRL			__REGW(MLC_START + 0x4a)
#define MLC_YUVB_TP_OADRH 			__REGW(MLC_START + 0x4c)

/* Source Address of Region B (MLC_YUVB_TP_EADRL, MLC_YUVB_TP_EADRH) */
#define MLC_YUVB_TP_EADRL			__REGW(MLC_START + 0x4e)
#define MLC_YUVB_TP_EADRH 			__REGW(MLC_START + 0x50)

/* Source Address of Region B (MLC_YUVB_BT_OADRL, MLC_YUVB_BT_OADRH) */
#define MLC_YUVB_BT_OADRL			__REGW(MLC_START + 0x52)
#define MLC_YUVB_BT_OADRH 			__REGW(MLC_START + 0x54)

/* Source Address of Region B (MLC_YUVB_BT_EADRL, MLC_YUVB_BT_EADRH) */
#define MLC_YUVB_BT_EADRL			__REGW(MLC_START + 0x56)
#define MLC_YUVB_BT_EADRH 			__REGW(MLC_START + 0x58)

/* Still Image Control Register */
#define MLC_STL_CNTL 				__REGW(MLC_START + 0x5a)
#define MLC_STL_CNTL_BPP 			(3 << 9)
#define MLC_STL_CNTL_BPP_4 			(0)
#define MLC_STL_CNTL_BPP_8 			(1)
#define MLC_STL_CNTL_BPP_16 			(2)
#define MLC_STL_CNTL_BPP_24 			(3)
#define MLC_STL_CNTL_5ACT 			(1 << 8)
#define MLC_STL_CNTL_4ACT 			(1 << 6)
#define MLC_STL_CNTL_3ACT 			(1 << 4)
#define MLC_STL_CNTL_2ACT 			(1 << 2)
#define MLC_STL_CNTL_1ACT 			(1 << 0)

/* Mix/ Mux Control Register */
#define MLC_STL_MIXMUX 				__REGW(MLC_START + 0x5c)
#define MLC_STL_MIXMUX_5			(3 << 8)
#define MLC_STL_MIXMUX_4			(3 << 6)
#define MLC_STL_MIXMUX_3			(3 << 4)
#define MLC_STL_MIXMUX_2			(3 << 2)
#define MLC_STL_MIXMUX_1			(3 << 0)
#define MLC_STL_MIXMUX_STL			(0)
#define MLC_STL_MIXMUX_CKEY			(1)
#define MLC_STL_MIXMUX_ABLEND			(2)

/* Alpha Blending Value Register */
#define MLC_STL_ALPHAL 				__REGW(MLC_START + 0x5e)
#define MLC_STL_ALPHAH_3 			(15 << 8)
#define MLC_STL_ALPHAH_2 			(15 << 4)
#define MLC_STL_ALPHAH_1 			(15 << 0)
/* Alpha Blending Value Register */
#define MLC_STL_ALPHAH 				__REGW(MLC_START + 0x60)
#define MLC_STL_ALPHAH_5 			(15 << 4)
#define MLC_STL_ALPHAH_4 			(15 << 0)

/* Coordinate Register of Still Image(RGB) Layer 1 */
#define MLC_STL1_STX 				__REGW(MLC_START + 0x62)
#define MLC_STL1_ENDX 				__REGW(MLC_START + 0x64)
#define MLC_STL1_STY 				__REGW(MLC_START + 0x66)
#define MLC_STL1_ENDY 				__REGW(MLC_START + 0x68)
/* Coordinate Register of Still Image(RGB) Layer 2 */
#define MLC_STL2_STX 				__REGW(MLC_START + 0x6a)
#define MLC_STL2_ENDX 				__REGW(MLC_START + 0x6c)
#define MLC_STL2_STY 				__REGW(MLC_START + 0x6e)
#define MLC_STL2_ENDY 				__REGW(MLC_START + 0x70)
/* Coordinate Register of Still Image(RGB) Layer 3 */
#define MLC_STL3_STX 				__REGW(MLC_START + 0x72)
#define MLC_STL3_ENDX 				__REGW(MLC_START + 0x74)
#define MLC_STL3_STY 				__REGW(MLC_START + 0x76)
#define MLC_STL3_ENDY 				__REGW(MLC_START + 0x78)
/* Coordinate Register of Still Image(RGB) Layer 4 */
#define MLC_STL4_STX 				__REGW(MLC_START + 0x7a)
#define MLC_STL4_ENDX 				__REGW(MLC_START + 0x7c)
#define MLC_STL4_STY 				__REGW(MLC_START + 0x7e)
#define MLC_STL4_ENDY 				__REGW(MLC_START + 0x80)

/* Color Key Register of Still Image (RGB) Layer */
#define MLC_STL_CKEY_GR 			__REGW(MLC_START + 0x82) 
#define MLC_STL_CKEY_GR_G 			(0xff << 8)
#define MLC_STL_CKEY_GR_R 			(0xff << 0)

/* Color Key Register of Still Image (RGB) Layer */
#define MLC_STL_CKEY_B 				__REGW(MLC_START + 0x84) 
#define MLC_STL_CKEY_B_B 			(0xff << 0)

/* Scale Factor Register of Still Image (RGB) Layer */
#define MLC_STL_HSC 				__REGW(MLC_START + 0x86) 

/* Scale Factor Register of Still Image (RGB) Layer */
#define MLC_STL_VSCL 				__REGW(MLC_START + 0x88)
#define MLC_STL_VSCH 				__REGW(MLC_START + 0x8a) 

/* Horizontal Width Register of Still Image(RGB) Layer */
#define MLC_STL_HW 				__REGW(MLC_START + 0x8c) /* [14:0] */

/* Source Address of Still Image (RGB) Layer */
#define MLC_STL_OADRL				__REGW(MLC_START + 0x8e)
#define MLC_STL_OADRH				__REGW(MLC_START + 0x90)
#define MLC_STL_EADRL				__REGW(MLC_START + 0x92)
#define MLC_STL_EADRH				__REGW(MLC_START + 0x94)

/* Source Address of OSD */
#define MLC_OSD_OADRL				__REGW(MLC_START + 0x96)
#define MLC_OSD_OADRH				__REGW(MLC_START + 0x98)
#define MLC_OSD_EADRL				__REGW(MLC_START + 0x9a)
#define MLC_OSD_EADRH				__REGW(MLC_START + 0x9c)

/* Hardware Cursor Control Register */
#define MLC_HWC_CNTR 				__REGW(MLC_START + 0x9e)
#define MLC_HWC_CNTR_FALPH 			(15 << 12) /* Foreground Alpha */
#define MLC_HWC_CNTR_BALPH 			(15 << 8) /* Background Alpha */
/* [7] Reserved */
#define MLC_HWC_CNTR_SZ 			(63 << 0) /* Cursor Size */
#define MLC_HWC_CNTR_SZ_32 			(0x20) /* 32x32 */
#define MLC_HWC_CNTR_SZ_64 			(0x40) /* 64x64 */

/* Hardware Cursor Horizontal Starting Position Register */
#define MLC_HWC_STX 				__REGW(MLC_START + 0xa0) /* [11:0] */

/* Hardware Cursor Vertical Starting Position Register (MLC_HWC_STY) */
#define MLC_HWC_STY 				__REGW(MLC_START + 0xa2) /* [11:0] */

/* Hardware Cursor Foreground Color(GR) Register */
#define MLC_HWC_FGR 				__REGW(MLC_START + 0xa4)
#define MLC_HWC_FGR_G 				(0xff << 8)
#define MLC_HWC_FGR_R 				(0xff << 0)

/* Hardware Cursor Foreground Color(B) Register */
#define MLC_HWC_FB 				__REGW(MLC_START + 0xa6)
#define MLC_HWC_FB_B 				(0xff << 0)

/* Hardware Cursor Background Color(GR) Register */
#define MLC_HWC_BGR 				__REGW(MLC_START + 0xa8)
#define MLC_HWC_BGR_G 				(0xff << 8)
#define MLC_HWC_BGR_R 				(0xff << 0)

/* Hardware Cursor Background Color(B) Register */
#define MLC_HWC_BB 				__REGW(MLC_START + 0xaa)
#define MLC_HWC_BB_B 				(0xff << 0)

/* Source Address of Hardware Cursor */
#define MLC_HWC_OADRL				__REGW(MLC_START + 0xac)
#define MLC_HWC_OADRH				__REGW(MLC_START + 0xae)
#define MLC_HWC_EADRL				__REGW(MLC_START + 0xb0)
#define MLC_HWC_EADRH				__REGW(MLC_START + 0xb2)

#if 0
/* Luminance Enhancement Control Register (MLC_LUMA_ENH) */
Address : C000 2934h
[15:11] - Reserved -
[10:8] R/W MLC_CNTRST Contrast control
0 : 1.00 1 : 1.125 2 : 1.25 3 : 1.375
4 : 1.5 5 : 1.625 6 : 1.75 7 : 1.875
3’b0
[7:0] R/W MLC_BRIGHT Brightness Control
twos compliment : - 128 ~ +127
8’b0

/* Chrominance Enhancement Control Register (MLC_HUECB1AB) */
Address : C000 2936h
[15:8] R/W MLC_HUECB1A 8’h40
[7:0] R/W MLC_HUECB1B
1st quadrant Blue control : Decimal Point = [S.1.6]
(B-Y) = (B-Y) x HUECB1_A + (R-Y) x HUECB1_B
8’b0

/* Chrominance Enhancement Control Register (MLC_HUECR1AB) */
Address : C000 2938h
[15:8] R/W MLC_HUECR1A 8’b0
[7:0] R/W MLC_HUECR1B
1st quadrant Red control : Decimal Point = [S.1.6]
(R-Y) = (B-Y) x HUECR1_A + (R-Y) x HUECR1_B
8’h40

/* Chrominance Enhancement Control Register (MLC_HUECB2AB) */
Address : C000 293Ah
[15:8] R/W MLC_HUECB2A 8’h40
[7:0] R/W MLC_HUECB2B
2nd quadrant Blue control : Decimal Point = [S.1.6]
(B-Y) = (B-Y) x HUECB2_A + (R-Y) x HUECB2_B
8’b0

/* Chrominance Enhancement Control Register (MLC_HUECR2AB) */
Address : C000 293Ch
[15:8] R/W MLC_HUECR2A 8’b0
[7:0] R/W MLC_HUECR2B
2nd quadrant Red control : Decimal Point = [S.1.6]
(R-Y) = (B-Y) x HUECR2_A + (R-Y) x HUECR2_B
8’h40

/* Chrominance Enhancement Control Register (MLC_HUECB3AB) */
Address : C000 293Eh
[15:8] R/W MLC_HUECB3A 8’h40
[7:0] R/W MLC_HUECB3B
3rd quadrant Blue control : Decimal Point = [S.1.6]
(B-Y) = (B-Y) x HUECB3_A + (R-Y) x HUECB3_B
8’b0

/* Chrominance Enhancement Control Register (MLC_HUECR3AB) */
Address : C000 2940h
[15:8] R/W MLC_HUECR3A 8’b0
[7:0] R/W MLC_HUECR3B
3rd quadrant Red control : Decimal Point = [S.1.6]
(R-Y) = (B-Y) x HUECR3_A + (R-Y) x HUECR3_B
8’h40

/* Chrominance Enhancement Control Register (MLC_HUECB4AB) */
Address : C000 2942h
[15:8] R/W MLC_HUECB4A 8’h40
[7:0] R/W MLC_HUECB4B
4th quadrant Blue control : Decimal Point = [S.1.6]
(B-Y) = (B-Y) x HUECB4_A + (R-Y) x HUECB4_B
8’b0

/* Chrominance Enhancement Control Register (MLC_HUECR4AB) */
Address : C000 2944h
[15:8] R/W MLC_HUECR4A 8’b0
[7:0] R/W MLC_HUECR4B
4th quadrant Red control : Decimal Point = [S.1.6]
(R-Y) = (B-Y) x HUECR4_A + (R-Y) x HUECR4_B
8’h40

/* Dither Control Register (MLC_DITHER) */
Address : C000 2946h
[15:2] - Reserved -
[1] R/W MLC_DITHER_ON Dithering Skip Enable. 0 : Dither ON 1 : Dither OFF 1
[0] R/W MLC_DITHER_TY Dithering Type. 0 : 2x2 1 : 4x4 0

Reserved Register
Address : C000 2948h ~ C000 2952h
[15:0] - Reserved -

/* OSD Palette Table Index Register (MLC_OSD_PALLT_A) */
Address : C000 2954h
[15:9] - Reserved -
[8:0] R/W MLC_OSD_PALLT_A Palette table index of OSD
The table index will be increased automatically whenever the data is written on
MLC_OSD_PALLT_D
9’b0

/* OSD Palette Table Data Register (MLC_OSD_PALLT_D) */
Address : C000 2956h
[15:0] R/W MLC_OSD_PALLT_D Palette table data of OSD 16’b0
Still Image(RGB) Layer Palette Table Index Register (MLC_STL_PALLT_A)
Address : C000 2958h
[15:9] - Reserved -
[8:0] R/W MLC_STL_PALLT_A Palette table index of RGB Layer
The table index will be increased automatically whenever the data is written on
MLC_STL_PALLT_D
9’b0

/* Still Image(RGB) Layer Palette Table Data Register (MLC_STL_PALLT_D) */
Address : C000 295Ah
[15:0] R/W MLC_STL_PALLT_D Palette table data of RGB Layer 16’b0
Gamma Table Index Register (MLC_GAMMA_A)
Address : C000 295Ch
[15:9] - Reserved -
[8:0] R/W MLC_GAMMA_A Gamma table index
The table index will be increased automatically whenever the data is written on
MLC_GAMMA_D
9’b0
/* Gamma Table Data Register (MLC_GAMMA_D) */
Address : C000 295Eh
[15:0] R/W MLC_GAMMA_D Gamma table data 16’b0
Reserved Register
Address : C000 2960h ~ C000 2972h
[15:0] - Reserved -

/* Sub-Picture Unit Control Register (MLC_SPU_CTRL) */
Address : C000 2974h
[15:3] - Reserved -
[2] R/W MLC_SPU_HL_ON Highlight mode on/off (1 = on) 0
[1] R/W MLC_SPU_DISP_MODE Frame/Field display mode selection.
0 : frame mode 1 : field mode
0
[0] R/W MLC_SPU_ON SPU block enable/disable bit.
0 : SPU disable 1 : SPU enable
0
/* Sub-Picture Decode Start Delay Register (MLC_SPU_DELAY) */
Address : C000 2976h
[15:10] - Reserved -
[9:0] R/W MLC_SPU_DELAY_VAL SPU decode start delay.
The actual sub-picture decode operation is initiated after the cycle delay the
number of this register value from VSYNC.
10’b0

/* Sub-Picture Base Command Information Start Address Register (MLC_SPU_BASEI_ADDR) */
Address : C000 2978h / C000 297Ah
[15:0] R/W MLC_SPU_BASEI_ADDRL 16’b0
[15:0] R/W MLC_SPU_BASEI_ADDRH
Base command information start address
16’b0

/* Sub-Picture Highlight Command Information Start Address Register (MLC_SPU_HLI_ADDR) */
Address : C000 297Ch / C000 297Eh
[15:0] R/W MLC_SPU_HLI_ADDRL 16’b0
[15:0] R/W MLC_SPU_HLI_ADDRH
Highlight command information start address
16’b0

/* Sub-Picture Palette Table Register (MLC_SPU_PAL) */
Address : C000 2980h ~ C000 29BEh
C000 2980 / 2984 / 2988 / 298C / 2990 / 2994 / 2998 / 299C / 29A0 / 29A4 / 29A8 / 29AC / 29B0 / 29B4 / 29B8 / 29BC h
[15:8] R/W MLC_SPU_PALn_CR Cr of Palette table[n] of SPU, n = 0 ~ 15 8’b0
[7:0] R/W MLC_SPU_PALn_CB Cb of Palette table[n] of SPU, n = 0 ~ 15 8’b0
C000 2982 / 2986 / 298A / 298E / 2992 / 2996 / 299A / 299E / 29A2 / 29A6 / 29AA / 29AE / 29B2 / 29B6 / 29BA / 29BE h
[15:8] - Reserved -
[7:0] R/W MLC_SPU_PALn_Y Y of Palette table[n] of SPU, n = 0 ~ 15 8’b0
#endif

#endif

