/*
 * TP - Activation de LEDs sur le port B par le bouton PB2, via la
 * bibliothèque PinFunc.
 *
 * PB2 (entrée, haute impédance) : bouton de commande
 *   PB2 appuyé (= 1) -> PB0 et PB1 clignotent à 1 Hz
 *   PB2 relâché (= 0) -> PB0 et PB1 éteintes
 *
 * Remarque matérielle : on utilise Port_SetInputHiZ (pas de pull-up
 * interne) car la carte tire déjà PB2 à la masse au repos et le bouton
 * l'amène à VCC en cas d'appui. Avec Port_SetInputPullUp, la broche
 * restait active en permanence (conflit avec le circuit de la carte).
 * Si ton bouton est câblé dans l'autre sens (relié à la masse quand on
 * appuie), remplace Port_SetInputHiZ par Port_SetInputPullUp et inverse
 * le test (!= 0 devient == 0).
 *
 * Aucun accès direct aux registres du port B : uniquement les fonctions
 * de PinFunc.h (Port_SetOutput, Port_SetInputHiZ, Port_ToggleOutput,
 * Port_SetOutputLow, Port_ReadInput).
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

    TPort bouton = {
        .pport = &PORTB,
        .ppin  = &PINB,
        .pddr  = &DDRB,
        .mask  = (1 << PB2)
    };

    // Configuration de la direction des broches
    Port_SetOutput(&led0);      // PB0 en sortie
    Port_SetOutput(&led1);      // PB1 en sortie
    Port_SetInputHiZ(&bouton);  // PB2 en entrée, sans pull-up interne

    while (1)
    {
        if (Port_ReadInput(&bouton) != 0)   // bouton appuyé -> actif
        {
            Port_ToggleOutput(&led0);
            Port_ToggleOutput(&led1);
            _delay_ms(500);   // demi-période -> 1 Hz (période complète = 1 s)
        }
        else                                  // bouton relâché -> éteint
        {
            Port_SetOutputLow(&led0);
            Port_SetOutputLow(&led1);
        }
    }

    return 0;
}
