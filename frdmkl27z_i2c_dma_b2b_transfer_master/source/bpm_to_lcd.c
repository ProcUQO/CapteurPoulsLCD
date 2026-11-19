#include "bpm_to_lcd.h"

#include <stdio.h>

#include "TC2004_LCD.h"

void bpm_to_lcd(int bpm)
{
    char texte[21];

    lcd_goto(1, 0);
    if (bpm > 0)
    {
        (void)snprintf(texte, sizeof(texte), "BPM: %d", bpm);
        lcd_text(texte);
    }
    else
    {
        lcd_text("BPM: ---");
    }

    lcd_text("           ");
}
