#include "../Inc/usb_driver.h"

void USB_Init()
{
    SetPinsUSB();

    RCC->APB1ENR |= RCC_APB1ENR_USBEN;

    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
    NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 1); //Не знаю, насколько актуально
   
    USB_Reset();
}

void USB_Reset()
{
    //Указываем на местоположение BTABLE (таблица дескрипторов буферов эндпоинтов) относительно начала PMA
    USB->BTABLE = 0x00;

    //Ниже заносим в BTABLE адреса буферов эндпоинтов. На каждый эндопинт по 16*4 = 64 = 8 байт. Макс размер BTABLE = 64 байта = 0x3f
    //Поэтому первый буфер эндпоинта 0 будет лежать после BTABLE по адресу 0x40 (относительно начала PMA при нулевом смещении BTABLE)

    //EP0
    PMA[1*0] = 0x40;    //TxADR   - адрес буфера передачи. Размер - 8 байт (64 бита)
    PMA[1*1] = 0;       //TxCOUNT - количество байт для передачи      
    PMA[1*2] = 0x80;    //RxCount - адрес буфера приема. Размер - 8 байт (64 бита)
    PMA[1*3] = 0x840;   //См. стр. 649 и таблицу 177 в RM0008

    //EP1, 2, 3 и так далее по тому же принципу
}

//Настройка пинов PA11 и PA12 в AFIO floating input - для работы с USB
void SetPinsUSB()
{
    RCC->APB1ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB1ENR |= RCC_APB2ENR_AFIOEN;

    //Сброс конфигурации пинов
    GPIOA->CRH &= ~(GPIO_CRH_MODE11 | GPIO_CRH_CNF11 | GPIO_CRH_MODE12 | GPIO_CRH_CNF12); 
    GPIOA->CRH |=  (GPIO_CRH_CNF11_1 | GPIO_CRH_CNF12_1); // Floating input --- Не факт что пины должны быть настроены именно так ---
}



void USB_LP_CAN1_RX0_IRQHandler()
{
    uint16_t istr = USB->ISTR; //InterruptStatusRegister

    if(istr & USB_ISTR_RESET)
    {
        USB->ISTR &= ~USB_ISTR_RESET; //Снимаем флаг при отработке прерывания USB_RESET
        USB_Reset();
    }
    else if(istr & USB_ISTR_CTR)
    {
        //что-то с эндпоинтами
    }
}