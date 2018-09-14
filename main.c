/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvitry <jvitry@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/21 16:52:36 by jvitry            #+#    #+#             */
/*   Updated: 2018/07/11 16:17:33 by jvitry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "medor.h"

// un interupt qui se declenche quand IFS0bits.T2IF == 1
// http://microchipdeveloper.com/32bit:mx-arch-exceptions-usage
// http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf page 90

void __ISR(19, IPL7SRS) Button(void)
{
	IFS0bits.INT4IF = 0;
	if (PR1 > 200)
	PR1 = PR1 / 2;
	else
		PR1 = 62500;
	if (PR2 > 6000)
	PR2 = PR2 / 2;
	else
		PR2 = 12500;

}
void __ISR(20, IPL2SRS) PWM(void)
{
	PWM_timer ++;
	if (PWM_timer >= 100)
		PWM_timer = 0;
	if (PWM_timer < 100)
	{
	LATCbits.LATC4 = 1;	//PWM2
	LATCbits.LATC1 = 1;	//PWM2
	}
	else
	{
	LATCbits.LATC4 = 0;	//PWM2
	LATCbits.LATC1 = 0;	//PWM2
	}
}

void __ISR(16, IPL3SRS) MedorISRsensor_echo(void) // check echo
{
    IFS0bits.T4IF = 0; // Reset T4 interupt flag
	if (last_echo[0] == 1 && PORTBbits.RB13 == 0)
	{
		last_echo[0] = 0;
		distance[0] = (TMR3 + (distance[0] * 2)) / 3;
	}
	if (last_echo[0] == 0 && PORTBbits.RB13 == 1)
		last_echo[0] = 1;

	if (last_echo[1] == 1 && PORTBbits.RB12 == 0)
	{
		last_echo[1] = 0;
		distance[1] = (TMR3 + (distance[1] * 2)) / 3;
	}
	if (last_echo[1] == 0 && PORTBbits.RB12 == 1)
		last_echo[1] = 1;

	if (last_echo[2] == 1 && PORTBbits.RB11 == 0)
	{
		last_echo[2] = 0;
		distance[2] = (TMR3 + (distance[2] * 2)) / 3;
	}
	if (last_echo[2] == 0 && PORTBbits.RB11 == 1)
		last_echo[2] = 1;

	if (last_echo[3] == 1 && PORTBbits.RB10 == 0)
	{
		last_echo[3] = 0;
		distance[3] = TMR3;
	}
	if (last_echo[3] == 0 && PORTBbits.RB10 == 1)
		last_echo[3] = 1;

	if (last_echo[4] == 1 && PORTBbits.RB7 == 0)
	{
		last_echo[4] = 0;
		distance[4] = TMR3;
	}
	if (last_echo[4] == 0 && PORTBbits.RB7 == 1)
		last_echo[4] = 1;

	if (last_echo[5] == 1 && PORTBbits.RB6 == 0)
	{
		last_echo[5] = 0;
		distance[5] = TMR3;
	}
	if (last_echo[5] == 0 && PORTBbits.RB6 == 1)
		last_echo[5] = 1;

/*	if (last_echo[6] == 1 && PORTBbits.RB5 == 0)
	{
		last_echo[6] = 0;
		distance[6] = TMR3;
	}
	if (last_echo[6] == 0 && PORTBbits.RB5 == 1)
		last_echo[6] = 1;*/
}

void __ISR(12, IPL4SRS) MedorISRsensor_trig(void) // trig sensors
{
    IFS0bits.T3IF = 0; // Reset T2 interupt flag
	trig_sensor();
	if (mode == 0)
	{
		if (timer < 10)
		{
			stop();
		}
		else if (distance[0] > 300)
		{
			rouler();
		}
		else
		{
			tourner();
		}
	}
	else if (mode == 1)
	{
		u32 arriere = distance[1] + distance[0] + distance[2];
		u32 avant = distance[3] + distance[4] + distance[5];
		
		if (timer < 10)
		{
			stop();
		}
		else if (avant > arriere)
		{
			if (avant > 900)
				rouler();
			else
				tourner();
		}
		else
		{
			if (arriere > 900)
				reculer();
			else
				tourner_inv();
		}

	}
}

