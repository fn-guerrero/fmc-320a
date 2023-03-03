/*
 * fm_lcd_module.h
 *
 *  Created on: 2 ene. 2023
 *      Author: DHS
 */

#ifndef LCD_MODULE_H_
#define LCD_MODULE_H_

#include "main.h"

void lcd_module_init();
void lcd_module_puts(const char *p_str, uint32_t len, uint8_t row);
void lcd_module_refresh();

// Defines.

#endif /* LCD_MODULE_H_ */

/*** end of file ***/
