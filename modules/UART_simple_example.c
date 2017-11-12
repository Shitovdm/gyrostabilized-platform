#include <stdio.h>
#include <delay.h>
#include <mega8.h>
#include <io.h>
#include <interrupt.h>

#define F_CPU 8000000 // Рабочая частота контроллера
#define BAUD 9600L // Скорость обмена данными
#define UBRRL_value (F_CPU/(BAUD*16))-1 //Согластно заданной скорости подсчитываем значение для регистра UBRR

void init_USART() {
    UBRRL = UBRRL_value;       //Младшие 8 бит UBRRL_value
    UBRRH = UBRRL_value >> 8;  //Старшие 8 бит UBRRL_value
    UCSRB |=(1<<TXEN);         //Бит разрешения передачи
    UCSRC |=(1<< URSEL)|(1<< UCSZ0)|(1<< UCSZ1); //Устанавливем формат 8 бит данных
}

void send_UART(char value) {
    while(!( UCSRA & (1 << UDRE)));   // Ожидаем когда очистится буфер передачи
    UDR = value; // Помещаем данные в буфер, начинаем передачу
}

void main(void)
{
    init_USART();    //инициализация USART в режиме 9600/8-N-1
    while(1)
    {    
        send_UART(0x53); //посылаем ASCII код знака 'S'
        send_UART(0x2D); //посылаем ASCII код знака '-'
        send_UART(0x45); //посылаем ASCII код знака 'E'
        delay_ms(1000);
    }
}