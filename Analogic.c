/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motor.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvitry <jvitry@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/21 16:52:36 by jvitry            #+#    #+#             */
/*   Updated: 2018/07/11 16:17:33 by jvitry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "medor.h"

void analog_mesure()
{
	u16 i = 0;

	TRISBbits.TRISB2 = 1;//ADC AN4 pot IN
	ANSELB = 0; // AN des port B disable
	ANSELBbits.ANSB2 = 1; // AN4 de RB2 enable analogique for potentiometre
	AD1CHSbits.CH0SA = 4;// select MUX A sur AN4

//	AD1PCFGbits.PCFG0 = 1;
	AD1CON1bits.ON = 1;
		AD1CON1bits.SAMP = 1; // lance la mesure
		while (i++ < 10)
			;
		AD1CON1bits.SAMP = 0;
		while (!(AD1CON1bits.DONE))
			;
//		AD1CON1bits.DONE = 0;
		pot = ADC1BUF0;
}

void set_mode()
{

	ANSELAbits.ANSA1 = 0; // AN4 de RB2 enable analogique for potentiometre
	TRISAbits.TRISA1 = 1;//ADC AN4 pot IN
	mode = PORTAbits.RA1;
}