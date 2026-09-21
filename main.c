/*
 * TP - Clignotement de LED sélectionné par PB2, via la bibliothèque PinFunc
 *
 * PB2 (entrée, pull-up) : sélectionne la LED à faire clignoter
 *   PB2 = 1 -> PB0 clignote à 1 Hz
 *   PB2 = 0 -> PB1 clignote à 1 Hz
 *
 * Aucun accès direct aux registres du port B : uniquement les fonctions
 * de PinFunc.h (Port_SetOutput, Port_SetInputPullUp, Port_ToggleOutput,
 * Port_ReadInput).
 */

#include "Def.h"
#include "PinFunc.h"

int main(void)
{
    // Déclaration des broches via la structure TPort
    TPort led0 = {
        .pport = &PORTB,
        .ppin  = &PINB,
        .pddr  = &DDRB,
        .mask  = (1 << PB0)
    };

    TPort led1 = {
        .pport = &PORTB,
        .ppin  = &PINB,
        .pddr  = &DDRB,
        .mask  = (1 << PB1)
    };

    TPort selecteur = {
        .pport = &PORTB,
        .ppin  = &PINB,
        .pddr  = &DDRB,
        .mask  = (1 << PB2)
    };

    // Configuration de la direction des broches
    Port_SetOutput(&led0);            // PB0 en sortie
    Port_SetOutput(&led1);            // PB1 en sortie
    Port_SetInputPullUp(&selecteur);  // PB2 en entrée avec résistance de rappel

    while (1)
    {
        if (Port_ReadInput(&selecteur) != 0)   // PB2 = 1
        {
            Port_ToggleOutput(&led0);          // clignotement de PB0
        }
        else                                     // PB2 = 0
        {
            Port_ToggleOutput(&led1);          // clignotement de PB1
        }

        _delay_ms(500);   // demi-période -> 1 Hz (période complète = 1 s)
    }

    return 0;
}
