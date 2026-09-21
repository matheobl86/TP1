/*
 * TP - Clignotement de LED sélectionné par PB2, via la bibliothèque
 * PinFunc, avec un 1 Hz exact généré par le Timer1 (mode CTC).
 *
 * PB2 (entrée, pull-up) : sélectionne la LED à faire clignoter
 *   PB2 = 1 -> PB0 clignote à 1 Hz
 *   PB2 = 0 -> PB1 clignote à 1 Hz
 *
 * Calcul du Timer1 (F_CPU = 8 000 000 Hz, voir Def.h) :
 *   Prescaler /256 -> horloge du timer = 8 000 000 / 256 = 31 250 Hz
 *   Pour une demi-période de 0,5 s (toggle deux fois par seconde = 1 Hz) :
 *   31 250 * 0,5 = 15 625 ticks -> OCR1A = 15 625 - 1 = 15624
 *   (valeur entière exacte, donc pas d'erreur d'arrondi comme avec
 *   une boucle de _delay_ms)
 *
 * Aucun accès direct aux registres du port B : uniquement les fonctions
 * de PinFunc.h (Port_SetOutput, Port_SetInputPullUp, Port_ToggleOutput,
 * Port_ReadInput).
 */

#include "Def.h"      // avr/io.h, F_CPU, util/delay.h, avr/interrupt.h
#include "PinFunc.h"   // structure TPort + fonctions Port_xxx

// --- Déclaration des broches (globales : utilisées dans l'ISR) ---------
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

// --- Routine d'interruption du Timer1 -----------------------------------
// Appelée automatiquement toutes les 0,5 s exactement (voir Timer1_Init).
// Deux appels consécutifs (0,5 s + 0,5 s) forment une période complète
// de 1 s -> fréquence de clignotement de la LED sélectionnée = 1 Hz.
ISR(TIMER1_COMPA_vect)
{
    if (Port_ReadInput(&selecteur) != 0)   // PB2 = 1
    {
        Port_ToggleOutput(&led0);          // clignotement de PB0
    }
    else                                     // PB2 = 0
    {
        Port_ToggleOutput(&led1);          // clignotement de PB1
    }
}

// --- Configuration du Timer1 en mode CTC --------------------------------
static void Timer1_Init(void)
{
    TCCR1B |= (1 << WGM12);   // mode CTC, TOP = OCR1A
    TCCR1B |= (1 << CS12);    // prescaler /256

    OCR1A = 15624;            // 0,5 s exactement (voir calcul en en-tête)

    TIMSK |= (1 << OCIE1A);   // active l'interruption de comparaison A
}

int main(void)
{
    // Configuration de la direction des broches
    Port_SetOutput(&led0);            // PB0 en sortie
    Port_SetOutput(&led1);            // PB1 en sortie
    Port_SetInputPullUp(&selecteur);  // PB2 en entrée avec résistance de rappel

    Timer1_Init();   // démarre le Timer1 en mode CTC
    sei();            // active les interruptions globales

    while (1)
    {
        // Tout le comportement (lecture de PB2, clignotement) est géré
        // par l'interruption du Timer1 : la boucle principale reste libre.
    }

    return 0;
}
