#include "LibCHAPAJAS.h"

#define FREQ_INTERRUPT_US 500L
#define FREQ_INTERRUPT_MS (FREQ_INTERRUPT_US / 1000.0)
#define TIMER_DELAY_MS 50UL    // Delai entre les mesures et ajustements


void timer_init();
