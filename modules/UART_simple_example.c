#include <stdio.h>
#include <delay.h>
#include <mega8.h>
#include <io.h>
#include <interrupt.h>

#define F_CPU 8000000 // ������� ������� �����������
#define BAUD 9600L // �������� ������ �������
#define UBRRL_value (F_CPU/(BAUD*16))-1 //��������� �������� �������� ������������ �������� ��� �������� UBRR

void init_USART() {
    UBRRL = UBRRL_value;       //������� 8 ��� UBRRL_value
    UBRRH = UBRRL_value >> 8;  //������� 8 ��� UBRRL_value
    UCSRB |=(1<<TXEN);         //��� ���������� ��������
    UCSRC |=(1<< URSEL)|(1<< UCSZ0)|(1<< UCSZ1); //������������ ������ 8 ��� ������
}

void send_UART(char value) {
    while(!( UCSRA & (1 << UDRE)));   // ������� ����� ��������� ����� ��������
    UDR = value; // �������� ������ � �����, �������� ��������
}

void main(void)
{
    init_USART();    //������������� USART � ������ 9600/8-N-1
    while(1)
    {    
        send_UART(0x53); //�������� ASCII ��� ����� 'S'
        send_UART(0x2D); //�������� ASCII ��� ����� '-'
        send_UART(0x45); //�������� ASCII ��� ����� 'E'
        delay_ms(1000);
    }
}