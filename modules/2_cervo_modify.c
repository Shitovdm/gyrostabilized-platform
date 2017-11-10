#include <stdio.h>
#include <delay.h>
#include <mega8.h>
#include <io.h>

void main() {
    char mode=0; // ����� - �� ��������� 0
    int direct=0;

    #define ICR_MAX 1000000/50  // ICR1(TOP) = fclk/(N*f) ; N-��������; f-�������;  1000000/1/50 = 20000
    #define OCR_MIN 1100
    #define OCR_MAX 1900
    #define OCR_CENTER (OCR_MIN+OCR_MAX)/2

    // �� ����� ������ �������� ������������� ��������
    DDRB &= ~(1<<4);
    PORTB |= (1<<4);

    // ����������� ���� �����������
    DDRC = (1<<1) | (1<<2) | (1<<3);
    PORTC |= (1<<(mode+1));
  
    // ����������� PWM �� ������ 1 (����� �� ����� PB1, PB2)
    TCCR1A = 0; // ��������� PWM ���� ����� ���������������
    ICR1 = ICR_MAX; // ������� ������ 50 ��

    // �������� Fast PWM mode via ICR1 �� ������� 1 ��� �������� �������
    TCCR1A = (1<<WGM11);
    TCCR1B = (1<<WGM13) | (1<<WGM12) | (1<<CS10);

    // ������������ PB1 � PB2 ��� �����
    DDRB |= (1<<1) | (1<<2);
    
    // �������� PWM �� port B1 � B2
    TCCR1A |= (1<<COM1A1) | (1<<COM1B1);
    

    while(1){
        // ���� ������ ������ (������������ �������)
        if ((PINB & (1<<4)) == 0) 
        {
            // ���� ���� ������ ��������
            while ((PINB & (1<<4)) == 0) {
                delay_ms(200);
            }

            mode++;
            if (mode > 2) {
                mode = 0;
            }

            // �������� ������ ���������
            PORTC &= ~((1<<1) | (1<<2) | (1<<3));
            PORTC |= (1<<(mode+1));
        }

        switch (mode){
        case 0:{
                // ������ �������� ��������������, � ����������� �� ��������� ������������� adc_result
                //OCR1A = OCR_MIN+(512 * (OCR_MAX-OCR_MIN)/1024); 
                OCR1A =  OCR_MIN; 
                OCR1B = OCR1A;
                break;
            }
        case 1:{
                // ������ ����������� ��������� ��������������
                direct=0;
                OCR1A = OCR_CENTER;
                OCR1B = OCR1A;
                break;
            }
        case 2:{
                // ����������� ��������� ��������� ������������
                if (direct==0){
                    OCR1A++;
                    OCR1B++;
                    if (OCR1A >= OCR_MAX) {
                        direct=1;
                    }
                }
                if (direct==1){
                    OCR1A--;
                    OCR1B--;
                    if (OCR1A <= OCR_MIN) {
                        direct=0;
                    }
                }
                delay_ms(1);
                break;
            }
        }
    }
}