#include <stdio.h>
#include <delay.h>
#include <mega8.h>
#include <io.h>
#include <interrupt.h>

#define CYCLE 5000
 
volatile int angle1 = 1100;
volatile int angle2 = 1100;
volatile int angle3 = 1100;

volatile int takt = 0;
 
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
 
void main () {
sei();
timer1_init();
timer1_start();
DDRB  = 0b00000111;
PORTB = 0;
while (1){
    //Îñíîâíîé öèêë â êîòîðîì ìåíÿþòñÿ çíà÷åíèÿ angle1, angle2, angle3, angle4  
    while(angle1<1900){
        angle1 = angle1+5;
        angle2 = angle1+2;
        delay_ms(100);
    }
    angle1 = 1100;
    angle2 = 1100;
}}
ISR(TIM1_COMPA, ISR_NOBLOCK) {
    // ÏÅÐÂÛÉ ÏÎÐÒ.
    if (takt == 0) {
        PORTB |= (1<<0);
        OCR1A = angle1;
    }
    if (takt == 1) {
        PORTB &= ~(1<<0);
        OCR1A = CYCLE - angle1;
    } 
    
    // ÂÒÎÐÎÉ ÏÎÐÒ.
    if (takt == 3) {
        PORTB |= (1<<1);
        OCR1A = angle2;}
    if (takt == 4) {
        PORTB &= ~(1<<1);
        OCR1A = CYCLE - angle2;
    }  
    
    // ÒÐÅÒÈÉ ÏÎÐÒ.
    if (takt == 5) {
        PORTB |= (1<<2);
        OCR1A = angle3;}
    if (takt == 6) {
        PORTB &= ~(1<<2);
        OCR1A = CYCLE - angle3;
    }
    takt++;
    if (takt == 7) takt = 0;
}; 
