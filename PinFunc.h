#ifndef PINFUNC_H
#define PINFUNC_H

#include "Def.h"

#include <stdint.h>

//Une structure pour la gestion des ports
typedef struct{
	volatile uint8_t *pport;	//Pointeur sur le registre PORT
	volatile uint8_t *ppin;		//Pointeur sur le registre PIN
	volatile uint8_t *pddr;		//Pointeur sur le registre DDR
	
	uint8_t mask;				//Les bits a modifier
}TPort;

//!Fonction pour fixer les broches en entree et mettre une resistance de rappel
/*!
	\param pport Un pointeur sur la structure avec le port, la variable mask doit contenir la ou les broches a configurer.
*/
void Port_SetInputPullUp(const TPort *pport);

//!Fonction pour fixer les broches en entree et mettre en haut impedance
/*!
	\param pport Un pointeur sur la structure avec le port, la variable mask doit contenir la ou les broches a configurer.
*/
void Port_SetInputHiZ(const TPort *pport);

//!Fonction pour fixer les broches en sortie.
/*!
	\param pport Un pointeur sur la structure avec le port, la variable mask doit contenir la ou les broches a configurer.
*/
void Port_SetOutput(const TPort *pport);

//!Fonction pour fixer une ou des sorties à 1. 
/*!
	\param pport Un pointeur sur la structure avec le port, la variable mask doit contenir la ou les broches a modifier.
*/
void Port_SetOutputHigh(const TPort *pport);

//!Fonction pour fixer une ou des sorties à 0. 
/*!
	\param pport Un pointeur sur la structure avec le port, la variable mask doit contenir la ou les broches a modifier.
*/
void Port_SetOutputLow(const TPort *pport);

//!Fonction pour inverser une ou des sorties.
/*!
	\param pport Un pointeur sur la structure 
*/
void Port_ToggleOutput(const TPort *pport);

//!Fonction pour lire un port.
/*!
	\param pport Un pointeur sur la structure avec le port, la variable mask doit contenir la ou les broches a lire.
	\return La valeur des broches (entree).
*/
uint8_t Port_ReadInput(const TPort *pport);

//!Fonction pour lire un port.
/*!
	\param pport Un pointeur sur la structure avec le port, la variable mask doit contenir la ou les broches a lire.
	\return La valeur des broches (sortie).
*/
uint8_t Port_ReadOutput(const TPort *pport);


#endif //PINFUNC_H