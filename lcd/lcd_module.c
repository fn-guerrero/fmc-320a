/** @file lcd_module.c
 *
 * @brief Este es el modulo de mas alto nivel, las API, funciones que seran
 * usadas a lo largo de la aplicacion.
 *
 */

// Seccion de includess.
#include "lcd.h"
#include "lcd_module.h"
#include "pcf8553.h"

// Varibles global static, solo visibles en este modulo.

/*
 * Cantidad maxima de digitos por fila.
 *
 */
static const uint8_t row_digits[2] =
{
	8,
	7
};

/*
 * @brief
 *
 *
 */
void lcd_module_init()
{
	pcf8553_init();
}

/*
 *  @brief Convert unsigned to string, max
 *
 */
void lcd_module_utoa_right(char *dest, uint32_t num, lcd_row_t row)
{
	uint8_t n;

	n = row_digits[row];
	dest[n] = '\0';

	while (n)
	{
		n--;
		dest[n] = num % 10 + '0'; // @suppress("Avoid magic numbers")
		num /= 10;  // @suppress("Avoid magic numbers")
	}
}

/*
 * @brief
 *
 */
void lcd_module_puts(char *c, lcd_row_t row)
{
	uint8_t col = 0;
	while (*c)
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
