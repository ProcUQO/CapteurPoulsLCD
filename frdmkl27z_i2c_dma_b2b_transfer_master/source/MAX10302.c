#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_i2c.h"
#include "fsl_debug_console.h"
#include "MAX10302.h"
#include <string.h>

#define MAX_I2C I2C1
#define MAX_ADDR 87 // 0x57

// VOIR PAGE 10 du manuel pour les adresses

// 0x04-0x07 (FIFO) couvert entre les pages 13-17
// pointeur pour indiquer oèu écrire le prochain échantillon
#define REG_FIFO_WR_PTR 0x04
// overflow, quand la fifo est pleine (on compte échantillons perdus)
#define REG_FIFO_OVF 0x05
// fifo read pointer (prochain à lire)
#define REG_FIFO_RD_PTR 0x06
// fifo data register (récupére données de la fifo)
#define REG_FIFO_DATA 0x07

// juste la configuration de la fifo
#define REG_FIFO_CONFIG 0x08
// mode de fonctionnement du capteur
#define REG_MODE_CONFIG 0x09
// spO2 configuration ( bit 6-5 plage mesure ADC,
// 4-2 fréquence échantillon, 1 et 0 durée impulsion led par mesure)
#define REG_SPO2_CONFIG 0x0A
#define REG_LED1_PA 0x0C // LED rouge
#define REG_LED2_PA 0x0D // LED infrarouge

// les defines pour le traitement du bpm
#define FS_HZ 100 // 100 échantilons par seconde
#define FINGER_MIN MAX30102_FINGER_MIN // IR sous ce seuil = pas de doigt (pour ne pas spammer)
#define STABILIZATION 200 // environ 2s pour les premières secondes qu'on pose un doigt
// (sinon les valeurs ne sont pas rapport pendant qu'on le pose).
#define SEUIL_BATTEMENT 30
#define INTERVALLE_MIN 40 // 400ms bref 150bpm
#define INTERVALLE_MAX 150 // 1.5s 40bpm

// J'utilise 32 bits vu que le capteur (son adc) est de 18 bits (voir doc)
typedef struct
{
    int32_t moyenne; // niv moyen du signal
    uint32_t nb_echantillons; // compteur DEPUIS que le doigt est là
    uint32_t dernier_battement; // numéro d'échantillon du dernier battement
    int32_t bpm; // lissé (stabilisé avec la moyenne)
    uint8_t en_battement; // 1( true) tant que le signal est pas redescendu
} suivi_bpm_t;
static suivi_bpm_t bsuivi;

// pour parler au capteur via I2C (voir la doc en cas de doute!!!) (envoie cte valeur au registre de max30)
static status_t max_write(uint8_t registre, uint8_t valeur)
{
    i2c_master_transfer_t transfer = {0}; // type de variable pour le transfert I2C de nxp
    transfer.slaveAddress = MAX_ADDR; // 87 parce que le slave est 1010111 (page 29)
    transfer.direction = kI2C_Write; // merci le sdk nxp, donne la direction qu'on veut au transfert
    transfer.subaddress = registre;
    transfer.subaddressSize = 1; // 8 bit
    transfer.data = &valeur;
    transfer.dataSize = 1;
    return I2C_MasterTransferBlocking(MAX_I2C, &transfer);
}

// pour recevoir les données I2C (j'y vais un peu avec une mentalité de réseau, on reçoit le paquet)
static status_t max_read(uint8_t registre, uint8_t *paquet, size_t taille)
{
    i2c_master_transfer_t transfer = {0};
    transfer.slaveAddress = MAX_ADDR;
    transfer.direction = kI2C_Read;
    transfer.subaddress = registre;
    transfer.subaddressSize = 1;
    transfer.data = paquet;
    transfer.dataSize = taille;
    return I2C_MasterTransferBlocking(MAX_I2C, &transfer);
}

