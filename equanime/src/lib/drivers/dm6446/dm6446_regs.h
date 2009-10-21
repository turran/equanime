#ifndef _DM6446_REGS_H
#define _DM6446_REGS_H

#define	VPBE_REG_BASE				IO_ADDRESS(0x01c72780)
#define	VENC_REG_BASE				IO_ADDRESS(0x01c72400)
#define	OSD_REG_BASE				IO_ADDRESS(0x01c72600)
#define OSD_REG_SIZE				0x00000180

/* offset 0x2400 */
struct dm6446_venc_regs
{
	uint32_t vmod;
	uint32_t vidctl;
	uint32_t vdpro;
	uint32_t syncctl;
	uint32_t hspls;
	uint32_t vspls;
	uint32_t hint;
	uint32_t hstart;
	uint32_t hvalid;
	uint32_t vint;
	uint32_t vstart;
	uint32_t vvalid;
	uint32_t hsdly;
	uint32_t vsdly;
	uint32_t ycctl;
	uint32_t rgbctl;
	uint32_t rgbclp;
	uint32_t linectl;
	uint32_t cullline;
	uint32_t lcdout;
	uint32_t brts;
	uint32_t brtw;
	uint32_t acctl;
	uint32_t pwmp;
	uint32_t pwmw;
	uint32_t dclkctl;
	uint32_t dclkptn0;
	uint32_t dclkptn1;
	uint32_t dclkptn2;
	uint32_t dclkptn3;
	uint32_t dclkptn0a;
	uint32_t dclkptn1a;
	uint32_t dclkptn2a;
	uint32_t dclkptn3a;
	uint32_t dclkhs;
	uint32_t dclkhsa;
	uint32_t dclkhr;
	uint32_t dclkvs;
	uint32_t dclkvr;
	uint32_t capctl;
	uint32_t capdo;
	uint32_t capde;
	uint32_t atr0;
	uint32_t atr1;
	uint32_t atr2;
	uint32_t rsvd0;
#if 0
#define	VENC_EPSON_LCDCTL			(VENC_REG_BASE + 0xB4)
#define	VENC_CASIO_LCDCTL			(VENC_REG_BASE + 0xB4)
#define	VENC_UDISP_LCDCT			(VENC_REG_BASE + 0xB4)
#define	VENC_STN_LCDCT				(VENC_REG_BASE + 0xB4)
#endif

	uint32_t vstat;
	uint32_t ramadr;
	uint32_t ramport;
	uint32_t dactst;
	uint32_t ycolvl;
	uint32_t scprog;
	uint32_t rsvd1;;
	uint32_t rsvd2;;
	uint32_t rsvd3;;
	uint32_t cvbs;
	uint32_t cmpnt;
	uint32_t etmg0;
	uint32_t etmg1;
	uint32_t etmg2;
	uint32_t etmg3;
	uint32_t dacsel;
	uint32_t rsvd4;;
	uint32_t rsvd5;;
	uint32_t argbx0;
	uint32_t argbx1;
	uint32_t argbx2;
	uint32_t argbx3;
	uint32_t argbx4;
	uint32_t drgbx0;
	uint32_t drgbx1;
	uint32_t drgbx2;
	uint32_t drgbx3;
	uint32_t drgbx4;
	uint32_t vstarta;
	uint32_t osdclk0;
	uint32_t osdclk1;
	uint32_t hvldcl0;
	uint32_t hvldcl1;
	uint32_t osdhad;
};

