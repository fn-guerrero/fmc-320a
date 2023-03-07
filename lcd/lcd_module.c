/* @file lcd_module.c
 *
 * @brief Este es el modulo de mas alto nivel, las API, funciones que seran
 * usadas a lo largo de la aplicacion.
 *
 * COPYRIGHT NOTE: (c) 2023 FLOWMEET. All right reserved.
 *
 */

// Seccion de includess.
#include "lcd.h"
#include "lcd_module.h"
#include "pcf8553.h"

// Varibles global static, solo visibles en este modulo.

/*
 * @brief
 *
 *
 */
void lcd_module_init()
{
	lcd_init();
}

/*
 * @brief
 *
 *
 *
 */
void lcd_module_puts(const char *c, uint32_t len, const lcd_row_t row)
{
	uint8_t col = 0;
	while ((*c) && (col < len))
	{
		if (*c >= '0' && *c <= '9')
		{
			lcd_put_char(*c, col, row);
		}
		col++;
		c++;
	}
}

/*
 * brief
 *
 */
void lcd_module_refresh()
{
	pcf8553_dump();
}

/*** end of file ***/