/* Configuration des registres.

*/
void max_init(void)
{
    uint8_t m;
    // juste un petit reset avant de démarrer (selon la doc (p10, p18), le bit 6 (0x40) est le reset)
    max_write(REG_MODE_CONFIG, 0x40);
    do
    {
        // Reset
        max_read(REG_MODE_CONFIG, &m, 1);
    } while (m & 0x40); // ptit bitwise check pour savoir quand c'est au bit 6 (p.18)

    // comme reommandé par la doc (p14), je met tout ça à zéro
    max_write(REG_FIFO_WR_PTR, 0);
    max_write(REG_FIFO_OVF, 0);
    max_write(REG_FIFO_RD_PTR, 0);

    // config mémoire fifo et des mesures
    max_write(REG_FIFO_CONFIG, 0x1F); // activation du rollover (page 17)
    max_write(REG_SPO2_CONFIG, 0x27); // plage 4096 nA, 100 éch/s, impulsion 411 us (18 bits)

    // le courant pour le rouge et infrarouge
    max_write(REG_LED1_PA, 0x24);     // rouge
    max_write(REG_LED2_PA, 0x24);     // infrarouge
    max_write(REG_MODE_CONFIG, 0x03); // j'utilise le sp02, je ne vois pas pourquoi on n'utiliserais pas l'IR avec rouge (page 18)
}

// regarde si le max30102 a de nouvelles mesures, on les prend et on sépare les valeurs rouge/infrarouge
int max_read_sample(uint32_t *rouge, uint32_t *infrarouge)
{
    uint8_t write_pointer;
    uint8_t read_pointer;
    uint8_t octets[6]; // 6 car rouge = 18 bit, infrarouge = 18 bit [0-2 poru rouge, 3-5 pour infrarouge]

    max_read(REG_FIFO_WR_PTR, &write_pointer, 1);
    max_read(REG_FIFO_RD_PTR, &read_pointer, 1);
    if(write_pointer == read_pointer)
        return 0; // rien de neuf

    max_read(REG_FIFO_DATA, octets, 6);
    // vu que c'est en 3 morceaux, faut reconstruire les valeurs 18 bits
    *rouge = (((uint32_t)octets[0] << 16) |
    ((uint32_t)octets[1] << 8) |
    octets[2]) & 0x3FFFF;

    *infrarouge = (((uint32_t)octets[3] << 16) |
    ((uint32_t)octets[4] << 8) |
    octets[5]) & 0x3FFFF;

    return 1;
}

/* une fois par échantillon. Retourne le BPM lissé quand un
   nouveau battement est détecté, sinon 0. */
int bpm_update(uint32_t ir_raw)
{
    int32_t ir = (int32_t)ir_raw;

    if (ir < FINGER_MIN)
    { // pas de doigt, on pitch ça à zéro
        memset(&bsuivi, 0, sizeof(bsuivi));
        return 0;
    }

    bsuivi.nb_echantillons++;
    if (bsuivi.nb_echantillons == 1){
        bsuivi.moyenne = ir;
    }
    bsuivi.moyenne += (ir - bsuivi.moyenne) / 32;


    if (bsuivi.nb_echantillons <= STABILIZATION)
    { // on attend un peu parce que les premières valeurs sont farfelues
        return 0;
    }

    // un battement = le signal monte (au dessus de sa moyenne de seuil)
    int32_t variation = ir - bsuivi.moyenne;
    if(variation < 0){
        bsuivi.en_battement = 0; // signal redescendu
    }
    if(variation < SEUIL_BATTEMENT || bsuivi.en_battement){
        return 0;
    }
    bsuivi.en_battement = 1;

    // intervalle depuis le battement d'avant
    uint32_t avant = bsuivi.dernier_battement;
    uint32_t intervalle = bsuivi.nb_echantillons - avant;
    if(avant != 0 && intervalle < INTERVALLE_MIN){
        return 0;
    } // trop rapproché, on peut assumer que c'est du bruit
    bsuivi.dernier_battement = bsuivi.nb_echantillons;
    if(avant == 0 || intervalle > INTERVALLE_MAX){
        return 0;
    } // premier battement (ou trop lent)

    // la moyenne de bpm est full simple, c'est le 3/4 de l'ancienne valeur + 1/4 de la nouvelle
    // moyenne mobile exponentielle (EMA)
    int32_t bpm_instant = (FS_HZ * 60) / intervalle;
    if(bsuivi.bpm == 0){
        bsuivi.bpm = bpm_instant;
    } else {
        bsuivi.bpm = (bsuivi.bpm * 3 + bpm_instant) / 4;
    }
    return bsuivi.bpm;
}
