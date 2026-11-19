#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_i2c.h"
#include "fsl_debug_console.h"

#include "MAX10302.h"
#include "TC2004_LCD.h"
#include "bpm_to_lcd.h"


int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    // Initialisation des deux bus I2C
    i2c_master_config_t cfg;
    I2C_MasterGetDefaultConfig(&cfg);

    I2C_MasterInit(I2C0, &cfg, CLOCK_GetBusClkFreq()); // LCD
    I2C_MasterInit(I2C1, &cfg, CLOCK_GetBusClkFreq()); // MAX30102

    // Initialisation des périphériques
    lcd_init();
    max_init();

    uint32_t red, ir;
    int bpm;
    int dernier_bpm = -1;

    lcd_goto(0, 0);
    lcd_text("Moniteur cardiaque");

    bpm_to_lcd(0);

    while (1)
    {
        if (max_read_sample(&red, &ir))
        {
            bpm = bpm_update(ir);

            // Pas de doigt
            if (ir < MAX30102_FINGER_MIN)
            {
                if (dernier_bpm != 0)
                {
                    bpm_to_lcd(0);
                    dernier_bpm = 0;
                }
            }
            // Nouveau battement valide
            else if (bpm > 0 && bpm != dernier_bpm)
            {
                bpm_to_lcd(bpm);
                PRINTF("BPM=%d\r\n", bpm);

                dernier_bpm = bpm;
            }
        }
    }
}
