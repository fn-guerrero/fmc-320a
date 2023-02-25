/* @file fm-160lcd.c
 *
 * @brief
 *
 *
 */

// Includes.
#include "lcd.h"

#include "pcf8553.h"
#include "ctype.h"

#define SEG_A 0
#define SEG_B 1
#define SEG_C 2
#define SEG_D 3
#define SEG_E 4
#define SEG_F 5
#define SEG_G 6
#define SEG_H 7

// Variables globables declaradas en otros modulos, tipo extern.

/*
 *
 */
extern uint8_t g_lcd_map[20];

// Variables gloables visibles solo en este modulo

/*
 * Buscar los comentariosn en los modulos donde fueron declaradas.
 */
static uint8_t g_buf[2][8];
static uint8_t g_cursor;
static uint8_t g_line;

/*
 *  Para un caracter octal, en la linea superior del LCD, inicializo el
 *  siguiente arreglo. Ver tabla memoria del PCF8553, el encendido/apagado
 *  de cada segmento esta controlado por un bit en esta tabla, son 20 registros
 *  de 8 bits cada uno, 160 bits par controlar la misma cantidad de segmentos.
 *  Los valores de inicializacion de la siguiente tabla corresponde al caracter
 *  ubicado mas a la derecha en la tabla superior, ver datasheet del LCD.
 *  Cada par de valores pos y reg se corresponden a la direccion de un registro
 *  y el bit correspondiente que controlan los segmentos de A a G.
 *  Solo se necesitan los datos de este caracter, las posiciones de los demas
 *  se obtienen por aritmetica dentro de la funcion lcd_write_line
 *
 */
oct_t oct_u[8] =
{
	{
		.pos = 6,
		.reg = 7
	},
	{
		.pos = 6,
		.reg = 2
	},
	{
		.pos = 7,
		.reg = 2
	},
	{
		.pos = 6,
		.reg = 17
	},
	{
		.pos = 6,
		.reg = 12
	},
	{
		.pos = 7,
		.reg = 17
	},
	{
		.pos = 7,
		.reg = 12
	},
	{
		.pos = 7,
		.reg = 7
	},
};

oct_t oct_d[8] =
{
	{
		.pos = 7,
		.reg = 5
	},
	{
		.pos = 7,
		.reg = 0
	},
	{
		.pos = 6,
		.reg = 0
	},
	{
		.pos = 7,
		.reg = 15
	},
	{
		.pos = 7,
		.reg = 10
	},
	{
		.pos = 6,
		.reg = 15
	},
	{
		.pos = 6,
		.reg = 10
	},
	{
		.pos = 6,
		.reg = 5
	},
};

/*
 *
 */
void lcd_write_line(uint8_t seg, uint8_t data)
{
	uint8_t reg = 0;
	uint8_t pos = 0;

	if (g_line == 1)
	{
		pos = oct_u[seg].pos;
		pos += g_cursor * 2;
		reg = (pos / 8) + oct_u[seg].reg;
		pos = pos % 8;
	}
	else
	{
		pos = oct_d[seg].pos;
		pos += g_cursor * 2;
		reg = (pos / 8) + oct_d[seg].reg;
		pos = pos % 8;
	}

	if (data)
	{
		g_lcd_map[reg] |= 1 << pos;
	}
	else
	{
		g_lcd_map[reg] &= ~(1 << pos);
	}
}

/*
 *
 */
