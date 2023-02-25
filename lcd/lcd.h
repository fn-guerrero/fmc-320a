/*
 * fm-160lcd.h
 *
 *
 * @par
 *
 * COPYRIGHT NOTE: (c) 2023 FLOWMEET. All right reserved.
 *
 */

#ifndef INC_FM_160LCD_H_
#define INC_FM_160LCD_H_

#include "main.h"

// Typedefs.

/*
 *
 *
 */
typedef enum
{
	LCD_ROW_0 = 0, LCD_ROW_1 = 1,
} lcd_row_t;

/*
 *
 *
 */
typedef struct
{
	uint8_t pos;
	uint8_t reg;
} oct_t;

// Public function prototypes.

void lcd_put_char(char c, uint8_t col, lcd_row_t row);

#endif /* INC_FM_160LCD_H_ */

/*** end of file ***/
