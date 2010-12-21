/*
 * Copyright (c) 2003, 2007-8 Matteo Frigo
 * Copyright (c) 2003, 2007-8 Massachusetts Institute of Technology
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
/* Generated on Sun Jul 12 06:43:58 EDT 2009 */

#include "codelet-rdft.h"

#ifdef HAVE_FMA

/* Generated by: ../../../genfft/gen_hc2hc -fma -reorder-insns -schedule-for-pipeline -compact -variables 4 -pipeline-latency 4 -n 20 -dit -name hf_20 -include hf.h */

/*
 * This function contains 246 FP additions, 148 FP multiplications,
 * (or, 136 additions, 38 multiplications, 110 fused multiply/add),
 * 100 stack variables, 4 constants, and 80 memory accesses
 */
#include "hf.h"

static void hf_20(R *cr, R *ci, const R *W, stride rs, INT mb, INT me, INT ms)
{
     DK(KP951056516, +0.951056516295153572116439333379382143405698634);
     DK(KP559016994, +0.559016994374947424102293417182819058860154590);
     DK(KP250000000, +0.250000000000000000000000000000000000000000000);
     DK(KP618033988, +0.618033988749894848204586834365638117720309180);
     INT m;
     for (m = mb, W = W + ((mb - 1) * 38); m < me; m = m + 1, cr = cr + ms, ci = ci - ms, W = W + 38, MAKE_VOLATILE_STRIDE(rs)) {
	  E T54, T5a, T5c, T56, T53, T55, T5b, T57;
	  {
	       E T4N, T4q, T8, T2i, T4r, T2n, T4O, Tl, T2v, T3v, T43, T4b, TN, T2b, T3F;
	       E T3a, T2R, T3z, T3T, T4f, T27, T2f, T3J, T3i, T2K, T3y, T3W, T4e, T1G, T2e;
	       E T3I, T3p, T2C, T3w, T40, T4c, T1e, T2c, T3G, T33;
	       {
		    E T1, T4p, T3, T6, T2, T5;
		    T1 = cr[0];
		    T4p = ci[0];
		    T3 = cr[WS(rs, 10)];
		    T6 = ci[WS(rs, 10)];
		    T2 = W[18];
		    T5 = W[19];
		    {
			 E Ta, Td, Tg, T2j, Tb, Tj, Tf, Tc, Ti;
			 {
			      E T4n, T4, T9, T4o, T7;
			      Ta = cr[WS(rs, 5)];
			      Td = ci[WS(rs, 5)];
			      T4n = T2 * T6;
			      T4 = T2 * T3;
			      T9 = W[8];
			      Tg = cr[WS(rs, 15)];
			      T4o = FNMS(T5, T3, T4n);
			      T7 = FMA(T5, T6, T4);
			      T2j = T9 * Td;
			      Tb = T9 * Ta;
			      T4N = T4p - T4o;
			      T4q = T4o + T4p;
			      T8 = T1 + T7;
			      T2i = T1 - T7;
			      Tj = ci[WS(rs, 15)];
			      Tf = W[28];
			 }
			 Tc = W[9];
			 Ti = W[29];
			 {
			      E T36, Ts, T2t, TL, TB, TE, TD, T38, Ty, T2q, TC;
			      {
				   E TH, TK, TJ, T2s, TI;
				   {
					E To, Tr, Tp, T35, Tq, TG;
					{
					     E T2k, Te, T2m, Tk, T2l, Th, Tn;
					     To = cr[WS(rs, 4)];
					     T2l = Tf * Tj;
					     Th = Tf * Tg;
					     T2k = FNMS(Tc, Ta, T2j);
					     Te = FMA(Tc, Td, Tb);
					     T2m = FNMS(Ti, Tg, T2l);
					     Tk = FMA(Ti, Tj, Th);
					     Tr = ci[WS(rs, 4)];
					     Tn = W[6];
					     T4r = T2k + T2m;
					     T2n = T2k - T2m;
					     T4O = Te - Tk;
					     Tl = Te + Tk;
					     Tp = Tn * To;
					     T35 = Tn * Tr;
					}
					Tq = W[7];
					TH = cr[WS(rs, 19)];
					TK = ci[WS(rs, 19)];
					TG = W[36];
					T36 = FNMS(Tq, To, T35);
					Ts = FMA(Tq, Tr, Tp);
					TJ = W[37];
					T2s = TG * TK;
					TI = TG * TH;
				   }
				   {
					E Tu, Tx, Tt, Tw, T37, Tv, TA;
					Tu = cr[WS(rs, 14)];
					Tx = ci[WS(rs, 14)];
					T2t = FNMS(TJ, TH, T2s);
					TL = FMA(TJ, TK, TI);
					Tt = W[26];
					Tw = W[27];
					TB = cr[WS(rs, 9)];
					TE = ci[WS(rs, 9)];
					T37 = Tt * Tx;
					Tv = Tt * Tu;
					TA = W[16];
					TD = W[17];
					T38 = FNMS(Tw, Tu, T37);
					Ty = FMA(Tw, Tx, Tv);
					T2q = TA * TE;
					TC = TA * TB;
				   }
			      }
			      {
				   E T39, T42, Tz, T2p, T2r, TF;
				   T39 = T36 - T38;
				   T42 = T36 + T38;
				   Tz = Ts + Ty;
				   T2p = Ts - Ty;
				   T2r = FNMS(TD, TB, T2q);
				   TF = FMA(TD, TE, TC);
				   {
					E T41, T2u, TM, T34;
					T41 = T2r + T2t;
					T2u = T2r - T2t;
					TM = TF + TL;
					T34 = TL - TF;
					T2v = T2p - T2u;
					T3v = T2p + T2u;
					T43 = T41 - T42;
					T4b = T42 + T41;
					TN = Tz - TM;
					T2b = Tz + TM;
					T3F = T39 + T34;
					T3a = T34 - T39;
				   }
			      }
			 }
		    }
	       }
	       {
		    E T3e, T1M, T2P, T25, T1V, T1Y, T1X, T3g, T1S, T2M, T1W;
		    {
			 E T21, T24, T23, T2O, T22;
			 {
			      E T1I, T1L, T1H, T1K, T3d, T1J, T20;
			      T1I = cr[WS(rs, 12)];
			      T1L = ci[WS(rs, 12)];
			      T1H = W[22];
			      T1K = W[23];
			      T21 = cr[WS(rs, 7)];
			      T24 = ci[WS(rs, 7)];
			      T3d = T1H * T1L;
			      T1J = T1H * T1I;
			      T20 = W[12];
			      T23 = W[13];
			      T3e = FNMS(T1K, T1I, T3d);
			      T1M = FMA(T1K, T1L, T1J);
			      T2O = T20 * T24;
			      T22 = T20 * T21;
			 }
			 {
			      E T1O, T1R, T1N, T1Q, T3f, T1P, T1U;
			      T1O = cr[WS(rs, 2)];
			      T1R = ci[WS(rs, 2)];
			      T2P = FNMS(T23, T21, T2O);
			      T25 = FMA(T23, T24, T22);
			      T1N = W[2];
			      T1Q = W[3];
			      T1V = cr[WS(rs, 17)];
			      T1Y = ci[WS(rs, 17)];
			      T3f = T1N * T1R;
			      T1P = T1N * T1O;
			      T1U = W[32];
			      T1X = W[33];
			      T3g = FNMS(T1Q, T1O, T3f);
			      T1S = FMA(T1Q, T1R, T1P);
			      T2M = T1U * T1Y;
			      T1W = T1U * T1V;
			 }
		    }
		    {
			 E T3h, T3S, T1T, T2L, T2N, T1Z;
			 T3h = T3e - T3g;
			 T3S = T3e + T3g;
			 T1T = T1M + T1S;
			 T2L = T1M - T1S;
			 T2N = FNMS(T1X, T1V, T2M);
			 T1Z = FMA(T1X, T1Y, T1W);
			 {
			      E T3R, T2Q, T26, T3c;
			      T3R = T2N + T2P;
			      T2Q = T2N - T2P;
			      T26 = T1Z + T25;
			      T3c = T25 - T1Z;
			      T2R = T2L - T2Q;
			      T3z = T2L + T2Q;
			      T3T = T3R - T3S;
			      T4f = T3S + T3R;
			      T27 = T1T - T26;
			      T2f = T1T + T26;
			      T3J = T3h + T3c;
			      T3i = T3c - T3h;
			 }
		    }
	       }
	       {
		    E T3l, T1l, T2I, T1E, T1u, T1x, T1w, T3n, T1r, T2F, T1v;
		    {
			 E T1A, T1D, T1C, T2H, T1B;
			 {
			      E T1h, T1k, T1g, T1j, T3k, T1i, T1z;
			      T1h = cr[WS(rs, 8)];
			      T1k = ci[WS(rs, 8)];
			      T1g = W[14];
			      T1j = W[15];
			      T1A = cr[WS(rs, 3)];
			      T1D = ci[WS(rs, 3)];
			      T3k = T1g * T1k;
			      T1i = T1g * T1h;
			      T1z = W[4];
			      T1C = W[5];
			      T3l = FNMS(T1j, T1h, T3k);
			      T1l = FMA(T1j, T1k, T1i);
			      T2H = T1z * T1D;
			      T1B = T1z * T1A;
			 }
			 {
			      E T1n, T1q, T1m, T1p, T3m, T1o, T1t;
			      T1n = cr[WS(rs, 18)];
			      T1q = ci[WS(rs, 18)];
			      T2I = FNMS(T1C, T1A, T2H);
			      T1E = FMA(T1C, T1D, T1B);
			      T1m = W[34];
			      T1p = W[35];
			      T1u = cr[WS(rs, 13)];
			      T1x = ci[WS(rs, 13)];
			      T3m = T1m * T1q;
			      T1o = T1m * T1n;
			      T1t = W[24];
			      T1w = W[25];
			      T3n = FNMS(T1p, T1n, T3m);
			      T1r = FMA(T1p, T1q, T1o);
			      T2F = T1t * T1x;
			      T1v = T1t * T1u;
			 }
		    }
		    {
			 E T3o, T3V, T1s, T2E, T2G, T1y;
			 T3o = T3l - T3n;
			 T3V = T3l + T3n;
			 T1s = T1l + T1r;
			 T2E = T1l - T1r;
			 T2G = FNMS(T1w, T1u, T2F);
			 T1y = FMA(T1w, T1x, T1v);
			 {
			      E T3U, T2J, T1F, T3j;
			      T3U = T2G + T2I;
			      T2J = T2G - T2I;
			      T1F = T1y + T1E;
			      T3j = T1E - T1y;
			      T2K = T2E - T2J;
			      T3y = T2E + T2J;
			      T3W = T3U - T3V;
			      T4e = T3V + T3U;
			      T1G = T1s - T1F;
			      T2e = T1s + T1F;
			      T3I = T3o + T3j;
			      T3p = T3j - T3o;
			 }
		    }
	       }
	       {
		    E T2Z, TT, T2A, T1c, T12, T15, T14, T31, TZ, T2x, T13;
		    {
			 E T18, T1b, T1a, T2z, T19;
			 {
			      E TP, TS, TO, TR, T2Y, TQ, T17;
			      TP = cr[WS(rs, 16)];
			      TS = ci[WS(rs, 16)];
			      TO = W[30];
			      TR = W[31];
			      T18 = cr[WS(rs, 11)];
			      T1b = ci[WS(rs, 11)];
			      T2Y = TO * TS;
			      TQ = TO * TP;
			      T17 = W[20];
			      T1a = W[21];
			      T2Z = FNMS(TR, TP, T2Y);
			      TT = FMA(TR, TS, TQ);
			      T2z = T17 * T1b;
			      T19 = T17 * T18;
			 }
			 {
			      E TV, TY, TU, TX, T30, TW, T11;
			      TV = cr[WS(rs, 6)];
			      TY = ci[WS(rs, 6)];
			      T2A = FNMS(T1a, T18, T2z);
			      T1c = FMA(T1a, T1b, T19);
			      TU = W[10];
			      TX = W[11];
			      T12 = cr[WS(rs, 1)];
			      T15 = ci[WS(rs, 1)];
			      T30 = TU * TY;
			      TW = TU * TV;
			      T11 = W[0];
			      T14 = W[1];
			      T31 = FNMS(TX, TV, T30);
			      TZ = FMA(TX, TY, TW);
			      T2x = T11 * T15;
			      T13 = T11 * T12;
			 }
		    }
		    {
			 E T32, T3Z, T10, T2w, T2y, T16;
			 T32 = T2Z - T31;
			 T3Z = T2Z + T31;
			 T10 = TT + TZ;
			 T2w = TT - TZ;
			 T2y = FNMS(T14, T12, T2x);
			 T16 = FMA(T14, T15, T13);
			 {
			      E T3Y, T2B, T1d, T2X;
			      T3Y = T2y + T2A;
			      T2B = T2y - T2A;
			      T1d = T16 + T1c;
			      T2X = T1c - T16;
			      T2C = T2w - T2B;
			      T3w = T2w + T2B;
			      T40 = T3Y - T3Z;
			      T4c = T3Z + T3Y;
			      T1e = T10 - T1d;
			      T2c = T10 + T1d;
			      T3G = T32 + T2X;
			      T33 = T2X - T32;
			 }
		    }
	       }
	       {
		    E T4l, T4k, T4w, T4x, T4Q, T4R, T2o, T4X, T4W, T4C, T4D, T4J, T4h, T4j, T4I;
		    E T51, T52, T49, T3r, T3t, T58, T2D, T48, T2S, T59;
		    {
			 E T2a, T47, T45, T3u, T3x, T3N, T3L, T3A, T46, T3Q;
			 {
			      E Tm, T1f, T28, T3X, T44;
			      T4l = T3W + T3T;
			      T3X = T3T - T3W;
			      T44 = T40 - T43;
			      T4k = T43 + T40;
			      T2a = T8 + Tl;
			      Tm = T8 - Tl;
			      T1f = TN + T1e;
			      T4w = T1e - TN;
			      T4x = T1G - T27;
			      T28 = T1G + T27;
			      T47 = FMA(KP618033988, T3X, T44);
			      T45 = FNMS(KP618033988, T44, T3X);
			      {
				   E T3H, T29, T3P, T3K, T3O;
				   T3H = T3F - T3G;
				   T4Q = T3F + T3G;
				   T29 = T1f + T28;
				   T3P = T1f - T28;
				   T4R = T3I + T3J;
				   T3K = T3I - T3J;
				   T3u = T2i + T2n;
				   T2o = T2i - T2n;
				   T4X = T3v - T3w;
				   T3x = T3v + T3w;
				   ci[WS(rs, 9)] = Tm + T29;
				   T3O = FNMS(KP250000000, T29, Tm);
				   T3N = FNMS(KP618033988, T3H, T3K);
				   T3L = FMA(KP618033988, T3K, T3H);
				   T3A = T3y + T3z;
				   T4W = T3y - T3z;
				   T46 = FMA(KP559016994, T3P, T3O);
				   T3Q = FNMS(KP559016994, T3P, T3O);
			      }
			 }
			 {
			      E T2d, T2g, T3b, T3q, T2h;
			      {
				   E T4d, T3D, T3C, T4g, T3B, T3M, T3E;
				   T4C = T4b + T4c;
				   T4d = T4b - T4c;
				   T3D = T3x - T3A;
				   T3B = T3x + T3A;
				   ci[WS(rs, 1)] = FMA(KP951056516, T45, T3Q);
				   cr[WS(rs, 2)] = FNMS(KP951056516, T45, T3Q);
				   cr[WS(rs, 6)] = FMA(KP951056516, T47, T46);
				   ci[WS(rs, 5)] = FNMS(KP951056516, T47, T46);
				   cr[WS(rs, 5)] = T3u + T3B;
				   T3C = FNMS(KP250000000, T3B, T3u);
				   T4g = T4e - T4f;
				   T4D = T4e + T4f;
				   T2d = T2b + T2c;
				   T4J = T2b - T2c;
				   T3M = FNMS(KP559016994, T3D, T3C);
				   T3E = FMA(KP559016994, T3D, T3C);
				   T4h = FMA(KP618033988, T4g, T4d);
				   T4j = FNMS(KP618033988, T4d, T4g);
				   cr[WS(rs, 9)] = FNMS(KP951056516, T3L, T3E);
				   cr[WS(rs, 1)] = FMA(KP951056516, T3L, T3E);
				   ci[WS(rs, 6)] = FMA(KP951056516, T3N, T3M);
				   ci[WS(rs, 2)] = FNMS(KP951056516, T3N, T3M);
				   T4I = T2f - T2e;
				   T2g = T2e + T2f;
			      }
			      T3b = T33 - T3a;
			      T51 = T3a + T33;
			      T52 = T3p + T3i;
			      T3q = T3i - T3p;
			      T2h = T2d + T2g;
			      T49 = T2d - T2g;
			      T3r = FMA(KP618033988, T3q, T3b);
			      T3t = FNMS(KP618033988, T3b, T3q);
			      T58 = T2v - T2C;
			      T2D = T2v + T2C;
			      cr[0] = T2a + T2h;
			      T48 = FNMS(KP250000000, T2h, T2a);
			      T2S = T2K + T2R;
			      T59 = T2K - T2R;
			 }
		    }
		    {
			 E T4B, T4P, T4Y, T50, T4U, T4S;
			 {
			      E T4A, T4y, T4s, T4m, T4u, T4t, T4z, T4v;
			      {
				   E T2V, T2U, T4i, T4a, T2T, T2W, T3s;
				   T4i = FNMS(KP559016994, T49, T48);
				   T4a = FMA(KP559016994, T49, T48);
				   T2T = T2D + T2S;
				   T2V = T2D - T2S;
				   ci[WS(rs, 3)] = FMA(KP951056516, T4h, T4a);
				   cr[WS(rs, 4)] = FNMS(KP951056516, T4h, T4a);
				   cr[WS(rs, 8)] = FMA(KP951056516, T4j, T4i);
				   ci[WS(rs, 7)] = FNMS(KP951056516, T4j, T4i);
				   ci[WS(rs, 4)] = T2o + T2T;
				   T2U = FNMS(KP250000000, T2T, T2o);
				   T4A = FMA(KP618033988, T4w, T4x);
				   T4y = FNMS(KP618033988, T4x, T4w);
				   T4B = T4r + T4q;
				   T4s = T4q - T4r;
				   T2W = FMA(KP559016994, T2V, T2U);
				   T3s = FNMS(KP559016994, T2V, T2U);
				   ci[WS(rs, 8)] = FMA(KP951056516, T3r, T2W);
				   ci[0] = FNMS(KP951056516, T3r, T2W);
				   cr[WS(rs, 7)] = FNMS(KP951056516, T3t, T3s);
				   cr[WS(rs, 3)] = FMA(KP951056516, T3t, T3s);
				   T4m = T4k + T4l;
				   T4u = T4l - T4k;
			      }
			      cr[WS(rs, 10)] = T4m - T4s;
			      T4t = FMA(KP250000000, T4m, T4s);
			      T4P = T4N - T4O;
			      T54 = T4O + T4N;
			      T4Y = FNMS(KP618033988, T4X, T4W);
			      T50 = FMA(KP618033988, T4W, T4X);
			      T4z = FNMS(KP559016994, T4u, T4t);
			      T4v = FMA(KP559016994, T4u, T4t);
			      ci[WS(rs, 13)] = FMA(KP951056516, T4y, T4v);
			      cr[WS(rs, 14)] = FMS(KP951056516, T4y, T4v);
			      ci[WS(rs, 17)] = FMA(KP951056516, T4A, T4z);
			      cr[WS(rs, 18)] = FMS(KP951056516, T4A, T4z);
			      T4U = T4Q - T4R;
			      T4S = T4Q + T4R;
			 }
			 {
			      E T4M, T4K, T4E, T4G, T4T, T4V, T4Z, T4F, T4L, T4H;
			      ci[WS(rs, 14)] = T4S + T4P;
			      T4T = FNMS(KP250000000, T4S, T4P);
			      T4M = FNMS(KP618033988, T4I, T4J);
			      T4K = FMA(KP618033988, T4J, T4I);
			      T4V = FNMS(KP559016994, T4U, T4T);
			      T4Z = FMA(KP559016994, T4U, T4T);
			      cr[WS(rs, 17)] = -(FMA(KP951056516, T4Y, T4V));
			      cr[WS(rs, 13)] = FMS(KP951056516, T4Y, T4V);
			      ci[WS(rs, 18)] = FNMS(KP951056516, T50, T4Z);
			      ci[WS(rs, 10)] = FMA(KP951056516, T50, T4Z);
			      T4E = T4C + T4D;
			      T4G = T4C - T4D;
			      ci[WS(rs, 19)] = T4E + T4B;
			      T4F = FNMS(KP250000000, T4E, T4B);
			      T5a = FMA(KP618033988, T59, T58);
			      T5c = FNMS(KP618033988, T58, T59);
			      T4L = FMA(KP559016994, T4G, T4F);
			      T4H = FNMS(KP559016994, T4G, T4F);
			      ci[WS(rs, 11)] = FMA(KP951056516, T4K, T4H);
			      cr[WS(rs, 12)] = FMS(KP951056516, T4K, T4H);
			      ci[WS(rs, 15)] = FMA(KP951056516, T4M, T4L);
			      cr[WS(rs, 16)] = FMS(KP951056516, T4M, T4L);
			      T56 = T51 - T52;
			      T53 = T51 + T52;
			 }
		    }
	       }
	  }
	  cr[WS(rs, 15)] = T53 - T54;
	  T55 = FMA(KP250000000, T53, T54);
	  T5b = FMA(KP559016994, T56, T55);
	  T57 = FNMS(KP559016994, T56, T55);
	  cr[WS(rs, 19)] = -(FMA(KP951056516, T5a, T57));
	  cr[WS(rs, 11)] = FMS(KP951056516, T5a, T57);
	  ci[WS(rs, 16)] = FNMS(KP951056516, T5c, T5b);
	  ci[WS(rs, 12)] = FMA(KP951056516, T5c, T5b);
     }
}

