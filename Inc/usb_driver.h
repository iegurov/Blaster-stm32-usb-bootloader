#include "stm32f1xx.h"
#include <stdint.h>

#define PMA ((volatile uint16_t*) USB_PMAADDR)

void USB_Init();
void USB_Reset();
void SetPinsUSB();