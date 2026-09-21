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

#include "Def.h"      // avr/io.h, F_CPU, util/delay.h, avr/interrupt.h
#include "PinFunc.h"   // structure TPort + fonctions Port_xxx

// --- Déclaration des broches ------------------------------------------
// Ces TPort sont globales (et non locales à main) car l'ISR du Timer1,
// définie plus bas, a elle aussi besoin d'y accéder pour piloter les
// LEDs et lire le bouton.
TPort led0 = {
    .pport = &PORTB,        // registre de sortie du port B
    .ppin  = &PINB,         // registre de lecture du port B
    .pddr  = &DDRB,         // registre de direction du port B
    .mask  = (1 << PB0)     // bit concerné : PB0
};

TPort led1 = {
    .pport = &PORTB,
    .ppin  = &PINB,
    .pddr  = &DDRB,
    .mask  = (1 << PB1)     // bit concerné : PB1
};

TPort bouton = {
    .pport = &PORTB,
    .ppin  = &PINB,
    .pddr  = &DDRB,
    .mask  = (1 << PB2)     // bit concerné : PB2
};

// --- Routine d'interruption du Timer1 ----------------------------------
// Déclenchée automatiquement par le matériel toutes les 0,5 s exactement
// (quand le compteur du Timer1 atteint OCR1A, voir Timer1_Init).
// C'est ici que se joue tout le comportement du programme : le bouton
// est relu à chaque tick, donc l'état des LEDs réagit avec un délai
// maximum de 0,5 s après un appui/relâchement.
ISR(TIMER1_COMPA_vect)
{
    if (Port_ReadInput(&bouton) != 0)   // bouton appuyé (PB2 = 1) -> actif
    {
        // On inverse l'état courant des deux LEDs : si elles étaient
        // éteintes elles s'allument, si elles étaient allumées elles
        // s'éteignent. Comme cette routine est appelée toutes les
        // 0,5 s, deux inversions consécutives (allumé/éteint) forment
        // une période complète de 1 s -> fréquence de clignotement = 1 Hz.
        Port_ToggleOutput(&led0);
        Port_ToggleOutput(&led1);
    }
    else                                  // bouton relâché (PB2 = 0) -> éteint
    {
        // On force l'extinction à chaque tick (plutôt que de ne rien
        // faire) pour être sûr que les LEDs restent bien éteintes même
        // si elles étaient allumées juste avant le relâchement du bouton.
        Port_SetOutputLow(&led0);
        Port_SetOutputLow(&led1);
    }
}

// --- Configuration du Timer1 en mode CTC --------------------------------
// CTC = "Clear Timer on Compare match" : le compteur repart à 0 dès
// qu'il atteint la valeur OCR1A, et déclenche l'interruption
// TIMER1_COMPA_vect à ce moment-là. Le timer fonctionne alors en continu,
// sans aucune intervention du programme principal.
static void Timer1_Init(void)
{
    TCCR1B |= (1 << WGM12);   // sélectionne le mode CTC (TOP = OCR1A)
    TCCR1B |= (1 << CS12);    // sélectionne le prescaler /256

    // Nombre de tops d'horloge du timer pour atteindre 0,5 s (voir le
    // calcul détaillé en en-tête du fichier) : on compte de 0 à OCR1A
    // inclus, donc OCR1A = nombre_de_ticks - 1.
    OCR1A = 15624;

    TIMSK |= (1 << OCIE1A);   // autorise l'interruption "Compare Match A"
}

int main(void)
{
    // --- Configuration de la direction des broches ---
    Port_SetOutput(&led0);      // PB0 en sortie (pilotage de la LED)
    Port_SetOutput(&led1);      // PB1 en sortie (pilotage de la LED)
    Port_SetInputHiZ(&bouton);  // PB2 en entrée, sans pull-up interne
                                 // (le circuit de la carte gère déjà le
                                 // niveau de repos, voir remarque en tête
                                 // de fichier)

    Timer1_Init();   // démarre le Timer1 en mode CTC
    sei();            // active globalement les interruptions (indispensable,
                       // sinon l'ISR ne sera jamais appelée)

    while (1)
    {
        // La boucle principale ne fait volontairement rien : tout le
        // comportement (lecture du bouton, clignotement des LEDs) est
        // pris en charge par l'interruption du Timer1, de façon
        // totalement indépendante du temps d'exécution du reste du code.
    }

    return 0;
}
