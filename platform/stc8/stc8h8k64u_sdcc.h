#ifndef STC8H8K64U_SDCC_H
#define STC8H8K64U_SDCC_H

/* Minimal original declarations from STC8H Series Manual, register maps 8.5/16.1.
 * This probe is not a vendor header and does not establish board routing. */
__sfr __at (0x98) SCON;
__sfr __at (0x99) SBUF;
__sfr __at (0xA2) P_SW1;
__sfr __at (0x8E) AUXR;
__sfr __at (0xD6) T2H;
__sfr __at (0xD7) T2L;
__sbit __at (0x98) RI;
__sbit __at (0x99) TI;
__sbit __at (0xAC) ES;
__sbit __at (0xAF) EA;

#endif
