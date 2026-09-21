#include "PinFunc.h"

//!Fonction pour fixer les broches en entree et mettre une resistance de rappel
/*!
	\param pport Un pointeur sur la structure avec le port, la variable mask doit contenir la ou les broches a configurer.
*/
void Port_SetInputPullUp(const TPort *pport)
{
	*(pport->pddr) &= ~pport->mask;
	Port_SetOutputHigh(pport);
}

//!Fonction pour fixer les broches en entree et mettre en haut impedance
/*!
	\param pport Un pointeur sur la structure avec le port, la variable mask doit contenir la ou les broches a configurer.
*/
void Port_SetInputHiZ(const TPort *pport)
{
	*(pport->pddr) &= ~pport->mask;
	Port_SetOutputLow(pport);
}

//!Fonction pour fixer les broches en sortie.
/*!
	\param pport Un pointeur sur la structure avec le port, la variable mask doit contenir la ou les broches a configurer.
*/
void Port_SetOutput(const TPort *pport)
{
	*(pport->pddr) |= pport->mask;
}

//!Fonction pour fixer une ou des sorties � 1.
/*!
	\param pport Un pointeur sur la structure avec le port, la variable mask doit contenir la ou les broches a modifier.
*/
void Port_SetOutputHigh(const TPort *pport)
{
	*(pport->pport) |= pport->mask;
}

//!Fonction pour fixer une ou des sorties � 0.
/*!
	\param pport Un pointeur sur la structure avec le port, la variable mask doit contenir la ou les broches a modifier.
*/
void Port_SetOutputLow(const TPort *pport)
{
	*(pport->pport) &= ~pport->mask;
}

//!Fonction pour inverser une ou des sorties.
/*!
	\param pport Un pointeur sur la structure
*/
void Port_ToggleOutput(const TPort *pport)
{
	*(pport->pport) ^= pport->mask;
}

//!Fonction pour lire un port.
/*!
	\param pport Un pointeur sur la structure avec le port, la variable mask doit contenir la ou les broches a lire.
*/
uint8_t Port_ReadInput(const TPort *pport)
{
	return *(pport->ppin) & pport->mask;
}

//!Fonction pour lire un port.
/*!
	\param pport Un pointeur sur la structure avec le port, la variable mask doit contenir la ou les broches a lire.
	\return La valeur des broches (sortie).
*/
uint8_t Port_ReadOutput(const TPort *pport)
{
	return *(pport->pport) & pport->mask;
}
