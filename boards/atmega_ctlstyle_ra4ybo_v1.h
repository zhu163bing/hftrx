/* $Id$ */
//
// РџСЂРѕРµРєС‚ HF Dream Receiver (РљР’ РїСЂРёС‘РјРЅРёРє РјРµС‡С‚С‹)
// Р°РІС‚РѕСЂ Р“РµРЅР° Р—Р°РІРёРґРѕРІСЃРєРёР№ mgs2001@mail.ru
// UA1ARN
//
// Р°РїРїР°СЂР°С‚СѓС‚Р° С‚СЂР°РЅСЃРёРІРµСЂР° RA4YBO V1 - "РїРѕСЂС‚Р°С‚РёРІРЅС‹Р№ СЃ С‚СЋРЅРµСЂСЂРј Р±РµР· С‡Р°СЃРѕРІ"
//

#ifndef ATMEGA_CTLSTYLE_RA4YBO_V1_H_INCLUDED
#define ATMEGA_CTLSTYLE_RA4YBO_V1_H_INCLUDED 1

#if F_CPU != 8000000
	#error Set F_CPU right value in project file
#endif

#if \
	defined (__AVR_ATmega644__) || \
	defined (__AVR_ATmega644P__) || \
	defined (__AVR_ATmega644PA__) || \
	0

#else
	//#error Set CPU TYPE to ATMEGA644PA in project file
