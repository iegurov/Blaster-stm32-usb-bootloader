#include "stm32f1xx.h"
#include <stdint.h>
#include "../Inc/usb_driver.h"

/*

    Тестовая версия USB-загрузчика
    В будущем лучше выделить некоторые функции в отдельную общую библиотеку для STM32F1
    Некоторые функции сделать static

    //TIP: PA11 - DM, PA12 - DP
*/

void Set_Clock48MHz()
{
    //Включаем HSE - (High Speed External) - внешний кварц на 8МГц в Bluepill
    RCC->CR |= RCC_CR_HSEON; 

    //Ждем, пока HSE стабилизируется (Присутствует емкость)
    while(!(RCC->CR & RCC_CR_HSERDY)); 

    //Включаем префетч-буфер. МК будет "угадывать" какие команды будут вызваны, тем самым ускорится доступ к памяти
    FLASH->ACR |= FLASH_ACR_PRFTBE; 
    
    //Задержка в 1 такт при обращении к памяти для частоты >=48МГц
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |= FLASH_ACR_LATENCY_1; 

    //Настройка тактирования шин
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;    //AHB  - 48МГц
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;   //APB1 - 24МГц - максимум 36МГц
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;   //APB2 - 48МГц
    
    //Конфигурируем умножитель частоты PLL (HSE*6 = 8*6 = 48МГц)
    RCC->CFGR |= RCC_CFGR_PLLSRC;
    RCC->CFGR &= ~RCC_CFGR_PLLMULL;
    RCC->CFGR |= RCC_CFGR_PLLMULL6;

    //Включаем умножитель частоты PLL
    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY));
    
    //Устанавливаем PLL как источник тактирования
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SW_PLL);
}

int main(void)
{
    Set_Clock48MHz();
    USB_Init();

    while(1);

    return 0;
}