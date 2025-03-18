#include "pwm.h"

// Funktion för att initialisera Timer1 för PWM på utvalda kanaler.
// Parametern 'ch' är en bitmask där varje bit representerar en kanal:
// 0x1: Kanal 0 (PA0), 0x2: Kanal 1 (PA1), 0x4: Kanal 2 (PA2), 0x8: Kanal 3 (PA3)
void T1powerUpInitPWM(uint16_t ch)
{
    // Strukturer för timerinställningar:
    timer_oc_parameter_struct timer_ocinitpara;
    timer_parameter_struct timer_initpara;

    // Aktivera klockan för GPIOA och alternativ funktion (nödvändigt för att använda PWM-funktionalitet på stiften)
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);

    // Initiera de relevanta GPIO-stiften i alternativ funktion (push-pull) med hög hastighet (50MHz)
    if (ch & 0x1)
        gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
    if (ch & 0x2)
        gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    if (ch & 0x4)
        gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
    if (ch & 0x8)
        gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);

    // Aktivera klockan för Timer1
    rcu_periph_clock_enable(RCU_TIMER1);

    // Återställ Timer1 till standardläge för att säkerställa en ren start
    timer_deinit(TIMER1);
    // Initiera timerparametrar med standardvärden
    timer_struct_para_init(&timer_initpara);

    // Konfigurera timerinställningar:
    timer_initpara.prescaler = 107;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = 15999;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1, &timer_initpara);

    // Initiera strukturen för timerkanalens utgångsinställningar med standardvärden
    timer_channel_output_struct_para_init(&timer_ocinitpara);
    timer_ocinitpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocinitpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocinitpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocinitpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    // Konfigurera Timer1:s utgångskanaler baserat på bitmasken 'ch'
    if (ch & 0x1)
        timer_channel_output_config(TIMER1, TIMER_CH_0, &timer_ocinitpara);
    if (ch & 0x2)
        timer_channel_output_config(TIMER1, TIMER_CH_1, &timer_ocinitpara);
    if (ch & 0x4)
        timer_channel_output_config(TIMER1, TIMER_CH_2, &timer_ocinitpara);
    if (ch & 0x8)
        timer_channel_output_config(TIMER1, TIMER_CH_3, &timer_ocinitpara);

    // För varje vald kanal, konfigurera pulsbredd (initial duty cycle), PWM-läge och shadow-inställning
    if (ch & 0x1)
    {
        timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, 0);                  // Sätt initial pulsbredd till 0
        timer_channel_output_mode_config(TIMER1, TIMER_CH_0, TIMER_OC_MODE_PWM0);        // Välj PWM-läge (PWM0)
        timer_channel_output_shadow_config(TIMER1, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE); // Inaktivera shadow-funktionen
    }
    if (ch & 0x2)
    {
        timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_1, 0);
        timer_channel_output_mode_config(TIMER1, TIMER_CH_1, TIMER_OC_MODE_PWM0);
        timer_channel_output_shadow_config(TIMER1, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);
    }
    if (ch & 0x4)
    {
        timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_2, 11999);
        timer_channel_output_mode_config(TIMER1, TIMER_CH_2, TIMER_OC_MODE_PWM0);
        timer_channel_output_shadow_config(TIMER1, TIMER_CH_2, TIMER_OC_SHADOW_DISABLE);
    }
    if (ch & 0x8)
    {
        timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_3, 11999);
        timer_channel_output_mode_config(TIMER1, TIMER_CH_3, TIMER_OC_MODE_PWM0);
        timer_channel_output_shadow_config(TIMER1, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);
    }

    // Aktivera auto-reload shadow-funktion så att nya auto-reload värden laddas vid uppdateringshändelsen
    timer_auto_reload_shadow_enable(TIMER1);
    // Starta Timer1 så att den börjar generera PWM-signaler
    timer_enable(TIMER1);
}

// Funktion för att ändra pulsbredden (duty cycle) för kanal 2
void T1setPWMch2(int value)
{
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_2, value);
}

// Funktion för att styra PWM-signalerna för motor B.
// Parametern 'throttel' anges i procent (0-100) och omvandlas till en pulsbredd som motsvarar timerns upplösning
void T1setPWMmotorB(int throttel)
{
    // Omvandla 'throttel' från procent till pulsbredd (baserat på att 100% motsvarar 16000 steg)
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_1, (throttel / 100.0) * 16000);
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, (throttel / 100.0) * 16000);
}