void __ISR(8, IPL6SRS) MedorISRsec(void)
{
    IFS0bits.T2IF = 0; // Reset T2 interupt flag
	timer ++;
	if (timer % 2 == 0) //RA1 Bouton mode
	{
		I2C_Master_Write(0x40, 1, 0b0);
		I2C_Master_Write(0x40, 0, 0b0);
		I2C_Master_Write(0x50, 1, 0b11100);
		I2C_Master_Write(0x50, 0, 0b1);
		I2C_Master_Write(0x60, 1, 0b11100);
		I2C_Master_Write(0x60, 0, 0b1);
	}
	if (mode == 0 && timer % 2 != 0 && timer > 5)
	{
		I2C_Master_Write(0x40, 1, 0b11100);
		I2C_Master_Write(0x40, 0, 0b1);
		I2C_Master_Write(0x50, 1, 0b0);
		I2C_Master_Write(0x50, 0, 0b0);
		I2C_Master_Write(0x60, 1, 0b11100);
		I2C_Master_Write(0x60, 0, 0b1);
	}
	if (mode == 1 && timer % 2 != 0 && timer > 5)
	{
		I2C_Master_Write(0x40, 1, 0b11100);
		I2C_Master_Write(0x40, 0, 0b1);
		I2C_Master_Write(0x50, 1, 0b11100);
		I2C_Master_Write(0x50, 0, 0b1);
		I2C_Master_Write(0x60, 1, 0b0);
		I2C_Master_Write(0x60, 0, 0b0);
	}
}

void __ISR(4, IPL5SRS) MedorISRsound(void) // 64/sec : sound
{
	    IFS0bits.T1IF = 0; // Reset T2 interupt flag
		set_sound(note, gamme);
		mode = PORTAbits.RA1;
		if (timer > 10)
		{
			note = note + 5;
			if (note > 100)
			{
				note = 1;
				gamme = gamme + 1;
			}
			if (gamme > 20)
				gamme = 1;
		}
		if (timer < 10)
		{
			analog_mesure();
			set_mode();
		}
		if (timer < 1)
		{
			set_sound(60, 1);
			set_sound(80, 1);
		}
}
 
// un interupt qui se declenche quand PORTDbits.RD8 == 0
/*void   __ISR(7, IPL7SRS) BtnISR(void)
{
    IFS0bits.INT1IF = 0; // reset btn interupt flag
 
    PR2 = ((PR2 >= 62500 / 10) ? PR2/2 : 62500);
}*/

