/* $Id$ */
//
// Проект HF Dream Receiver (КВ приёмник мечты)
// автор Гена Завидовский mgs2001@mail.ru
// UA1ARN
//

#include "hardware.h"
#include "board.h"
#include "audio.h"

#include "display/display.h"
#include "formats.h"

#include <string.h>
#include <math.h>
#include "gui.h"
#include "touch/touch.h"

#if (DIM_X < GUIMINX || DIM_Y < GUIMINY) && WITHTOUCHGUI	// не соблюдены минимальные требования к разрешению экрана
#undef WITHTOUCHGUI											// для функционирования touch GUI
#endif

#if LCDMODE_LTDC

#include "keyboard.h"
#include "./display/fontmaps.h"

/*-----------------------------------------------------  V_Bre
 * void V_Bre (int xn, int yn, int xk, int yk)
 *
 * Подпрограмма иллюстрирующая построение вектора из точки
 * (xn,yn) в точку (xk, yk) методом Брезенхема.
 *
 * Построение ведется от точки с меньшими  координатами
 * к точке с большими координатами с единичным шагом по
 * координате с большим приращением.
 *
 * В общем случае исходный вектор проходит не через вершины
 * растровой сетки, а пересекает ее стороны.
 * Пусть приращение по X больше приращения по Y и оба они > 0.
 * Для очередного значения X нужно выбрать одну двух ближайших
 * координат сетки по Y.
 * Для этого проверяется как проходит  исходный  вектор - выше
 * или ниже середины расстояния между ближайшими значениями Y.
 * Если выше середины,  то Y-координату  надо  увеличить на 1,
 * иначе оставить прежней.
 * Для этой проверки анализируется знак переменной s,
 * соответствующей разности между истинным положением и
 * серединой расстояния между ближайшими Y-узлами сетки.
 */

void display_line(int xn, int yn, int xk, int yk, COLORMAIN_T color)
{
	int  dx, dy, s, sx, sy, kl, incr1, incr2;
	char swap;

	/* Вычисление приращений и шагов */
	sx = 0;
	if ((dx= xk-xn) < 0)
	{
		dx = - dx;
		-- sx;
	}
	else if (dx > 0)
		++ sx;
	sy = 0;

	if ((dy= yk-yn) < 0)
	{
		dy = - dy;
		-- sy;
	}
	else if (dy>0)
		++ sy;
	/* Учет наклона */
	swap = 0;
	if ((kl= dx) < (s= dy))
	{
		dx= s;  dy= kl;  kl= s; ++swap;
	}
	s = (incr1= 2 * dy) - dx; /* incr1 - констан. перевычисления */
	/* разности если текущее s < 0  и  */
	/* s - начальное значение разности */
	incr2 = 2 * dx;         /* Константа для перевычисления    */
	/* разности если текущее s >= 0    */
	display_putpixel(xn, yn, color); /* Первый  пиксел вектора       */

	while (--kl >= 0)
	{
		if (s >= 0)
		{
			if (swap) xn+= sx;
			else yn+= sy;
			s-= incr2;
		}
		if (swap)
			yn+= sy;
		else
			xn+= sx;
		s += incr1;
		display_putpixel(xn, yn, color); /* Текущая  точка  вектора   */
	}
}  /* V_Bre */

const int sin90 [91] =
{
	0, 175, 349, 523, 698, 872,1045,1219,1392,   /*  0..8        */
	1564,1736,1908,2079,2250,2419,2588,2756,2924,   /*  9..17       */
	3090,3256,3420,3584,3746,3907,4067,4226,4384,   /* 18..26       */
	4540,4695,4848,5000,5150,5299,5446,5592,5736,
	5878,6018,6157,6293,6428,6561,6691,6820,6947,
	7071,7193,7314,7431,7547,7660,7771,7880,7986,
	8090,8192,8290,8387,8480,8572,8660,8746,8829,
	8910,8988,9063,9135,9205,9272,9336,9397,9455,
	9511,9563,9613,9659,9703,9744,9781,9816,9848,
	9877,9903,9925,9945,9962,9976,9986,9994,9998,   /* 81..89       */
	10000                                           /* 90           */
};


static int muldiv(int a, int b, unsigned c)
{
	return  (unsigned) ((a * (long) b + (c / 2)) / c);
}

static int isin(unsigned alpha, unsigned r)
{
	while (alpha >= 360)
		alpha -= 360;

	if (alpha < 90)         /* 0..3 hours   */
		return muldiv(sin90 [ alpha ], r, 10000);
	if (alpha < 180)        /* 9..0 hours   */
		return muldiv(sin90 [ 180 - alpha ], r, 10000);
	if (alpha < 270)        /* 6..9 hours   */
		return - muldiv(sin90 [ alpha - 180], r, 10000);
				/* 3..6 hours   */
	return - muldiv(sin90 [ 360 - alpha ], r, 10000);
}

static  int icos(unsigned alpha, unsigned r)
{
	return isin(alpha + 90, r * 20 / 10);
}


// Рисование радиусов
void
display_radius(int xc, int yc, unsigned gs, unsigned r1, unsigned r2, COLORMAIN_T color)
{
	int     x, y;
	int     x2, y2;

	x = xc + icos(gs, r1);
	y = yc + isin(gs, r1);
	x2 = xc + icos(gs, r2);
	y2 = yc + isin(gs, r2);

	display_line(x, y, x2, y2, color);

}

void polar_to_dek(uint_fast16_t xc, uint_fast16_t yc, uint_fast16_t gs, uint_fast16_t r, uint_fast16_t * x, uint_fast16_t * y)
{
	* x = xc + icos(gs, r);
	* y = yc + isin(gs, r);
}

// круговой интерполятор
// нач.-x, нач.-y, градус начала, градус конуа, радиус, шаг приращения угла
void
display_segm(int xc, int yc, unsigned gs, unsigned ge, unsigned r, int step, COLORMAIN_T color)
{
	int     x, y;
	int     xo, yo;
	char     first;
	int     vcos, vsin;

	if (gs == ge)   return;
	first = 1;
	while (gs != ge)
	{
		vsin = isin(gs, r);
		vcos = icos(gs, r);
		x = xc + vcos;
		y = yc + vsin;

		if (first != 0) // 1-я точка
		{
			// переместить к началу рисования
			xo = x, yo = y;
			first = 0;
		}
		else
		{  // рисовать элемент окружности
			display_line(xo, yo, x, y, color);
			xo = x, yo = y;
		}
		if (ge == 360)
			ge = 0;
		if (step < 0)
		{
			gs += step;
			if (gs >= 360)
				gs += 360;
		}
		else
		{
			gs += step;
			if (gs >= 360)
				gs -= 360;
		}
	}

	if (first == 0)
	{
		// завершение окружности
		vsin = isin(ge, r);
		vcos = icos(ge, r);
		x = xc + vcos;
		y = yc + vsin;

		display_line(xo, yo, x, y, color); // рисовать линию
	}

}

#endif /* LCDMODE_LTDC */

#if LCDMODE_LTDC

#include "./display/fontmaps.h"

static
uint_fast16_t normalize(
	uint_fast16_t raw,
	uint_fast16_t rawmin,
	uint_fast16_t rawmax,
	uint_fast16_t range
	)
{
	if (rawmin < rawmax)
	{
		// Normal direction
		const uint_fast16_t distance = rawmax - rawmin;
		if (raw < rawmin)
			return 0;
		raw = raw - rawmin;
		if (raw > distance)
			return range;
		return (uint_fast32_t) raw * range / distance;
	}
	else
	{
		// reverse direction
		const uint_fast16_t distance = rawmin - rawmax;
		if (raw >= rawmin)
			return 0;
		raw = rawmin - raw;
		if (raw > distance)
			return range;
		return (uint_fast32_t) raw * range / distance;
	}
}

static
uint_fast16_t normalize3(
	uint_fast16_t raw,
	uint_fast16_t rawmin,
	uint_fast16_t rawmid,
	uint_fast16_t rawmax,
	uint_fast16_t range1,
	uint_fast16_t range2
	)
{
	if (raw < rawmid)
		return normalize(raw, rawmin, rawmid, range1);
	else
		return normalize(raw - rawmid, 0, rawmax - rawmid, range2 - range1) + range1;
}

