#include <stdio.h>
#include <mega8.h>
#include <lcd.h>
#include <delay.h>
#include <io.h>
#include <interrupt.h>
#asm
   .equ __lcd_port=0x18; PORTD /* ЖКИ дисплей подключили к порту I/O D */
#endasm

int i;
int lng;
char mystr[]="Atmega8 UART ready!";
char rc;
 unsigned char temp = 123;
            unsigned char string[20];
void lampOFF()
{
    lcd_gotoxy(0,3);
    lcd_putsf("Fuck off");
}

void lampON()
{
    lcd_gotoxy(0,3);
    
    lcd_putsf("suck");
}

void initUART()
{
    UCSRB|=(1<<3)|(1<<4);   
    UBRRH=0x00;
    UBRRL=0x33;
}

void main(void)
{

    initUART();

    //lng=strlen(mystr);

    for (i=0; i < 19; i++)    {
        while ( !( UCSRA & (1<<5)) ) {}
        UDR=mystr[i];
    }
    
    lcd_init(16);
    lcd_clear();
    lcd_gotoxy(0,0);
    lcd_putsf("LCD 16x04");
    lcd_gotoxy(0,1);
    lcd_putsf("Gurostable");
    lcd_gotoxy(0,2);
    lcd_putsf("platform");
    
     
    while (1)
    {
        if ((UCSRA & (1<<7)))
        {
            rc=UDR; 

            
            sprintf(string, "temp = %05d\n", temp);
            lcd_puts(string);
            /*if(rc==255){   
                lcd_gotoxy(0,3);
                lcd_putsf("Preambula.");
            }*/
        }                     

        switch (rc)
        {
            case '1': lampON(); break;
            case '0': lampOFF(); break;
        }

    }

}
