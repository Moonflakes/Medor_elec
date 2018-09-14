/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvitry <jvitry@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/21 16:52:36 by jvitry            #+#    #+#             */
/*   Updated: 2018/07/11 16:17:33 by jvitry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "medor.h"

void	I2C_Init()
{
	I2C1CON = 0; // initialisation du registre a 0
	I2C1BRG = 8; //100kHz
	I2C1CONbits.ON = 1;
}

void	I2C_Master_Wait()
{
	while (I2C1CONbits.PEN || I2C1CONbits.SEN || I2C1CONbits.ACKEN
			 || I2C1CONbits.RCEN || I2C1CONbits.RSEN || I2C1STATbits.TBF);
}

void	 I2C_Master_Start()
{
	 __builtin_disable_interrupts(); // verif la regularite du signal bleu (trig)
	I2C1CONbits.SEN = 1; // envoyer start condition
    I2C_Master_Wait(); // attendre que la condition start se termine (SEN)
}

void	 I2C_Master_Restart() // est-ce qu'on l'utilise ?
{
	I2C1CONbits.RSEN = 1;
	I2C_Master_Wait();
}

void	 I2C_Master_Stop()
{
	I2C1CONbits.PEN = 1; // envoyer stop condition
	I2C_Master_Wait(); // attendre que la condition stop se termine (PEN)
	  __builtin_enable_interrupts();
}

u8 I2C_Send(u8 d)
{
	while (I2C1STATbits.TRSTAT); // attendre que la transmission de l'ancienne valeur soit terminee ??
	I2C1TRN = d;
	while (I2C1STATbits.TRSTAT || I2C1STATbits.TBF); // attendre que la transmission de TRN soit terminee et que TRN soit a nouveau vide
	return (I2C1STATbits.ACKSTAT); // retourner le status de la reception
}

u8 I2C_Read() // est-ce qu'on l'utilise ?
{
	while (I2C1STATbits.TRSTAT);
	I2C1CONbits.RCEN = 1;
	while (I2C1CONbits.RCEN);
	I2C1STATbits.I2COV = 0;
	return (I2C1RCV);
}

void I2C_Master_Send(u8 address, u8 cmd) // est-ce qu'on l'utilise ?
{
	I2C_Master_Start();
	I2C_Send(address << 1 | 0);
	I2C_Send(cmd);
	I2C_Master_Wait();
	I2C_Master_Stop();
}

u8 I2C_Master_Receive(u8 address)  // est-ce qu'on l'utilise ?
{
	I2C_Master_Start();
	if (I2C_Send(address << 1 | 1))
		Nop();
	I2C_Master_Wait();
	u8 r = I2C_Read();
	I2C_Master_Stop();
	return (r);
}

u8 I2C_Master_Read(u8 address, u8 registre) // est-ce qu'on l'utilise ?
{
	I2C_Master_Send(address, registre);
	u8 r = I2C_Master_Receive(address);
	return (r);
}

void I2C_Master_Write(u8 address, u8 registre, u8 data)
{
	I2C_Master_Start();
	I2C_Send(address << 1 | 0);
	I2C_Master_Wait(); // attendre que la transmission de la data soit terminee (ACKEN)
	I2C_Send(registre);
	I2C_Master_Wait();
	I2C_Send(data);
	I2C_Master_Wait();
	I2C_Master_Stop();
	I2C_Master_Wait(); // attendre que tout soit termine
}