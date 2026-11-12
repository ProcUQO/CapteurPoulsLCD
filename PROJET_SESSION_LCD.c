/*
 * Ecran LCD 20x4 (TC2004 + carte I2C PCF8574) sur la FRDM-KL27Z
 *
 * Branchement (bus I2C0) :
 *   LCD SDA -> PTB1 (J4 broche 10)
 *   LCD SCL -> PTB0 (J4 broche 12)
 *   LCD VCC -> P5V_USB
 *   LCD GND -> GND
 *
 * PTB0/PTB1 sont configurees en I2C0 par l'outil Pins
 * (BOARD_InitPins, appele dans BOARD_InitBootPins).
 */

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_i2c.h"
#include "fsl_debug_console.h"

#define LCD_I2C   I2C0          // Bus de l'ecran (PTB0 = SCL, PTB1 = SDA)
#define LCD_ADDR  0x27U         // Adresse I2C de la carte PCF8574 de l'ecran

/* ---------- I2C ---------- */

/* Envoie 1 octet brut au PCF8574 de l'ecran.
   Le PCF8574 n'a pas de registres : l'octet regle directement ses 8 sorties. */
static void pcf_write(uint8_t val)
{
    i2c_master_transfer_t x = {0};      // Description du transfert, tout a zero
    x.slaveAddress = LCD_ADDR;          // Adresse de l'ecran
    x.direction = kI2C_Write;           // On envoie
    x.data = &val;                      // Adresse de l'octet a envoyer
    x.dataSize = 1;                     // 1 seul octet
    I2C_MasterTransferBlocking(LCD_I2C, &x);   // Envoie et attend la fin du transfert
}

/* ---------- LCD ---------- */
/* Octet envoye au PCF8574 :
 *
 *   bit :  7   6   5   4  |  3  |  2  |  1  |  0
 *          D7  D6  D5  D4 |  BL |  E  |  RW |  RS
 *          4 bits donnees | lum.| lis |  0  | 0 = commande, 1 = caractere
 *
 * Seuls 4 fils de donnees sont branches : chaque octet destine a
 * l'ecran est donc envoye en 2 moities (mode 4 bits).
 */

/* Envoie un octet a l'ecran.
 *   val : commande ou caractere (code ASCII)
 *   rs  : 0 = commande, 1 = caractere a afficher */
static void lcd_send(uint8_t val, uint8_t rs)
{
    uint8_t ctrl = 0x08U | rs;                    // Retroeclairage (bit 3) + RS (bit 0)
    uint8_t hi = (val & 0xF0U) | ctrl;            // Moitie haute (deja dans les bits 7-4) + controle
    uint8_t lo = (uint8_t)(val << 4) | ctrl;      // Moitie basse montee dans les bits 7-4 + controle

    pcf_write(hi | 0x04U);              // E = 1 : donnees en place
    pcf_write(hi);                      // E = 0 : l'ecran lit la moitie haute
    pcf_write(lo | 0x04U);              // E = 1
    pcf_write(lo);                      // E = 0 : l'ecran lit la moitie basse
    SDK_DelayAtLeastUs(2000U, SystemCoreClock);   // 2 ms pour que l'ecran traite l'octet
}

/* Affiche un texte caractere par caractere (jusqu'au '\0' de fin) */
static void lcd_text(const char *s)
{
    while (*s)
    {
        lcd_send((uint8_t)*s++, 1U);    // RS = 1 : caractere, puis caractere suivant
    }
}

/* Place le curseur. ligne : 0 a 3, colonne : 0 a 19.
   Adresses de debut de ligne d'un ecran 20x4 : 0x00, 0x40, 0x14, 0x54 */
static void lcd_goto(uint8_t ligne, uint8_t colonne)
{
    static const uint8_t debut[4] = {0x00U, 0x40U, 0x14U, 0x54U};
    lcd_send((uint8_t)(0x80U | (debut[ligne] + colonne)), 0U);   // 0x80 = "deplace le curseur"
}

/* Initialise l'ecran (sequence de la datasheet HD44780) */
static void lcd_init(void)
{
    SDK_DelayAtLeastUs(50000U, SystemCoreClock);  // 40 ms minimum apres la mise sous tension
    lcd_send(0x33U, 0U);                // Moities 3, 3 : retour a un etat connu...
    lcd_send(0x32U, 0U);                // ...moities 3, 2 : passage en mode 4 bits
    lcd_send(0x28U, 0U);                // Function set : 4 bits, plusieurs lignes, police 5x8
    lcd_send(0x0CU, 0U);                // Display control : ecran allume, curseur cache
    lcd_send(0x06U, 0U);                // Entry mode : le curseur avance vers la droite
    lcd_send(0x01U, 0U);                // Clear display : efface l'ecran
}

/* ---------- Programme principal ---------- */

int main(void)
{
    BOARD_InitBootPins();               // UART + PTB0/PTB1 en I2C0 (outil Pins)
    BOARD_InitBootClocks();             // Horloges du micro
    BOARD_InitDebugConsole();           // PRINTF vers Tera Term

    i2c_master_config_t cfg;            // Configuration du module I2C
    I2C_MasterGetDefaultConfig(&cfg);   // Valeurs par defaut : 100 kHz
    I2C_MasterInit(LCD_I2C, &cfg, CLOCK_GetBusClkFreq());   // Demarre I2C0

    lcd_init();                         // Prepare l'ecran

    lcd_goto(0, 0); lcd_text("Ligne 1 : Hello");
    lcd_goto(1, 0); lcd_text("Ligne 2 : KL27Z");
    lcd_goto(2, 0); lcd_text("Ligne 3 : I2C0");
    lcd_goto(3, 0); lcd_text("Ligne 4 : OK");

    while (1)                           // Un programme embarque ne sort jamais de main
    {
    }
}
