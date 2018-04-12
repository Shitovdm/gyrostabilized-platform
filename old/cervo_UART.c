#include <stdio.h>
#include <delay.h>
#include <mega8.h>
#include <io.h>
#include <interrupt.h>
#include <string.h>

#define CYCLE 5000
#define coefficient 400/90 //   Коэффициент преобразования значения ширины импульса. 
//  400 - разница между значением ширины импульса центрольного положения сервы и максимально отклоненным(90 град).
//  90 - максимальный угор отклонения сервопривода.

#define LEN 28  //  Максимальная длина пакета байт.

char buffer[LEN];   //  Массив принимаемых данных.
volatile unsigned char done;
volatile unsigned char IDX = 0;


void USART_Transmit( unsigned char data );
void USART_Init( unsigned int ubrr);

void USART_Init( unsigned int ubrr)//Инициализация модуля USART
{
    /* Задаем скорость работы USART */	
    UBRRH = 0;	
    UBRRL = 51  ;
    /* Разрешаем прием и передачу по USART */
    UCSRB=(1<<RXEN)|( 1<<TXEN);
    UCSRB |= (1<<RXCIE);
 
    //UCSRB=(1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
    /* Устанавливаем формат данных 8 бит данных, 2 стоп бита */
    UCSRC=0x86;// Инициализация именно для ATmega8
    UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}
 
void USART_Transmit( unsigned char data ) //Функция отправки данных
{
    while ( !(UCSRA & (1<<UDRE)) ); //Ожидание опустошения буфера приема
        UDR = data; //Начало передачи данных			        
}

 
volatile int pitch = 0;
volatile int roll = 0;

volatile int takt = 0;
int i = 0; 
void timer1_init() {
    OCR1A = 20000;
    TCCR1A = 0;
    TCCR1B |= (1 << WGM12);
}
 
void timer1_start() {
    TCNT1 = 0;
    TCCR1B |= (1<<CS10);
    TIMSK |= (1 << OCIE1A);
}
 
void timer1_stop() {
    TCCR1B &= 0b11111000;
    TIMSK &= !(1 << OCIE1A);
}

int toPulseWidth(int grad){
    //  Соответствие градусной мере поворота:
    //  -30 grad = 1366 ms
    int pulse_width = 0;   //   Ширина импульса.
    if(grad > 30) grad = 30;    //  Предел поворота платформы.
    if(grad < -30) grad = -30;   //  Предел поворота платформы.
    pulse_width =  (1500 + (grad * coefficient))*8;
    return pulse_width;
}

void initUART()
{
    UCSRB|=(1<<3)|(1<<4);   
    UBRRH=0x00;
    UBRRL=0x33;
}
 
void main () {
int pitch_grad = 0;
int roll_grad = 0;



timer1_init();
timer1_start();
DDRB  = 0b00000111;
PORTB = 0;



USART_Init (8);
 
/*USART_Transmit('O');//Передаем при включении
USART_Transmit('k');//сообщение "Ok!", что свидетельствует
USART_Transmit('!');//о правильно работе программы
 
USART_Transmit(0x0d);//переход в начало строки
USART_Transmit(0x0a);//переход на новую строку */



while (1){
    //Основной цикл в котором меняются значения тангажа и крена.
    //  Предел по крену и тангажу +-30 град.
    pitch_grad = -25;  
    roll_grad = 18;
    pitch = toPulseWidth(pitch_grad);
    roll = toPulseWidth(roll_grad); 
    sei();
    if(done == 1){ 
        for(i = 0; i < 4; i++){
            USART_Transmit(buffer[i]);
            //USART_Transmit('k');
            //USART_Transmit(0x0d); 
            //USART_Transmit(0x0a); 
            buffer[i] = 0xff;
        } 
    
        done = 0;
        //USART_Transmit(0x0d);//переход в начало строки.
        //USART_Transmit(0x0a);//переход на новую строку.
    }
     
    
    
    
    
    //delay_ms(1000);   
}}
ISR(TIM1_COMPA, ISR_NOBLOCK) {

    if(done == 0){
        char bf = UDR;
	    //buffer[IDX] = bf; 
        buffer[IDX] = 0xFF;
	    IDX++;
    
        if (IDX >= LEN){
		    IDX = 0;
		    done = 1;
	    }
    }
    



    // ПЕРВЫЙ ПОРТ. pitch
    if (takt == 0) {
        PORTB |= (1<<0);
        OCR1A = pitch;
    }
    if (takt == 1) {
        PORTB &= ~(1<<0);
        OCR1A = CYCLE - pitch;
    } 
    
    // ВТОРОЙ ПОРТ. roll
    if (takt == 3) {
        PORTB |= (1<<1);
        OCR1A = roll;}
    if (takt == 4) {
        PORTB &= ~(1<<1);
        OCR1A = CYCLE - roll;
    }  
    
    takt++;
    if (takt == 5) takt = 0;
}; 
