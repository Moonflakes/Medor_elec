/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   medor.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvitry <jvitry@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/21 16:52:36 by jvitry            #+#    #+#             */
/*   Updated: 2018/07/11 16:17:33 by jvitry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MEDOR_H_
#define	_MEDOR_H_

// modification des parametres pour une vitesse a 80M

#include <xc.h>

#include "types.h"

u8		PWM_timer = 0;
u32		timer = 0;
u8		mode = 0;
u8		note = 0;
u8		gamme = 0;
int		pot = 0;
u8		last_echo[] = {0, 0, 0, 0, 0, 0, 0};
u32		distance[] = {10000, 10000, 10000, 10000, 10000, 10000, 10000};

#include "i2c.c"
#include "led.c"
#include "sound.c"
#include "motor.c"
#include "analogic.c"
#include "sensor.c"
#include "config.c"

// I2C
/*extern void	I2C_Init();
extern void	I2C_Master_Start();
extern void	I2C_Master_Wait();
extern void	I2C_Master_Restart();
extern void	I2C_Master_Stop();
extern u8      I2C_Send(u8 d);
extern u8      I2C_Read();
extern void    I2C_Master_Send(u8 address, u8 cmd);
extern u8      I2C_Master_Receive(u8 address);
extern u8      I2C_Master_Read(u8 address, u8 registre);
extern void    I2C_Master_Write(u8 address, u8 registre, u8 data);
*/

#endif