#endif

	/* РјРѕРґРµР»Рё СЃРёРЅС‚РµР·Р°С‚РѕСЂРѕРІ - СЃС…РµРјС‹ С‡Р°СЃС‚РѕС‚РѕРѕР±СЂР°Р·РѕРІР°РЅРёСЏ */

	// --- РІР°СЂРёР°С†РёРё РїСЂРѕС€РёРІРєРё, СЃРїРµС†РёС„РёС‡РµСЃРєРёРµ РґР»СЏ СЂР°Р·РЅС‹С… РїР»Р°С‚
	#define CTLREGMODE_RA4YBO_V1	1		/* СѓРїСЂР°РІР»СЏСЋС‰РёР№ СЂРµРіРёСЃС‚СЂ 	*/

	#define WITHPREAMPATT2_10DB		1	// РЈРїСЂР°РІР»РµРЅРёРµ РЈР’Р§ Рё РґРІСѓС…РєР°СЃРєР°РґРЅС‹Рј Р°С‚С‚РµРЅСЋР°С‚РѕСЂРѕРј

	#define	BOARD_AGCCODE_0		0x00
	#define	BOARD_AGCCODE_1		0x01
	#define	BOARD_AGCCODE_2		0x02
	#define	BOARD_AGCCODE_3		0x03

	#define BOARD_AGCCODE_OFF 255
	#define	BOARD_AGCCODE_FAST	BOARD_AGCCODE_0
	#define	BOARD_AGCCODE_MED	BOARD_AGCCODE_1
	#define	BOARD_AGCCODE_SLOW	BOARD_AGCCODE_2
	#define	BOARD_AGCCODE_LONG	BOARD_AGCCODE_3
	#define WITHAGCMODE4STAGES	1	// 4 СЃРєРѕСЂРѕСЃС‚Рё. РІС‹РєР»СЋС‡РµРЅРЅРѕ РЅРµ Р±С‹РІР°РµС‚

	#define	FONTSTYLE_ITALIC	1	//
	#define DSTYLE_UR3LMZMOD	1

	// +++ РІР°СЂРёР°С†РёРё РїСЂРѕС€РёРІРєРё, СЃРїРµС†РёС„РёС‡РµСЃРєРёРµ РґР»СЏ РїР»Р°С‚ РЅР° ATMega
	// --- РІР°СЂРёР°С†РёРё РїСЂРѕС€РёРІРєРё, СЃРїРµС†РёС„РёС‡РµСЃРєРёРµ РґР»СЏ РїР»Р°С‚ РЅР° ATMega

	// +++ РћСЃРѕР±С‹Рµ РІР°СЂРёР°РЅС‚С‹ СЂР°СЃРїРѕР»РѕР¶РµРЅРёСЏ РєРЅРѕРїРѕРє РЅР° РєР»Р°РІРёР°С‚СѓСЂРµ
	#define KEYB_RA4YBO_V1	1	/*  */
	//#define KEYB_RA1AGG	1	/* СЂР°СЃРїРѕР»РѕР¶РµРЅРёРµ РєРЅРѕРїРѕРє РґР»СЏ РєРѕСЂРїСѓСЃР°, СЃРґРµР»Р°РЅРЅРѕРіРѕ RA1AGG - С‚СЂРё РіСЂСѓРїРїС‹ РїРѕ С‡РµС‚С‹СЂРµ Р»РёРЅРµР№РєРё РєРЅРѕРїРѕРє РґСЂСѓРі Р·Р° РґСЂСѓРіРѕРј РІ РѕРґРёРЅ СЂСЏРґ. */
	//#define KEYB_UA1CEI	1	/* СЂР°СЃРїРѕР»РѕР¶РµРЅРёРµ РєРЅРѕРїРѕРє РґР»СЏ UA1CEI */
	//#define KEYB_VERTICAL_REV	1	/* СЂР°СЃРїРѕР»РѕР¶РµРЅРёРµ РєРЅРѕРїРѕРє РґР»СЏ РїР»Р°С‚ "Р’РѕСЂРѕР±РµР№" Рё "РљРѕР»РёР±СЂРё" */
	//#define KEYBOARD_USE_ADC6	1	/* С€РµСЃС‚СЊ РєРЅРѕРїРѕРє РЅР° РєР°Р¶РґРѕРј РІС…РѕРґРµ ADCx	*/
	// --- РћСЃРѕР±С‹Рµ РІР°СЂРёР°РЅС‚С‹ СЂР°СЃРїРѕР»РѕР¶РµРЅРёСЏ РєРЅРѕРїРѕРє РЅР° РєР»Р°РІРёР°С‚СѓСЂРµ

	// +++ РћРґРЅР° РёР· СЌС‚РёС… СЃС‚СЂРѕРє РѕРїСЂРµРґРµР»СЏРµС‚ С‚РёРї РґРёСЃРїР»РµСЏ, РґР»СЏ РєРѕС‚РѕСЂРѕРіРѕ РєРѕРјРїРёР»РёСЂСѓРµС‚СЃСЏ РїСЂРѕС€РёРІРєР°
	//#define LCDMODE_HARD_SPI	1		/* LCD over SPI line */
	//#define LCDMODE_WH2002	1	/* С‚РёРї РїСЂРёРјРµРЅСЏРµРјРѕРіРѕ РёРЅРґРёРєР°С‚РѕСЂР° 20*2, РІРѕР·РјРѕР¶РЅРѕ РІРјРµСЃС‚Рµ СЃ LCDMODE_HARD_SPI */
	//#define LCDMODE_WH1602	1	/* С‚РёРї РїСЂРёРјРµРЅСЏРµРјРѕРіРѕ РёРЅРґРёРєР°С‚РѕСЂР° 16*2 */
	//#define LCDMODE_WH2004	1	/* С‚РёРї РїСЂРёРјРµРЅСЏРµРјРѕРіРѕ РёРЅРґРёРєР°С‚РѕСЂР° 20*4 */
	//#define LCDMODE_RDX0077	1	/* РРЅРґРёРєР°С‚РѕСЂ 128*64 СЃ РєРѕРЅС‚СЂРѕР»Р»РµСЂРѕРј UC1601.  */
	//#define LCDMODE_RDX0154	1	/* РРЅРґРёРєР°С‚РѕСЂ 132*64 СЃ РєРѕРЅС‚СЂРѕР»Р»РµСЂРѕРј UC1601. */
	//#define LCDMODE_RDX0120	1	/* РРЅРґРёРєР°С‚РѕСЂ 64*32 СЃ РєРѕРЅС‚СЂРѕР»Р»РµСЂРѕРј UC1601.  */
	//#define LCDMODE_UC1601S_XMIRROR	1	/* РРЅРґРёРєР°С‚РѕСЂ 132*64 СЃ РєРѕРЅС‚СЂРѕР»Р»РµСЂРѕРј UC1601.  */
	//#define LCDMODE_UC1601S_TOPDOWN	1	/* LCDMODE_RDX0154 - РїРµСЂРµРІРµСЂРЅСѓС‚СЊ РёР·РѕР±СЂР°Р¶РµРЅРёРµ */
	//#define LCDMODE_UC1601S_EXTPOWER	1	/* LCDMODE_RDX0154 - Р±РµР· РІРЅСѓС‚СЂРµРЅРЅРµРіРѕ РїСЂРµРѕР±СЂР°Р·РѕРІР°С‚РµР»СЏ */
	//#define LCDMODE_TIC154	1	/* РРЅРґРёРєР°С‚РѕСЂ 133*65 СЃ РєРѕРЅС‚СЂРѕР»Р»РµСЂРѕРј PCF8535 */
	//#define LCDMODE_TIC218	1	/* РРЅРґРёРєР°С‚РѕСЂ 133*65 СЃ РєРѕРЅС‚СЂРѕР»Р»РµСЂРѕРј PCF8535 */
	//#define LCDMODE_PCF8535_TOPDOWN	1	/* РљРѕРЅС‚СЂРѕР»Р»РµСЂ PCF8535 - РїРµСЂРµРІРµСЂРЅСѓС‚СЊ РёР·РѕР±СЂР°Р¶РµРЅРёРµ */
	#define LCDMODE_LS020 	1	/* РРЅРґРёРєР°С‚РѕСЂ 176*132 Sharp LS020B8UD06 СЃ РєРѕРЅС‚СЂРѕР»Р»РµСЂРѕРј LR38826 */
	#define LCDMODE_LS020_TOPDOWN	1	/* LCDMODE_LS020 - РїРµСЂРµРІРµСЂРЅСѓС‚СЊ РёР·РѕР±СЂР°Р¶РµРЅРёРµ */
	//#define LCDMODE_LPH88		1	/* РРЅРґРёРєР°С‚РѕСЂ 176*132 LPH8836-2 СЃ РєРѕРЅС‚СЂРѕР»Р»РµСЂРѕРј Hitachi HD66773 */
	//#define LCDMODE_LPH88_TOPDOWN	1	/* LCDMODE_LPH88 - РїРµСЂРµРІРµСЂРЅСѓС‚СЊ РёР·РѕР±СЂР°Р¶РµРЅРёРµ */
	//#define LCDMODE_ILI9163	1	/* РРЅРґРёРєР°С‚РѕСЂ LPH9157-2 176*132 СЃ РєРѕРЅС‚СЂРѕР»Р»РµСЂРѕРј ILITEK ILI9163 - Р›РµРЅС‚Р° РґРёСЃРїР»РµСЏ СЃРїСЂР°РІР°, Р° РІС‹РІРѕРґС‹ СЃР»РµРІР°. */
	//#define LCDMODE_ILI9163_TOPDOWN	1	/* LCDMODE_ILI9163 - РїРµСЂРµРІРµСЂРЅСѓС‚СЊ РёР·РѕР±СЂР°Р¶РµРЅРёРµ (РґР»СЏ РІС‹РІРѕРґРѕРІ СЃРїСЂР°РІР°, Р»РµРЅС‚Р° РґРёСЃРїР»РµСЏ СЃР»РµРІР°) */
	//#define LCDMODE_L2F50	1	/* РРЅРґРёРєР°С‚РѕСЂ 176*132 СЃ РєРѕРЅС‚СЂРѕР»Р»РµСЂРѕРј Epson L2F50126 */
	//#define LCDMODE_L2F50_TOPDOWN	1	/* РџРµСЂРµРІРѕСЂРѕС‚ РёР·РѕР±СЂР°Р¶РµРЅРё СЏ РІ СЃР»СѓС‡Р°Рµ LCDMODE_L2F50 */
	//#define LCDMODE_S1D13781	1	/* РРЅРЅРґРёРєР°С‚РѕСЂ 480*272 СЃ РєРѕРЅС‚СЂРѕР»Р»РµСЂРѕРј Epson S1D13781 */
	//#define LCDMODE_S1D13781_TOPDOWN	1	/* LCDMODE_S1D13781 - РїРµСЂРµРІРµСЂРЅСѓС‚СЊ РёР·РѕР±СЂР°Р¶РµРЅРёРµ */
	//#define LCDMODE_ILI9225	1	/* РРЅРґРёРєР°С‚РѕСЂ 220*176 SF-TC220H-9223A-N_IC_ILI9225C_2011-01-15 СЃ РєРѕРЅС‚СЂРѕР»Р»РµСЂРѕРј ILI9225РЎ */
	//#define LCDMODE_ILI9225_TOPDOWN	1	/* LCDMODE_ILI9225 - РїРµСЂРµРІРµСЂРЅСѓС‚СЊ РёР·РѕР±СЂР°Р¶РµРЅРёРµ (РґР»СЏ РІС‹РІРѕРґРѕРІ СЃР»РµРІР° РѕС‚ СЌРєСЂР°РЅР°) */
	//#define LCDMODE_UC1608	1		/* РРЅРґРёРєР°С‚РѕСЂ 240*128 СЃ РєРѕРЅС‚СЂРѕР»Р»РµСЂРѕРј UC1608.- РјРѕРЅРѕС…СЂРѕРјРЅС‹Р№ */
	//#define LCDMODE_UC1608_TOPDOWN	1	/* LCDMODE_UC1608 - РїРµСЂРµРІРµСЂРЅСѓС‚СЊ РёР·РѕР±СЂР°Р¶РµРЅРёРµ (РґР»СЏ РІС‹РІРѕРґРѕРІ СЃРІРµСЂС…Сѓ) */
	//#define LCDMODE_ST7735	1	/* РРЅРґРёРєР°С‚РѕСЂ 160*128 СЃ РєРѕРЅС‚СЂРѕР»Р»РµСЂРѕРј Sitronix ST7735 */
	//#define LCDMODE_ST7735_TOPDOWN	1	/* LCDMODE_ST7735 - РїРµСЂРµРІРµСЂРЅСѓС‚СЊ РёР·РѕР±СЂР°Р¶РµРЅРёРµ (РґР»СЏ РІС‹РІРѕРґРѕРІ СЃРїСЂР°РІР°) */
	//#define LCDMODE_ST7565S	1	/* РРЅРґРёРєР°С‚РѕСЂ WO12864C2-TFH# 128*64 СЃ РєРѕРЅС‚СЂРѕР»Р»РµСЂРѕРј Sitronix ST7565S */
	//#define LCDMODE_ST7565S_TOPDOWN	1	/* LCDMODE_ST7565S - РїРµСЂРµРІРµСЂРЅСѓС‚СЊ РёР·РѕР±СЂР°Р¶РµРЅРёРµ (РґР»СЏ РІС‹РІРѕРґРѕРІ СЃРІРµСЂС…Сѓ) */
	//#define LCDMODE_ILI9320	1	/* РРЅРґРёРєР°С‚РѕСЂ 248*320 СЃ РєРѕРЅС‚СЂРѕР»Р»РµСЂРѕРј ILI9320 */
	// --- РћРґРЅР° РёР· СЌС‚РёС… СЃС‚СЂРѕРє РѕРїСЂРµРґРµР»СЏРµС‚ С‚РёРї РґРёСЃРїР»РµСЏ, РґР»СЏ РєРѕС‚РѕСЂРѕРіРѕ РєРѕРјРїРёР»РёСЂСѓРµС‚СЃСЏ РїСЂРѕС€РёРІРєР°

	#define ENCRES_DEFAULT ENCRES_128
	//#define ENCRES_DEFAULT ENCRES_100
	//#define ENCRES_DEFAULT ENCRES_24
	#define	WITHENCODER	1	/* РґР»СЏ РёР·РјРµРЅРµРЅРёСЏ С‡Р°СЃС‚РѕС‚С‹ РёРјРµРµС‚СЃСЏ РµРЅРєРѕРґРµСЂ */

	// +++ Р­С‚Рё СЃС‚СЂРѕРєРё РјРѕР¶РЅРѕ РѕС‚РєР»СЋС‡Р°С‚СЊ, СѓРјРµРЅСЊС€Р°СЏ С„СѓРЅРєС†РёРѕРЅР°Р»СЊРЅРѕСЃС‚СЊ РіРѕС‚РѕРІРѕРіРѕ РёР·РґРµР»РёСЏ
	#define WITHTX		1	/* РІРєР»СЋС‡РµРЅРѕ СѓРїСЂР°РІР»РµРЅРёРµ РїРµСЂРµРґР°С‚С‡РёРєРѕРј - СЃРёРєРІРµРЅСЃРѕСЂ, СЌР»РµРєС‚СЂРѕРЅРЅС‹Р№ РєР»СЋС‡. */
	//#define WITHCAT		1	/* РёСЃРїРѕР»СЊР·СѓРµС‚СЃСЏ CAT */
	//#define WITHDEBUG		1	/* РћС‚Р»Р°РґРѕС‡РЅР°СЏ РїРµС‡Р°С‚СЊ С‡РµСЂРµР· COM-РїРѕСЂС‚. Р‘РµР· CAT (WITHCAT) */
	//#define WITHVOX		1	/* РёСЃРїРѕР»СЊР·СѓРµС‚СЃСЏ VOX */
	#define WITHSWRMTR	1		/* РР·РјРµСЂРёС‚РµР»СЊ РљРЎР’ РёР»Рё */
	#define WITHSHOWSWRPWR	1	/* РЅР° РґРёСЃРїР»РµРµ РѕРґРЅРѕРІСЂРµРјРµРЅРЅРѕ РѕС‚РѕР±СЂР°Р¶Р°СЋС‚СЃСЏ SWR-meter Рё PWR-meter */
	//#define WITHPWRMTR	1	/* РРЅРґРёРєР°С‚РѕСЂ РІС‹С…РѕРґРЅРѕР№ РјРѕС‰РЅРѕСЃС‚Рё  */
	#define WITHBARS	1	/* РѕС‚РѕР±СЂР°Р¶РµРЅРёРµ S-РјРµС‚СЂР° Рё SWR-РјРµС‚СЂР° */
	#define WITHVOLTLEVEL	1	/* РѕС‚РѕР±СЂР°Р¶РµРЅРёРµ РЅР°РїСЂСЏР¶РµРЅРёСЏ РђРљР‘ */
	//#define WITHSWLMODE	1	/* РїРѕРґРґРµСЂР¶РєР° Р·Р°РїРѕРјРёРЅР°РЅРёСЏ РјРЅРѕР¶РµСЃС‚РІР° С‡Р°СЃС‚РѕС‚ РІ swl-mode */
	//#define WITHPBT		1	/* РёСЃРїРѕР»СЊР·СѓРµС‚СЃСЏ PBT */
	#define WITHIFSHIFT	1	/* РёСЃРїРѕР»СЊР·СѓРµС‚СЃСЏ IF SHIFT */
	//#define WITHIFSHIFTOFFSET	(-250)	/* РќР°С‡Р°Р»СЊРЅРѕРµ Р·Р°РЅС‡РµРЅРёРµ IF SHIFT */
	#define WITHMODESETFULLNFM 1
	#define WITHWFM	1			/* РёСЃРїРѕР»СЊР·СѓРµС‚СЃСЏ WFM */

	#define	WITHAUTOTUNER	1	/* Р•СЃС‚СЊ С„СѓРЅРєС†РёСЏ Р°РІС‚РѕС‚СЋРЅРµСЂР° */
	#define SHORTSET7 1
	//#define FULLSET7 1

	#define WITHMENU 	1	/* С„СѓРЅРєС†РёРѕРЅР°Р»СЊРЅРѕСЃС‚СЊ РјРµРЅСЋ РјРѕР¶РµС‚ Р±С‹С‚СЊ РѕС‚РєР»СЋС‡РµРЅР° - РµСЃР»Рё РЅР°СЃС‚СЂР°РёРІР°С‚СЊ РЅРµС‡РµРіРѕ */
	#define WITHVIBROPLEX	1	/* РІРѕР·РјРѕР¶РЅРѕСЃС‚СЊ СЌРјСѓР»СЏС†РёРё РїРµСЂРµРґР°С‡Рё РІРёР±СЂРѕРїР»РµРєСЃРѕРј */
	#define WITHSLEEPTIMER	1	/* РІС‹РєР»СЋС‡РёС‚СЊ РёРЅРґРёРєР°С‚РѕСЂ Рё РІС‹РІРѕРґ Р·РІСѓРєР° РїРѕ РёСЃС‚РµС‡РµРЅРёРёРё СѓРєР°Р·Р°РЅРЅРѕРіРѕ РІСЂРµРјРµРЅРё */
	// --- Р­С‚Рё СЃС‚СЂРѕРєРё РјРѕР¶РЅРѕ РѕС‚РєР»СЋС‡Р°С‚СЊ, СѓРјРµРЅСЊС€Р°СЏ С„СѓРЅРєС†РёРѕРЅР°Р»СЊРЅРѕСЃС‚СЊ РіРѕС‚РѕРІРѕРіРѕ РёР·РґРµР»РёСЏ

	//#define LO1PHASES	1		/* РџСЂСЏРјРѕР№ СЃРёРЅС‚РµР· РїРµСЂРІРѕРіРѕ РіРµС‚РµСЂРѕРґРёРЅР° РґРІСѓРјСЏ DDS СЃ РїСЂРѕРіСЂР°РјРёРјСЂСѓРµРјС‹Рј СЃРґРІРёРіРѕРј С„Р°Р·С‹ */
	#define DEFPREAMPSTATE 	1	/* РЈР’Р§ РїРѕ СѓРјРѕР»С‡Р°РЅРёСЋ РІРєР»СЋС‡С‘РЅ (1) РёР»Рё РІС‹РєР»СЋС‡РµРЅ (0) */
	
	//#define REQUEST_FOR_RN4NAB 1	/* РЎРїРµС†РёР°Р»СЊРЅС‹Р№ РІР°СЂРёР°РЅС‚ РґРёР°РїР°Р·РѕРЅРЅС‹С… С„РёР»СЊС‚СЂРѕРІ */

	/* С‡С‚Рѕ Р·Р° РїР°РјСЏС‚СЊ РЅР°СЃС‚СЂРѕРµРє Рё С‡Р°СЃС‚РѕС‚ РёСЃРїРѕР»СЊР·СѓРµС‚СЃСЏ РІ РєРѕРЅС‚СЂРѕР»Р»РµСЂРµ */
	//#define NVRAM_TYPE NVRAM_TYPE_FM25XXXX	// SERIAL FRAM AUTODETECT
	//#define NVRAM_TYPE NVRAM_TYPE_FM25L04	// РўР°Рє Р¶Рµ РїСЂРё РёСЃРїРѕР»СЊР·РѕРІР°РЅРёРё FM25040A - 5 РІРѕР»СЊС‚, 512 Р±Р°Р№С‚
	#define NVRAM_TYPE NVRAM_TYPE_FM25L16
	//#define NVRAM_TYPE NVRAM_TYPE_FM25L64
	//#define NVRAM_TYPE NVRAM_TYPE_FM25L256	// FM25L256, FM25W256
	//#define NVRAM_TYPE NVRAM_TYPE_CPUEEPROM

	//#define NVRAM_TYPE NVRAM_TYPE_AT25040A
	//#define NVRAM_TYPE NVRAM_TYPE_AT25L16
	//#define NVRAM_TYPE NVRAM_TYPE_AT25256A

	// End of NVRAM definitions section

	//#define FTW_RESOLUTION 28	/* СЂР°Р·СЂСЏРґРЅРѕСЃС‚СЊ FTW РІС‹Р±СЂР°РЅРЅРѕРіРѕ DDS */
	#define FTW_RESOLUTION 32	/* СЂР°Р·СЂСЏРґРЅРѕСЃС‚СЊ FTW РІС‹Р±СЂР°РЅРЅРѕРіРѕ DDS */
	//#define FTW_RESOLUTION 31	/* СЂР°Р·СЂСЏРґРЅРѕСЃС‚СЊ FTW РІС‹Р±СЂР°РЅРЅРѕРіРѕ DDS - ATINY2313 */

	//#define WITHLO1LEVELADJ		1	/* РІРєР»СЋС‡РµРЅРѕ СѓРїСЂР°РІР»РµРЅРёРµ СѓСЂРѕРІРЅРµРј (Р°РјРїР»РёС‚СѓРґРѕР№) LO1 */
	/* Board hardware configuration */
	//#define DDS1_TYPE DDS_TYPE_AD9951
	//#define DDS1_TYPE DDS_TYPE_AD9835
	//#define DDS1_TYPE DDS_TYPE_AD9851
	#define PLL1_TYPE PLL_TYPE_SI570
	#define PLL1_FRACTIONAL_LENGTH	28	/* Si570: lower 28 bits is a fractional part */
	//#define DDS1_TYPE DDS_TYPE_ATTINY2313

	//#define DDS1_TYPE DDS_TYPE_AD9834

	//#define WITHFIXEDBFO	1	/* РџРµСЂРµРІРѕСЂРѕС‚ Р±РѕРєРѕРІС‹С… Р·Р° СЃС‡С‘С‚ 1-РіРѕ РіРµС‚СЂРѕРґРёРЅР° (РѕСЃРѕР±РµРЅРЅРѕ, РµСЃР»Рё РЅРµС‚ РїРѕРґСЃС‚СЂР°РёРІР°РµРјРѕРіРѕ BFO) */
	//#define WITHDUALFLTR	1	/* РџРµСЂРµРІРѕСЂРѕС‚ Р±РѕРєРѕРІС‹С… Р·Р° СЃС‡С‘С‚ РїРµСЂРµРєР»СЋС‡РµРЅРёСЏ С„РёР»СЊС‚СЂР° РІРµСЂС…РЅРµР№ РёР»Рё РЅРёР¶РЅРµР№ Р±РѕРєРѕРІРѕР№ РїРѕР»РѕСЃС‹ */
	#define WITHSAMEBFO	1		/* РёСЃРїРѕР»СЊР·РѕРІР°РЅРёРµ РѕР±С‰РёС… РЅР°СЃС‚СЂРѕРµРє BFO РґР»СЏ РїСЂРёС‘РјР° Рё РїРµСЂРµРґР°С‡Рё */
	#define DDS2_TYPE DDS_TYPE_AD9834
	//#define PLL1_TYPE PLL_TYPE_ADF4001
	//#define PLL2_TYPE PLL_TYPE_ADF4001	/* Р”РµР»РёС‚РµР»СЊ РѕРєС‚Р°РІРЅРѕРіРѕ РіРµС‚РµСЂРѕРґРёРЅР° РґР»СЏ РїРѕР»СѓС‡РµРЅРёСЏ СЃРєРІРѕР·РЅРѕРіРѕ РґРёР°РїР°Р·РѕРЅР° */
	#define PLL2_TYPE PLL_TYPE_LMX2306	/* Р”РµР»РёС‚РµР»СЊ РѕРєС‚Р°РІРЅРѕРіРѕ РіРµС‚РµСЂРѕРґРёРЅР° РґР»СЏ РїРѕР»СѓС‡РµРЅРёСЏ СЃРєРІРѕР·РЅРѕРіРѕ РґРёР°РїР°Р·РѕРЅР° */

	//#define DDS1_CLK_DIV	7		/* Р”РµР»РёС‚РµР»СЊ РѕРїРѕСЂРЅРѕР№ С‡Р°СЃС‚РѕС‚С‹ РїРµСЂРµРґ РїРѕРґР°С‡РµР№ РІ DDS1 ATTINY2313 */
	#define DDS1_CLK_DIV	1		/* Р”РµР»РёС‚РµР»СЊ РѕРїРѕСЂРЅРѕР№ С‡Р°СЃС‚РѕС‚С‹ РїРµСЂРµРґ РїРѕРґР°С‡РµР№ РІ DDS1 */
	#define DDS2_CLK_DIV	1		/* Р”РµР»РёС‚РµР»СЊ РѕРїРѕСЂРЅРѕР№ С‡Р°СЃС‚РѕС‚С‹ РїРµСЂРµРґ РїРѕРґР°С‡РµР№ РІ DDS2 */

	//#define RTC1_TYPE RTC_TYPE_M41T81	/* ST M41T81M6 RTC clock chip with I2C interface */

	/* РќР°Р·РЅР°С‡РµРЅРёРµ Р°РґСЂРµСЃРѕРІ РЅР° SPI С€РёРЅРµ */
	//#define targetdds1 SPI_CSEL0 	/* DDS1 */
	#define targetdds2 SPI_CSEL1 	/* DDS2 - LO3 output */
	//#define targetpll1 SPI_CSEL2 	/* ADF4001 after DDS1 - divide by r1 and scale to n1. Р”Р»СЏ РґРІРѕР№РЅРѕР№ DDS РїРµСЂРІРѕРіРѕ РіРµС‚РµСЂРѕРґРёРЅР° - РІС‚РѕСЂР°СЏ DDS */
	//#define targetpll2 SPI_CSEL3 	/* ADF4001 - fixed 2-nd LO generate or LO1 divider */ 
	#define targetpll2 SPI_CSEL2 	/* RA4YBO: LMX2306 - fixed 2-nd LO generate or LO1 divider */ 
	#define targetext1 SPI_CSEL4 	/* external devices control */ 
	//#define targetupd1 SPI_CSEL5 	/* DDS IOUPDATE signals at output of SPI chip-select register */
	#define targetctl1 SPI_CSEL6 	/* control register */
	#define targetnvram SPI_CSEL7  	/* serial nvram */

	#define targetlcd targetext1 	/* LCD over SPI line devices control */ 
	#define targetuc1608 SPI_CSEL255	/* LCD with positive chip select signal	*/
	

	/* РєРѕРґС‹ РІС…РѕРґРѕРІ РєРѕРјРјСѓС‚Р°С‚РѕСЂР° РёСЃС‚РѕС‡РЅРёРєРѕРІ СЃРёРіРЅР°Р»Р° РґР»СЏ РЈРќР§ РїСЂРёС‘РјРЅРёРєР° */
	#define BOARD_DETECTOR_SSB 0x03
	#define BOARD_DETECTOR_AM 0x02
	#define BOARD_DETECTOR_FM 0x01
	#define BOARD_DETECTOR_WFM 0x00
	#define BOARD_DETECTOR_TUNE 0x01	/* РєРѕРЅС„РёРіСѓСЂР°С†РёСЏ РїР»Р°С‚С‹ РґР»СЏ СЂРµР¶РёРјР° TUNE (CWZ РЅР° РїРµСЂРµРґР°С‡Сѓ) */

	#define BOARD_DETECTOR_MUTE 4

	/* РєРѕРґС‹ С„РёР»СЊС‚СЂРѕРІ РІС‚РѕСЂРѕР№ РџР§, РІС‹РґР°РІР°РµРјС‹Рµ РЅР° РґРµС€РёС„СЂР°С‚РѕСЂС‹ */
	#define	BOARD_FILTER_0P3	0x04
	#define	BOARD_FILTER_0P5	0x05
	#define	BOARD_FILTER_1P5	0x06
	#define	BOARD_FILTER_2P4	0x03
	#define	BOARD_FILTER_3P1	0x00
	#define	BOARD_FILTER_7P8	0x02
	#define	BOARD_FILTER_17P0	0x01
	#define	BOARD_FILTER_WFM	0x07

	//#define IF3_FMASK (IF3_FMASK_17P0 | IF3_FMASK_7P8 | IF3_FMASK_3P1 | IF3_FMASK_2P4 | IF3_FMASK_1P5 | IF3_FMASK_0P5 | IF3_FMASK_0P3)
	#define IF3_FMASK (IF3_FMASK_3P1)
	#define IF3_FHAVE IF3_FMASK
	#define IF3_FMASKTX	(IF3_FMASK_3P1)

	#define WITHSPLIT	1	/* СѓРїСЂР°РІР»РµРЅРёРµ СЂРµР¶РёРјР°РјРё СЂР°СЃСЃС‚СЂРѕР№РєРё РѕРґРЅРѕР№ РєРЅРѕРїРєРѕР№ */
	//#define WITHSPLITEX	1	/* РўСЂРµС…РєРЅРѕРїРѕС‡РЅРѕРµ СѓРїСЂР°РІР»РµРЅРёРµ СЂРµР¶РёРјР°РјРё СЂР°СЃСЃС‚СЂРѕР№РєРё */
	#define WITHCATEXT	1	/* Р Р°СЃС€РёСЂРµРЅРЅС‹Р№ РЅР°Р±РѕСЂ РєРѕРјР°РЅРґ CAT */
	#define WITHELKEY	1
	#define WITHKBDENCODER 1	// РїРµСЂРµСЃС‚СЂРѕР№РєР° С‡Р°СЃС‚РѕС‚С‹ РєРЅРѕРїРєР°РјРё
	#define WITHKEYBOARD 1	/* РІ РґР°РЅРЅРѕРј СѓСЃС‚СЂРѕР№СЃС‚РІРµ РµСЃС‚СЊ РєР»Р°РІРёР°С‚СѓСЂР° */
	#define KEYBOARD_USE_ADC	1	/* РЅР° РѕРґРЅРѕР№ Р»РёРЅРёРё СѓСЃС‚Р°РЅРѕРІР»РµРЅРѕ  С‡РµС‚С‹СЂРµ  РєР»Р°РІРёС€Рё. РЅР° vref - 6.8K, РґР°Р»РµРµ 2.2Рљ, 4.7Рљ Рё 13K. */

	// РќР°Р·РЅР°С‡РµРЅРёСЏ РІС…РѕРґРѕРІ РђР¦Рџ РїСЂРѕС†РµСЃСЃРѕСЂР°.
	enum 
	{ 
		VOLTSOURCE = 1, // РЎСЂРµРґРЅСЏСЏ С‚РѕС‡РєР° РґРµР»РёС‚РµР»СЏ РЅР°РїСЂСЏР¶РµРЅРёСЏ, РґР»СЏ РђРљР‘
		SMETERIX = 0,	// S-meter
		PWRI = 3, 
		FWD = 3, REF = 2,	// SWR-meter
		KI0 = 4, KI1 = 5, KI2 = 6, KI3 = 7	// РєР»Р°РІРёР°С‚СѓСЂР°
	};
	#define KI_COUNT 4	// РєРѕР»РёС‡РµСЃС‚РІРѕ РёСЃРїРѕР»СЊР·СѓРµРјС‹С… РїРѕРґ РєР»Р°РІРёР°С‚СѓСЂСѓ РІС…РѕРґРѕРІ РђР¦Рџ


	#define VOLTLEVEL_UPPER		47	// 4.7 kOhm - РІРµСЂС…РЅРёР№ СЂРµР·РёСЃС‚РѕСЂ РґРµР»РёС‚РµР»СЏ РґР°С‚С‡РёРєР° РЅР°РїСЂСЏР¶РµРЅРёСЏ
	#define VOLTLEVEL_LOWER		10	// 1.0 kOhm - РЅРёР¶РЅРёР№ СЂРµР·РёСЃС‚РѕСЂ


	/* РџРµСЂРІР°СЏ РџР§ - 64455, РІС‚РѕСЂР°СЏ - 500/455 РєР“С† */

	/* РћРїРёСЃР°РЅРёРµ СЃС‚СЂСѓРєС‚СѓСЂС‹ РїСЂРµРѕР±СЂР°Р·РѕРІР°РЅРёР№ С‡Р°СЃС‚РѕС‚С‹ РІ С‚СЂР°РєС‚Рµ */
	//#define LO1_SIDE	LOCODE_UPPER	/* РџСЂРё РїСЂРµРѕР±СЂР°Р·РѕРІР°РЅРёРё РЅР° СЌС‚РѕРј РіРµС‚РµСЂРѕРґРёРЅРµ РїСЂРѕРёСЃС…РѕРґРёС‚ РёРЅРІРµСЂСЃРёСЏ СЃРїРµРєС‚СЂР° */
	#define LO1_SIDE_F(freq) (((freq) < 64455000UL) ? LOCODE_UPPER : LOCODE_LOWER)	/* СЃС‚РѕСЂРѕРЅР° Р·Р°РІРёСЃРёС‚ РѕС‚ СЂР°Р±РѕС‡РµР№ С‡Р°СЃС‚РѕС‚С‹ */
	#define LO2_SIDE	LOCODE_LOWER	/* РџСЂРё РїСЂРµРѕР±СЂР°Р·РѕРІР°РЅРёРё РЅР° СЌС‚РѕРј РіРµС‚РµСЂРѕРґРёРЅРµ РЅРµС‚ РёРЅРІРµСЂСЃРёРё СЃРїРµРєС‚СЂР° */
	#define LO3_SIDE	LOCODE_INVALID	/* РїСЂРё РѕС‚СЃСѓС‚СЃС‚РІСѓСЋС‰РµРј РіРµС‚РµСЂРѕРґРёРЅРµ - РЅР° РЅС‘Рј РЅРµС‚ РёРЅРІРµСЂСЃРёРё СЃРїРµРєС‚СЂР° */
	#if WITHIF4DSP
		// Р°СЂС…РёС‚РµРєС‚СѓСЂР° РґР»СЏ DSP РЅР° РїРѕСЃР»РµРґРЅРµР№ РџР§
		#define LO4_SIDE	LOCODE_LOWER	/* РїР°СЂР°РјРµС‚СЂ РїСЂРѕРІРµСЂРµРЅ РїРѕ РѕС‚СЃСѓС‚СЃС‚РІРёСЋ РїРѕРјРµС…Рё РїСЂРё РїСЂРёС‘РјРµ РґР»СЏ FQMODEL_80455. РџСЂРё РїСЂРµРѕР±СЂР°Р·РѕРІР°РЅРёРё РЅР° СЌС‚РѕРј РіРµС‚РµСЂРѕРґРёРЅРµ РЅРµС‚ РёРЅРІРµСЂСЃРёРё СЃРїРµРєС‚СЂР° */
		#define LO5_SIDE	LOCODE_LOWER	/* РџСЂРё РїСЂРµРѕР±СЂР°Р·РѕРІР°РЅРёРё РЅР° СЌС‚РѕРј РіРµС‚РµСЂРѕРґРёРЅРµ РЅРµС‚ РёРЅРІРµСЂСЃРёРё СЃРїРµРєС‚СЂР° */
	#else /* WITHIF4DSP */
		// РѕР±С‹С‡РЅР°СЏ Р°СЂС…РёС‚РµРєС‚СѓСЂР° РґР»СЏ СЃР»СѓС…РѕРІРѕРіРѕ РїСЂРёС‘РјРІ
		#define LO4_SIDE	LOCODE_TARGETED	/* Р­С‚РѕС‚ РіРµС‚РµСЂРѕРґРёРЅ СѓРїСЂР°РІР»СЏРµС‚СЃСЏ РґР»СЏ РїРѕР»СѓС‡РµРЅРёСЏ С‚СЂРµР±СѓРµРјРѕР№ Р±РѕРєРѕРІРѕР№ */
		//#define LO5_SIDE	LOCODE_INVALID	/* 12->baseband: РїСЂРё РѕС‚СЃСѓС‚СЃС‚РІСѓСЋС‰РµРј РіРµС‚РµСЂРѕРґРёРЅРµ - РЅР° РЅС‘Рј РЅРµС‚ РёРЅРІРµСЂСЃРёРё СЃРїРµРєС‚СЂР° */
	#endif /* WITHIF4DSP */

	//#define	LO3_FREQADJ	1	/* РїРѕРґСЃС‚СЂРѕР№РєР° С‡Р°СЃС‚РѕС‚С‹ РіРµС‚РµСЂРѕРґРёРЅР° С‡РµСЂРµР· РјРµРЅСЋ. */

	#define	LO2_POWER2	0		/* 0 - РЅРµС‚ РґРµР»РёС‚РµР»СЏ РїРѕСЃР»Рµ РіРµРЅРµСЂР°С‚РѕСЂР° LO2 РїРµСЂРµРґ РїРѕРґР°С‡РµР№ РЅР° СЃРјРµСЃРёС‚РµР»СЊ */

	#define LO1MODE_DIRECT	1
	#define REFERENCE_FREQ	13000000UL	/* LO2 = 72595 kHz - for 73050 kHz filter from Kenwood with 455 kHz IF2 */
	#define DDS1_CLK_MUL	1 		/* РЈРјРЅРѕР¶РёС‚РµР»СЊ РІ DDS1 - "Р·Р°С‚С‹С‡РєР°" */
	#define DDS2_CLK_MUL	1		/* РЈРјРЅРѕР¶РёС‚РµР»СЊ РІ DDS2 - AD9834 */

	#define	WFM_IF1	10700000ul