int main()
{
    __builtin_disable_interrupts();
	
	// pic neutre sur les pattes RA0 et RA1 -> action sur la composant impossible (pas d'effet de LAT), lecture seule
	TRISAbits.TRISA0 = 1;
	TRISAbits.TRISA1 = 1;

	TRISBbits.TRISB13 = 1; // echo en mode read
	TRISBbits.TRISB12 = 1;
	TRISBbits.TRISB11 = 1;
	TRISBbits.TRISB10 = 1;
	TRISBbits.TRISB7 = 1;
	TRISBbits.TRISB6 = 1;
	TRISBbits.TRISB5 = 1;

	TRISCbits.TRISC0 = 0; // control Moteur
	TRISCbits.TRISC1 = 0;
	TRISCbits.TRISC2 = 0;
	TRISCbits.TRISC3 = 0;
	TRISCbits.TRISC4 = 0;
	TRISCbits.TRISC5 = 0;

		// Gestion clock Timer 5 (5000/sec)
    OSCCONbits.FRCDIV = 1; // 8Mhz/2=4Mhz // 001 -> divided by 2 (http://ww1.microchip.com/downloads/en/DeviceDoc/61112H.pdf page 5)
    T5CONbits.ON = 1; // actionner la clock
    T5CONbits.TGATE = 0; // pour "selectionner" PBCLK (http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf page 105)
    T5CONbits.TCKPS = 0; // 500K/3 = 62500 // // 3 -> divise par 8 (http://ww1.microchip.com/downloads/en/DeviceDoc/61105F.pdf page 9)
    TMR5 = 0; // Valeur initiale du timer
    PR5 = 100; // nombre de tours avant de remettre IFS0bits.T2IF a 1

	// Gestion clock Timer 4 (1000/sec)
    OSCCONbits.FRCDIV = 1; // 8Mhz/2=4Mhz // 001 -> divided by 2 (http://ww1.microchip.com/downloads/en/DeviceDoc/61112H.pdf page 5)
    T4CONbits.ON = 1; // actionner la clock
    T4CONbits.TGATE = 0; // pour "selectionner" PBCLK (http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf page 105)
    T4CONbits.TCKPS = 0; // 500K/3 = 62500 // // 3 -> divise par 8 (http://ww1.microchip.com/downloads/en/DeviceDoc/61105F.pdf page 9)
    TMR4 = 0; // Valeur initiale du timer
    PR4 = 500; // nombre de tours avant de remettre IFS0bits.T2IF a 1

		// Gestion clock Timer 3 (30/sec)
    OSCCONbits.FRCDIV = 1; // 8Mhz/2=4Mhz // 001 -> divided by 2 (http://ww1.microchip.com/downloads/en/DeviceDoc/61112H.pdf page 5)
    T3CONbits.ON = 1; // actionner la clock
    T3CONbits.TGATE = 0; // pour "selectionner" PBCLK (http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf page 105)
    T3CONbits.TCKPS = 3; // 500K/3 = 62500 // // 3 -> divise par 8 (http://ww1.microchip.com/downloads/en/DeviceDoc/61105F.pdf page 9)
    TMR3 = 0; // Valeur initiale du timer
    PR3 = 6240; // nombre de tours avant de remettre IFS0bits.T2IF a 1

	// Gestion clock Timer 2 (1 sec)
    OSCCONbits.FRCDIV = 1; // 8Mhz/2=4Mhz // 001 -> divided by 2 (http://ww1.microchip.com/downloads/en/DeviceDoc/61112H.pdf page 5)
    T2CONbits.ON = 1; // actionner la clock
    T2CONbits.TGATE = 0; // pour "selectionner" PBCLK (http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf page 105)
    T2CONbits.TCKPS = 3; // 500K/3 = 62500 // // 3 -> divise par 8 (http://ww1.microchip.com/downloads/en/DeviceDoc/61105F.pdf page 9)
    TMR2 = 0; // Valeur initiale du timer
    PR2 = 62500; // nombre de tours avant de remettre IFS0bits.T2IF a 1

	// Gestion clock sound Timer 1 (64/sec)
    OSCCONbits.FRCDIV = 1; // 8Mhz/2=4Mhz // 001 -> divided by 2 (http://ww1.microchip.com/downloads/en/DeviceDoc/61112H.pdf page 5)
    T1CONbits.ON = 1; // actionner la clock
    T1CONbits.TGATE = 0; // pour "selectionner" PBCLK (http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf page 105)
    T1CONbits.TCKPS = 2; // 500K/64 = 62500 // // 2 -> divise par 64 (http://ww1.microchip.com/downloads/en/DeviceDoc/61105F.pdf page 9)
    TMR1 = 0; // Valeur initiale du timer
    PR1 = 1040; // nombre de tours avant de remettre IFS0bits.T2IF a 1

	PMAEN = 0;
	INT4R = 0b0101;

	CFGCONbits.IOLOCK = 0;

    INTCONbits.MVEC = 1; // MultiVector section 8. Interrupt page 5

	// Gestion interupt btn
	IPC4bits.INT4IP = 7;
    IFS0bits.INT4IF = 0;
    IEC0bits.INT4IE = 1;

	// Gestion interupt clock sound
    IPC1bits.T1IP = 5; // P -> priority
    IFS0bits.T1IF = 0; // F -> Flag (IRQ flag)
    IEC0bits.T1IE = 1; // E -> Enable

    // Gestion interupt clock sec
    IPC2bits.T2IP = 6; // P -> priority
    IFS0bits.T2IF = 0; // F -> Flag (IRQ flag)
    IEC0bits.T2IE = 1; // E -> Enable

	// Gestion interupt timer trig
    IPC3bits.T3IP = 4; // P -> priority
    IFS0bits.T3IF = 0; // F -> Flag (IRQ flag)
    IEC0bits.T3IE = 1; // E -> Enable

	// Gestion interupt timer echo
    IPC4bits.T4IP = 3; // P -> priority
    IFS0bits.T4IF = 0; // F -> Flag (IRQ flag)
    IEC0bits.T4IE = 1; // E -> Enable

		// Gestion interupt timer PWM
    IPC5bits.T5IP = 2; // P -> priority
    IFS0bits.T5IF = 0; // F -> Flag (IRQ flag)
    IEC0bits.T5IE = 1; // E -> Enable

	__builtin_enable_interrupts();

	// initialisation_I2C
	I2C_Init();

	// initialisation AN
	//init_analog();

		I2C_Master_Write(0x40, 1, 0b11100);
		I2C_Master_Write(0x40, 0, 0b1);
		// initialisation controleur 0x50
		I2C_Master_Write(0x50, 1, 0b11100);
		I2C_Master_Write(0x50, 0, 0b1);
		I2C_Master_Write(0x60, 1, 0b11100);
		I2C_Master_Write(0x60, 0, 0b1);

	while (1)
	{
/*		I2C_Master_Write(0x40, 1, 0b11100);
		I2C_Master_Write(0x40, 0, 0b1);
		// initialisation controleur 0x50
		I2C_Master_Write(0x50, 1, 0b11100);
		I2C_Master_Write(0x50, 0, 0b1);
		I2C_Master_Write(0x60, 1, 0b11100);
		I2C_Master_Write(0x60, 0, 0b1);*/
	}

	while (1)
	{
		if ((timer / 60) <= (pot * 60 / 32))
			break;
	}
		    __builtin_disable_interrupts();
			while (1)
			{
				stop();
			}
    return (0);
}