#ifndef GUI_STRUCTS_H_INCLUDED
#define GUI_STRUCTS_H_INCLUDED

#include "hardware.h"

#ifndef WITHGUIMAXX
#define WITHGUIMAXX		DIM_X
#endif

#ifndef WITHGUIMAXY
#define WITHGUIMAXY		DIM_Y
#endif

#if WITHTOUCHGUI

enum {
	button_round_radius = 5,
	window_title_height = 26,
};

typedef enum {
	TYPE_DUMMY,
	TYPE_BUTTON,
	TYPE_LABEL,
	TYPE_SLIDER
} element_type_t;

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
	WINDOW_UIF,
	WINDOW_SWR_SCANNER,
	WINDOW_AUDIOSETTINGS,			// настройки аудиопараметров
	WINDOW_AP_MIC_EQ,				// эквалайзер микрофона
	WINDOW_AP_REVERB_SETT,			// параметры ревербератора
	WINDOW_AP_MIC_SETT,				// настройки микрофона
	WINDOW_AP_MIC_PROF,				// профили микрофона
	WINDOW_TX_SETTINGS,				// настройки, относящиеся к режиму передачи
	WINDOW_TX_VOX_SETT,				// настройки VOX
	WINDOW_TX_POWER,					// выходная мощность
	WINDOWS_COUNT
};

enum {
	NAME_ARRAY_SIZE = 30,
	TEXT_ARRAY_SIZE = 30,
	MENU_ARRAY_SIZE = 30,
	TOUCH_ARRAY_SIZE = 100
};

typedef struct {
	uint8_t w;
	uint8_t h;
	PACKEDCOLORMAIN_T * bg_non_pressed;
	PACKEDCOLORMAIN_T * bg_pressed;
	PACKEDCOLORMAIN_T * bg_locked;
	PACKEDCOLORMAIN_T * bg_locked_pressed;
	PACKEDCOLORMAIN_T * bg_disabled;
} btn_bg_t;

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

typedef enum {
	FONT_LARGE,		// S1D13781_smallfont_LTDC
	FONT_MEDIUM,	// S1D13781_smallfont2_LTDC
	FONT_SMALL		// S1D13781_smallfont3_LTDC
} font_size_t;

typedef struct {
	uint16_t x;
	uint16_t y;
	uint8_t parent;
	uint8_t state;
	uint8_t is_trackable;
	uint8_t visible;
	char name [NAME_ARRAY_SIZE];
	char text [TEXT_ARRAY_SIZE];
	font_size_t font_size;
	PACKEDCOLORMAIN_T color;
	void (*onClickHandler) (void);
} label_t;

typedef enum  {
	ORIENTATION_VERTICAL,
	ORIENTATION_HORIZONTAL
} orientation_t;

enum {
	sliders_width = 8,		// ширина шкалы
	sliders_w = 12,			// размеры ползунка
	sliders_h = 18			// от центра (*2)
};

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t x1_p;			// координаты ползунка
	uint16_t y1_p;			// для update_touch_list_list
	uint16_t x2_p;
	uint16_t y2_p;
	orientation_t orientation;
	uint8_t parent;
	char name [NAME_ARRAY_SIZE];
	uint8_t state;
	uint8_t visible;
	uint16_t size;			// длина шкалы в пикселях
	uint8_t value;			// 0..100 %
	uint8_t value_old;		// для перерасчетов при изменении значения
	uint16_t value_p;		// в пикселях от начала шкалы
	float step;
} slider_t;

typedef enum {
	ALIGN_LEFT_X 	= WITHGUIMAXX >> 2,					// вертикальное выравнивание по центру левой половины экрана
	ALIGN_CENTER_X 	= WITHGUIMAXX >> 1,					// вертикальное выравнивание по центру экрана
	ALIGN_RIGHT_X 	= ALIGN_LEFT_X + ALIGN_CENTER_X,	// вертикальное выравнивание по центру правой половины экрана
	ALIGN_Y 		= WITHGUIMAXY >> 1					// горизонтальное выравнивание всегда по центру экрана
} align_t;

typedef struct {
	uint8_t window_id;			// в окне будут отображаться элементы с соответствующим полем for_window
	uint8_t parent_id;			// UINT8_MAX - нет parent window
	align_t align_mode;			// вертикаль выравнивания окна
	uint16_t x1;
	uint16_t y1;
	uint16_t w;
	uint16_t h;
	char name[NAME_ARRAY_SIZE];	// текст, выводимый в заголовке окна
	uint8_t state;
	uint8_t first_call;			// признак первого вызова для различных инициализаций
	void (*onVisibleProcess) (void);
	button_t * bh_ptr;			// указатели на массивы оконных элементов
	uint8_t bh_count;
	label_t * lh_ptr;
	uint8_t lh_count;
	slider_t * sh_ptr;
	uint8_t sh_count;
	LIST_ENTRY item;
} window_t;

typedef struct {
	int16_t rotate;			// признак поворота второго энкодера
	uint8_t press;			// короткое нажание
	uint8_t hold;			// длинное нажатие
	uint8_t busy;			// второй энкодер выделен для обработки данных окна
	uint8_t rotate_done;	// событие поворота от энкодера обработано, можно получать новые данные
	uint8_t press_done;		// событие нажатия от энкодера обработано, можно получать новые данные
} enc2_t;

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

typedef struct {
	char name [TEXT_ARRAY_SIZE];
	uint16_t menupos;
	uint8_t exitkey;
} menu_by_name_t;

typedef struct {
	element_type_t type;		// тип элемента, поддерживающего реакцию на касания
	window_t * win;
	void * link;
	uint8_t pos;
	uint8_t state;				// текущее состояние элемента
	uint8_t visible;			// текущая видимость элемента
	uint8_t is_trackable;		// поддерживает ли элемент возврат относительных координат перемещения точки нажатия
	uint16_t x1;				// координаты окна
	uint16_t y1;
	uint16_t x2;
	uint16_t y2;
	LIST_ENTRY item;
} touch_t;

typedef struct {
	uint16_t last_pressed_x; 	  // последняя точка касания экрана
	uint16_t last_pressed_y;
	uint8_t kbd_code;
	element_type_t selected_type; // тип последнего выбранного элемента
	touch_t * selected_link;	  // ссылка на выбранный элемент
	uint8_t state;				  // последнее состояние
	uint8_t is_touching_screen;   // есть ли касание экрана в данный момент
	uint8_t is_after_touch; 	  // есть ли касание экрана после выхода точки касания из элемента (при is_tracking == 0)
	uint8_t is_tracking;		  // получение относительных координат точки перемещения нажатия
	int16_t vector_move_x;	 	  // в т.ч. и за границами элемента, при state == PRESSED
	int16_t vector_move_y;
	uint8_t timer_1sec_updated;	  // для периодических обновлений состояния
} gui_t;

#endif /* WITHTOUCHGUI */
#endif /* GUI_STRUCTS_H_INCLUDED */