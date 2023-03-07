/* @file lcd_module.h
 *
 * @brief Libreria para manejo de lcd driver de la marca nxp pcf8553
 *
 * COPYRIGHT NOTE: (c) 2023 FLOWMEET. All right reserved.
 *
 */

#ifndef PCF8553_H_
#define PCF8553_H_

// Includes.

#include "main.h"

// Macros for microcontroller taken pins (dhs).

/*
 * Pines dedicados al PCF8553, no me confio de siempre usar el cubemx para
 * y su funcion de label name para los pines. De ser posible siempre declaro
 * los recursos usados del harware lo mas arriba posible en los modulos, debe
 * ser coherente con el code style del proyecto.
 *
 */
#define PCF8553_CE_PORT		GPIOA
#define PCF8553_CE_PIN 		GPIO_PIN_4
#define PCF8553_RESET_PORT	GPIOA
#define PCF8553_RESET_PIN	GPIO_PIN_5

// Typedef.

/*
 *
 *
 */
typedef enum
{
	LCD_BLINK_OFF = 0,
	LCD_BLINK_TWO_SECOND = 1,
	LCD_BLINK_ONE_SECOND = 2,
	LCD_BLINK_HALF_SECOND = 3,
} pcf_blink_mode_t;

/*
 *  Modeling PCF8553 Device_ctrl register.
 *
 */
typedef union
{
	uint8_t reg_data;
	struct
	{
		uint8_t clock_ouput :1;
		uint8_t internal_oscilator :1;
		uint8_t frame_frequency :2;
		uint8_t default_value :4; // @suppress("Avoid magic numbers")
	} reg_bits;
} device_ctrl_t;

/*
 * Modeling PCF8553 Display_ctrl_1 register.
 *
 * VERY IMPORTANT: DE (Display Enabled bit) default is Power-On
 * is disabled, must be change to 1 to turn on Display.
 *
 */
typedef union
{
	uint8_t reg_data;
	struct
	{
		uint8_t display_enabled :1;
		uint8_t bias_mode :1;
		uint8_t mux :2;
		uint8_t boost :1;
		uint8_t default_value :3; // @suppress("Avoid magic numbers")
	} reg_bits;
} display_ctrl_1_t;

/*
 * Modeling PCF8553 Display_ctrl_2 register.
 *
 */
typedef union
{
	uint8_t reg_data;
	struct
	{
		uint8_t inversion :1;
		uint8_t blink :2;
		uint8_t default_value;
	} reg_bits;
} display_ctrl_2_t;

// Defines.

/*
 * Tamaño de la memoria interna del pcf8553 para controlas el encendido/apgado
 * de los segmentos, con 20 bytes se controlan 20 * 8 =  160 segmentos.
 *
 */
#define PCF8553_DATA_SIZE 20

// Public function prototypes.

/*
 * Las funciones cumplen con:
 * Se listan alfabeticamente aqui y en el .c
 * Para los nombres se usa la regla: noun-verb
 * Las funciones publicas comienzan con el nombre del modulo
 *
 */
void pcf8553_blink(pcf_blink_mode_t mode);
void pcf8553_init();
void pcf8553_dump();
void pcf8553_reset();
void pcf8553_write_all(uint8_t data);
void pcf8553_write_byte(uint8_t add, uint8_t data);

#endif /* PCF8553_H_ */

/*** end of file ***/
