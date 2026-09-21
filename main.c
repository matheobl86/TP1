/*
 * TP - Activation de LEDs sur le port B par le bouton PB2, via la
 * bibliothèque PinFunc, avec un clignotement à 1 Hz EXACT généré par le
 * Timer1 (mode CTC), au lieu de _delay_ms (qui dérive/n'est pas précis
 * à cause du temps pris par le reste du code entre deux appels).
 *
 * PB2 (entrée, haute impédance) : bouton de commande
 *   PB2 appuyé (= 1) -> PB0 et PB1 clignotent à 1 Hz
 *   PB2 relâché (= 0) -> PB0 et PB1 éteintes
 *
 * Calcul du Timer1 (F_CPU = 8 000 000 Hz, voir Def.h) :
 *   Prescaler /256 -> horloge du timer = 8 000 000 / 256 = 31 250 Hz
 *   Pour une demi-période de 0,5 s (toggle deux fois par seconde = 1 Hz) :
 *   31 250 * 0,5 = 15 625 ticks -> OCR1A = 15 625 - 1 = 15624
 *   (valeur entière exacte, donc pas d'erreur d'arrondi contrairement à
 *   une boucle de _delay_ms)
 *
 * Remarque matérielle : on utilise Port_SetInputHiZ (pas de pull-up
 * interne) car la carte tire déjà PB2 à la masse au repos et le bouton
 * l'amène à VCC en cas d'appui. Si ton bouton est câblé dans l'autre
 * sens (relié à la masse quand on appuie), remplace Port_SetInputHiZ
 * par Port_SetInputPullUp et inverse le test (!= 0 devient == 0).
 *
 * Aucun accès direct aux registres du port B : uniquement les fonctions
 * de PinFunc.h (Port_SetOutput, Port_SetInputHiZ, Port_ToggleOutput,
 * Port_SetOutputLow, Port_ReadInput).
 */

#include "Def.h"
#include "PinFunc.h"

// Déclaration des broches (globales pour être accessibles depuis l'ISR)
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

// Appelée toutes les 0,5 s exactement par le Timer1
ISR(TIMER1_COMPA_vect)
{
    if (Port_ReadInput(&bouton) != 0)   // bouton appuyé -> actif
    {
        Port_ToggleOutput(&led0);
        Port_ToggleOutput(&led1);
    }
    else                                  // bouton relâché -> éteint
    {
        Port_SetOutputLow(&led0);
        Port_SetOutputLow(&led1);
    }
}

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
    Port_SetOutput(&led0);      // PB0 en sortie
    Port_SetOutput(&led1);      // PB1 en sortie
    Port_SetInputHiZ(&bouton);  // PB2 en entrée, sans pull-up interne

    Timer1_Init();
    sei();   // active les interruptions globales

    while (1)
    {
        // Tout le comportement (lecture du bouton, clignotement) est
        // géré dans l'ISR du Timer1 : la boucle principale reste libre.
    }

    return 0;
}
