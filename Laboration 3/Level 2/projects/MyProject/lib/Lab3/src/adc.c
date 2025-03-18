#include "adc.h"
#include "gd32vf103.h"

// Initierar ADC0 med möjlighet att inkludera temperatursensorn (om 'tmp' är sann)
void ADC3powerUpInit(int tmp)
{
    // Aktivera klockor för GPIOA och ADC0
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_ADC0);

    // Ställ in ADC-klockan: dividera APB2-klockan med 8
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV8);

    // Konfigurera GPIOA-pinnar som analoga ingångar
    // PIN3 används för ADC-mätning och PIN2 är reserverad (markerad med "SWIM!")
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_MODE_OUT_PP, GPIO_PIN_3);
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_MODE_OUT_PP, GPIO_PIN_2); // SWIM!

    // Återställ ADC0 till standardvärden
    adc_deinit(ADC0);

    // Konfigurera ADC0 i fritt läge (oberoende drift)
    adc_mode_config(ADC_MODE_FREE);

    // Inaktivera kontinuerligt läge – varje sample triggas separat
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE);

    // Inaktivera scan-läge – endast en kanal konverteras åt gången
    adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE);

    // Justera ADC-data till höger (resultatet placeras med rätt justering)
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);

    // Ställ in längden på den regelbundna kanalkonverteringen till 1 (en kanal per omgång)
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1);

    // Konfigurera den första (och enda) regelbundna kanalen:
    // Välj kanal 3 med ett sampeltid på 13,5 cykler
    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_3, ADC_SAMPLETIME_13POINT5);

    // Välj ingen extern triggerkälla för den regelbundna kanalen (SW-trigger används)
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_NONE);

    // Aktivera extern trigger för den regelbundna kanalen
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);

    // Om temperatursensorn ska inkluderas (tmp != 0)
    if (tmp)
    {
        // Aktivera automatisk insatt kanalkonfiguration
        adc_special_function_config(ADC0, ADC_INSERTED_CHANNEL_AUTO, ENABLE);

        // Ange att endast 1 insatt kanal ska konverteras
        adc_channel_length_config(ADC0, ADC_INSERTED_CHANNEL, 1);

        // Aktivera temperatursensor och intern referensspänning
        adc_tempsensor_vrefint_enable();

        // Konfigurera den insatta kanalen till att mäta kanal 17 med ett långsammare sampeltid (239,5 cykler)
        // Observera: Tidigare konfiguration med kanal 16 är kommenterad ut
        adc_inserted_channel_config(ADC0, 0, ADC_CHANNEL_17, ADC_SAMPLETIME_239POINT5); // SWIM!
    }

    // Aktivera ADC0
    adc_enable(ADC0);

    // Vänta ca 1 ms för att ADC0 ska stabiliseras (enkelt tidsfördröjningsloop)
    for (int i = 0; i < 0xFFFF; i++)
        ;

    // Starta kalibrering av ADC0
    adc_calibration_enable(ADC0);

    // Vänta ytterligare ca 1 ms efter kalibreringen
    for (int i = 0; i < 0xFFFF; i++)
        ;

    // Starta en mjukvarutriggerad konversion på den regelbundna kanalen
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}