#if 1
	// Р’РµСЂСЃРёСЏ РїРѕРґ С„РёР»СЊС‚СЂ 1-Р№ РџР§ 64.455
	// IF2=500 kHz
	#define LO2_PLL_N500	1284
	#define LO2_PLL_R500	261
	// IF2=455 kHz
	#define LO2_PLL_N455	1280
	#define LO2_PLL_R455	260
#else
	// Р’РµСЂСЃРёСЏ РїРѕРґ С„РёР»СЊС‚СЂ 1-Р№ РџР§ 80.455
	// IF2=500 kHz
	#define LO2_PLL_N500	1593
	#define LO2_PLL_R500	259
	// IF2=455 kHz
	#define LO2_PLL_N455	1600
	#define LO2_PLL_R455	260
#endif

	#define LO1_POWER2	0		/* РµСЃР»Рё 0 - РґРµР»РёС‚РµР»РµР№ РІ С‚СЂР°РєС‚Рµ РїРµСЂРІРѕРіРѕ РіРµС‚РµСЂРѕРґРёРЅР° РїРµСЂРµРґ СЃРјРµСЃРёС‚РµР»РµРј РЅРµС‚. */
	#define LO4_POWER2 0

	#define TUNE_BOTTOM 30000L		/* РЅРёР¶РЅСЏСЏ С‡Р°СЃС‚РѕС‚Р° РЅР°СЃС‚СЂРѕР№РєРё */
	#define TUNE_TOP 180000000L		/* РІРµСЂС…РЅСЏСЏ С‡Р°СЃС‚РѕС‚Р° РЅР°СЃС‚СЂРѕР№РєРё */


#endif /* ATMEGA_CTLSTYLE_RA4YBO_V1_H_INCLUDED */
