/*
 * Copyright (c) 2003, 2007-11 Matteo Frigo
 * Copyright (c) 2003, 2007-11 Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/* This file was automatically generated --- DO NOT EDIT */
/* Generated on Wed Jul 27 06:16:29 EDT 2011 */

#include "codelet-rdft.h"

#ifdef HAVE_FMA

/* Generated by: ../../../genfft/gen_r2cf.native -fma -reorder-insns -schedule-for-pipeline -compact -variables 4 -pipeline-latency 4 -n 8 -name r2cf_8 -include r2cf.h */

/*
 * This function contains 20 FP additions, 4 FP multiplications,
 * (or, 16 additions, 0 multiplications, 4 fused multiply/add),
 * 18 stack variables, 1 constants, and 16 memory accesses
 */
#include "r2cf.h"

static void r2cf_8(R *R0, R *R1, R *Cr, R *Ci, stride rs, stride csr, stride csi, INT v, INT ivs, INT ovs)
{
     DK(KP707106781, +0.707106781186547524400844362104849039284835938);
     {
	  INT i;
	  for (i = v; i > 0; i = i - 1, R0 = R0 + ivs, R1 = R1 + ivs, Cr = Cr + ovs, Ci = Ci + ovs, MAKE_VOLATILE_STRIDE(rs), MAKE_VOLATILE_STRIDE(csr), MAKE_VOLATILE_STRIDE(csi)) {
	       E T4, T7, T3, Tj, Td, T5, T8, T9;
	       {
		    E T1, T2, Tb, Tc;
		    T1 = R0[0];
		    T2 = R0[WS(rs, 2)];
		    Tb = R1[WS(rs, 3)];
		    Tc = R1[WS(rs, 1)];
		    T4 = R0[WS(rs, 1)];
		    T7 = T1 - T2;
		    T3 = T1 + T2;
		    Tj = Tb + Tc;
		    Td = Tb - Tc;
		    T5 = R0[WS(rs, 3)];
		    T8 = R1[0];
		    T9 = R1[WS(rs, 2)];
	       }
	       {
		    E T6, Tf, Ta, Ti;
		    T6 = T4 + T5;
		    Tf = T4 - T5;
		    Ta = T8 - T9;
		    Ti = T8 + T9;
		    {
			 E Th, Tk, Te, Tg;
			 Th = T3 + T6;
			 Cr[WS(csr, 2)] = T3 - T6;
			 Tk = Ti + Tj;
			 Ci[WS(csi, 2)] = Tj - Ti;
			 Te = Ta + Td;
			 Tg = Td - Ta;
			 Cr[0] = Th + Tk;
			 Cr[WS(csr, 4)] = Th - Tk;
			 Ci[WS(csi, 3)] = FMA(KP707106781, Tg, Tf);
			 Ci[WS(csi, 1)] = FMS(KP707106781, Tg, Tf);
			 Cr[WS(csr, 1)] = FMA(KP707106781, Te, T7);
			 Cr[WS(csr, 3)] = FNMS(KP707106781, Te, T7);
		    }
	       }
	  }
     }
}

static const kr2c_desc desc = { 8, "r2cf_8", {16, 0, 4, 0}, &GENUS };

void X(codelet_r2cf_8) (planner *p) {
     X(kr2c_register) (p, r2cf_8, &desc);
}

#else				/* HAVE_FMA */

/* Generated by: ../../../genfft/gen_r2cf.native -compact -variables 4 -pipeline-latency 4 -n 8 -name r2cf_8 -include r2cf.h */

/*
 * This function contains 20 FP additions, 2 FP multiplications,
 * (or, 20 additions, 2 multiplications, 0 fused multiply/add),
 * 14 stack variables, 1 constants, and 16 memory accesses
 */
#include "r2cf.h"

static void r2cf_8(R *R0, R *R1, R *Cr, R *Ci, stride rs, stride csr, stride csi, INT v, INT ivs, INT ovs)
{
     DK(KP707106781, +0.707106781186547524400844362104849039284835938);
     {
	  INT i;
	  for (i = v; i > 0; i = i - 1, R0 = R0 + ivs, R1 = R1 + ivs, Cr = Cr + ovs, Ci = Ci + ovs, MAKE_VOLATILE_STRIDE(rs), MAKE_VOLATILE_STRIDE(csr), MAKE_VOLATILE_STRIDE(csi)) {
	       E T3, T7, Td, Tj, T6, Tg, Ta, Ti;
	       {
		    E T1, T2, Tb, Tc;
		    T1 = R0[0];
		    T2 = R0[WS(rs, 2)];
		    T3 = T1 + T2;
		    T7 = T1 - T2;
		    Tb = R1[WS(rs, 3)];
		    Tc = R1[WS(rs, 1)];
		    Td = Tb - Tc;
		    Tj = Tb + Tc;
	       }
	       {
		    E T4, T5, T8, T9;
		    T4 = R0[WS(rs, 1)];
		    T5 = R0[WS(rs, 3)];
		    T6 = T4 + T5;
		    Tg = T4 - T5;
		    T8 = R1[0];
		    T9 = R1[WS(rs, 2)];
		    Ta = T8 - T9;
		    Ti = T8 + T9;
	       }
	       Cr[WS(csr, 2)] = T3 - T6;
	       Ci[WS(csi, 2)] = Tj - Ti;
	       {
		    E Te, Tf, Th, Tk;
		    Te = KP707106781 * (Ta + Td);
		    Cr[WS(csr, 3)] = T7 - Te;
		    Cr[WS(csr, 1)] = T7 + Te;
		    Tf = KP707106781 * (Td - Ta);
		    Ci[WS(csi, 1)] = Tf - Tg;
		    Ci[WS(csi, 3)] = Tg + Tf;
		    Th = T3 + T6;
		    Tk = Ti + Tj;
		    Cr[WS(csr, 4)] = Th - Tk;
		    Cr[0] = Th + Tk;
	       }
	  }
     }
}

static const kr2c_desc desc = { 8, "r2cf_8", {20, 2, 0, 0}, &GENUS };

void X(codelet_r2cf_8) (planner *p) {
     X(kr2c_register) (p, r2cf_8, &desc);
}

#endif				/* HAVE_FMA */
