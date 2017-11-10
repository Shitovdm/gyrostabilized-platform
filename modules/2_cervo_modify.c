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
    
    /*** ��������� ��� ***/
ADCSRA |= (1 << ADEN)|(1 << ADPS1)|(1 << ADPS0);    // ������������ ��������������� �� 8
ADMUX |= (0 << REFS1)|(0 << REFS0)|(0 << MUX0)|(0 << MUX1)|(0 << MUX2)|(0 << MUX3); // ���� PC0
    DDRD = 0xFF;
    PORTD = 0x00;

    while(1){
        // ������ � ���.
        unsigned int u;
        ADCSRA |= (1 << ADSC);    // �������� ��������������
        while ((ADCSRA&(1 << ADIF))== 0){ // ���� ����� ��������� ��������������   
          
        u = (ADCL|ADCH << 8); // ���������  ADC
                  
        if (u > 128) // 0.625V
        PORTD = 0b00000001;         
        else
        PORTD = 0b00000000;
        if (u > 256) // 1.25V
             PORTD = 0b00000011;    
        if (u > 384) // 1.875V 
             PORTD = 0b00000111;
        if (u > 512) // 2.5V
             PORTD = 0b00001111;
        if (u > 640) // 3.125V
             PORTD = 0b00011111;
        if (u > 768) // 3.75V
             PORTD = 0b00111111;
        if (u > 896) // 4.375V
             PORTD = 0b01111111;
        if (u > 1020) // 5V
             PORTD = 0b11111111;   
        delay_ms(30);
        }
        
          
        // ��������� ������� ������ (������������ �������)
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
        
        // ������ ������ �������������.
        switch (mode){
        case 0:{
                // ������ �������� ��������������, � ����������� �� ��������� ������������� adc_result
                //OCR1A = OCR_MIN+(512 * (OCR_MAX-OCR_MIN)/1024); 
                OCR1A =  1100+(0.78*u); 
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