void lcd_put_char(char c, uint8_t col, uint8_t row)
{
	g_line = row + 1;

	if (g_buf[row][col] == c)
	{
		return;
	}
	;

	g_buf[row][col] = c;

	switch (row)
	{
		case 0:
			if (col > 7)
				return;
			g_cursor = col;
		break;
		case 1:
			if (col > 6)
				return;
			g_cursor = 6 - col;
		break;
		default:
			return;
		break;
	}

	switch (c)
	{
		case '0':
			lcd_write_line(SEG_A, 1);
			lcd_write_line(SEG_B, 1);
			lcd_write_line(SEG_C, 1);
			lcd_write_line(SEG_D, 0);
			lcd_write_line(SEG_E, 1);
			lcd_write_line(SEG_F, 1);
			lcd_write_line(SEG_G, 1);
			lcd_write_line(SEG_H, 0);
		break;
		case '1':
			lcd_write_line(SEG_A, 0);
			lcd_write_line(SEG_B, 0);
			lcd_write_line(SEG_C, 1);
			lcd_write_line(SEG_D, 0);
			lcd_write_line(SEG_E, 0);
			lcd_write_line(SEG_F, 1);
			lcd_write_line(SEG_G, 0);
			lcd_write_line(SEG_H, 0);
		break;
		case '2':
			lcd_write_line(SEG_A, 1);
			lcd_write_line(SEG_B, 1);
			lcd_write_line(SEG_C, 0);
			lcd_write_line(SEG_D, 1);
			lcd_write_line(SEG_E, 0);
			lcd_write_line(SEG_F, 1);
			lcd_write_line(SEG_G, 1);
			lcd_write_line(SEG_H, 0);
		break;
		case '3':
			lcd_write_line(SEG_A, 1);
			lcd_write_line(SEG_B, 0);
			lcd_write_line(SEG_C, 1);
			lcd_write_line(SEG_D, 1);
			lcd_write_line(SEG_E, 0);
			lcd_write_line(SEG_F, 1);
			lcd_write_line(SEG_G, 1);
			lcd_write_line(SEG_H, 0);
		break;
		case '4':
			lcd_write_line(SEG_A, 0);
			lcd_write_line(SEG_B, 0);
			lcd_write_line(SEG_C, 1);
			lcd_write_line(SEG_D, 1);
			lcd_write_line(SEG_E, 1);
			lcd_write_line(SEG_F, 1);
			lcd_write_line(SEG_G, 0);
			lcd_write_line(SEG_H, 0);
		break;
		case '5':
			lcd_write_line(SEG_A, 1);
			lcd_write_line(SEG_B, 0);
			lcd_write_line(SEG_C, 1);
			lcd_write_line(SEG_D, 1);
			lcd_write_line(SEG_E, 1);
			lcd_write_line(SEG_F, 0);
			lcd_write_line(SEG_G, 1);
			lcd_write_line(SEG_H, 0);
		break;
		case '6':
			lcd_write_line(SEG_A, 1);
			lcd_write_line(SEG_B, 1);
			lcd_write_line(SEG_C, 1);
			lcd_write_line(SEG_D, 1);
			lcd_write_line(SEG_E, 1);
			lcd_write_line(SEG_F, 0);
			lcd_write_line(SEG_G, 1);
			lcd_write_line(SEG_H, 0);
		break;
		case '7':
			lcd_write_line(SEG_A, 0);
			lcd_write_line(SEG_B, 0);
			lcd_write_line(SEG_C, 1);
			lcd_write_line(SEG_D, 0);
			lcd_write_line(SEG_E, 0);
			lcd_write_line(SEG_F, 1);
			lcd_write_line(SEG_G, 1);
			lcd_write_line(SEG_H, 0);
		break;
		case '8':
			lcd_write_line(SEG_A, 1);
			lcd_write_line(SEG_B, 1);
			lcd_write_line(SEG_C, 1);
			lcd_write_line(SEG_D, 1);
			lcd_write_line(SEG_E, 1);
			lcd_write_line(SEG_F, 1);
			lcd_write_line(SEG_G, 1);
			lcd_write_line(SEG_H, 0);
		break;
		case '9':
			lcd_write_line(SEG_A, 0);
			lcd_write_line(SEG_B, 0);
			lcd_write_line(SEG_C, 1);
			lcd_write_line(SEG_D, 1);
			lcd_write_line(SEG_E, 1);
			lcd_write_line(SEG_F, 1);
			lcd_write_line(SEG_G, 1);
			lcd_write_line(SEG_H, 0);
		break;
		default:
		break;
	}
}

/*** end of file ***/
