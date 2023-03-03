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
	LCD_ROW_0 = 0, LCD_ROW_1 = 1
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

// Defines.

/*
 * Cantidad de filas y columnas de la pantalla, notar que se tiene dos filas
 * donde:
 * La fila 0 tiene 8 caracteres, equivalente a 8 columas
 * La fila 1 tiene 7 caracteres, equiavlente a 7 columas
 *
 * Aun asi estas macros son para definir un buffer de 2x8, luego los timites
 * para cada fila del buffer seran 8 y 7 elementos para las filas 0 y 1
 * respectivamente.
 *
 */
#define LCD_ROWS 2
#define LCD_COLS 8

/*
 * Cantidad de columnas para cada fila.
 *
 */
#define LCD_ROW_0_SIZE 8
#define LCD_ROW_1_SIZE 7

// Public function prototypes.

void lcd_put_char(char c, uint8_t col, lcd_row_t row);
void lcd_init();

#endif /* INC_FM_160LCD_H_ */

/*** end of file ***/
