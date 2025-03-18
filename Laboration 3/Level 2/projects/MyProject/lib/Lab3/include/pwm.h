#include "gd32vf103.h"

// Initierar Timer1 för att generera PWM-signaler.
// Parametern 'ch' är en bitmask som specificerar vilka kanaler (ex. kanal 0, 1, 2, 3) som ska konfigureras för PWM.
void T1powerUpInitPWM(uint16_t ch);

// Ställer in pulsbredden för kanal 2 på Timer1, vilket påverkar PWM-signalens duty cycle.
// Parametern 'value' bestämmer pulsbredden (antal räknarenheter).
void T1setPWMch2(int value);

// Konfigurerar PWM-signalerna för motor B baserat på ett throttle-värde.
// Om 'throttel' är positivt appliceras ett PWM-värde på en kanal (ex. framåtriktad rotation),
// medan ett negativt värde applicerar PWM på en annan kanal (ex. bakåtriktad rotation).
void T1setPWMmotorB(int throttel);