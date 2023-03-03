/* @file fm-160lcd.c
 *
 * @brief Este es un driver intermedio entre lcd_module.c y pcf8553. Entre las
 * responsabilidades de este modulo estan:
 * Dar accesos a la escritura de cada caracter y simbolo en el LCD custom:
 * Linea 1 de 8 caracteres
 * Linea 2 de 7 caracteres
 * Simbolos RATE, BACH, TTL, ACM, H, M, S, D....
 *
 *
 *
 */

// Includes.
#include "lcd.h"
#include "pcf8553.h"
#include "ctype.h"

// Typedef.

/*
 * Ver hoja de datos del lcd, cada segmento se identifica con una letra, se
 * codifica a numero. Cada segmento esta conectado a un pin del pcf8553, luego
 * cada pin esta relacionado con bit de un resgitro de 8 bits en el mapa de
 * memoria del pcf8553. Se debe construir una matiz que relacione los segmentos
 * con el bit que se quiere encender, esto se hara para un solo digito, luego
 * por se obtendran las posiciones de los siguientes digitos.
 *
 */
#define SEG_A 0
#define SEG_B 1
#define SEG_C 2
#define SEG_D 3
#define SEG_E 4
#define SEG_F 5
#define SEG_G 6
#define SEG_H 7

// Project variables, non-static, at least usedd in other file.


// Extern variables.

extern uint8_t g_lcd_map[PCF8553_DATA_SIZE];

// Global variables, statics.

/*
 * Lo que se quiera escribir en las lineas 1 y 2 primero se vuelca a este
 * buffer. Leer el buffer es la unica manera practica que se tiene para
 * saber que esta escrito en la pantalla, no de debe corromper esta ,condicion.
 *
 * Notar que la primera linea tiene
 *
 */
static uint8_t g_buf[LCD_COLS][LCD_ROWS];
static uint8_t g_col;
static uint8_t g_row;

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
 *  se obtienen por aritmetica dentro de la funcion lcd_write_line.
 */
oct_t oct_u[LCD_ROW_0_SIZE] =
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

oct_t oct_d[LCD_ROW_1_SIZE] =
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
};

// Private function prototypes.

void lcd_write_line(uint8_t seg, uint8_t data);

// Public function bodies.

/*
 * brief
 *
 */
void lcd_init()
{
	/*
	 * Inicializo a cero todos los elemento del buffer.
	 */

	for(int row = 0; row < LCD_ROWS; row++)
	{
		for(int col = 0; col < LCD_COLS; col++)
		{
			g_buf[row][col] = 0;
		}
	}

	pcf8553_init();
}

/*
 * @brief
 *
 */
void lcd_put_char(char c, uint8_t col, uint8_t row)
{
	g_row = row + 1;

	/*
	 * Si el buffer contiene lo mismo que se va a esribir salteo la escritura.
	 * Aunque esto mejora mucho la eficiencia, no tengo que escribir
	 * por SPI este caracter, considero mu resgiosa esta tecnica, la
	 * sincronizacion entre el buffer y el contenido de la memoria del pcf8553
	 * se debe asegurar. El uso del return prematuro no se si es buena practica.
	 *
	 */
	if (g_buf[row][col] == c)
	{
		return;
	}

	g_buf[row][col] = c;

	switch (row)
	{
		case 0:
			if (col < LCD_ROW_0_SIZE)
			{
				g_col = col;
			}
			else
			{
				return;
			}
		break;
		case 1:
			if (col < LCD_ROW_1_SIZE)
			{
				g_col = 6 - col; // @suppress("Avoid magic numbers")
			}
			else
			{
				return;
			}

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

// Private function bodies.

/*
 * @brief Esta funcion ajusta el estado de un segmento, encendido o apagado,
 * por cada llamada. La caracter a manipular, linea 1 o linea 2 y la posicion
 * del cursor, son informaciones guardadas en dos variables globales, g_cursor
 * y g_line. Con el mapeo de los segmentos, y sabiendo que segmento se quiere
 * manipular, notar que para el primer carcter de cada linea se tiene una matriz
 * con la posicion en el mapa de mempria para el pcf8553, con lo cual no se
 * requiere calculo, el calculo es de utilidad para los caracteres que no ocupan
 * la primera posicion de cada liena.
 *
 */
void lcd_write_line(uint8_t seg, uint8_t data)
{
	uint8_t reg = 0;
	uint8_t pos = 0;

	/*
	 *
	 *
	 */
	switch (g_row)
	{
		case 1:
			pos = oct_u[seg].pos;
			pos += g_col * 2;
			reg = (pos / 8) + oct_u[seg].reg; // @suppress("Avoid magic numbers")
			pos = pos % 8; // @suppress("Avoid magic numbers")
		break;
		case 2:
			pos = oct_d[seg].pos;
			pos += g_col * 2;
			reg = (pos / 8) + oct_d[seg].reg; // @suppress("Avoid magic numbers")
			pos = pos % 8; // @suppress("Avoid magic numbers")
		break;
		default:
		break;
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

/*** end of file ***/