// ширина занимаемого места - 15 ячеек (240/16 = 15)
void
display2_smeter15(
	uint_fast8_t xgrid,
	uint_fast8_t ygrid,
	dctx_t * pctx
	)
{
	/* получение координат прямоугольника с изображением */
	const uint_fast16_t width = GRID2X(15);
	const uint_fast16_t height = GRID2Y(14);
	const uint_fast16_t x0 = GRID2X(xgrid);
	const uint_fast16_t y0 = GRID2Y(ygrid);
	const int xc = x0 + width / 2;
	const int yc = y0 + 120;

	enum { halfsect = 30 };
	enum { gm = 270 };
	enum { gs = gm - halfsect };
	const int ge = gm + halfsect;
	const int stripewidth = 12; //16;
	const int r1 = 7 * GRID2Y(3) - 8;	//350;
	const int r2 = r1 - stripewidth;

	const uint_fast8_t is_tx = hamradio_get_tx();

	int gv, gv_trace = gs, gswr = gs;
	uint_fast16_t swr10; 														// swr10 = 0..30 for swr 1..4

	//colpip_rect(colmain_fb_draw(), DIM_X, DIM_Y, x0, y0 - 8, x0 + width - 1, y0 + height - 1 + 15, DESIGNCOLORSTATE, 0);

	if (is_tx)																	// угол поворота стрелки; 246 минимум, 270 середина, 294 максимум
	{																			// добавить учет калибровок
		enum { gx_hyst = 3 };		// гистерезис в градусах
		/* фильтрация - (в градусах) */
		static uint_fast16_t gv_smooth = gs;
		static uint_fast16_t gswr_smooth = gs;

		uint_fast8_t power, tracemax;
		adcvalholder_t forward, reflected;

		power = board_getpwrmeter(& tracemax);
		power = power * power * (ge - gs) / (maxpwrcali * maxpwrcali);
		gv = gs + normalize(power, 0, 200, ge - gs);

		forward = board_getswrmeter(& reflected, swrcalibr);
		const uint_fast16_t fullscale = (SWRMIN * 40 / 10) - SWRMIN;
									// рассчитанное  значение
		if (forward < minforward)
			swr10 = 0;				// SWR=1
		else if (forward <= reflected)
			swr10 = fullscale;		// SWR is infinite
		else
			swr10 = (forward + reflected) * SWRMIN / (forward - reflected) - SWRMIN;
		gswr = gs + normalize(swr10, 0, 30, ge - gs);

		if (gv > gs)
			gv_smooth = gv;

		if (gv == gs && gv_smooth > gs)
			gv = (gv_smooth -= gx_hyst) > gs ? gv_smooth : gs;

		if (gswr > gs)
			gswr_smooth = gswr;

		if (gswr == gs && gswr_smooth > gs)
			gswr = (gswr_smooth -= gx_hyst) > gs ? gswr_smooth : gs;
	}
	else
	{
		uint_fast8_t tracemax;
		uint_fast8_t value = board_getsmeter(& tracemax, 0, UINT8_MAX, 0);
		tracemax = value > tracemax ? value : tracemax;	// защита от рассогласования значений

		gv =
			gs + normalize3(value, 		s9level - s9delta, s9level, s9level + s9_60_delta, gm - gs, ge - gs);
		gv_trace =
			gs + normalize3(tracemax, 	s9level - s9delta, s9level, s9level + s9_60_delta, gm - gs, ge - gs);
	}

	int rv1 = 7 * GRID2Y(3);	//350;
	int rv2 = rv1 - 3 * GRID2Y(3);
	enum { step1 = 3 };		// шаг для оцифровки S
	enum { step2 = 4 };		// шаг для оцифровки плюсов
	enum { step3 = 20 };	// swr
	static const int markersTX_pwr [] =
	{
		gs,
		gs + 2 * step1,
		gs + 4 * step1,
		gs + 6 * step1,
		gs + 8 * step1,
		gs + 10 * step1,
		gs + 12 * step1,
		gs + 14 * step1,
		gs + 16 * step1,
		gs + 18 * step1,
		gs + 20 * step1,
	};
	static const int markersTX_swr [] =
	{
		gs,
		gs + step3,
		gs + 2 * step3,
		gs + 3 * step3,
	};
	static const int markers [] =
	{
		//gs + 0 * step1,
		gs + 2 * step1,		// S1
		gs + 4 * step1,		// S3
		gs + 6 * step1,		// S5
		gs + 8 * step1,		// S7
		gs + 10 * step1,	// S9
	};
	static const int markersR [] =
	{
		gm + 2 * step2,	//
		gm + 4 * step2,
		gm + 6 * step2,
	};
	static const int markers2 [] =
	{
		//gs + 1 * step1,
		gs + 3 * step1,		// S2
		gs + 5 * step1,		// S4
		gs + 7 * step1,		// S6
		gs + 9 * step1,		// S8
	};
	static const int markers2R [] =
	{
		gm + 1 * step2,
		gm + 3 * step2,
		gm + 5 * step2,
	};

	const COLORMAIN_T smeter = COLORMAIN_WHITE;
	const COLORMAIN_T smeterplus = COLORMAIN_DARKRED;
	const uint_fast16_t pad2w3 = strwidth3("ZZ");

//	static uint_fast8_t first_run = 1;
//	static uint_fast16_t x1, y1, x2, y2;
//
//	if(first_run)
//	{
//		uint_fast16_t xx, yy;
//		first_run = 0;
//		polar_to_dek(xc, yc, gm, rv1 + 8 + SMALLCHARH3, & xx, & yy);
//		y1 = yy;
//		polar_to_dek(xc, yc, gs, rv2, & xx, & yy);
//		y2 = yy;
//		polar_to_dek(xc, yc, gs, r1 + 8 + SMALLCHARW3 * 2, & xx, & yy);
//		x1 = xx;
//		polar_to_dek(xc, yc, ge, r1 + 8 + SMALLCHARW3 * 2, & xx, & yy);
//		x2 = xx;
//		PRINTF("xc/yc - %d/%d\n", xc, yc);
//		PRINTF("x1/y1 - %d/%d\n", x1, y1);
//		PRINTF("x2/y2 - %d/%d\n", x2, y2);
//	}
//	colpip_rect(colmain_fb_draw(), DIM_X, DIM_Y, x1, y1, x2, y2, COLORPIP_WHITE, 0);

	if (is_tx)																	// шкала при передаче
	{
		unsigned p;
		unsigned i;
		for (p = 0, i = 0; i < sizeof markersTX_pwr / sizeof markersTX_pwr [0]; ++ i, p += 10)
		{
			char buf [10];
			uint_fast16_t xx, yy;
			if (i % 2 == 0)
			{
				display_radius(xc, yc, markersTX_pwr [i], r1, r1 + 8, smeter);
				polar_to_dek(xc, yc, markersTX_pwr [i], r1 + 8, & xx, & yy);
				local_snprintf_P(buf, sizeof buf / sizeof buf [0], PSTR("%u"), p);
				display_string3_at_xy(xx - strwidth3(buf) / 2, yy - pad2w3, buf, COLORMAIN_YELLOW, COLORMAIN_BLACK);
			}
			else
				display_radius(xc, yc, markersTX_pwr [i], r1, r1 + 4, smeter);
		}

		for (p = 1, i = 0; i < sizeof markersTX_swr / sizeof markersTX_swr [0]; ++ i, p += 1)
		{
			char buf [10];
			uint_fast16_t xx, yy;
			display_radius(xc, yc, markersTX_swr [i], r2, r2 - 8, smeter);
			polar_to_dek(xc, yc, markersTX_swr [i], r2 - 16, & xx, & yy);
			local_snprintf_P(buf, sizeof buf / sizeof buf [0], PSTR("%u"), p);
			display_string3_at_xy(xx - SMALLCHARW3 / 2, yy - SMALLCHARW3 / 2, buf, COLORMAIN_YELLOW, COLORMAIN_BLACK);
		}
	}
	else																		// шкала при приеме
	{
		unsigned p;
		unsigned i;
		for (p = 1, i = 0; i < sizeof markers / sizeof markers [0]; ++ i, p += 2)
		{
			char buf [10];
			uint_fast16_t xx, yy;
			display_radius(xc, yc, markers [i], r1, r1 + 8, smeter);
			polar_to_dek(xc, yc, markers [i], r1 + 8, & xx, & yy);
			local_snprintf_P(buf, sizeof buf / sizeof buf [0], PSTR("%u"), p);
			display_string3_at_xy(xx - SMALLCHARW3 / 2, yy - pad2w3, buf, COLORMAIN_YELLOW, COLORMAIN_BLACK);
		}
		for (i = 0; i < sizeof markers2 / sizeof markers2 [0]; ++ i)
		{
			display_radius(xc, yc, markers2 [i], r1, r1 + 4, smeter);
		}

		for (p = 20, i = 0; i < sizeof markersR / sizeof markersR [0]; ++ i, p += 20)
		{
			char buf [10];
			uint_fast16_t xx, yy;
			display_radius(xc, yc, markersR [i], r1, r1 + 8, smeterplus);
			polar_to_dek(xc, yc, markersR [i], r1 + 8, & xx, & yy);
			local_snprintf_P(buf, sizeof buf / sizeof buf [0], PSTR("+%u"), p);
			display_string3_at_xy(xx - strwidth3(buf) / 2, yy - pad2w3, buf, COLORMAIN_RED, COLORMAIN_BLACK);
		}
		for (i = 0; i < sizeof markers2R / sizeof markers2R [0]; ++ i)
		{
			display_radius(xc, yc, markers2R [i], r1, r1 + 4, smeterplus);
		}
	}

	display_segm(xc, yc, gs, gm, r1, 1, smeter);
	display_segm(xc, yc, gm, ge, r1, 1, is_tx ? smeter : smeterplus);
	display_segm(xc, yc, gs, ge, r2, 1, COLORMAIN_WHITE);

	if (is_tx)
	{
		// TX state
		if (gswr > gs)
		{
			uint_fast16_t xx, yy;
			display_segm(xc, yc, gs, gswr, r2 + 2, 1, COLORMAIN_YELLOW);
			display_segm(xc, yc, gs, gswr, r1 - 2, 1, COLORMAIN_YELLOW);
			display_radius(xc, yc, gs, r1 - 2, r2 + 2, COLORMAIN_YELLOW);
			display_radius(xc, yc, gswr, r1 - 2, r2 + 2, COLORMAIN_YELLOW);
			polar_to_dek(xc, yc, gswr - 1, r1 - 4, & xx, & yy);
			display_floodfill(xx, yy, COLORMAIN_YELLOW, COLORMAIN_BLACK);
		}

	}
	else
	{
		// RX state
		const COLORMAIN_T ct = gv_trace > gm ? COLORMAIN_RED : COLORMAIN_YELLOW;
		display_radius(xc - 1, yc, gv_trace, r1 - 2, r2 + 2, ct);
		display_radius(xc, yc, gv_trace, r1 - 2, r2 + 2, ct);
		display_radius(xc + 1, yc, gv_trace, r1 - 2, r2 + 2, ct);
	}

	{
		const COLORMAIN_T ct = gv > gm ? COLORMAIN_RED : COLORMAIN_GREEN;
		display_radius(xc - 1, yc, gv, rv1, rv2, ct);
		display_radius(xc, yc, gv, rv1, rv2, ct);
		display_radius(xc + 1, yc, gv, rv1, rv2, ct);
	}
}

#endif /* LCDMODE_LTDC */

// S-METER
/* отображение S-метра на приёме или передаче */
// Функция вызывается из display2.c
void
display2_bars_rx(
	uint_fast8_t x,
	uint_fast8_t y,
	dctx_t * pctx
	)
{
#if WITHBARS
	uint_fast8_t tracemax;
	uint_fast8_t v = board_getsmeter(& tracemax, 0, UINT8_MAX, 0);
	display_smeter(x, y, v, tracemax, s9level, s9delta, s9_60_delta);
#endif /* WITHBARS */
}

// SWR-METER, POWER-METER
/* отображение P-метра и SWR-метра на приёме или передаче */
// Функция вызывается из display2.c
void
display2_bars_tx(
	uint_fast8_t x,
	uint_fast8_t y,
	dctx_t * pctx
	)
{
#if WITHBARS
#if WITHTX
	#if (WITHSWRMTR || WITHSHOWSWRPWR)
		uint_fast8_t pwrtrace;
		const uint_fast8_t pwr = board_getpwrmeter(& pwrtrace);
		adcvalholder_t reflected;
		const adcvalholder_t forward = board_getswrmeter(& reflected, swrcalibr);
		#if WITHSHOWSWRPWR
			display_swrmeter(x, y, forward, reflected, minforward);
			display_pwrmeter(x, y, pwr, pwrtrace, maxpwrcali);
		#else
			if (swrmode || getactualtune())
				display_swrmeter(x, y, forward, reflected, minforward);
			else
				display_pwrmeter(x, y, pwr, pwrtrace, maxpwrcali);
		#endif
	#elif WITHPWRMTR
		uint_fast8_t pwrtrace;
		const uint_fast8_t pwr = board_getpwrmeter(& pwrtrace);
		display_pwrmeter(x, y, pwr, pwrtrace, maxpwrcali);
	#endif

#endif /* WITHTX */
#endif /* WITHBARS */
}

#if CTLSTYLE_RA4YBO_AM0

// S-METER, SWR-METER, POWER-METER
/* отображение S-метра или SWR-метра на приёме или передаче */
// Вызывается из display2.c (версия для CTLSTYLE_RA4YBO_AM0)
void
display2_bars_amv0(
	uint_fast8_t x,
	uint_fast8_t y,
	dctx_t * pctx
	)
{
#if WITHBARS
	if (gtx)
	{
#if WITHTX
	#if (WITHSWRMTR || WITHSHOWSWRPWR)
		uint_fast8_t pwrtrace;
		const uint_fast8_t pwr = board_getpwrmeter(& pwrtrace);
		const uint_fast8_t modulaton = board_getadc_filtered_u8(REF, 0, UINT8_MAX);
		display_modulationmeter_amv0(x, y, modulaton, UINT8_MAX);
		display_pwrmeter_amv0(x, y, pwr, pwrtrace, maxpwrcali);
	#elif WITHPWRMTR
		uint_fast8_t pwrtrace;
		const uint_fast8_t pwr = board_getpwrmeter(& pwrtrace);
		display_pwrmeter_amv0(x, y, pwr, pwrtrace, maxpwrcali);
	#endif

#endif
	}
	else
	{
		uint_fast8_t tracemax;
		uint_fast8_t v = board_getsmeter(& tracemax, 0, UINT8_MAX, 0);
		display_smeter_amv0(x, y, v, tracemax, s9level, s9delta, s9_60_delta);
	}
#endif /* WITHBARS */
}

#endif /* CTLSTYLE_RA4YBO_AM0 */

#ifndef WITHGUIMAXX
#define WITHGUIMAXX		DIM_X
#endif

#ifndef WITHGUIMAXY
#define WITHGUIMAXY		DIM_Y
#endif

#if WITHGUIMAXX < GUIMINX
#undef WITHGUIMAXX
#define WITHGUIMAXX 	GUIMINX
#endif

#if WITHGUIMAXY < GUIMINY
#undef WITHGUIMAXY
#define WITHGUIMAXY 	GUIMINY
#endif

#if WITHTOUCHGUI

