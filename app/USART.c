/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 13.04.2018
Author  : 
Company : 
Comments: 


Chip type               : ATmega8
Program type            : Application
AVR Core Clock frequency: 8,000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

#include <mega8.h>
#include <delay.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

// Declare your global variables here

#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)
#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<DOR)

// USART Receiver buffer
#define RX_BUFFER_SIZE 28
char rx_buffer[RX_BUFFER_SIZE];

#if RX_BUFFER_SIZE <= 256
unsigned char rx_wr_index=0,rx_rd_index=0;
#else
unsigned int rx_wr_index=0,rx_rd_index=0;
#endif

#if RX_BUFFER_SIZE < 256
unsigned char rx_counter=0;
#else
unsigned int rx_counter=0;
#endif

// This flag is set on USART Receiver buffer overflow
bit rx_buffer_overflow;

// USART Receiver interrupt service routine
interrupt [USART_RXC] void usart_rx_isr(void){
    char status,data;
    status=UCSRA;
    data=UDR;
    if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
       {
       rx_buffer[rx_wr_index++]=data;
    #if RX_BUFFER_SIZE == 256
       // special case for receiver buffer size=256
       if (++rx_counter == 0) rx_buffer_overflow=1;
    #else
       if (rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
       if (++rx_counter == RX_BUFFER_SIZE)
          {
          rx_counter=0;
          rx_buffer_overflow=1;
          }
    #endif
       }
}

#ifndef _DEBUG_TERMINAL_IO_
// Get a character from the USART Receiver buffer
#define _ALTERNATE_GETCHAR_
#pragma used+
char getchar(void){
    char data;
    while (rx_counter==0);
    data=rx_buffer[rx_rd_index++];
    #if RX_BUFFER_SIZE != 256
    if (rx_rd_index == RX_BUFFER_SIZE) rx_rd_index=0;
    #endif
    #asm("cli")
    --rx_counter;
    #asm("sei")
    return data;
}
#pragma used-
#endif

// USART Transmitter buffer
#define TX_BUFFER_SIZE 28
char tx_buffer[TX_BUFFER_SIZE];

#if TX_BUFFER_SIZE <= 256
unsigned char tx_wr_index=0,tx_rd_index=0;
#else
unsigned int tx_wr_index=0,tx_rd_index=0;
#endif

#if TX_BUFFER_SIZE < 256
unsigned char tx_counter=0;
#else
unsigned int tx_counter=0;
#endif

// USART Transmitter interrupt service routine
interrupt [USART_TXC] void usart_tx_isr(void){
    if (tx_counter)
       {
       --tx_counter;
       UDR=tx_buffer[tx_rd_index++];
    #if TX_BUFFER_SIZE != 256
       if (tx_rd_index == TX_BUFFER_SIZE) tx_rd_index=0;
    #endif
       }
}

#ifndef _DEBUG_TERMINAL_IO_
// Write a character to the USART Transmitter buffer
#define _ALTERNATE_PUTCHAR_
#pragma used+
void putchar(char c){
    while (tx_counter == TX_BUFFER_SIZE);
    #asm("cli")
    if (tx_counter || ((UCSRA & DATA_REGISTER_EMPTY)==0))
       {
       tx_buffer[tx_wr_index++]=c;
    #if TX_BUFFER_SIZE != 256
       if (tx_wr_index == TX_BUFFER_SIZE) tx_wr_index=0;
    #endif
       ++tx_counter;
       }
    else
       UDR=c;
    #asm("sei")
}
#pragma used-
#endif

// Standard Input/Output functions
#include <stdio.h>


void write_float(float f){
    unsigned char *ptr;
    char j;
    ptr = (unsigned char *)&f;
    for (j=0;j<4;j++){
        while(!(UCSRA&(1<<UDRE))){};
        UDR = *(ptr+j);
    }
}


void send_UART(char value) {
    while(!( UCSRA & (1 << UDRE)));   // Ожидаем когда очистится буфер передачи
    UDR = value; // Помещаем данные в буфер, начинаем передачу
}

void print_float(float f){
    char mystr[16];
    double floatNum;
    int i;    
    sprintf(mystr, "%s", floatNum);
    for(i = 0; i < 16; i++){
        while(!(UCSRA&(1<<UDRE))){};
       UDR = mystr[i];
    }
    
}


void main(void)
{
// Declare your local variables here
int _counter = 0;
char _data;
int pocket_len = 28;
char pocket_data[pocket_len];
unsigned char pitch_hex[4];
float pitch_float;


float *point_a;
float a;

//  Рабочий способ преобразования 4 байт во float.
/*float *point_a;
float a;
unsigned char buf_float[4] = {0,0,0x60,0x40};
point_a = &a;
memcpy(point_a,buf_float,4);
write_float(a);*/


        
// Input/Output Ports initialization
// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (0<<DDB0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

// Port C initialization
// Function: Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRC=(0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
// State: Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTC=(0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
TCCR0=(0<<CS02) | (0<<CS01) | (0<<CS00);
TCNT0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer1 Stopped
// Mode: Normal top=0xFFFF
// OC1A output: Disconnected
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer2 Stopped
// Mode: Normal top=0xFF
// OC2 output: Disconnected
ASSR=0<<AS2;
TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (0<<CS21) | (0<<CS20);
TCNT2=0x00;
OCR2=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<TOIE0);

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);

// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud Rate: 115200
UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (0<<U2X) | (0<<MPCM);
UCSRB=(1<<RXCIE) | (1<<TXCIE) | (0<<UDRIE) | (1<<RXEN) | (1<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
UCSRC=(1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCPOL);
UBRRH=0x00;
UBRRL=0x03;

// Analog Comparator initialization
// Analog Comparator: Off
// The Analog Comparator's positive input is
// connected to the AIN0 pin
// The Analog Comparator's negative input is
// connected to the AIN1 pin
ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);
SFIOR=(0<<ACME);

// ADC initialization
// ADC disabled
ADCSRA=(0<<ADEN) | (0<<ADSC) | (0<<ADFR) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (0<<ADPS1) | (0<<ADPS0);

// SPI initialization
// SPI disabled
SPCR=(0<<SPIE) | (0<<SPE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);

// TWI initialization
// TWI disabled
TWCR=(0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE);

// Global enable interrupts
#asm("sei")

while (1)
      {  
       _data = getchar();
      // Place your code here 
        //putchar(getchar());
        if(_data == 255){
           while(_counter < 28){
                pocket_data[_counter] = getchar();     //   Формируем пакет данных.
                //putchar( pocket_data[_counter]);    //  Пересылаем данные raspberry pi.
                _counter++; 
                
                if(pocket_data[_counter] == 0){
                    //write_float(pocket_data[_counter]);
                
                }
                if(_counter == 5){
                     //UDR = pocket_data[0];
                }
                 
           }
           _counter = 0;
           pitch_hex[0] = pocket_data[21];  
           pitch_hex[1] = pocket_data[22]; 
           pitch_hex[2] = pocket_data[23]; 
           pitch_hex[3] = pocket_data[24]; 
                
           
            
            point_a = &a;
            memcpy(point_a,pitch_hex,4);
            write_float(floor(a));
            
            a = 0; 
           
             /*
             04 C4 39 7F 
             D8 3B D5 99 
             03 3B E3 78
             63 3B 85 08
            1F 3C E2 C0 F0 BB 79 88 C7 3C 0B 84 88 3C FE 79
            16 BD 86 F4 58 BB 10 66 A5 BB 79 F7 A3 BB 49 6A
            6B 3C B0 12 

             */
           
           
            
            //x=(float *)pitch_hex; 
            //UDR = pocket_data[1] + pocket_data[2];
            //  Функция выводит float число в терминал в 16-тиричном формате в инверсном виде.
            //  Перевести можно в калькуляторе:  https://gregstoll.dyndns.org/~gregstoll/floattohex/
            //write_float(25.365); 
            
            //send_UART(x);
           
          
           
        }
        
        /*for(_counter = 0; _counter < 28; _counter++){
             putchar( pocket_data[_counter]);
        }*/ 
        
        /*
        strcpy(pitch_hex, &pocket_data[0]);
        strcpy(pitch_hex, &pocket_data[1]);
        strcpy(pitch_hex, &pocket_data[2]);
        strcpy(pitch_hex, &pocket_data[3]);
        */
        //printf(pocket_data[0]);
        
        /*
        _data = getchar();
        if(_data == 0xFF && _readingComplete == 1){  //  Находим преамбулу. 
            _readingComplete = 0; 
            pocket_data[0] =  _data; // Включаем преамбулу в пакет.
            while(_counter < 27){ 
                pocket_data[_counter] = getchar();  //  Формируем полный пакет.
                putchar(getchar()); 
                _counter++; 
                _readingComplete = 1;
            }
            _counter = 1;  //   Обнуляем счетчик данных в пакете.
            putchar(0);
        }  */ 
         //  Достаем необходимые данных из пакета.

        // 3C25C43D == 0.0101

        //send_UART(12.23);
         
        //uint8_t *pocket_data = (uint8_t *)&pitch_float;
        
        //strcat(pitch_hex, (pocket_data[24])); 
        //pitch_hex=strcat(pocket_data[24]); 
        //pitch_float = typecast(uint32(hex2dec(pitch_hex)), 'single');
        
             
            
             
             //putchar(getchar());
             
             //delay_ms(200);
      }
}
