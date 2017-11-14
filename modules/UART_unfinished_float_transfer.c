/*
*  ����������� ������� ������ � ��������� float �����.
*  ��������, �� ������� �����.
*
*/
#include <stdio.h>
#include <delay.h>
#include <mega8.h>
#include <io.h>
#include <interrupt.h>
#include <string.h>

#define F_CPU 8000000 // ������� ������� �����������
#define BAUD 9600L // �������� ������ �������
#define UBRRL_value (F_CPU/(BAUD*16))-1 //��������� �������� �������� ������������ �������� ��� �������� UBRR
const unsigned char MAX_STRING = 32;
register unsigned char i = 0;
int k;
float angel1,angel2,angel3 = 1500;
char *str;

float our_float = 25.36;
// ������� ������ UART.
void writeSerial(char* str){
    for(i=0;(i < MAX_STRING && str[i]!=':'); i++)
    {
        while(!(UCSRA&(1<<UDRE))){}; // wait ready of port
        UDR = str[i];
    }
}

// ������� ���������� 4� �������� float; ����� float � uart ��������.
void write_float(float f){
    unsigned char *ptr;
    char j;
    ptr = (unsigned char *)&f;
    for (j=0;j<4;j++){
        while(!(UCSRA&(1<<UDRE))){};
        UDR = *(ptr+j);
    }
}
// ������� ������ UART.
char readSerial(char* str){
    i=0;
    do {
        while(!(UCSRA&(1<<RXC))) {};
        str[i] = UDR;
        
        i++;
    } while (str[i-1] != ' ' && i <MAX_STRING);
    return  *str;  
}

void main(void){
    DDRB  = 0b00000111;
    PORTB = 0;

    UBRRL=UBRRL_value;
    UCSRB=(1<<TXEN)|(1<<RXEN);
    UCSRC=(1<<URSEL)|(3<<UCSZ0);
        while(1){
            char data[MAX_STRING];
            //*str = readSerial(data); // ��������� ����������� ������.
            //writeSerial(data);
            write_float(0.233);
            //*ptr = *ptr+1;
            //writeSerial(*our_float);
            if(*str == '0'){
                 PORTB |= (1<<0);
            }else{
                 PORTB &= ~(1<<0);
            }
            
            delay_ms(1000);
            for(k=0; k < MAX_STRING; k++) data[k]=0; // ������� ������.  
        }
       
}