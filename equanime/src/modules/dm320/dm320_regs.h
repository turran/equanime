#ifndef DM320_REGS_H_
#define DM320_REGS_H_

/* OSD */
#define OSDMODE 	__REGH(0x00030680) /* OSD Mode Setup */
#define VIDWINMD 	__REGH(0x00030682) /* Video Window Mode Setup */
#define VIDWINMD_VFF1 	(1 << 9)
#define VIDWINMD_ACT1 	(1 << 8)
#define VIDWINMD_VFF0 	(1 << 1)
#define VIDWINMD_ACT0 	(1 << 0)
#define OSDWIN0MD 	__REGH(0x00030684) /* OSD Window 0 Mode Setup */
#define OSDWIN1MD 	__REGH(0x00030686) /* OSD Window 1 Mode Setup */
#define OSDWINXMD_ACT 		(1 << 0)
#define OSDWINXMD_OFF 		(1 << 1)
#define OSDWINXMD_BLND_TRANS	0x0
#define OSDWINXMD_BLND_OPAQUE	0x7
#define OSDWINXMD_BLND		(OSDWINXMD_BLND_OPAQUE << 3)
#define OSDWINXMD_BLNDv(v)	(v << 3)
#define OSDWINXMD_BMW 		(0x3 << 6)
#define OSDWINXMD_BMWv(v)	(v << 6)
#define OSDWINXMD_BMW_1BIT 	(0x0 << 6)
#define OSDWINXMD_BMW_2BIT 	(0x1 << 6)
#define OSDWINXMD_BMW_4BIT 	(0x2 << 6)
#define OSDWINXMD_BMW_8BIT 	(0x3 << 6)
#define OSDWINXMD_OVZ 		(0x3 << 8)
#define OSDWINXMD_OVZv(v) 	(v << 8)
#define OSDWINXMD_OHZ 		(0x3 << 10)
#define OSDWINXMD_OHZv(v) 	(v << 10)
#define OSDWINXMD_RGBE 		(1 << 13)
#define OSDATTRMD 		OSDWIN1MD  /* OSD Attribute Window Mode Setup */
#define OSDATTRMD_OASW 		(1 << 15)
#define OSDATTRMD_OASWv(v) 	(v << 15)
#define RECTCUR 	__REGH(0x00030688) /* Rectangular Cursor Setup */
#define RSV0 		__REGH(0x0003068A) /* Reserved */
#define VIDWIN0OFST 	__REGH(0x0003068C) /* Video Window 0 Offset */
#define VIDWIN1OFST 	__REGH(0x0003068E) /* Video Window 1 Offset */
#define OSDWIN0OFST 	__REGH(0x00030690) /* OSD Window 0 Offset */
#define OSDWIN1OFST 	__REGH(0x00030692) /* OSD Window 1 Offset */
#define VIDWINADH 	__REGH(0x00030694) /* Video Window 0/1 Address - High */
#define VIDWINADH_01AH 	0x7f00
#define VIDWINADH_00AH 	0x007f
#define VIDWIN0ADL 	__REGH(0x00030696) /* Video Window 0 Address - Low */
#define VIDWIN1ADL 	__REGH(0x00030698) /* Video Window 1 Address - Low */
#define OSDWINADH 	__REGH(0x0003069A) /* OSD Window 0/1 Address - High */
#define OSDWINADH_01AH 	0x7f00
#define OSDWINADH_00AH 	0x007f
#define OSDWIN0ADL 	__REGH(0x0003069C) /* OSD Window 0 Address - Low */
#define OSDWIN1ADL 	__REGH(0x0003069E) /* OSD Window 1 Address - Low */
#define BASEPX 		__REGH(0x000306A0) /* Base Pixel X */
#define BASEPY 		__REGH(0x000306A2) /* Base Pixel Y */
#define VIDWIN0XP 	__REGH(0x000306A4) /* Video Window 0 X-Position */
#define VIDWIN0YP 	__REGH(0x000306A6) /* Video Window 0 Y-Position */
#define VIDWIN0XL 	__REGH(0x000306A8) /* Video Window 0 X-Size */
#define VIDWIN0YL 	__REGH(0x000306AA) /* Video Window 0 Y-Size */
#define VIDWIN1XP 	__REGH(0x000306AC) /* Video Window 1 X-Position */
#define VIDWIN1YP 	__REGH(0x000306AE) /* Video Window 1 Y-Position */
#define VIDWIN1XL 	__REGH(0x000306B0) /* Video Window 1 X-Size */
#define VIDWIN1YL 	__REGH(0x000306B2) /* Video Window 1 Y-Size */
#define OSDWIN0XP	__REGH(0x000306B4) /* OSD Bitmap Window 0 X-Position */
#define OSDWIN0YP 	__REGH(0x000306B6) /* OSD Bitmap Window 0 Y-Position */
#define OSDWIN0XL 	__REGH(0x000306B8) /* OSD Bitmap Window 0 X-Size */
#define OSDWIN0YL	__REGH(0x000306BA) /* OSD Bitmap Window 0 Y-Size */
#define OSDWIN1XP 	__REGH(0x000306BC) /* OSD Bitmap Window 1 X-Position */
#define OSDWIN1YP 	__REGH(0x000306BE) /* OSD Bitmap Window 1 Y-Position */
#define OSDWIN1XL	__REGH(0x000306C0) /* OSD Bitmap Window 1 X-Size */
#define OSDWIN1YL 	__REGH(0x000306C2) /* OSD Bitmap Window 1 Y-Size */


#endif /*DM320_REGS_H_*/
