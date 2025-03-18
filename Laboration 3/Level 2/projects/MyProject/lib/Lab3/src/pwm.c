#include "pwm.h" // Inkludera headerfilen för PWM-funktioner och -definitioner

// Funktion för att initiera och starta PWM på Timer1 med de specificerade kanalerna.
// Parametern 'ch' används som en bitmask för att välja vilka kanaler (0-3) som ska aktiveras.
void T1powerUpInitPWM(uint16_t ch)
{
    // Deklarera strukturer för timerutgångs- och timerparametrar
    timer_oc_parameter_struct timer_ocinitpara;
    timer_parameter_struct timer_initpara;

    // Aktivera klockor för GPIOA och alternativa funktioner
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);

    // Initiera GPIOA-pinnar för de aktiva kanalerna med alternativ funktion push-pull och 50 MHz hastighet
    if (ch & 0x1)
        gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0); // Kanal 0
    if (ch & 0x2)
        gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1); // Kanal 1
    if (ch & 0x4)
        gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2); // Kanal 2
    if (ch & 0x8)
        gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3); // Kanal 3

    // Aktivera klockan för Timer1
    rcu_periph_clock_enable(RCU_TIMER1);

    // Återställ (deinit) Timer1 till standardinställningar
    timer_deinit(TIMER1);

    // Initiera timerparametrarna med förinställda värden
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler = 107;                     // Prescalervärde (delar ner klockfrekvensen)
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;    // Kantstyrt läge
    timer_initpara.counterdirection = TIMER_COUNTER_UP; // Upp-räkning
    timer_initpara.period = 15999;                      // Periodvärde, påverkar PWM-frekvensen
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;    // Ingen klockdelning
    timer_initpara.repetitioncounter = 0;               // Repetitionsräknare (används ej här)
    timer_init(TIMER1, &timer_initpara);                // Initiera Timer1 med ovan inställningar

    // Initiera kanalutgångsparametrarna med standardvärden för PWM
    timer_channel_output_struct_para_init(&timer_ocinitpara);
    timer_ocinitpara.outputstate = TIMER_CCX_ENABLE;          // Aktivera utgången
    timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;       // Inaktivera kompletterande utgång
    timer_ocinitpara.ocpolarity = TIMER_OC_POLARITY_HIGH;     // Sätt aktiv nivå till hög
    timer_ocinitpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;   // Sätt kompletterande utgångs polaritet
    timer_ocinitpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;   // Idle-läge: låg nivå
    timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW; // Idle-läge för kompletterande utgång

    // Konfigurera Timer1-kanalernas utgångar beroende på vilka kanaler som valts (via bitmasken 'ch')
    if (ch & 0x1)
        timer_channel_output_config(TIMER1, TIMER_CH_0, &timer_ocinitpara); // Kanal 0
    if (ch & 0x2)
        timer_channel_output_config(TIMER1, TIMER_CH_1, &timer_ocinitpara); // Kanal 1
    if (ch & 0x4)
        timer_channel_output_config(TIMER1, TIMER_CH_2, &timer_ocinitpara); // Kanal 2
    if (ch & 0x8)
        timer_channel_output_config(TIMER1, TIMER_CH_3, &timer_ocinitpara); // Kanal 3

    // För varje aktiverad kanal, konfigurera pulsbredd, PWM-läge och inaktivera skuggregistret
    if (ch & 0x1)
    {
        timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, 11999);              // Sätt pulsbredd för kanal 0
        timer_channel_output_mode_config(TIMER1, TIMER_CH_0, TIMER_OC_MODE_PWM0);        // Välj PWM-läge 0
        timer_channel_output_shadow_config(TIMER1, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE); // Inaktivera skuggningsregister
    }
    if (ch & 0x2)
    {
        timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_1, 11999);              // Sätt pulsbredd för kanal 1
        timer_channel_output_mode_config(TIMER1, TIMER_CH_1, TIMER_OC_MODE_PWM0);        // Välj PWM-läge 0
        timer_channel_output_shadow_config(TIMER1, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE); // Inaktivera skuggningsregister
    }
    if (ch & 0x4)
    {
        timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_2, 11999);              // Sätt pulsbredd för kanal 2
        timer_channel_output_mode_config(TIMER1, TIMER_CH_2, TIMER_OC_MODE_PWM0);        // Välj PWM-läge 0
        timer_channel_output_shadow_config(TIMER1, TIMER_CH_2, TIMER_OC_SHADOW_DISABLE); // Inaktivera skuggningsregister
    }
    if (ch & 0x8)
    {
        timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_3, 11999);              // Sätt pulsbredd för kanal 3
        timer_channel_output_mode_config(TIMER1, TIMER_CH_3, TIMER_OC_MODE_PWM0);        // Välj PWM-läge 0
        timer_channel_output_shadow_config(TIMER1, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE); // Inaktivera skuggningsregister
    }

    // Aktivera auto-reload-skuggningsfunktionen och starta Timer1
    timer_auto_reload_shadow_enable(TIMER1);
    timer_enable(TIMER1);
}

// Funktion för att ändra PWM-värdet (pulsbredden) för kanal 2
void T1setPWMch2(int value)
{
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_2, value); // Uppdatera pulsbredden för kanal 2
}

// Funktion för att styra motor B med en "throttle" (gas) parameter.
// Vid positivt värde körs motorn i en riktning (PWM-signal ges på kanal 2),
// vid negativt värde körs motorn i motsatt riktning (PWM-signal ges på kanal 3).
void T1setPWMmotorB(int throttel)
{
    if (throttel > 0)
    {
        // Vid positiv throttle: beräkna pulsbredd proportionellt (0-16000) och sätt signal på kanal 2
        timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_2, (throttel / 100.0) * 16000);
        timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_3, 0); // Ingen signal på kanal 3
    }
    else
    {
        // Vid negativ throttle: sätt kanal 2 till 0 och beräkna pulsbredd för kanal 3 utifrån absolutvärdet
        timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_2, 0);
        timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_3, (-throttel / 100.0) * 16000);
    }
}
