#ifndef _MP2530F_REGS_H_
#define _MP2530F_REGS_H_

#include <stdint.h>

struct mp2530f_mlc_layer_reg
{
	union
	{
		uint32_t reg;
		struct {
			int16_t left : 12;
			uint8_t rsvd0 : 4;
			int16_t top : 12;
			uint8_t rsvd1 : 4;
		} bits;
	} lt;
	union
	{
		uint32_t reg;
		struct {
			int16_t right : 12;
			uint8_t rsvd0 : 4;
			int16_t bottom : 12;
			uint8_t rsvd1 : 4;
		} bits;
	} rb;
	union
	{
		uint32_t reg;
		struct {
			uint8_t tpenb : 1;
			uint8_t invenb : 1;
			uint8_t blendenb : 1;
			uint8_t rsvd0 : 1;
			uint8_t dirtyflag : 1;
			uint8_t layerenb : 1;
			uint8_t rsvd1 : 2;
			uint8_t grp3denb : 1;
			uint8_t rsvd2 : 3;
			uint8_t locksize : 2;
			uint8_t rsvd3 : 2;
			uint16_t format : 16;
		} bits;
	} control;
	union
	{
		uint32_t reg;
		struct {
			uint32_t hstride : 31;
			uint8_t rsvd0: 1;
		} bits;
	} hstride;
	uint32_t vstride;
	union
	{
		uint32_t reg;
		struct {
			uint32_t tpcolor: 24;
			uint8_t rsvd0 : 4;
			uint8_t alpha : 4;
		} bits;
	} tpcolor;
	union
	{
		uint32_t reg;
		struct {
			uint32_t invcolor: 24;
			uint8_t rsvd0 : 8;
		} bits;
	} invcolor;
	uint32_t address;
};

struct mp2530f_mlc_reg
{
	union
	{
		uint32_t reg;
		struct {
			uint8_t fieldnenb : 1;
			uint8_t mlcenb : 1;
			uint8_t rsvd0 : 1;
			uint8_t dittyflag : 1;
			uint8_t rsvd1 : 4;
			uint8_t priority : 4;
			uint32_t rsvd2 : 20;
		} bits;
	} controlt;
	union
	{
		uint32_t reg;
		struct {
			uint16_t width : 12;
			uint8_t rsvd0 : 4;
			uint16_t height : 12;
			uint8_t rsvd1 : 4;
		} bits;
	} screensize;
	union
	{
		uint32_t reg;
		struct {
			uint32_t width : 24;
			uint8_t rsvd0 : 8;
		} bits;
	} bgcolor;
	struct mp2530f_mlc_layer_reg l0;
	struct mp2530f_mlc_layer_reg l1;
	struct mp2530f_mlc_layer_reg l2;
	struct mp2530f_mlc_layer_reg l3;
	/* TODO add the missing registers */
};

struct mp2530f_rotator_reg
{
	union
	{
		uint32_t reg;
		struct {
			uint8_t run : 1;
			uint32_t rsvd0 : 31;
		};
	} trunreg;
	union
	{
		uint32_t reg;
		struct {
			uint8_t int_pend : 1;
			uint8_t rsvd0 : 7;
			uint8_t int_clr : 1;
			uint8_t rsvd1 : 7;
			uint8_t int_enb : 1;
			uint8_t rsvd2 : 7;
			uint8_t busy : 1;
			uint8_t rsvd3 : 7;
		} bits;
	} intreg;
	union
	{
		uint32_t reg;
		struct {
			uint8_t dir_col : 1;
			uint16_t rsvd0 : 15;
			uint8_t dir_row : 1;
			uint16_t rsvd1 : 15;
		} bits;
	} direction;
	union
	{
		uint32_t reg;
		struct {
			uint16_t basex : 12;
			uint16_t basey : 12;
			uint8_t seg : 4;
			uint8_t rsvd0 : 4;
		} bits;
	} src, dst;
	union
	{
		uint32_t reg;
		struct {
			uint16_t width : 12;
			uint8_t rsvd0 : 4;
			uint16_t height : 12;
			uint8_t rsvd1 : 4;
		} bits;
	} size;
	uint32_t rsvd0[43];
	union
	{
		uint32_t reg;
		struct {
			uint8_t blckenb : 2;
			uint8_t rsvd0 : 1;
			uint8_t pclkmode : 1;
			uint32_t rsvd1 : 28;
		} bits;
	} clkenb;
};

#endif
