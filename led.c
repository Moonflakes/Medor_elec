/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   led.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvitry <jvitry@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/21 16:52:36 by jvitry            #+#    #+#             */
/*   Updated: 2018/07/11 16:17:33 by jvitry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "medor.h"

void    set_led(u8 driver, u8 led, u16 brightness)
{
	// 2 registres pour faire les 12 bits
    u8 brightness_l = brightness; // 8 premiers bits
    u8 brightness_h = brightness >> 8; // ercupere ce qu'il y a derriere les 8 premiers bits

    led = led * 4;
    {
    I2C_Master_Write(driver, (led + 6), 0); // intialise la valeur de l'allumage
    I2C_Master_Write(driver, (led + 7), 0);
    I2C_Master_Write(driver, (led + 8), brightness_l); // intialise la valeur de l'eteignage (2 registres de 8 bits pour aller jusqu'a 4096)
    I2C_Master_Write(driver, (led + 9), brightness_h);
    }
}

void led_off()
{
	u8 led;
    u16 brightness;

	 __builtin_disable_interrupts();
	while (1)
	{
		led = 0;
		brightness = 0;
		while (led <15)
		{
			{
			set_led(0x40, led, 0);
			set_led(0x50, led, 0);
			set_led(0x60, led, 0);
			}
			led++;
		}
		led = 0;
		while (led <15)
		{
			set_led(0x40, led, 0);
			set_led(0x50, led, 0);
			set_led(0x60, led, 0);
			led++;
		}
	}
	__builtin_enable_interrupts();
}

// u3 : 64 -> 0x40
// u4 : 65 -> 0x41
// u5 : 66 -> 0x42