static void button1_handler(void);
static void button2_handler(void);
static void button3_handler(void);
static void button4_handler(void);
static void button5_handler(void);
static void button6_handler(void);
static void button7_handler(void);
static void button8_handler(void);
static void button9_handler(void);
static void labels_menu_handler (void);
static void buttons_mode_handler(void);
static void buttons_bp_handler(void);
static void buttons_freq_handler(void);
static void buttons_menu_handler(void);
static void buttons_uif_handler(void);
static void window_bp_process(void);
static void window_menu_process(void);
static void window_freq_process(void);
static void window_uif_process(void);
static void window_mode_process(void);
static void window_agc_process(void);
static void window_enc2_process(void);
static void gui_main_process(void);

	enum {
		TYPE_DUMMY,
		TYPE_BUTTON,
		TYPE_LABEL
	};

	enum {
		PRESSED,						// нажато
		RELEASED,						// отпущено после нажатия внутри элемента
		CANCELLED,						// первоначальное состояние или отпущено после нажатия вне элемента
		DISABLED						// заблокировано для нажатия
	};

	enum {
		BUTTON_NON_LOCKED,
		BUTTON_LOCKED
	};

	enum {
		BUTTON_CODE_BK = 20,
		BUTTON_CODE_OK = 30,
		BUTTON_CODE_DONE = 99
	};

	enum {								// button_handler.visible & windowpip.is_show
		NON_VISIBLE,					// parent window закрыто, кнопка не отрисовывается
		VISIBLE							// parent window на экране, кнопка отрисовывается
	};

	enum {								// button_handler.parent & windowpip.window_id
		WINDOW_MAIN,					// постоянно отображаемые кнопки внизу экрана
		WINDOW_MODES,					// переключение режимов работы, видов модуляции
		WINDOW_BP,						// регулировка полосы пропускания фильтров выбранного режима
		WINDOW_AGC,						// выбор пресетов настроек АРУ для текущего режима модуляции
		WINDOW_FREQ,
		WINDOW_MENU,
		WINDOW_ENC2,
		WINDOW_UIF
	};

	enum {
		NAME_ARRAY_SIZE = 20,
		TEXT_ARRAY_SIZE = 20,
		MENU_ARRAY_SIZE = 30,
		TOUCH_ARRAY_SIZE = 40
	};

	enum {
		BTN_BUF_W = 96,
		BTN_BUF_H = 64
	};

	typedef ALIGNX_BEGIN PACKEDCOLORPIP_T bg_t [BTN_BUF_W][BTN_BUF_H] ALIGNX_END;

	typedef ALIGNX_BEGIN struct {
		uint8_t w;
		uint8_t h;
		bg_t bg_non_pressed;
		bg_t bg_pressed;
		bg_t bg_locked;
		bg_t bg_locked_pressed;
		bg_t bg_disabled;
	} ALIGNX_END btn_bg_t;

	static btn_bg_t btn_bg [] = {
			{ 86, 44, },
			{ 50, 50, },
			{ 40, 40, },
	};

	enum { BG_COUNT = sizeof btn_bg / sizeof btn_bg[0] };

	typedef struct {
		uint16_t x1;				// координаты от начала экрана
		uint16_t y1;
		uint8_t w;
		uint8_t h;
		void(*onClickHandler) (void);	// обработчик события RELEASED
		uint8_t state;				// текущее состояние кнопки
		uint8_t is_locked;			// признак фиксации кнопки
		uint8_t is_trackable;		// получение относительных координат точки перемещения нажатия, нужно ли для кнопок?
		uint8_t parent;				// индекс окна, в котором будет отображаться кнопка
		uint8_t visible;			// рисовать ли кнопку на экране
		uintptr_t payload;
		char name [NAME_ARRAY_SIZE];
		char text [TEXT_ARRAY_SIZE]; // текст внутри кнопки, разделитель строк |, не более 2х строк
	} button_t;

	static button_t button_handlers [] = {
	//   x1, y1, w, h,  onClickHandler,        state,   is_locked, is_trackable, parent,    visible,      payload,	              name, 		text
		{ },
		{ 0, 0, 86, 44, button1_handler, 	  CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_MAIN, 	  	 VISIBLE,     UINTPTR_MAX, 		"btnMode", 		"Mode", },
		{ 0, 0, 86, 44, button2_handler, 	  CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_MAIN, 	  	 VISIBLE,     UINTPTR_MAX, 		"btnAF", 		"AF|filter", },
		{ 0, 0, 86, 44, button3_handler, 	  CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_MAIN, 	  	 VISIBLE,     UINTPTR_MAX, 		"btnAGC", 		"AGC", },
		{ 0, 0, 86, 44, button4_handler, 	  CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_MAIN, 	  	 VISIBLE,     UINTPTR_MAX, 		"btn4", 		"Freq", },
		{ 0, 0, 86, 44, button5_handler, 	  CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_MAIN, 	 	 VISIBLE,     UINTPTR_MAX, 		"btn5", 		"", },
		{ 0, 0, 86, 44, button6_handler, 	  CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_MAIN, 	  	 VISIBLE,     UINTPTR_MAX, 		"btn6", 		"", },
		{ 0, 0, 86, 44, button7_handler, 	  CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_MAIN, 	  	 VISIBLE,     UINTPTR_MAX, 		"btn7", 		"", },
		{ 0, 0, 86, 44, button8_handler, 	  CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_MAIN, 	  	 VISIBLE,     UINTPTR_MAX, 		"btn8", 		"", },
		{ 0, 0, 86, 44, button9_handler, 	  CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_MAIN, 	  	 VISIBLE,     UINTPTR_MAX, 		"btnSysMenu", 	"System|settings", },
		{ 0, 0, 86, 44, buttons_mode_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_MODES, NON_VISIBLE, SUBMODE_LSB, 		"btnModeLSB", 	"LSB", },
		{ 0, 0, 86, 44, buttons_mode_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_MODES, NON_VISIBLE, SUBMODE_CW,  		"btnModeCW", 	"CW", },
		{ 0, 0, 86, 44, buttons_mode_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_MODES, NON_VISIBLE, SUBMODE_AM,  		"btnModeAM", 	"AM", },
		{ 0, 0, 86, 44, buttons_mode_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_MODES, NON_VISIBLE, SUBMODE_DGL, 		"btnModeDGL", 	"DGL", },
		{ 0, 0, 86, 44, buttons_mode_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_MODES, NON_VISIBLE, SUBMODE_USB, 		"btnModeUSB", 	"USB", },
		{ 0, 0, 86, 44, buttons_mode_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_MODES, NON_VISIBLE, SUBMODE_CWR, 		"btnModeCWR", 	"CWR", },
		{ 0, 0, 86, 44, buttons_mode_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_MODES, NON_VISIBLE, SUBMODE_NFM, 		"btnModeNFM", 	"NFM", },
		{ 0, 0, 86, 44, buttons_mode_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_MODES, NON_VISIBLE, SUBMODE_DGU, 		"btnModeDGU", 	"DGU", },
		{ 0, 0, 86, 44, buttons_bp_handler,	  CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_BP,    NON_VISIBLE, UINTPTR_MAX, 		"btnAF_1", 		"", },
		{ 0, 0, 86, 44, buttons_bp_handler,   CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_BP,    NON_VISIBLE, UINTPTR_MAX, 		"btnAF_OK", 	"OK", },
		{ 0, 0, 86, 44, buttons_bp_handler,   CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_BP,    NON_VISIBLE, UINTPTR_MAX, 		"btnAF_2", 		"", },
		{ 0, 0, 86, 44, hamradio_set_agc_off, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_AGC,   NON_VISIBLE, UINTPTR_MAX, 		"btnAGCoff", 	"AGC|off", },
		{ 0, 0, 86, 44, hamradio_set_agc_slow,CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_AGC,   NON_VISIBLE, UINTPTR_MAX, 		"btnAGCslow", 	"AGC|slow", },
		{ 0, 0, 86, 44, hamradio_set_agc_fast,CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_AGC,   NON_VISIBLE, UINTPTR_MAX, 		"btnAGCfast", 	"AGC|fast", },
		{ 0, 0, 50, 50, buttons_freq_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_FREQ,  NON_VISIBLE, 1, 		 		"btnFreq1", 	"1", },
		{ 0, 0, 50, 50, buttons_freq_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_FREQ,  NON_VISIBLE, 2, 		 		"btnFreq2", 	"2", },
		{ 0, 0, 50, 50, buttons_freq_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_FREQ,  NON_VISIBLE, 3, 		 		"btnFreq3", 	"3", },
		{ 0, 0, 50, 50, buttons_freq_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_FREQ,  NON_VISIBLE, BUTTON_CODE_BK, 	"btnFreqBK", 	"<-", },
		{ 0, 0, 50, 50, buttons_freq_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_FREQ,  NON_VISIBLE, 4, 	 			"btnFreq4", 	"4", },
		{ 0, 0, 50, 50, buttons_freq_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_FREQ,  NON_VISIBLE, 5, 				"btnFreq5", 	"5", },
		{ 0, 0, 50, 50, buttons_freq_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_FREQ,  NON_VISIBLE, 6, 				"btnFreq6", 	"6", },
		{ 0, 0, 50, 50, buttons_freq_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_FREQ,  NON_VISIBLE, BUTTON_CODE_OK, 	"btnFreqOK", 	"OK", },
		{ 0, 0, 50, 50, buttons_freq_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_FREQ,  NON_VISIBLE, 7, 				"btnFreq7", 	"7", },
		{ 0, 0, 50, 50, buttons_freq_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_FREQ,  NON_VISIBLE, 8,  				"btnFreq8", 	"8", },
		{ 0, 0, 50, 50, buttons_freq_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_FREQ,  NON_VISIBLE, 9, 		 		"btnFreq9", 	"9", },
		{ 0, 0, 50, 50, buttons_freq_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_FREQ,  NON_VISIBLE, 0, 	 			"btnFreq0", 	"0", },
		{ 0, 0, 40, 40, buttons_menu_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_MENU,  NON_VISIBLE, UINTPTR_MAX, 		"btnSysMenu-",	"-", },
		{ 0, 0, 40, 40, buttons_menu_handler, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_MENU,  NON_VISIBLE, UINTPTR_MAX, 		"btnSysMenu+",	"+", },
		{ 0, 0, 40, 40, buttons_uif_handler,  CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_UIF,   NON_VISIBLE, UINTPTR_MAX, 		"btnUIF-", 		"-", },
		{ 0, 0, 40, 40, buttons_uif_handler,  CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_UIF,   NON_VISIBLE, UINTPTR_MAX, 		"btnUIF+", 		"+", },
//		{ 375, 120, buttons_uif_handler, 	NULL, CANCELLED, BUTTON_NON_LOCKED, 0, WINDOW_UIF,   NON_VISIBLE, UINTPTR_MAX, 			"btnUIF_OK", "OK", },
	};
	enum { BUTTON_HANDLERS_COUNT = sizeof button_handlers / sizeof button_handlers[0] };

	typedef struct {
		uint16_t x;
		uint16_t y;
		uint8_t parent;
		uint8_t state;
		uint8_t is_trackable;
		uint8_t visible;
		char name [NAME_ARRAY_SIZE];
		char text [TEXT_ARRAY_SIZE];
		PACKEDCOLORPIP_T color;
		void (*onClickHandler) (void);
	} label_t;

	static label_t labels[] = {
	//     x,   y,  parent,      state,        visible,     name,   Text,  color
		{ },
		{ 0, 0, WINDOW_BP,   DISABLED,  0, NON_VISIBLE, "lbl_low",  "", COLORPIP_YELLOW, },
		{ 0, 0, WINDOW_BP,   DISABLED,  0, NON_VISIBLE, "lbl_high", "", COLORPIP_YELLOW, },
		{ 0, 0, WINDOW_MENU, CANCELLED, 1, NON_VISIBLE, "lbl_group", "", COLORPIP_WHITE, labels_menu_handler, },
		{ 0, 0, WINDOW_MENU, CANCELLED, 1, NON_VISIBLE, "lbl_group", "", COLORPIP_WHITE, labels_menu_handler, },
		{ 0, 0, WINDOW_MENU, CANCELLED, 1, NON_VISIBLE, "lbl_group", "", COLORPIP_WHITE, labels_menu_handler, },
		{ 0, 0, WINDOW_MENU, CANCELLED, 1, NON_VISIBLE, "lbl_group", "", COLORPIP_WHITE, labels_menu_handler, },
		{ 0, 0, WINDOW_MENU, CANCELLED, 1, NON_VISIBLE, "lbl_group", "", COLORPIP_WHITE, labels_menu_handler, },
		{ 0, 0, WINDOW_MENU, CANCELLED, 1, NON_VISIBLE, "lbl_group", "", COLORPIP_WHITE, labels_menu_handler, },
		{ 0, 0, WINDOW_MENU, CANCELLED, 1, NON_VISIBLE, "lbl_params", "", COLORPIP_WHITE, labels_menu_handler, },
		{ 0, 0, WINDOW_MENU, CANCELLED, 1, NON_VISIBLE, "lbl_params", "", COLORPIP_WHITE, labels_menu_handler, },
		{ 0, 0, WINDOW_MENU, CANCELLED, 1, NON_VISIBLE, "lbl_params", "", COLORPIP_WHITE, labels_menu_handler, },
		{ 0, 0, WINDOW_MENU, CANCELLED, 1, NON_VISIBLE, "lbl_params", "", COLORPIP_WHITE, labels_menu_handler, },
		{ 0, 0, WINDOW_MENU, CANCELLED, 1, NON_VISIBLE, "lbl_params", "", COLORPIP_WHITE, labels_menu_handler, },
		{ 0, 0, WINDOW_MENU, CANCELLED, 1, NON_VISIBLE, "lbl_params", "", COLORPIP_WHITE, labels_menu_handler, },
		{ 0, 0, WINDOW_MENU, CANCELLED, 0, NON_VISIBLE, "lbl_vals", "", COLORPIP_WHITE, labels_menu_handler, },
		{ 0, 0, WINDOW_MENU, CANCELLED, 0, NON_VISIBLE, "lbl_vals", "", COLORPIP_WHITE, labels_menu_handler, },
		{ 0, 0, WINDOW_MENU, CANCELLED, 0, NON_VISIBLE, "lbl_vals", "", COLORPIP_WHITE, labels_menu_handler, },
		{ 0, 0, WINDOW_MENU, CANCELLED, 0, NON_VISIBLE, "lbl_vals", "", COLORPIP_WHITE, labels_menu_handler, },
		{ 0, 0, WINDOW_MENU, CANCELLED, 0, NON_VISIBLE, "lbl_vals", "", COLORPIP_WHITE, labels_menu_handler, },
		{ 0, 0, WINDOW_MENU, CANCELLED, 0, NON_VISIBLE, "lbl_vals", "", COLORPIP_WHITE, labels_menu_handler, },
		{ 0, 0, WINDOW_ENC2, DISABLED,  0, NON_VISIBLE, "lbl_enc2_param", "", COLORPIP_WHITE, },
		{ 0, 0, WINDOW_ENC2, DISABLED,  0, NON_VISIBLE, "lbl_enc2_val",	  "", COLORPIP_WHITE, },
		{ 0, 0,	WINDOW_UIF,  DISABLED,  0, NON_VISIBLE, "lbl_uif_param",  "", COLORPIP_WHITE, },
		{ 0, 0,	WINDOW_UIF,  DISABLED,  0, NON_VISIBLE, "lbl_uif_val", 	  "", COLORPIP_WHITE, },
		{ 0, 0,	WINDOW_FREQ, DISABLED,  0, NON_VISIBLE, "lbl_freq_val",   "", COLORPIP_YELLOW, },
};
	enum { LABELS_COUNT = sizeof labels / sizeof labels[0] };

	typedef struct {
		uint16_t last_pressed_x; 	 // последняя точка касания экрана
		uint16_t last_pressed_y;
		uint8_t kbd_code;
		uint8_t selected_id;		 // индекс последнего выбранного элемента
		uint8_t selected_type;		 // тип последнего выбранного элемента
		uint8_t state;				 // последнее состояние
		uint8_t is_touching_screen;  // есть ли касание экрана в данный момент
		uint8_t is_after_touch; 	 // есть ли касание экрана после выхода точки касания из элемента (при is_tracking == 0)
		uint8_t is_tracking;		 // получение относительных координат точки перемещения нажатия
		int16_t vector_move_x;	 	 // в т.ч. и за границами элемента, при state == PRESSED
		int16_t vector_move_y;
		uint8_t timer_1sec_updated;	 // для периодических обновлений состояния
	} gui_t;

	static gui_t gui = { 0, 0, KBD_CODE_MAX, 0, TYPE_DUMMY, CANCELLED, 0, 0, 0, 0, 0, 1, };

	enum align_t {
		ALIGN_LEFT_X 	= WITHGUIMAXX >> 2,					// вертикальное выравнивание по центру левой половины экрана
		ALIGN_CENTER_X 	= WITHGUIMAXX >> 1,					// вертикальное выравнивание по центру экрана
		ALIGN_RIGHT_X 	= ALIGN_LEFT_X + ALIGN_CENTER_X,	// вертикальное выравнивание по центру правой половины экрана
		ALIGN_Y 		= WITHGUIMAXY >> 1					// горизонтальное выравнивание всегда по центру экрана
	};

	typedef struct {
		uint8_t window_id;			// в окне будут отображаться элементы с соответствующим полем for_window
		enum align_t align_mode;	// вертикаль выравнивания окна
		uint16_t x1;
		uint16_t y1;
		uint16_t w;
		uint16_t h;
		char name[NAME_ARRAY_SIZE];	// текст, выводимый в заголовке окна
		uint8_t state;
		uint8_t first_call;			// признак первого вызова для различных инициализаций
		void (*onVisibleProcess) (void);
	} window_t;

	static window_t windows[] = {
	//     window_id,   align_mode,     x1, y1, w, h,   title,      is_show, first_call, onVisibleProcess
		{ WINDOW_MAIN, 	ALIGN_LEFT_X,	0, 0,   0,   0, "",  	   	   NON_VISIBLE,	0, gui_main_process, },
		{ WINDOW_MODES, ALIGN_CENTER_X, 0, 0, 402, 150, "Select mode", NON_VISIBLE, 0, window_mode_process, },
		{ WINDOW_BP,    ALIGN_CENTER_X, 0, 0, 372, 205, "Bandpass",    NON_VISIBLE, 0, window_bp_process, },
		{ WINDOW_AGC,   ALIGN_CENTER_X, 0, 0, 372, 110, "AGC control", NON_VISIBLE, 0, window_agc_process, },
		{ WINDOW_FREQ,  ALIGN_CENTER_X, 0, 0, 250, 215, "Freq:", 	   NON_VISIBLE, 0, window_freq_process, },
		{ WINDOW_MENU,  ALIGN_CENTER_X, 0, 0, 550, 240, "Settings",	   NON_VISIBLE, 0, window_menu_process, },
		{ WINDOW_ENC2, 	ALIGN_RIGHT_X, 	0, 0, 185, 105, "Fast menu",   NON_VISIBLE, 0, window_enc2_process, },
		{ WINDOW_UIF, 	ALIGN_LEFT_X, 	0, 0, 200, 145, "",   		   NON_VISIBLE, 0, window_uif_process, },
	};
	enum { windows_count = sizeof windows / sizeof windows[0] };

	typedef struct {
		int16_t rotate;			// признак поворота второго энкодера
		uint8_t press;			// короткое нажание
		uint8_t hold;			// длинное нажатие
		uint8_t busy;			// второй энкодер выделен для обработки данных окна
		uint8_t rotate_done;	// событие поворота от энкодера обработано, можно получать новые данные
		uint8_t press_done;		// событие нажатия от энкодера обработано, можно получать новые данные
	} enc2_t;

	static enc2_t encoder2 = { 0, 0, 0, 0, 1, 1, };

	typedef struct {
		uint32_t val;
		uint8_t num;
		uint8_t key;
	} editfreq_t;

	static editfreq_t editfreq;

	enum {
		MENU_OFF,
		MENU_GROUPS,
		MENU_PARAMS,
		MENU_VALS,
		MENU_COUNT
	};

	typedef struct {
		uint8_t first_id;			// первое вхождение номера метки уровня
		uint8_t last_id;			// последнее вхождение номера метки уровня
		uint8_t num_rows;			// число меток уровня
		uint8_t count;				// число значений уровня
		int8_t selected_str;		// выбранная строка уровня
		int8_t selected_label;		// выбранная метка уровня
		uint8_t add_id;				// номер строки уровня, отображаемой первой
		menu_names_t menu_block [MENU_ARRAY_SIZE];	// массив значений уровня меню
	} menu_t;

	static menu_t menu[MENU_COUNT];

	typedef struct {
		char name [TEXT_ARRAY_SIZE];
		uint16_t menupos;
		uint8_t exitkey;
	} menu_by_name_t;

	menu_by_name_t menu_uif;

	typedef struct {
		uint_fast16_t type;			// тип элемента, поддерживающего реакцию на касания
		uint8_t state;				// текущее состояние элемента
		uint8_t visible;			// текущая видимость элемента
		uint8_t id;					// номер элемента из структуры описания
		uint8_t is_trackable;		// поддерживает ли элемент возврат относительных координат перемещения точки нажатия
		uint16_t x1;				// координаты окна
		uint16_t y1;
		uint16_t x2;
		uint16_t y2;
	} touch_t;

	static touch_t touch_elements[TOUCH_ARRAY_SIZE];
	static uint_fast8_t touch_count = 0;
	static uint_fast8_t menu_label_touched = 0;
	static uint_fast8_t menu_level;
	static enc2_menu_t * gui_enc2_menu;

	void gui_timer_update (void * arg)
	{
		gui.timer_1sec_updated = 1;
	}

	static uint_fast8_t find_button (uint_fast8_t id_window, const char * name)				// возврат id кнопки окна по ее названию
	{
		for (uint_fast8_t i = 1; i < BUTTON_HANDLERS_COUNT; i++)
		{
			if (button_handlers[i].parent == id_window && strcmp(button_handlers[i].name, name) == 0)
				return i;
		}
		return 0;
	}

	static uint_fast8_t find_label (uint_fast8_t id_window, const char * name)				// возврат id метки окна по ее названию
	{
		for (uint_fast8_t i = 1; i < LABELS_COUNT; i++)
		{
			if (labels[i].parent == id_window && strcmp(labels[i].name, name) == 0)
				return i;
		}
		return 0;
	}

	static void footer_buttons_state (uint_fast8_t state, const char * name)					// блокируются все, кроме name == text
	{
		static uint_fast8_t id = 0;
		if (state == DISABLED)
		{
			id = find_button(WINDOW_MAIN, name);
			button_handlers[id].is_locked = BUTTON_LOCKED;
		} else
			button_handlers[id].is_locked = BUTTON_NON_LOCKED;

		for (uint_fast8_t i = 1; i < BUTTON_HANDLERS_COUNT; i++)
		{
			if (button_handlers[i].parent != WINDOW_MAIN)
				break;
			button_handlers[i].state = button_handlers[i].name == name ? DISABLED : state;
		}
	}

	static void set_window(uint_fast8_t parent, uint_fast8_t value)
	{
		windows[parent].state = value;
		if (! value)
			windows[parent].first_call = 1;

		for (uint_fast8_t i = 1; i < BUTTON_HANDLERS_COUNT; i++)
		{
			if (button_handlers[i].parent == parent)
			{
				button_handlers[i].visible = value;
				button_handlers[i].is_locked = 0;
				if (button_handlers[i].visible)
				{
					touch_elements[touch_count].id = i;
					touch_elements[touch_count].type = TYPE_BUTTON;
					touch_count++;
				}
				else
					touch_count--;
			}
		}
		for (uint_fast8_t i = 1; i < LABELS_COUNT; i++)
		{
			if (labels[i].parent == parent)
			{
				labels[i].visible = value;
				if (labels[i].visible && labels[i].onClickHandler)
				{
					touch_elements[touch_count].id = i;
					touch_elements[touch_count].type = TYPE_LABEL;
					touch_count++;
				}
				if(! labels[i].visible && labels[i].onClickHandler)
					touch_count--;
			}
		}
	}

	static void calculate_window_position(uint_fast8_t window_id)
	{
		window_t * win = & windows[window_id];
		win->y1 = ALIGN_Y - win->h / 2;

		switch (windows[window_id].align_mode)
		{
		case ALIGN_LEFT_X:
			if (ALIGN_LEFT_X - win->w / 2 < 0)
				win->x1 = 0;
			else
				win->x1 = ALIGN_LEFT_X - win->w / 2;
			break;

		case ALIGN_RIGHT_X:
			if (ALIGN_RIGHT_X + win->w / 2 > WITHGUIMAXX)
				win->x1 = WITHGUIMAXX - win->w;
			else
				win->x1 = ALIGN_RIGHT_X - win->w / 2;
			break;

		case ALIGN_CENTER_X:
		default:
			win->x1 = ALIGN_CENTER_X - win->w / 2;
			break;
		}
	}

	static void buttons_bp_handler(void)
	{
		if(gui.selected_type == TYPE_BUTTON)
		{
			uint_fast8_t id_button_high = find_button(WINDOW_BP, "btnAF_2");
			uint_fast8_t id_button_low = find_button(WINDOW_BP, "btnAF_1");
			uint_fast8_t id_button_OK = find_button(WINDOW_BP, "btnAF_OK");

			if (gui.selected_id == id_button_low)
			{
				button_handlers[id_button_high].is_locked = 0;
				button_handlers[id_button_low].is_locked = 1;
			}
			else if (gui.selected_id == id_button_high)
			{
				button_handlers[id_button_high].is_locked = 1;
				button_handlers[id_button_low].is_locked = 0;
			}
			else if (gui.selected_id == id_button_OK)
			{
				set_window(WINDOW_BP, NON_VISIBLE);
				encoder2.busy = 0;
				footer_buttons_state(CANCELLED, "");
				hamradio_disable_keyboard_redirect();
			}
		}
	}

	static void window_bp_process(void)
	{
		static uint_fast8_t val_high, val_low, val_c, val_w;
		static uint_fast16_t x_h, x_l, x_c, x_0, y_0;
		static window_t * win = & windows[WINDOW_BP];
		uint_fast16_t x_size = 290;
		static label_t * lbl_low, * lbl_high;
		static button_t * button_high, * button_low;

		if (win->first_call == 1)
		{
			uint_fast8_t interval = 20, id = 0, x, y;
			uint_fast8_t col1_int = 35, row1_int = 20;
			calculate_window_position(WINDOW_BP);
			x_0 = win->x1 + 50;											// оконные координаты нулевой точки графика
			y_0 = win->y1 + 90;

			while(button_handlers[++id].parent != WINDOW_BP)			// первое вхождение кнопки WINDOW_BP
				;
			x = win->x1 + col1_int;
			y = win->y1 + win->h - button_handlers[id].h - row1_int;
			do {
				button_handlers[id].x1 = x;
				button_handlers[id].y1 = y;
				x = x + interval + button_handlers[id].w;
				if (x + button_handlers[id].w > win->x1 + win->w)
				{
					x = win->x1 + col1_int;
					y = y + button_handlers[id].h + interval;
				}
			} while (button_handlers[++id].parent == WINDOW_BP);

			button_high = & button_handlers[find_button(WINDOW_BP, "btnAF_2")];
			button_low = & button_handlers[find_button(WINDOW_BP, "btnAF_1")];

			lbl_low = & labels[find_label(WINDOW_BP, "lbl_low")];
			lbl_high = & labels[find_label(WINDOW_BP, "lbl_high")];

			lbl_low->y = y_0 + SMALLCHARH;
			lbl_high->y = y_0 + SMALLCHARH;

			val_high = hamradio_get_high_bp(0);
			val_low = hamradio_get_low_bp(0);
			if (hamradio_get_bp_type())			// BWSET_WIDE
			{
				strcpy(button_high->text, "High|cut");
				strcpy(button_low->text, "Low|cut");
				button_high->is_locked = 1;
			}
			else						// BWSET_NARROW
			{
				strcpy(button_high->text, "Pitch");
				strcpy(button_low->text, "Width");
				button_low->is_locked = 1;
			}
		}

		if (encoder2.rotate != 0 || win->first_call)
		{
			char buf[TEXT_ARRAY_SIZE];

			if (win->first_call)
				win->first_call = 0;

			if (hamradio_get_bp_type())			// BWSET_WIDE
			{
				if (button_high->is_locked == 1)
					val_high = hamradio_get_high_bp(encoder2.rotate);
				else if (button_low->is_locked == 1)
					val_low = hamradio_get_low_bp(encoder2.rotate * 10);
				encoder2.rotate_done = 1;

				x_h = x_0 + normalize(val_high, 0, 50, x_size);
				x_l = x_0 + normalize(val_low / 10, 0, 50, x_size);
				x_c = x_l + (x_h - x_l) / 2;

				local_snprintf_P(buf, sizeof buf / sizeof buf[0], PSTR("%d"), val_high * 100);
				strcpy(lbl_high->text, buf);
				lbl_high->x = (x_h + strwidth(lbl_high->text) > x_0 + x_size) ?
						(x_0 + x_size - strwidth(lbl_high->text)) : x_h;

				local_snprintf_P(buf, sizeof buf / sizeof buf[0], PSTR("%d"), val_low * 10);
				strcpy(lbl_low->text, buf);
				lbl_low->x = (x_l - strwidth(lbl_low->text) < x_0 - 10) ? (x_0 - 10) : (x_l - strwidth(lbl_low->text));
			}
			else						// BWSET_NARROW
			{
				if (button_high->is_locked == 1)
				{
					val_c = hamradio_get_high_bp(encoder2.rotate);
					val_w = hamradio_get_low_bp(0) / 2;
				}
				else if (button_low->is_locked == 1)
				{
					val_c = hamradio_get_high_bp(0);
					val_w = hamradio_get_low_bp(encoder2.rotate) / 2;
				}
				encoder2.rotate_done = 1;
				x_c = x_0 + x_size / 2;
				x_l = x_c - normalize(val_w , 0, 500, x_size);
				x_h = x_c + normalize(val_w , 0, 500, x_size);

				local_snprintf_P(buf, sizeof buf / sizeof buf[0], PSTR("%d"), val_w * 20);
				strcpy(lbl_high->text, buf);
				lbl_high->x = x_c - strwidth(lbl_high->text) / 2;

				local_snprintf_P(buf, sizeof buf / sizeof buf[0], PSTR("P %d"), val_c * 10);
				strcpy(lbl_low->text, buf);
				lbl_low->x = x_0 + x_size - strwidth(lbl_low->text);
			}
		}
		PACKEDCOLORMAIN_T * fr = colmain_fb_draw();
		colpip_line(fr, DIM_X, DIM_Y, x_0 - 10, y_0, x_0 + x_size, y_0, COLORPIP_WHITE);
		colpip_line(fr, DIM_X, DIM_Y, x_0, y_0 - 45, x_0, y_0 + 5, COLORPIP_WHITE);
		colpip_line(fr, DIM_X, DIM_Y, x_l, y_0 - 40, x_l - 4, y_0 - 3, COLORPIP_YELLOW);
		colpip_line(fr, DIM_X, DIM_Y, x_h, y_0 - 40, x_h + 4, y_0 - 3, COLORPIP_YELLOW);
		colpip_line(fr, DIM_X, DIM_Y, x_l, y_0 - 40, x_h, y_0 - 40, COLORPIP_YELLOW);
		colpip_line(fr, DIM_X, DIM_Y, x_c, y_0 - 45, x_c, y_0 + 5, COLORPIP_RED);
	}

	static void buttons_freq_handler (void)
	{
		if (button_handlers[gui.selected_id].parent == WINDOW_FREQ && editfreq.key == BUTTON_CODE_DONE)
			editfreq.key = button_handlers[gui.selected_id].payload;
	}

	static void window_freq_process (void)
	{
		static uint_fast8_t lbl_id;
		static window_t * win = & windows[WINDOW_FREQ];

		if (win->first_call == 1)
		{
			uint_fast8_t interval = 6, id = 0, x, y;
			uint_fast8_t col1_int = 20, row1_int = 40;
			win->first_call = 0;
			calculate_window_position(WINDOW_FREQ);

			while(button_handlers[++id].parent != WINDOW_FREQ)			// первое вхождение кнопки WINDOW_FREQ
				;
			x = win->x1 + col1_int;
			y = win->y1 + row1_int;
			do {
				button_handlers[id].x1 = x;
				button_handlers[id].y1 = y;
				x = x + interval + button_handlers[id].w;
				if (x + button_handlers[id].w > win->x1 + win->w)
				{
					x = win->x1 + col1_int;
					y = y + button_handlers[id].h + interval;
				}
			} while (button_handlers[++id].parent == WINDOW_FREQ);
			button_handlers[find_button(WINDOW_FREQ, "btnFreqOK")].is_locked = BUTTON_LOCKED;

			lbl_id = find_label(WINDOW_FREQ, "lbl_freq_val");
			labels[lbl_id].x = win->x1 + strwidth(win->name) + strwidth(" ") + 20;
			labels[lbl_id].y = win->y1 + 10;
			strcpy(labels[lbl_id].text, "     0 k");
			labels[lbl_id].color = COLORPIP_YELLOW;

			editfreq.val = 0;
			editfreq.num = 0;
			editfreq.key = BUTTON_CODE_DONE;

			return;
		}

		if (editfreq.key != BUTTON_CODE_DONE)
		{
			labels[lbl_id].color = COLORPIP_YELLOW;
			char buf[TEXT_ARRAY_SIZE];
			switch (editfreq.key)
			{
			case BUTTON_CODE_BK:
				if (editfreq.num > 0)
				{
					editfreq.val /= 10;
					editfreq.num --;
				}
				break;

			case BUTTON_CODE_OK:
				if(hamradio_set_freq(editfreq.val * 1000))
				{
					set_window(WINDOW_FREQ, NON_VISIBLE);
					footer_buttons_state(CANCELLED, "");
					hamradio_set_lockmode(0);
					hamradio_disable_keyboard_redirect();
				} else
					labels[lbl_id].color = COLORPIP_RED;
				break;

			default:
				if (editfreq.num < 6)
				{
					editfreq.val  = editfreq.val * 10 + editfreq.key;
					if (editfreq.val)
						editfreq.num ++;
				}
			}
			editfreq.key = BUTTON_CODE_DONE;
			local_snprintf_P(buf, sizeof buf / sizeof buf[0], PSTR("%6d k"), editfreq.val);
			strcpy(labels[lbl_id].text, buf);
		}
	}

	void gui_uif_editmenu(const char * name, uint_fast16_t menupos, uint_fast8_t exitkey)
	{
		if (windows[WINDOW_UIF].state == NON_VISIBLE)
		{
			set_window(WINDOW_UIF, VISIBLE);
			windows[WINDOW_UIF].first_call = 1;
			footer_buttons_state(DISABLED, "");
			strcpy(menu_uif.name, name);
			menu_uif.menupos = menupos;
			menu_uif.exitkey = exitkey;
		}
		else
		{
			set_window(WINDOW_UIF, NON_VISIBLE);
			footer_buttons_state(CANCELLED, "");
		}
	}

	void gui_put_keyb_code (uint_fast8_t kbch)
	{
		// После обработки события по коду кнопки
		// сбрасывать gui.kbd_code в KBD_CODE_MAX.
		gui.kbd_code = gui.kbd_code == KBD_CODE_MAX ? kbch : gui.kbd_code;
	}

	static void buttons_uif_handler(void)
	{
		if (gui.selected_type == TYPE_BUTTON && gui.selected_id == find_button(WINDOW_UIF, "btnUIF+"))
			encoder2.rotate = 1;
		else if (gui.selected_type == TYPE_BUTTON && gui.selected_id == find_button(WINDOW_UIF, "btnUIF-"))
			encoder2.rotate = -1;
		else if (gui.selected_type == TYPE_BUTTON && gui.selected_id == find_button(WINDOW_UIF, "btnUIF_OK"))
		{
			hamradio_disable_keyboard_redirect();
			set_window(WINDOW_UIF, NON_VISIBLE);
			footer_buttons_state(CANCELLED, "");
		}
	}

	static void window_uif_process(void)
	{
		static uint_fast8_t id_lbl_uif_param, id_lbl_uif_val, window_center_x;
		static uint_fast8_t id_button_up = 0, id_button_down = 0;
		uint_fast8_t button_menu_w = 40, button_menu_h = 40;

		if (windows[WINDOW_UIF].first_call == 1)
		{
			windows[WINDOW_UIF].first_call = 0;
			calculate_window_position(WINDOW_UIF);
			window_center_x =  windows[WINDOW_UIF].x1 + windows[WINDOW_UIF].w / 2;

			id_lbl_uif_param = find_label(WINDOW_UIF, "lbl_uif_param");
			id_lbl_uif_val = find_label(WINDOW_UIF, "lbl_uif_val");
			strcpy(labels[id_lbl_uif_param].text, menu_uif.name);
			const char * v = hamradio_gui_edit_menu_item(menu_uif.menupos, 0);
			strcpy(labels[id_lbl_uif_val].text, v);

			labels[id_lbl_uif_param].x = window_center_x - (strwidth(labels[id_lbl_uif_param].text) / 2);
			labels[id_lbl_uif_param].y = windows[WINDOW_UIF].y1 + SMALLCHARH;
			labels[id_lbl_uif_val].x = window_center_x - (strwidth(labels[id_lbl_uif_val].text) / 2);
			labels[id_lbl_uif_val].y = windows[WINDOW_UIF].y1 + SMALLCHARH * 4;

			id_button_up = find_button(WINDOW_UIF, "btnUIF+");
			id_button_down = find_button(WINDOW_UIF, "btnUIF-");

			button_handlers[id_button_down].x1 = labels[id_lbl_uif_val].x - button_menu_w - 10;
			button_handlers[id_button_down].y1 = (labels[id_lbl_uif_val].y + SMALLCHARH / 2) - (button_menu_h / 2);
			button_handlers[id_button_up].x1 = labels[id_lbl_uif_val].x + strwidth(labels[id_lbl_uif_val].text) + 10;
			button_handlers[id_button_up].y1 = button_handlers[id_button_down].y1;

			hamradio_enable_keyboard_redirect();
			return;
		}

		if (encoder2.rotate != 0)
		{
			const char * v = hamradio_gui_edit_menu_item(menu_uif.menupos, encoder2.rotate);
			strcpy(labels[id_lbl_uif_val].text, v);
			labels[id_lbl_uif_val].x = window_center_x - (strwidth(labels[id_lbl_uif_val].text) / 2);
			encoder2.rotate_done = 1;

			button_handlers[id_button_down].x1 = labels[id_lbl_uif_val].x - button_menu_w - 10;
			button_handlers[id_button_up].x1 = labels[id_lbl_uif_val].x + strwidth(labels[id_lbl_uif_val].text) + 10;
		}

		if (gui.kbd_code != KBD_CODE_MAX)
		{
			if (gui.kbd_code == menu_uif.exitkey)
			{
				hamradio_disable_keyboard_redirect();
				set_window(WINDOW_UIF, NON_VISIBLE);
				footer_buttons_state(CANCELLED, "");
			}
			gui.kbd_code = KBD_CODE_MAX;
		}
	}

	static void labels_menu_handler (void)
	{
		if (gui.selected_type == TYPE_LABEL)
		{
			if(strcmp(labels[gui.selected_id].name, "lbl_group") == 0)
			{
				menu[MENU_GROUPS].selected_label = gui.selected_id - menu[MENU_GROUPS].first_id;
				menu_label_touched = 1;
				menu_level = MENU_GROUPS;
			}
			else if(strcmp(labels[gui.selected_id].name, "lbl_params") == 0)
			{
				menu[MENU_PARAMS].selected_label = gui.selected_id - menu[MENU_PARAMS].first_id;
				menu_label_touched = 1;
				menu_level = MENU_PARAMS;
			}
			else if(strcmp(labels[gui.selected_id].name, "lbl_vals") == 0)
			{
				menu[MENU_VALS].selected_label = gui.selected_id - menu[MENU_VALS].first_id;
				menu[MENU_PARAMS].selected_label = menu[MENU_VALS].selected_label;
				menu_label_touched = 1;
				menu_level = MENU_VALS;
			}
		}
	}

	static void buttons_menu_handler(void)
	{
		if (gui.selected_type == TYPE_BUTTON && gui.selected_id == find_button(WINDOW_MENU, "btnSysMenu+"))
			encoder2.rotate = 1;
		else if (gui.selected_type == TYPE_BUTTON && gui.selected_id == find_button(WINDOW_MENU, "btnSysMenu-"))
			encoder2.rotate = -1;
	}

	static void window_menu_process(void)
	{
		static uint_fast8_t str_step = 0, menu_is_scrolling = 0, start_str_group = 0, start_str_params = 0;
		static uint_fast8_t id_button_up = 0, id_button_down = 0, button_pressed = 0;
		static uint_fast8_t button_menu_w = 0, button_menu_h = 0;

		if (windows[WINDOW_MENU].first_call == 1)
		{
			windows[WINDOW_MENU].align_mode = ALIGN_CENTER_X;						// выравнивание окна системных настроек только по центру
			calculate_window_position(WINDOW_MENU);

			windows[WINDOW_MENU].first_call = 0;
			hamradio_set_menu_cond(VISIBLE);

			uint_fast8_t int_cols = 200, int_rows = 35;
			uint_fast8_t col1_int = 50, row1_int = 40;
			uint_fast8_t xn, yn;

			id_button_up = find_button(WINDOW_MENU, "btnSysMenu+");
			id_button_down = find_button(WINDOW_MENU, "btnSysMenu-");
			button_handlers[id_button_up].visible = NON_VISIBLE;
			button_handlers[id_button_down].visible = NON_VISIBLE;

			button_menu_w = button_handlers[id_button_up].w;
			button_menu_h = button_handlers[id_button_up].h;

			menu[MENU_GROUPS].add_id = 0;
			menu[MENU_GROUPS].selected_str = 0;
			menu[MENU_GROUPS].selected_label = 0;
			menu[MENU_PARAMS].add_id = 0;
			menu[MENU_PARAMS].selected_str = 0;
			menu[MENU_PARAMS].selected_label = 0;
			menu[MENU_VALS].add_id = 0;
			menu[MENU_VALS].selected_str = 0;
			menu[MENU_VALS].selected_label = 0;

			menu[MENU_GROUPS].first_id = 1;
			while (labels[++menu[MENU_GROUPS].first_id].parent != WINDOW_MENU);				// первое вхождение метки group
			menu[MENU_GROUPS].last_id = menu[MENU_GROUPS].first_id;
			while (strcmp(labels[++menu[MENU_GROUPS].last_id].name, "lbl_group") == 0);
			menu[MENU_GROUPS].last_id--;													// последнее вхождение метки group
			menu[MENU_GROUPS].num_rows = menu[MENU_GROUPS].last_id - menu[MENU_GROUPS].first_id;

			menu[MENU_PARAMS].first_id = menu[MENU_GROUPS].last_id + 1;						// первое вхождение метки params
			menu[MENU_PARAMS].last_id = menu[MENU_PARAMS].first_id;
			while (strcmp(labels[++menu[MENU_PARAMS].last_id].name, "lbl_params") == 0)
				;
			menu[MENU_PARAMS].last_id--;													// последнее вхождение метки params
			menu[MENU_PARAMS].num_rows = menu[MENU_PARAMS].last_id - menu[MENU_PARAMS].first_id;

			menu[MENU_VALS].first_id = menu[MENU_PARAMS].last_id + 1;						// первое вхождение метки vals
			menu[MENU_VALS].last_id = menu[MENU_VALS].first_id;
			while (strcmp(labels[++menu[MENU_VALS].last_id].name, "lbl_vals") == 0)
				;
			menu[MENU_VALS].last_id--;														// последнее вхождение метки vals
			menu[MENU_VALS].num_rows = menu[MENU_VALS].last_id - menu[MENU_VALS].first_id;

			menu[MENU_GROUPS].count = hamradio_get_multilinemenu_block_groups(menu[MENU_GROUPS].menu_block) - 1;
			xn = windows[WINDOW_MENU].x1 + col1_int;
			yn = windows[WINDOW_MENU].y1 + row1_int;
			for(uint_fast8_t i = 0; i <= menu[MENU_GROUPS].num_rows; i++)
			{
				strcpy(labels[menu[MENU_GROUPS].first_id + i].text, menu[MENU_GROUPS].menu_block[i + menu[MENU_GROUPS].add_id].name);
				labels[menu[MENU_GROUPS].first_id + i].visible = VISIBLE;
				labels[menu[MENU_GROUPS].first_id + i].color = COLORPIP_WHITE;
				labels[menu[MENU_GROUPS].first_id + i].x = xn;
				labels[menu[MENU_GROUPS].first_id + i].y = yn;
				yn += int_rows;
			}

			menu[MENU_PARAMS].count = hamradio_get_multilinemenu_block_params(menu[MENU_PARAMS].menu_block, menu[MENU_GROUPS].menu_block[menu[MENU_GROUPS].selected_str].index) - 1;
			xn += int_cols;
			yn = windows[WINDOW_MENU].y1 + row1_int;
			for(uint_fast8_t i = 0; i <= menu[MENU_PARAMS].num_rows; i++)
			{
				labels[menu[MENU_PARAMS].first_id + i].x = xn;
				labels[menu[MENU_PARAMS].first_id + i].y = yn;
				yn += int_rows;
				labels[menu[MENU_PARAMS].first_id + i].visible = NON_VISIBLE;
				labels[menu[MENU_PARAMS].first_id + i].color = COLORPIP_WHITE;
				if (menu[MENU_PARAMS].count < i)
					continue;
				strcpy(labels[menu[MENU_PARAMS].first_id + i].text, menu[MENU_PARAMS].menu_block[i + menu[MENU_PARAMS].add_id].name);
				labels[menu[MENU_PARAMS].first_id + i].visible = VISIBLE;
			}

			menu[MENU_PARAMS].selected_str = 0;
			menu[MENU_VALS].count = menu[MENU_PARAMS].count < menu[MENU_VALS].num_rows ? menu[MENU_PARAMS].count :  menu[MENU_VALS].num_rows;
			hamradio_get_multilinemenu_block_vals(menu[MENU_VALS].menu_block, menu[MENU_PARAMS].menu_block[menu[MENU_PARAMS].selected_str].index, menu[MENU_VALS].count);
			xn += int_cols;
			yn = windows[WINDOW_MENU].y1 + row1_int;
			for(uint_fast8_t i = 0; i <= menu[MENU_VALS].num_rows; i++)
			{
				labels[menu[MENU_VALS].first_id + i].x = xn;
				labels[menu[MENU_VALS].first_id + i].y = yn;
				yn += int_rows;
				labels[menu[MENU_VALS].first_id + i].visible = NON_VISIBLE;
				labels[menu[MENU_VALS].first_id + i].color = COLORPIP_WHITE;
				if (menu[MENU_VALS].count < i)
					continue;
				strcpy(labels[menu[MENU_VALS].first_id + i].text, menu[MENU_VALS].menu_block[i + menu[MENU_VALS].add_id].name);
				labels[menu[MENU_VALS].first_id + i].visible = VISIBLE;
			};
			str_step = labels[menu[MENU_GROUPS].first_id + 1].y - labels[menu[MENU_GROUPS].first_id].y;
			menu_level = MENU_GROUPS;
			return;
		}

		if(gui.is_tracking && gui.selected_type == TYPE_LABEL && gui.vector_move_y != 0)
		{
			if (! menu_is_scrolling)
			{
				start_str_group = menu[MENU_GROUPS].add_id;
				start_str_params = menu[MENU_PARAMS].add_id;
			}
			ldiv_t r = ldiv(gui.vector_move_y, str_step);
			if(strcmp(labels[gui.selected_id].name, "lbl_group") == 0)
			{
				int_fast8_t q = start_str_group - r.quot;
				menu[MENU_GROUPS].add_id = q <= 0 ? 0 : q;
				menu[MENU_GROUPS].add_id = (menu[MENU_GROUPS].add_id + menu[MENU_GROUPS].num_rows) > menu[MENU_GROUPS].count ?
						(menu[MENU_GROUPS].count - menu[MENU_GROUPS].num_rows) : menu[MENU_GROUPS].add_id;
				menu[MENU_GROUPS].selected_str = menu[MENU_GROUPS].selected_label + menu[MENU_GROUPS].add_id;
				menu_level = MENU_GROUPS;
				menu[MENU_PARAMS].add_id = 0;
				menu[MENU_PARAMS].selected_str = 0;
				menu[MENU_PARAMS].selected_label = 0;
				menu[MENU_VALS].add_id = 0;
				menu[MENU_VALS].selected_str = 0;
				menu[MENU_VALS].selected_label = 0;
			}
			else if(strcmp(labels[gui.selected_id].name, "lbl_params") == 0 &&
					menu[MENU_PARAMS].count > menu[MENU_PARAMS].num_rows)
			{
				int_fast8_t q = start_str_params - r.quot;
				menu[MENU_PARAMS].add_id = q <= 0 ? 0 : q;
				menu[MENU_PARAMS].add_id = (menu[MENU_PARAMS].add_id + menu[MENU_PARAMS].num_rows) > menu[MENU_PARAMS].count ?
						(menu[MENU_PARAMS].count - menu[MENU_PARAMS].num_rows) : menu[MENU_PARAMS].add_id;
				menu[MENU_PARAMS].selected_str = menu[MENU_PARAMS].selected_label + menu[MENU_PARAMS].add_id;
				menu[MENU_VALS].add_id = menu[MENU_PARAMS].add_id;
				menu[MENU_VALS].selected_str = menu[MENU_PARAMS].selected_str;
				menu[MENU_VALS].selected_label = menu[MENU_PARAMS].selected_label;
				menu_level = MENU_PARAMS;
			}
			menu_is_scrolling = 1;
		}

		if(! gui.is_tracking && menu_is_scrolling)
		{
			menu_is_scrolling = 0;
			gui.vector_move_y = 0;
			gui.vector_move_x = 0;
		}

		if (! encoder2.press_done || menu_label_touched || menu_is_scrolling)
		{
			// выход из режима редактирования параметра  - краткое или длинное нажатие на энкодер
			if (encoder2.press && menu_level == MENU_VALS)
			{
				menu_level = MENU_PARAMS;
				encoder2.press = 0;
			}
			if (encoder2.press)
				menu_level = ++menu_level > MENU_VALS ? MENU_VALS : menu_level;
			if (encoder2.hold)
			{
				menu_level = --menu_level == MENU_OFF ? MENU_OFF : menu_level;
				if (menu_level == MENU_GROUPS)
				{
					menu[MENU_PARAMS].add_id = 0;
					menu[MENU_PARAMS].selected_str = 0;
					menu[MENU_PARAMS].selected_label = 0;
					menu[MENU_VALS].add_id = 0;
					menu[MENU_VALS].selected_str = 0;
					menu[MENU_VALS].selected_label = 0;
				}
			}

			// при переходе на следующий уровень пункт меню подсвечивается
			if (menu_level == MENU_VALS)
			{
				menu[MENU_VALS].selected_label = menu[MENU_PARAMS].selected_label;
				uint_fast8_t id_sel_label = menu[MENU_VALS].first_id + menu[MENU_VALS].selected_label;

				button_handlers[id_button_down].visible = VISIBLE;
				button_handlers[id_button_down].x1 = labels[id_sel_label].x - button_menu_w - 10;
				button_handlers[id_button_down].y1 = (labels[id_sel_label].y + SMALLCHARH / 2) - (button_menu_h / 2);

				button_handlers[id_button_up].visible = VISIBLE;
				button_handlers[id_button_up].x1 = labels[id_sel_label].x + strwidth(labels[id_sel_label].text) + 10;
				button_handlers[id_button_up].y1 = button_handlers[id_button_down].y1;
				for (uint8_t i = 0; i <= menu[MENU_GROUPS].num_rows; i++)
				{
					labels[i + menu[MENU_GROUPS].first_id].color = i == menu[MENU_GROUPS].selected_label ? COLORPIP_YELLOW : COLORPIP_GRAY;
					labels[i + menu[MENU_PARAMS].first_id].color = i == menu[MENU_PARAMS].selected_label ? COLORPIP_YELLOW : COLORPIP_GRAY;
					labels[i + menu[MENU_VALS].first_id].color = i == menu[MENU_PARAMS].selected_label ? COLORPIP_YELLOW : COLORPIP_GRAY;
				}
				menu_label_touched = 0;
			}
			if (menu_level == MENU_PARAMS)
			{
				button_handlers[id_button_down].visible = NON_VISIBLE;
				button_handlers[id_button_up].visible = NON_VISIBLE;
				for (uint8_t i = 0; i <= menu[MENU_GROUPS].num_rows; i++)
				{
					labels[i + menu[MENU_GROUPS].first_id].color = i == menu[MENU_GROUPS].selected_label ? COLORPIP_YELLOW : COLORPIP_GRAY;
					labels[i + menu[MENU_PARAMS].first_id].color = COLORPIP_WHITE;
					labels[i + menu[MENU_VALS].first_id].color = COLORPIP_WHITE;
				}
			}
			if (menu_level == MENU_GROUPS)
			{
				button_handlers[id_button_down].visible = NON_VISIBLE;
				button_handlers[id_button_up].visible = NON_VISIBLE;
				for (uint8_t i = 0; i <= menu[MENU_GROUPS].num_rows; i++)
				{
					labels[i + menu[MENU_GROUPS].first_id].color = COLORPIP_WHITE;
					labels[i + menu[MENU_PARAMS].first_id].color = COLORPIP_WHITE;
					labels[i + menu[MENU_VALS].first_id].color = COLORPIP_WHITE;
				}
			}

			encoder2.press = 0;
			encoder2.hold = 0;
			encoder2.press_done = 1;
		}

		if (menu_level == MENU_OFF)
		{
			set_window(WINDOW_MENU, NON_VISIBLE);
			encoder2.busy = 0;
			footer_buttons_state(CANCELLED, "");
			hamradio_set_menu_cond(NON_VISIBLE);
			return;
		}

		if (encoder2.rotate != 0 && menu_level == MENU_VALS)
		{
			encoder2.rotate_done = 1;
			menu[MENU_PARAMS].selected_str = menu[MENU_PARAMS].selected_label + menu[MENU_PARAMS].add_id;
			strcpy(labels[menu[MENU_VALS].first_id + menu[MENU_PARAMS].selected_label].text,
					hamradio_gui_edit_menu_item(menu[MENU_PARAMS].menu_block[menu[MENU_PARAMS].selected_str].index, encoder2.rotate));

			uint_fast8_t id_sel_label = menu[MENU_VALS].first_id + menu[MENU_VALS].selected_label;
			button_handlers[id_button_up].x1 = labels[id_sel_label].x + strwidth(labels[id_sel_label].text) + 10;
		}

		if ((menu_label_touched || menu_is_scrolling || encoder2.rotate != 0) && menu_level != MENU_VALS)
		{
			encoder2.rotate_done = 1;

			if (encoder2.rotate != 0)
			{
				menu[menu_level].selected_str = (menu[menu_level].selected_str + encoder2.rotate) <= 0 ? 0 : (menu[menu_level].selected_str + encoder2.rotate);
				menu[menu_level].selected_str = menu[menu_level].selected_str > menu[menu_level].count ? menu[menu_level].count : menu[menu_level].selected_str;
			}
			else if (menu_label_touched)
				menu[menu_level].selected_str = menu[menu_level].selected_label + menu[menu_level].add_id;

			menu[MENU_PARAMS].count = hamradio_get_multilinemenu_block_params(menu[MENU_PARAMS].menu_block, menu[MENU_GROUPS].menu_block[menu[MENU_GROUPS].selected_str].index) - 1;

			if (encoder2.rotate > 0)
			{
				// указатель подошел к нижней границе списка
				if (++menu[menu_level].selected_label > (menu[menu_level].count < menu[menu_level].num_rows ? menu[menu_level].count : menu[menu_level].num_rows))
				{
					menu[menu_level].selected_label = (menu[menu_level].count < menu[menu_level].num_rows ? menu[menu_level].count : menu[menu_level].num_rows);
					menu[menu_level].add_id = menu[menu_level].selected_str - menu[menu_level].selected_label;
				}
			}
			if (encoder2.rotate < 0)
			{
				// указатель подошел к верхней границе списка
				if (--menu[menu_level].selected_label < 0)
				{
					menu[menu_level].selected_label = 0;
					menu[menu_level].add_id = menu[menu_level].selected_str;
				}
			}

			if (menu_level == MENU_GROUPS)
				for(uint_fast8_t i = 0; i <= menu[MENU_GROUPS].num_rows; i++)
					strcpy(labels[menu[MENU_GROUPS].first_id + i].text, menu[MENU_GROUPS].menu_block[i + menu[MENU_GROUPS].add_id].name);

			menu[MENU_VALS].count = menu[MENU_PARAMS].count < menu[MENU_VALS].num_rows ? menu[MENU_PARAMS].count : menu[MENU_VALS].num_rows;
			hamradio_get_multilinemenu_block_vals(menu[MENU_VALS].menu_block,  menu[MENU_PARAMS].menu_block[menu[MENU_PARAMS].add_id].index, menu[MENU_VALS].count);

			for(uint_fast8_t i = 0; i <= menu[MENU_PARAMS].num_rows; i++)
			{
				labels[menu[MENU_PARAMS].first_id + i].visible = NON_VISIBLE;
				labels[menu[MENU_PARAMS].first_id + i].state = DISABLED;
				labels[menu[MENU_VALS].first_id + i].visible = NON_VISIBLE;
				labels[menu[MENU_VALS].first_id + i].state = DISABLED;
				if (i > menu[MENU_PARAMS].count)
					continue;
				strcpy(labels[menu[MENU_PARAMS].first_id + i].text, menu[MENU_PARAMS].menu_block[i + menu[MENU_PARAMS].add_id].name);
				strcpy(labels[menu[MENU_VALS].first_id + i].text,   menu[MENU_VALS].menu_block[i].name);
				labels[menu[MENU_PARAMS].first_id + i].visible = VISIBLE;
				labels[menu[MENU_PARAMS].first_id + i].state = CANCELLED;
				labels[menu[MENU_VALS].first_id + i].visible = VISIBLE;
				labels[menu[MENU_VALS].first_id + i].state = CANCELLED;
			}
			menu_label_touched = 0;
		}
		if (menu_level != MENU_VALS)
			colpip_string_tbg(colmain_fb_draw(), DIM_X, DIM_Y, labels[menu[menu_level].selected_label + menu[menu_level].first_id].x - SMALLCHARW,
					labels[menu[menu_level].selected_label + menu[menu_level].first_id].y, ">", COLORPIP_YELLOW);
	}

	uint_fast8_t gui_check_encoder2 (int_least16_t rotate)
	{
		if (encoder2.rotate_done || encoder2.rotate == 0)
		{
			encoder2.rotate = rotate;
			encoder2.rotate_done = 0;
		}
		return encoder2.busy;
	}

	void gui_set_encoder2_state (uint_fast8_t code)
	{
		if (code == KBD_ENC2_PRESS)
			encoder2.press = 1;
		if (code == KBD_ENC2_HOLD)
			encoder2.hold = 1;
		encoder2.press_done = 0;
	}

	static void remove_end_line_spaces(char * str)
	{
		size_t i = strlen(str);
		if (i == 0)
			return;
		for (; -- i > 0;)
		{
			if (str [i] != ' ')
				break;
		}
		str [i + 1] = '\0';
	}

	void gui_encoder2_menu (enc2_menu_t * enc2_menu)
	{
		if (windows[WINDOW_ENC2].state == NON_VISIBLE && enc2_menu->state != 0)
		{
			set_window(WINDOW_ENC2, VISIBLE);
			windows[WINDOW_ENC2].first_call = 1;
			footer_buttons_state(DISABLED, "");
			gui_enc2_menu = enc2_menu;
		}
		else if (windows[WINDOW_ENC2].state == VISIBLE && enc2_menu->state == 0)
		{
			set_window(WINDOW_ENC2, NON_VISIBLE);
			gui_enc2_menu = NULL;
			footer_buttons_state(CANCELLED, "");
		}
	}

	static void window_enc2_process(void)
	{
		static uint_fast8_t id_lbl_param, id_lbl_val;
		static uint_fast16_t window_center_x;

		if (windows[WINDOW_ENC2].first_call == 1)
		{
			calculate_window_position(WINDOW_ENC2);
			window_center_x =  windows[WINDOW_ENC2].x1 + windows[WINDOW_ENC2].w / 2;
			windows[WINDOW_ENC2].first_call = 0;
			id_lbl_param = find_label(WINDOW_ENC2, "lbl_enc2_param");
			id_lbl_val = find_label(WINDOW_ENC2, "lbl_enc2_val");
			labels[id_lbl_param].y = windows[WINDOW_ENC2].y1 + SMALLCHARH * 3;
			labels[id_lbl_val].y = labels[id_lbl_param].y + SMALLCHARH * 2;
			return;
		}
		if(gui_enc2_menu->updated)
		{
			strcpy(labels[id_lbl_param].text, gui_enc2_menu->param);
			remove_end_line_spaces(labels[id_lbl_param].text);
			strcpy(labels[id_lbl_val].text, gui_enc2_menu->val);
			labels[id_lbl_val].color = gui_enc2_menu->state == 2 ? COLORPIP_YELLOW : COLORPIP_WHITE;

			labels[id_lbl_param].x = window_center_x - strwidth(labels[id_lbl_param].text) / 2;
			labels[id_lbl_val].x = window_center_x - strwidth(labels[id_lbl_val].text) / 2;

			gui_enc2_menu->updated = 0;
		}
	}

	static void buttons_mode_handler(void)
	{
		if(gui.selected_type == TYPE_BUTTON)
		{
			if (windows[WINDOW_MODES].state && button_handlers[gui.selected_id].parent == WINDOW_MODES)
			{
				if (button_handlers[gui.selected_id].payload != UINTPTR_MAX)
					hamradio_change_submode(button_handlers[gui.selected_id].payload);

				set_window(WINDOW_MODES, NON_VISIBLE);
				footer_buttons_state(CANCELLED, "");
			}
		}
	}

	static void window_mode_process(void)
	{
		if (windows[WINDOW_MODES].first_call == 1)
		{
			uint_fast8_t interval = 6, id = 0, x, y;
			uint_fast8_t col1_int = 20, row1_int = 40;
			windows[WINDOW_MODES].first_call = 0;
			calculate_window_position(WINDOW_MODES);

			while(button_handlers[++id].parent != WINDOW_MODES)			// первое вхождение кнопки WINDOW_MODES
				;
			x = windows[WINDOW_MODES].x1 + col1_int;
			y = windows[WINDOW_MODES].y1 + row1_int;
			do {
				button_handlers[id].x1 = x;
				button_handlers[id].y1 = y;
				x = x + interval + button_handlers[id].w;
				if (x + button_handlers[id].w > windows[WINDOW_MODES].x1 + windows[WINDOW_MODES].w)
				{
					x = windows[WINDOW_MODES].x1 + col1_int;
					y = windows[WINDOW_MODES].y1 + row1_int + button_handlers[id].h + interval;
				}
			} while (button_handlers[++id].parent == WINDOW_MODES);
			return;
		}
	}

	static void window_agc_process(void)
	{
		if (windows[WINDOW_AGC].first_call == 1)
		{
			uint_fast8_t interval = 40, id = 0, x, y;
			uint_fast8_t col1_int = 20, row1_int = 40;
			windows[WINDOW_AGC].first_call = 0;
			calculate_window_position(WINDOW_AGC);

			while(button_handlers[++id].parent != WINDOW_AGC)			// первое вхождение кнопки WINDOW_AGC
				;
			x = windows[WINDOW_AGC].x1 + col1_int;
			y = windows[WINDOW_AGC].y1 + row1_int;
			do {
				button_handlers[id].x1 = x;
				button_handlers[id].y1 = y;
				x = x + interval + button_handlers[id].w;

			} while (button_handlers[++id].parent == WINDOW_AGC);
			return;
		}
	}

	void gui_open_sys_menu(void)
	{
		button9_handler();
	}

	static void button1_handler(void)
	{
		if(gui.selected_type == TYPE_BUTTON)
		{
			if (windows[WINDOW_MODES].state == NON_VISIBLE)
			{
				set_window(WINDOW_MODES, VISIBLE);
				windows[WINDOW_MODES].first_call = 1;
				footer_buttons_state(DISABLED, button_handlers[gui.selected_id].name);
			}
			else
			{
				set_window(WINDOW_MODES, NON_VISIBLE);
				footer_buttons_state(CANCELLED, "");
			}
		}
	}

	static void button2_handler(void)
	{
		if(gui.selected_type == TYPE_BUTTON)
		{
			if (windows[WINDOW_BP].state == NON_VISIBLE)
			{
				encoder2.busy = 1;
				set_window(WINDOW_BP, VISIBLE);
				windows[WINDOW_BP].first_call = 1;
				footer_buttons_state(DISABLED, button_handlers[gui.selected_id].name);
				hamradio_enable_keyboard_redirect();
			}
			else
			{
				set_window(WINDOW_BP, NON_VISIBLE);
				encoder2.busy = 0;
				footer_buttons_state(CANCELLED, "");
				hamradio_disable_keyboard_redirect();
			}
		}
	}

	static void button3_handler(void)
	{
		if(gui.selected_type == TYPE_BUTTON)
		{
			if (windows[WINDOW_AGC].state == NON_VISIBLE)
			{
				set_window(WINDOW_AGC, VISIBLE);
				windows[WINDOW_AGC].first_call = 1;
				footer_buttons_state(DISABLED, button_handlers[gui.selected_id].name);
			}
			else
			{
				set_window(WINDOW_AGC, NON_VISIBLE);
				footer_buttons_state(CANCELLED, "");
			}
		}
	}

	static void button4_handler(void)
	{
		if(gui.selected_type == TYPE_BUTTON)
		{
			if (windows[WINDOW_FREQ].state == NON_VISIBLE)
			{
				set_window(WINDOW_FREQ, VISIBLE);
				windows[WINDOW_FREQ].first_call = 1;
				hamradio_set_lockmode(1);
				hamradio_enable_keyboard_redirect();
				footer_buttons_state(DISABLED, button_handlers[gui.selected_id].name);
			}
			else
			{
				set_window(WINDOW_FREQ, NON_VISIBLE);
				hamradio_set_lockmode(0);
				hamradio_disable_keyboard_redirect();
				footer_buttons_state(CANCELLED, "");
			}
		}
	}

	static void button5_handler(void)
	{
		if(gui.selected_type == TYPE_BUTTON)
		{

		}

	}

	static void button6_handler(void)
	{
		if(gui.selected_type == TYPE_BUTTON)
		{

		}

	}

	static void button7_handler(void)
	{
		if(gui.selected_type == TYPE_BUTTON)
		{

		}
	}

	static void button8_handler(void)
	{
		if(gui.selected_type == TYPE_BUTTON)
		{

		}
	}

	static void button9_handler(void)
	{
		if(gui.selected_type == TYPE_BUTTON)
		{
			if (windows[WINDOW_MENU].state == NON_VISIBLE)
			{
				set_window(WINDOW_MENU, VISIBLE);
				windows[WINDOW_MENU].first_call = 1;
				footer_buttons_state(DISABLED, button_handlers[gui.selected_id].name);
				encoder2.busy = 1;
			}
			else
			{
				set_window(WINDOW_MENU, NON_VISIBLE);
				footer_buttons_state(CANCELLED, "");
				encoder2.busy = 0;
				hamradio_set_menu_cond(NON_VISIBLE);
			}
		}
	}

	static void gui_main_process(void)
	{
		static window_t * win = & windows[WINDOW_MAIN];
		PACKEDCOLORMAIN_T * fr = colmain_fb_draw();
		char buf [TEXT_ARRAY_SIZE];
		uint_fast16_t yt, xt;

		if (win->first_call == 1)
		{
			uint_fast8_t interval = 3, id = 0, x;
			win->first_call = 0;
			while(button_handlers[++id].parent != WINDOW_MAIN)			// первое вхождение кнопки WINDOW_MAIN
				;
			x = win->x1;
			button_t * bh = & button_handlers[id];
			do {
				bh = & button_handlers[id];
				bh->x1 = x;
				bh->y1 = WITHGUIMAXY - bh->h;
				x = x + interval + bh->w;
				id ++;
			} while (bh->parent == WINDOW_MAIN);
		}

		// текущее время
	#if defined (RTC1_TYPE)
		static uint_fast16_t year;
		static uint_fast8_t month, day, hour, minute, secounds;
		if(gui.timer_1sec_updated)
			board_rtc_getdatetime(& year, & month, & day, & hour, & minute, & secounds);
		local_snprintf_P(buf, sizeof buf / sizeof buf [0], PSTR("%02d.%02d"), day, month);
		colpip_string2_tbg(fr, DIM_X, DIM_Y, 5, 125, buf, COLORPIP_WHITE);
		local_snprintf_P(buf, sizeof buf / sizeof buf [0], PSTR("%02d%c%02d"), hour, ((secounds & 1) ? ' ' : ':'), minute);
		colpip_string2_tbg(fr, DIM_X, DIM_Y, 5, 145, buf, COLORPIP_WHITE);
	#endif 	/* defined (RTC1_TYPE) */

		// напряжение питания
	#if WITHVOLTLEVEL
		static ldiv_t v;
		if(gui.timer_1sec_updated)
			v = ldiv(hamradio_get_volt_value(), 10);
		local_snprintf_P(buf, sizeof buf / sizeof buf [0], PSTR("%d.%1dV "), v.quot, v.rem);
		colpip_string2_tbg(fr, DIM_X, DIM_Y, 75, 125, buf, COLORPIP_WHITE);
	#endif /* WITHVOLTLEVEL */

		// ток PA (при передаче)
	#if WITHCURRLEVEL
		if (hamradio_get_tx())
		{
			static int_fast16_t drain;
			if (gui.timer_1sec_updated)
			{
				drain = hamradio_get_pacurrent_value();	// Ток в десятках милиампер (может быть отрицательным)
				if (drain < 0)
				{
					drain = 0;	// FIXME: без калибровки нуля (как у нас сейчас) могут быть ошибки установки тока
				}
			}

		#if (WITHCURRLEVEL_ACS712_30A || WITHCURRLEVEL_ACS712_20A)
			// для больших токов (более 9 ампер)
			ldiv_t t = ldiv(drain / 10, 10);
			local_snprintf_P(buf, sizeof buf / sizeof buf [0], PSTR("%2d.%01dA "), t.quot, t.rem);

		#else /* (WITHCURRLEVEL_ACS712_30A || WITHCURRLEVEL_ACS712_20A) */
			// Датчик тока до 5 ампер
			ldiv_t t = ldiv(drain, 100);
			local_snprintf_P(buf, sizeof buf / sizeof buf [0], PSTR("%d.%02dA "), t.quot, t.rem);

		#endif /* (WITHCURRLEVEL_ACS712_30A || WITHCURRLEVEL_ACS712_20A) */

			colpip_string2_tbg(fr, DIM_X, DIM_Y, 75, 145, buf, COLORPIP_WHITE);
		}
	#endif /* WITHCURRLEVEL */
	#if WITHTHERMOLEVEL	// температура выходных транзисторов (при передаче)
		static ldiv_t t;
		if (hamradio_get_tx() && gui.timer_1sec_updated)
		{
			t = ldiv(hamradio_get_temperature_value(), 10);
			local_snprintf_P(buf, sizeof buf / sizeof buf [0], PSTR("%d.%dC "), t.quot, t.rem);
			PRINTF("%s\n", buf);		// пока вывод в консоль
		}
	#endif /* WITHTHERMOLEVEL */
		// ширина панорамы
	#if WITHIF4DSP
		static int_fast32_t z;
		if(gui.timer_1sec_updated)
			z = display_zoomedbw();
		local_snprintf_P(buf, sizeof buf / sizeof buf [0], PSTR("SPAN:%3dk"), (int) z / 1000);
		xt = WITHGUIMAXX - SMALLCHARW2 - strlen(buf) * SMALLCHARW2;
		display_transparency(xt - 5, 405, WITHGUIMAXX - 5, 428, DEFAULT_ALPHA);
		colpip_string2_tbg(fr, DIM_X, DIM_Y, xt, 410, buf, COLORPIP_YELLOW);
	#endif /* WITHIF4DSP */
		gui.timer_1sec_updated = 0;
	}

	/* Кнопка */
	static void draw_button_pip(uint_fast8_t id)
	{
		PACKEDCOLORMAIN_T * bg = NULL;
		PACKEDCOLORMAIN_T * fr = colmain_fb_draw();
		btn_bg_t * b1 = NULL;
		uint_fast8_t i = 0;
		static const char delimeters [] = "|";
		const button_t * const bh = & button_handlers[id];

		do {
			if (bh->h == btn_bg[i].h && bh->w == btn_bg[i].w)
			{
				b1 = & btn_bg[i];
				break;
			}
		} while (++i < BG_COUNT);

		if (b1 == NULL)				// если не найден заполненный буфер фона по размерам, программная отрисовка
		{
			PACKEDCOLORPIP_T c1, c2;
			c1 = bh->state == DISABLED ? COLOR_BUTTON_DISABLED : (bh->is_locked ? COLOR_BUTTON_LOCKED : COLOR_BUTTON_NON_LOCKED);
			c2 = bh->state == DISABLED ? COLOR_BUTTON_DISABLED : (bh->is_locked ? COLOR_BUTTON_PR_LOCKED : COLOR_BUTTON_PR_NON_LOCKED);
			colpip_rect(fr, DIM_X, DIM_Y, bh->x1, bh->y1, bh->x1 + bh->w, bh->y1 + bh->h - 2, bh->state == PRESSED ? c2 : c1, 1);
			colpip_rect(fr, DIM_X, DIM_Y, bh->x1, bh->y1, bh->x1 + bh->w, bh->y1 + bh->h - 1, COLORPIP_GRAY, 0);
			colpip_rect(fr, DIM_X, DIM_Y, bh->x1 + 2, bh->y1 + 2, bh->x1 + bh->w - 2, bh->y1 + bh->h - 3, COLORPIP_BLACK, 0);
		}
		else
		{
			if (bh->state == DISABLED)
				bg = (PACKEDCOLORPIP_T *) b1->bg_disabled;
			else if (bh->is_locked && bh->state == PRESSED)
				bg = (PACKEDCOLORPIP_T *) b1->bg_locked_pressed;
			else if (bh->is_locked && bh->state != PRESSED)
				bg = (PACKEDCOLORPIP_T *) b1->bg_locked;
			else if (! bh->is_locked && bh->state == PRESSED)
				bg = (PACKEDCOLORPIP_T *) b1->bg_pressed;
			else if (! bh->is_locked && bh->state != PRESSED)
				bg = (PACKEDCOLORPIP_T *) b1->bg_non_pressed;

			colpip_plot(fr, DIM_X, DIM_Y, bh->x1, bh->y1, bg, bh->w, bh->h);
		}

		if (strchr(bh->text, delimeters[0]) == NULL)
		{
			/* Однострочная надпись */
			colpip_string2_tbg(fr, DIM_X, DIM_Y, bh->x1 + (bh->w - (strwidth2(bh->text))) / 2,
					bh->y1 + (bh->h - SMALLCHARH2) / 2, bh->text, COLORPIP_BLACK);
		} else
		{
			/* Двухстрочная надпись */
			uint_fast8_t j = (bh->h - SMALLCHARH2 * 2) / 2;
			char buf [TEXT_ARRAY_SIZE];
			strcpy(buf, bh->text);
			char * text2 = strtok(buf, delimeters);
			colpip_string2_tbg(fr, DIM_X, DIM_Y, bh->x1 + (bh->w - (strwidth2(text2))) / 2,
					bh->y1 + j, text2, COLORPIP_BLACK);

			text2 = strtok(NULL, delimeters);
			colpip_string2_tbg(fr, DIM_X, DIM_Y, bh->x1 + (bh->w - (strwidth2(text2))) / 2,
					bh->h + bh->y1 - SMALLCHARH2 - j, text2, COLORPIP_BLACK);
		}
	}

	static void fill_button_bg_buf(btn_bg_t * v)
	{
		PACKEDCOLORPIP_T * buf;
		uint_fast8_t w, h;

		w = v->w;
		h = v->h;
		ASSERT(w < BTN_BUF_W);
		ASSERT(h < BTN_BUF_H);

		buf = * v->bg_non_pressed;
		colpip_rect(buf, w, h, 0, 0, w - 1, h - 1, COLOR_BUTTON_NON_LOCKED, 1);
		colpip_rect(buf, w, h, 0, 0, w - 1, h - 1, COLORPIP_GRAY, 0);
		colpip_rect(buf, w, h, 2, 2, w - 3, h - 3, COLORPIP_BLACK, 0);

		buf = * v->bg_pressed;
		colpip_rect(buf, w, h, 0, 0, w - 1, h - 1, COLOR_BUTTON_PR_NON_LOCKED, 1);
		colpip_rect(buf, w, h, 0, 0, w - 1, h - 1, COLORPIP_GRAY, 0);
		colpip_rect(buf, w, h, 2, 2, w - 3, h - 3, COLORPIP_BLACK, 0);

		buf = * v->bg_locked;
		colpip_rect(buf, w, h, 0, 0, w - 1, h - 1, COLOR_BUTTON_LOCKED, 1);
		colpip_rect(buf, w, h, 0, 0, w - 1, h - 1, COLORPIP_GRAY, 0);
		colpip_rect(buf, w, h, 2, 2, w - 3, h - 3, COLORPIP_BLACK, 0);

		buf = * v->bg_locked_pressed;
		colpip_rect(buf, w, h, 0, 0, w - 1, h - 1, COLOR_BUTTON_PR_LOCKED, 1);
		colpip_rect(buf, w, h, 0, 0, w - 1, h - 1, COLORPIP_GRAY, 0);
		colpip_rect(buf, w, h, 2, 2, w - 3, h - 3, COLORPIP_BLACK, 0);

		buf = * v->bg_disabled;
		colpip_rect(buf, w, h, 0, 0, w - 1, h - 1, COLOR_BUTTON_DISABLED, 1);
		colpip_rect(buf, w, h, 0, 0, w - 1, h - 1, COLORPIP_GRAY, 0);
		colpip_rect(buf, w, h, 2, 2, w - 3, h - 3, COLORPIP_BLACK, 0);
	}

	void gui_initialize (void)
	{
		uint_fast8_t i = 0;
		set_window(WINDOW_MAIN, VISIBLE);
		windows[WINDOW_MAIN].first_call = 1;

		do {
			fill_button_bg_buf(& btn_bg[i]);
		} while (++i < BG_COUNT) ;
	}

	static void update_touch(void)
	{
		for (uint_fast8_t i = 0; i < touch_count; i++)
		{
			touch_t * p = & touch_elements[i];
			if (p->type == TYPE_BUTTON)
			{
				const button_t * const bh = & button_handlers[p->id];
				p->x1 = bh->x1;
				p->x2 = bh->x1 + bh->w;
				p->y1 = bh->y1;
				p->y2 = bh->y1 + bh->h;
				p->state = bh->state;
				p->visible = bh->visible;
				p->is_trackable = bh->is_trackable;
			}
			else if (p->type == TYPE_LABEL)
			{
				const label_t * const lh = & labels[p->id];
				p->x1 = lh->x;
				p->x2 = lh->x + strwidth(lh->text);
				p->y1 = lh->y - SMALLCHARH;
				p->y2 = lh->y + SMALLCHARH * 2;
				p->state = lh->state;
				p->visible = lh->visible;
				p->is_trackable = lh->is_trackable;
			}
		}
	}

	static void set_state_record(touch_t * val)
	{
		ASSERT(val != NULL);
		gui.selected_id = val->id;
		switch (val->type)
		{
			case TYPE_BUTTON:
				ASSERT(val->id < BUTTON_HANDLERS_COUNT);
				gui.selected_type = TYPE_BUTTON;
				button_handlers[val->id].state = val->state;
				if (button_handlers[val->id].onClickHandler && button_handlers[val->id].state == RELEASED)
					button_handlers[val->id].onClickHandler();
				break;

			case TYPE_LABEL:
				ASSERT(val->id < LABELS_COUNT);
				gui.selected_type = TYPE_LABEL;
				labels[val->id].state = val->state;
				if (labels[val->id].onClickHandler && labels[val->id].state == RELEASED)
					labels[val->id].onClickHandler();
				break;
			default:
				ASSERT(0);
				break;
		}
	}

	static void process_gui(void)
	{
		uint_fast16_t tx, ty;
		static uint_fast16_t x_old = 0, y_old = 0;
		static touch_t * p = NULL;

	#if defined (TSC1_TYPE)
		if (board_tsc_getxy(& tx, & ty))
		{
			gui.last_pressed_x = tx;
			gui.last_pressed_y = ty;
			gui.is_touching_screen = 1;
			debug_printf_P(PSTR("last x/y=%d/%d\n"), gui.last_pressed_x, gui.last_pressed_y);
			update_touch();
		}
		else
	#endif /* defined (TSC1_TYPE) */
		{
			gui.is_touching_screen = 0;
			gui.is_after_touch = 0;
		}

		if (gui.state == CANCELLED && gui.is_touching_screen && ! gui.is_after_touch)
		{
			for (uint_fast8_t i = 0; i < touch_count; i++)
			{
				p = & touch_elements[i];

				if (p->x1 < gui.last_pressed_x && p->x2 > gui.last_pressed_x
				 && p->y1 < gui.last_pressed_y && p->y2 > gui.last_pressed_y
				 && p->state != DISABLED && p->visible == VISIBLE)
				{
					gui.state = PRESSED;
					break;
				}
			}
		}

		if (gui.is_tracking && ! gui.is_touching_screen)
		{
			gui.is_tracking = 0;
			gui.vector_move_x = 0;
			gui.vector_move_y = 0;
			x_old = 0;
			y_old = 0;
		}

		if (gui.state == PRESSED)
		{
			ASSERT(p != NULL);
			if (p->is_trackable && gui.is_touching_screen)
			{
				gui.vector_move_x = x_old ? gui.vector_move_x + gui.last_pressed_x - x_old : 0; // проверить, нужно ли оставить накопление
				gui.vector_move_y = y_old ? gui.vector_move_y + gui.last_pressed_y - y_old : 0;
				p->state = PRESSED;
				set_state_record(p);
				if (gui.vector_move_x != 0 || gui.vector_move_y != 0)
				{
					gui.is_tracking = 1;
//					debug_printf_P(PSTR("move x: %d, move y: %d\n"), gui.vector_move_x, gui.vector_move_y);
				}
				x_old = gui.last_pressed_x;
				y_old = gui.last_pressed_y;
			}
			else if (p->x1 < gui.last_pressed_x && p->x2 > gui.last_pressed_x
			 && p->y1 < gui.last_pressed_y && p->y2 > gui.last_pressed_y && ! gui.is_after_touch)
			{
				if (gui.is_touching_screen)
				{
					p->state = PRESSED;
					set_state_record(p);
				}
				else
					gui.state = RELEASED;
			}
			else
			{
				gui.state = CANCELLED;
				p->state = CANCELLED;
				set_state_record(p);
				gui.is_after_touch = 1; 	// точка непрерывного нажатия вышла за пределы выбранного элемента, не поддерживающего tracking
			}
		}
		if (gui.state == RELEASED)
		{
			p->state = RELEASED;			// для запуска обработчика нажатия
			set_state_record(p);
			p->state = CANCELLED;
			set_state_record(p);
			gui.is_after_touch = 0;
			gui.state = CANCELLED;
			gui.is_tracking = 0;
		}
	}

	void gui_update(uint_fast8_t x, uint_fast8_t y, dctx_t * pctx)
	{
		uint_fast8_t alpha = DEFAULT_ALPHA; // на сколько затемнять цвета
		char buf [TEXT_ARRAY_SIZE];
		char * text2 = NULL;
		uint_fast8_t str_len = 0;
		PACKEDCOLORMAIN_T * fr = colmain_fb_draw();

		process_gui();

		for (uint_fast8_t i = 0; i < windows_count; i ++)
		{
			const window_t * const win = & windows[i];
			if (win->state == VISIBLE)
			{
				// при открытии окна рассчитываются экранные координаты самого окна и его child элементов
				if (win->first_call == 0)
				{
					display_transparency(win->x1, win->y1, win->x1 + win->w, win->y1 + win->h, alpha);
				}

				// запуск процедуры фоновой обработки для окна
				win->onVisibleProcess();

				if (! win->first_call )
				{
					// вывод заголовка окна
					colpip_string_tbg(fr, DIM_X, DIM_Y, win->x1 + 20, win->y1 + 10, win->name, COLORPIP_YELLOW);

					// отрисовка принадлежащих окну элементов

					// метки
					for (uint_fast8_t i = 1; i < LABELS_COUNT; i++)
					{
						const label_t * const lh = & labels[i];
						if (lh->visible == VISIBLE && lh->parent == win->window_id)
							colpip_string_tbg(fr, DIM_X, DIM_Y, lh->x, lh->y, lh->text, lh->color);
					}

					// кнопки
					for (uint_fast8_t i = 1; i < BUTTON_HANDLERS_COUNT; i++)
					{
						const button_t * const bh = & button_handlers[i];
						if (bh->visible == VISIBLE && bh->parent == win->window_id)
							draw_button_pip(i);
					}
				}
			}
		}
	}
#endif /* WITHTOUCHGUI */