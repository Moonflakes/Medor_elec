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

void gauche()
{
	LATCbits.LATC3 = 1; //A3
	LATCbits.LATC0 = 0; //A4

	LATCbits.LATC2 = 0;	//A1
	LATCbits.LATC5 = 1; //A2
}

void droite()
{
	LATCbits.LATC3 = 0; //A3
	LATCbits.LATC0 = 1; //A4

	LATCbits.LATC2 = 1;	//A1
	LATCbits.LATC5 = 0; //A2
}

void rouler()
{
	//parametrage avancer : lat A1 = 0 et lat A2 = 1
//parametrage reculer : inverse


	LATCbits.LATC3 = 1; //A3
	LATCbits.LATC0 = 0; //A4


	LATCbits.LATC2 = 1;	//A1
	LATCbits.LATC5 = 0; //A2
}

void reculer()
{
	//parametrage avancer : lat A1 = 0 et lat A2 = 1
//parametrage reculer : inverse


	LATCbits.LATC3 = 0; //A3
	LATCbits.LATC0 = 1; //A4


	LATCbits.LATC2 = 0;	//A1
	LATCbits.LATC5 = 1; //A2
}

void tourner()
{
	//parametrage avancer : lat A1 = 0 et lat A2 = 1
	//parametrage reculer : inverse
	if (distance[1] > distance[2])
	{
		droite();
	}
	else
		gauche();
}

void tourner_inv()
{
	//parametrage avancer : lat A1 = 0 et lat A2 = 1
	//parametrage reculer : inverse
	if (distance[4] < distance[5])
	{
		droite();
	}
	else
		gauche();
}

void stop()
{
	//parametrage avancer : lat A1 = 0 et lat A2 = 1
//parametrage reculer : inverse

	LATCbits.LATC3 = 0; //A3
	LATCbits.LATC0 = 0; //A4

	LATCbits.LATC2 = 0;	//A1
	LATCbits.LATC5 = 0; //A2
}