/* offset 0x2600 */
struct dm6446_osd_regs
{
	uint32_t mode;
	uint32_t vidwinmd;
	uint32_t osdwin0md;
	uint32_t osdwin1md;
	uint32_t rectcur;
	uint32_t rsvd0;
	uint32_t vidwin0ofst;
	uint32_t vidwin1ofst;
	uint32_t osdwin0ofst;
	uint32_t osdwin1ofst;
	uint32_t vidwin0adr;
	uint32_t rsvd1;
	uint32_t vidwin1adr;
	uint32_t rsvd2;
	uint32_t osdwin0adr;
	uint32_t osdwin1adr;
	uint32_t basepx;
	uint32_t basepy;
	uint32_t vidwin0xp;
	uint32_t vidwin0yp;
	uint32_t vidwin0xl;
	uint32_t vidwin0yl;
	uint32_t vidwin1xp;
	uint32_t vidwin1yp;
	uint32_t vidwin1xl;
	uint32_t vidwin1yl;
	uint32_t osdwin0xp;
	uint32_t osdwin0yp;
	uint32_t osdwin0xl;
	uint32_t osdwin0yl;
	uint32_t osdwin1xp;
	uint32_t osdwin1yp;
	uint32_t osdwin1xl;
	uint32_t osdwin1yl;
	uint32_t curxp;
	uint32_t curyp;
	uint32_t curxl;
	uint32_t curyl;
	uint32_t rsvd3;
	uint32_t rsvd4;
	uint32_t w0bmp01;
	uint32_t w0bmp23;
	uint32_t w0bmp45;
	uint32_t w0bmp67;
	uint32_t w0bmp89;
	uint32_t w0bmpab;
	uint32_t w0bmpcd;
	uint32_t w0bmpef;
	uint32_t w1bmp0;
	uint32_t w1bmp2;
	uint32_t w1bmp4;
	uint32_t w1bmp6;
	uint32_t w1bmp8;
	uint32_t w1bmpa;
	uint32_t w1bmpc;
	uint32_t w1bmpe;
	uint32_t ti_tes;
	uint32_t rsvd5;
	uint32_t miscct;
	uint32_t clutramyc;
	uint32_t clutramc;
	uint32_t transpva;
	uint32_t rsvd6;
	uint32_t ppvwin0ad;
};

/* offset 0x2780 */
struct dm6446_vpbe_regs
{
	uint32_t pid;
	uint32_t pcr;
};

struct dm6446_vpss_regs
{
	uint32_t clkctl;
};

typedef enum _dm6446_vout
{
	DM6446_VOUT_16BIT_YCBCR,
	DM6446_VOUT_16BIT_YCRCB,
	DM6446_VOUT_8BIT_CBYCRY,
	DM6446_VOUT_8BIT_YCRYCB,
	DM6446_VOUT_8BIT_CRYCBY,
	DM6446_VOUT_8BIT_YCBYCR,
	DM6446_VOUT_PARALLEL,
} dm6446_vout;

typedef enum _dm6446_dout
{
	DM6446_DOUT_CVBS,
	DM6446_DOUT_SV_Y,
	DM6446_DOUT_SV_C,
	DM6446_DOUT_Y_G,
	DM6446_DOUT_Pb_B,
	DM6446_DOUT_Pr_R,
} dm6446_dout;

/*
 * TODO
 * we should have an array of components (outputs, inputs)
 * a flag to determine if the timmings are handled internally and how (VENC)
 */
struct dm6446
{
	volatile struct dm6446_venc_regs *venc;
	volatile struct dm6446_osd_regs *osd;
	volatile struct dm6446_vpbe_regs *vpbe;
	volatile struct dm6446_vpss_regs *vpss;
};

Eina_Bool dm6446_controller_init(Equ_Controller *c, struct dm6446 *dm6446);
void dm6446_venc_vout_set(struct dm6446 *dm6446, dm6446_vout vout);
void dm6446_venc_mode_set(struct dm6446 *dm6446, Equ_Mode *m,
		Eina_Bool internal);
void dm6446_venc_dac_set(struct dm6446 *dm6446, dm6446_dout dac0,
		dm6446_dout dac1, dm6446_dout dac2, dm6446_dout dac3);
void dm6446_venc_dac_enable(struct dm6446 *dm6446, Eina_Bool dac0,
		Eina_Bool dac1, Eina_Bool dac2, Eina_Bool dac3);

#endif