static const tw_instr twinstr[] = {
     {TW_FULL, 1, 20},
     {TW_NEXT, 1, 0}
};

static const hc2hc_desc desc = { 20, "hf_20", twinstr, &GENUS, {136, 38, 110, 0} };

void X(codelet_hf_20) (planner *p) {
     X(khc2hc_register) (p, hf_20, &desc);
}
#else				/* HAVE_FMA */

/* Generated by: ../../../genfft/gen_hc2hc -compact -variables 4 -pipeline-latency 4 -n 20 -dit -name hf_20 -include hf.h */

/*
 * This function contains 246 FP additions, 124 FP multiplications,
 * (or, 184 additions, 62 multiplications, 62 fused multiply/add),
 * 85 stack variables, 4 constants, and 80 memory accesses
 */
#include "hf.h"

static void hf_20(R *cr, R *ci, const R *W, stride rs, INT mb, INT me, INT ms)
{
     DK(KP587785252, +0.587785252292473129168705954639072768597652438);
     DK(KP951056516, +0.951056516295153572116439333379382143405698634);
     DK(KP250000000, +0.250000000000000000000000000000000000000000000);
     DK(KP559016994, +0.559016994374947424102293417182819058860154590);
     INT m;
     for (m = mb, W = W + ((mb - 1) * 38); m < me; m = m + 1, cr = cr + ms, ci = ci - ms, W = W + 38, MAKE_VOLATILE_STRIDE(rs)) {
	  E Tj, T1R, T4j, T4s, T2q, T37, T3Q, T42, T1r, T1O, T1P, T3i, T3l, T3J, T3D;
	  E T3E, T44, T1V, T1W, T1X, T2e, T2j, T2k, T2W, T2X, T4f, T33, T34, T35, T2J;
	  E T2O, T4q, TG, T13, T14, T3p, T3s, T3K, T3A, T3B, T43, T1S, T1T, T1U, T23;
	  E T28, T29, T2T, T2U, T4e, T30, T31, T32, T2y, T2D, T4p;
	  {
	       E T1, T3N, T6, T3M, Tc, T2n, Th, T2o;
	       T1 = cr[0];
	       T3N = ci[0];
	       {
		    E T3, T5, T2, T4;
		    T3 = cr[WS(rs, 10)];
		    T5 = ci[WS(rs, 10)];
		    T2 = W[18];
		    T4 = W[19];
		    T6 = FMA(T2, T3, T4 * T5);
		    T3M = FNMS(T4, T3, T2 * T5);
	       }
	       {
		    E T9, Tb, T8, Ta;
		    T9 = cr[WS(rs, 5)];
		    Tb = ci[WS(rs, 5)];
		    T8 = W[8];
		    Ta = W[9];
		    Tc = FMA(T8, T9, Ta * Tb);
		    T2n = FNMS(Ta, T9, T8 * Tb);
	       }
	       {
		    E Te, Tg, Td, Tf;
		    Te = cr[WS(rs, 15)];
		    Tg = ci[WS(rs, 15)];
		    Td = W[28];
		    Tf = W[29];
		    Th = FMA(Td, Te, Tf * Tg);
		    T2o = FNMS(Tf, Te, Td * Tg);
	       }
	       {
		    E T7, Ti, T4h, T4i;
		    T7 = T1 + T6;
		    Ti = Tc + Th;
		    Tj = T7 - Ti;
		    T1R = T7 + Ti;
		    T4h = T3N - T3M;
		    T4i = Tc - Th;
		    T4j = T4h - T4i;
		    T4s = T4i + T4h;
	       }
	       {
		    E T2m, T2p, T3O, T3P;
		    T2m = T1 - T6;
		    T2p = T2n - T2o;
		    T2q = T2m - T2p;
		    T37 = T2m + T2p;
		    T3O = T3M + T3N;
		    T3P = T2n + T2o;
		    T3Q = T3O - T3P;
		    T42 = T3P + T3O;
	       }
	  }
	  {
	       E T1f, T3g, T2a, T2H, T1N, T3j, T2i, T2N, T1q, T3h, T2d, T2I, T1C, T3k, T2f;
	       E T2M;
	       {
		    E T19, T2F, T1e, T2G;
		    {
			 E T16, T18, T15, T17;
			 T16 = cr[WS(rs, 8)];
			 T18 = ci[WS(rs, 8)];
			 T15 = W[14];
			 T17 = W[15];
			 T19 = FMA(T15, T16, T17 * T18);
			 T2F = FNMS(T17, T16, T15 * T18);
		    }
		    {
			 E T1b, T1d, T1a, T1c;
			 T1b = cr[WS(rs, 18)];
			 T1d = ci[WS(rs, 18)];
			 T1a = W[34];
			 T1c = W[35];
			 T1e = FMA(T1a, T1b, T1c * T1d);
			 T2G = FNMS(T1c, T1b, T1a * T1d);
		    }
		    T1f = T19 + T1e;
		    T3g = T2F + T2G;
		    T2a = T19 - T1e;
		    T2H = T2F - T2G;
	       }
	       {
		    E T1H, T2g, T1M, T2h;
		    {
			 E T1E, T1G, T1D, T1F;
			 T1E = cr[WS(rs, 17)];
			 T1G = ci[WS(rs, 17)];
			 T1D = W[32];
			 T1F = W[33];
			 T1H = FMA(T1D, T1E, T1F * T1G);
			 T2g = FNMS(T1F, T1E, T1D * T1G);
		    }
		    {
			 E T1J, T1L, T1I, T1K;
			 T1J = cr[WS(rs, 7)];
			 T1L = ci[WS(rs, 7)];
			 T1I = W[12];
			 T1K = W[13];
			 T1M = FMA(T1I, T1J, T1K * T1L);
			 T2h = FNMS(T1K, T1J, T1I * T1L);
		    }
		    T1N = T1H + T1M;
		    T3j = T2g + T2h;
		    T2i = T2g - T2h;
		    T2N = T1H - T1M;
	       }
	       {
		    E T1k, T2b, T1p, T2c;
		    {
			 E T1h, T1j, T1g, T1i;
			 T1h = cr[WS(rs, 13)];
			 T1j = ci[WS(rs, 13)];
			 T1g = W[24];
			 T1i = W[25];
			 T1k = FMA(T1g, T1h, T1i * T1j);
			 T2b = FNMS(T1i, T1h, T1g * T1j);
		    }
		    {
			 E T1m, T1o, T1l, T1n;
			 T1m = cr[WS(rs, 3)];
			 T1o = ci[WS(rs, 3)];
			 T1l = W[4];
			 T1n = W[5];
			 T1p = FMA(T1l, T1m, T1n * T1o);
			 T2c = FNMS(T1n, T1m, T1l * T1o);
		    }
		    T1q = T1k + T1p;
		    T3h = T2b + T2c;
		    T2d = T2b - T2c;
		    T2I = T1k - T1p;
	       }
	       {
		    E T1w, T2K, T1B, T2L;
		    {
			 E T1t, T1v, T1s, T1u;
			 T1t = cr[WS(rs, 12)];
			 T1v = ci[WS(rs, 12)];
			 T1s = W[22];
			 T1u = W[23];
			 T1w = FMA(T1s, T1t, T1u * T1v);
			 T2K = FNMS(T1u, T1t, T1s * T1v);
		    }
		    {
			 E T1y, T1A, T1x, T1z;
			 T1y = cr[WS(rs, 2)];
			 T1A = ci[WS(rs, 2)];
			 T1x = W[2];
			 T1z = W[3];
			 T1B = FMA(T1x, T1y, T1z * T1A);
			 T2L = FNMS(T1z, T1y, T1x * T1A);
		    }
		    T1C = T1w + T1B;
		    T3k = T2K + T2L;
		    T2f = T1w - T1B;
		    T2M = T2K - T2L;
	       }
	       T1r = T1f - T1q;
	       T1O = T1C - T1N;
	       T1P = T1r + T1O;
	       T3i = T3g - T3h;
	       T3l = T3j - T3k;
	       T3J = T3l - T3i;
	       T3D = T3g + T3h;
	       T3E = T3k + T3j;
	       T44 = T3D + T3E;
	       T1V = T1f + T1q;
	       T1W = T1C + T1N;
	       T1X = T1V + T1W;
	       T2e = T2a - T2d;
	       T2j = T2f - T2i;
	       T2k = T2e + T2j;
	       T2W = T2H - T2I;
	       T2X = T2M - T2N;
	       T4f = T2W + T2X;
	       T33 = T2a + T2d;
	       T34 = T2f + T2i;
	       T35 = T33 + T34;
	       T2J = T2H + T2I;
	       T2O = T2M + T2N;
	       T4q = T2J + T2O;
	  }
	  {
	       E Tu, T3n, T1Z, T2w, T12, T3r, T27, T2z, TF, T3o, T22, T2x, TR, T3q, T24;
	       E T2C;
	       {
		    E To, T2u, Tt, T2v;
		    {
			 E Tl, Tn, Tk, Tm;
			 Tl = cr[WS(rs, 4)];
			 Tn = ci[WS(rs, 4)];
			 Tk = W[6];
			 Tm = W[7];
			 To = FMA(Tk, Tl, Tm * Tn);
			 T2u = FNMS(Tm, Tl, Tk * Tn);
		    }
		    {
			 E Tq, Ts, Tp, Tr;
			 Tq = cr[WS(rs, 14)];
			 Ts = ci[WS(rs, 14)];
			 Tp = W[26];
			 Tr = W[27];
			 Tt = FMA(Tp, Tq, Tr * Ts);
			 T2v = FNMS(Tr, Tq, Tp * Ts);
		    }
		    Tu = To + Tt;
		    T3n = T2u + T2v;
		    T1Z = To - Tt;
		    T2w = T2u - T2v;
	       }
	       {
		    E TW, T25, T11, T26;
		    {
			 E TT, TV, TS, TU;
			 TT = cr[WS(rs, 1)];
			 TV = ci[WS(rs, 1)];
			 TS = W[0];
			 TU = W[1];
			 TW = FMA(TS, TT, TU * TV);
			 T25 = FNMS(TU, TT, TS * TV);
		    }
		    {
			 E TY, T10, TX, TZ;
			 TY = cr[WS(rs, 11)];
			 T10 = ci[WS(rs, 11)];
			 TX = W[20];
			 TZ = W[21];
			 T11 = FMA(TX, TY, TZ * T10);
			 T26 = FNMS(TZ, TY, TX * T10);
		    }
		    T12 = TW + T11;
		    T3r = T25 + T26;
		    T27 = T25 - T26;
		    T2z = T11 - TW;
	       }
	       {
		    E Tz, T20, TE, T21;
		    {
			 E Tw, Ty, Tv, Tx;
			 Tw = cr[WS(rs, 9)];
			 Ty = ci[WS(rs, 9)];
			 Tv = W[16];
			 Tx = W[17];
			 Tz = FMA(Tv, Tw, Tx * Ty);
			 T20 = FNMS(Tx, Tw, Tv * Ty);
		    }
		    {
			 E TB, TD, TA, TC;
			 TB = cr[WS(rs, 19)];
			 TD = ci[WS(rs, 19)];
			 TA = W[36];
			 TC = W[37];
			 TE = FMA(TA, TB, TC * TD);
			 T21 = FNMS(TC, TB, TA * TD);
		    }
		    TF = Tz + TE;
		    T3o = T20 + T21;
		    T22 = T20 - T21;
		    T2x = Tz - TE;
	       }
	       {
		    E TL, T2A, TQ, T2B;
		    {
			 E TI, TK, TH, TJ;
			 TI = cr[WS(rs, 16)];
			 TK = ci[WS(rs, 16)];
			 TH = W[30];
			 TJ = W[31];
			 TL = FMA(TH, TI, TJ * TK);
			 T2A = FNMS(TJ, TI, TH * TK);
		    }
		    {
			 E TN, TP, TM, TO;
			 TN = cr[WS(rs, 6)];
			 TP = ci[WS(rs, 6)];
			 TM = W[10];
			 TO = W[11];
			 TQ = FMA(TM, TN, TO * TP);
			 T2B = FNMS(TO, TN, TM * TP);
		    }
		    TR = TL + TQ;
		    T3q = T2A + T2B;
		    T24 = TL - TQ;
		    T2C = T2A - T2B;
	       }
	       TG = Tu - TF;
	       T13 = TR - T12;
	       T14 = TG + T13;
	       T3p = T3n - T3o;
	       T3s = T3q - T3r;
	       T3K = T3p + T3s;
	       T3A = T3n + T3o;
	       T3B = T3q + T3r;
	       T43 = T3A + T3B;
	       T1S = Tu + TF;
	       T1T = TR + T12;
	       T1U = T1S + T1T;
	       T23 = T1Z - T22;
	       T28 = T24 - T27;
	       T29 = T23 + T28;
	       T2T = T2w - T2x;
	       T2U = T2C + T2z;
	       T4e = T2T + T2U;
	       T30 = T1Z + T22;
	       T31 = T24 + T27;
	       T32 = T30 + T31;
	       T2y = T2w + T2x;
	       T2D = T2z - T2C;
	       T4p = T2D - T2y;
	  }
	  {
	       E T3e, T1Q, T3d, T3u, T3w, T3m, T3t, T3v, T3f;
	       T3e = KP559016994 * (T14 - T1P);
	       T1Q = T14 + T1P;
	       T3d = FNMS(KP250000000, T1Q, Tj);
	       T3m = T3i + T3l;
	       T3t = T3p - T3s;
	       T3u = FNMS(KP587785252, T3t, KP951056516 * T3m);
	       T3w = FMA(KP951056516, T3t, KP587785252 * T3m);
	       ci[WS(rs, 9)] = Tj + T1Q;
	       T3v = T3e + T3d;
	       ci[WS(rs, 5)] = T3v - T3w;
	       cr[WS(rs, 6)] = T3v + T3w;
	       T3f = T3d - T3e;
	       cr[WS(rs, 2)] = T3f - T3u;
	       ci[WS(rs, 1)] = T3f + T3u;
	  }
	  {
	       E T36, T38, T39, T2Z, T3c, T2V, T2Y, T3b, T3a;
	       T36 = KP559016994 * (T32 - T35);
	       T38 = T32 + T35;
	       T39 = FNMS(KP250000000, T38, T37);
	       T2V = T2T - T2U;
	       T2Y = T2W - T2X;
	       T2Z = FMA(KP951056516, T2V, KP587785252 * T2Y);
	       T3c = FNMS(KP587785252, T2V, KP951056516 * T2Y);
	       cr[WS(rs, 5)] = T37 + T38;
	       T3b = T39 - T36;
	       ci[WS(rs, 2)] = T3b - T3c;
	       ci[WS(rs, 6)] = T3c + T3b;
	       T3a = T36 + T39;
	       cr[WS(rs, 1)] = T2Z + T3a;
	       cr[WS(rs, 9)] = T3a - T2Z;
	  }
	  {
	       E T3x, T1Y, T3y, T3G, T3I, T3C, T3F, T3H, T3z;
	       T3x = KP559016994 * (T1U - T1X);
	       T1Y = T1U + T1X;
	       T3y = FNMS(KP250000000, T1Y, T1R);
	       T3C = T3A - T3B;
	       T3F = T3D - T3E;
	       T3G = FMA(KP951056516, T3C, KP587785252 * T3F);
	       T3I = FNMS(KP587785252, T3C, KP951056516 * T3F);
	       cr[0] = T1R + T1Y;
	       T3H = T3y - T3x;
	       ci[WS(rs, 7)] = T3H - T3I;
	       cr[WS(rs, 8)] = T3H + T3I;
	       T3z = T3x + T3y;
	       cr[WS(rs, 4)] = T3z - T3G;
	       ci[WS(rs, 3)] = T3z + T3G;
	  }
	  {
	       E T2l, T2r, T2s, T2Q, T2R, T2E, T2P, T2S, T2t;
	       T2l = KP559016994 * (T29 - T2k);
	       T2r = T29 + T2k;
	       T2s = FNMS(KP250000000, T2r, T2q);
	       T2E = T2y + T2D;
	       T2P = T2J - T2O;
	       T2Q = FMA(KP951056516, T2E, KP587785252 * T2P);
	       T2R = FNMS(KP587785252, T2E, KP951056516 * T2P);
	       ci[WS(rs, 4)] = T2q + T2r;
	       T2S = T2s - T2l;
	       cr[WS(rs, 3)] = T2R + T2S;
	       cr[WS(rs, 7)] = T2S - T2R;
	       T2t = T2l + T2s;
	       ci[0] = T2t - T2Q;
	       ci[WS(rs, 8)] = T2Q + T2t;
	  }
	  {
	       E T3U, T3L, T3V, T3T, T3X, T3R, T3S, T3Y, T3W;
	       T3U = KP559016994 * (T3K + T3J);
	       T3L = T3J - T3K;
	       T3V = FMA(KP250000000, T3L, T3Q);
	       T3R = T13 - TG;
	       T3S = T1r - T1O;
	       T3T = FNMS(KP587785252, T3S, KP951056516 * T3R);
	       T3X = FMA(KP587785252, T3R, KP951056516 * T3S);
	       cr[WS(rs, 10)] = T3L - T3Q;
	       T3Y = T3V - T3U;
	       cr[WS(rs, 18)] = T3X - T3Y;
	       ci[WS(rs, 17)] = T3X + T3Y;
	       T3W = T3U + T3V;
	       cr[WS(rs, 14)] = T3T - T3W;
	       ci[WS(rs, 13)] = T3T + T3W;
	  }
	  {
	       E T4g, T4k, T4l, T4d, T4n, T4b, T4c, T4o, T4m;
	       T4g = KP559016994 * (T4e - T4f);
	       T4k = T4e + T4f;
	       T4l = FNMS(KP250000000, T4k, T4j);
	       T4b = T33 - T34;
	       T4c = T30 - T31;
	       T4d = FNMS(KP587785252, T4c, KP951056516 * T4b);
	       T4n = FMA(KP951056516, T4c, KP587785252 * T4b);
	       ci[WS(rs, 14)] = T4k + T4j;
	       T4o = T4g + T4l;
	       ci[WS(rs, 10)] = T4n + T4o;
	       ci[WS(rs, 18)] = T4o - T4n;
	       T4m = T4g - T4l;
	       cr[WS(rs, 13)] = T4d + T4m;
	       cr[WS(rs, 17)] = T4m - T4d;
	  }
	  {
	       E T47, T45, T46, T41, T49, T3Z, T40, T4a, T48;
	       T47 = KP559016994 * (T43 - T44);
	       T45 = T43 + T44;
	       T46 = FNMS(KP250000000, T45, T42);
	       T3Z = T1S - T1T;
	       T40 = T1V - T1W;
	       T41 = FNMS(KP951056516, T40, KP587785252 * T3Z);
	       T49 = FMA(KP951056516, T3Z, KP587785252 * T40);
	       ci[WS(rs, 19)] = T45 + T42;
	       T4a = T47 + T46;
	       cr[WS(rs, 16)] = T49 - T4a;
	       ci[WS(rs, 15)] = T49 + T4a;
	       T48 = T46 - T47;
	       cr[WS(rs, 12)] = T41 - T48;
	       ci[WS(rs, 11)] = T41 + T48;
	  }
	  {
	       E T4w, T4r, T4x, T4v, T4z, T4t, T4u, T4A, T4y;
	       T4w = KP559016994 * (T4p + T4q);
	       T4r = T4p - T4q;
	       T4x = FMA(KP250000000, T4r, T4s);
	       T4t = T23 - T28;
	       T4u = T2e - T2j;
	       T4v = FMA(KP951056516, T4t, KP587785252 * T4u);
	       T4z = FNMS(KP587785252, T4t, KP951056516 * T4u);
	       cr[WS(rs, 15)] = T4r - T4s;
	       T4A = T4w + T4x;
	       ci[WS(rs, 12)] = T4z + T4A;
	       ci[WS(rs, 16)] = T4A - T4z;
	       T4y = T4w - T4x;
	       cr[WS(rs, 11)] = T4v + T4y;
	       cr[WS(rs, 19)] = T4y - T4v;
	  }
     }
}

static const tw_instr twinstr[] = {
     {TW_FULL, 1, 20},
     {TW_NEXT, 1, 0}
};

static const hc2hc_desc desc = { 20, "hf_20", twinstr, &GENUS, {184, 62, 62, 0} };

void X(codelet_hf_20) (planner *p) {
     X(khc2hc_register) (p, hf_20, &desc);
}
#endif				/* HAVE_FMA */
