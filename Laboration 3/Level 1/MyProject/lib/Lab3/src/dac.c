#include "gd32vf103.h"
#include "dac.h"

// Funktion för att initiera och "power up" DAC0.
void DAC0powerUpInit()
{
    // Aktivera klockan för GPIOA-porten. DAC0-utgången använder en GPIO-pin (PA4).
    rcu_periph_clock_enable(RCU_GPIOA);

    // Aktivera klockan för DAC periferin.
    rcu_periph_clock_enable(RCU_DAC);

    // Initiera GPIOA pin 4 i analogt läge.
    // Användning av GPIO_MODE_AIN (Analog In) säkerställer att digitala störningar minimeras.
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_4);

    // Återställ DAC-konfigurationen till standardläget.
    dac_deinit();

    // Inaktivera trigger för DAC0. Detta innebär att DAC inte uppdateras via externa triggerhändelser.
    dac_trigger_disable(DAC0);

    // Konfigurera DAC0 så att ingen vågform genereras (exempelvis brus eller triangelvåg).
    dac_wave_mode_config(DAC0, DAC_WAVE_DISABLE);

    // Aktivera utgångsbuffer för DAC0. En aktiverad buffer kan ge en mer stabil analog utgång genom att minska utgångsimpedansen.
    dac_output_buffer_enable(DAC0);

    // Sätt ett initialt digitalt värde för DAC0 med 12-bitars högerjusterad data.
    // Här sätts värdet till 0x07ff (2047 i decimal), vilket ofta representerar mittvärdet (om skalan är 0-4095).
    dac_data_set(DAC0, DAC_ALIGN_12B_R, 0x07ff);

    // Aktivera DAC0 så att den börjar omvandla digital data till en analog spänning.
    dac_enable(DAC0);
}

// Funktion för att uppdatera värdet på DAC0.
// Parametern 'value' representerar ett 12-bitars digitalt värde (0 - 4095) som omvandlas till en analog spänning.
void DAC0set(int value)
{
    dac_data_set(DAC0, DAC_ALIGN_12B_R, value); // Sätter det nya digitala värdet för DAC0.
}
