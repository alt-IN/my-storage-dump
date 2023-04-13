/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced

Project :
Version :
Date    : 30.12.2020
Author  :
Company :
Comments:


Chip type               : ATmega8
Program type            : Application
AVR Core Clock frequency: 1,000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

#include <mega8.h>
#include <stdlib.h>         // itoa
#include <delay.h>
#include <stdio.h>
#include <1wire.h>          //библиотека для работы с 1wire
#include <ds18b20.h>        //библиотека для работы с ds18b20
#include <alcd.h>           // Alphanumeric LCD functions
#include <sleep.h>
#include <eeprom.h>

#define PDJ 1
#define AKB 2
#define ZUM 3
#define DSP 1

#asm
.equ __w1_port=0x18 ;PORTB          // сообщаем куда подключен датчик
.equ __w1_bit=0;
#endasm

// Declare your global variables here
char lcd_buffer[11]="0123456789";
long adc_data = 0;
unsigned int adc_counter=0;
unsigned int display = 1210;
unsigned int Vmin;
unsigned char O[]={128,79,84,75,167,176,171,69,72,165,69,32,65,75,160,0}; /* [0] "ОТКЛЮЧЕНИЕ АКБ" */
unsigned char Mes0[]={128,32,32,79,120,112,97,189,97,32,66,75,167,32,32,32,0}; /* [0] "Охрана ВКЛ." */
int i=0;
int tempc=0;
eeprom int _Vmin = 1070;

// Declare your function here

void sleep_mk(void)
{
lcd_clear();
delay_us(200);
PORTC &= ~(1<<DSP); // выключение подсветки дисплея
PORTB &= ~(1<<AKB); // выход на реле отключения АКБ
PORTB &= ~(1<<ZUM); //выключение зуммера
//Команда сон процессора
sleep_enable();
powerdown();
}

void lcd_v(void)
{
lcd_gotoxy(0,1);
lcd_puts("Vmin=");
lcd_putchar (lcd_buffer[Vmin/1000]);
lcd_putchar (lcd_buffer[Vmin/100%10]);
lcd_putchar('.');
lcd_putchar (lcd_buffer[Vmin/10%10]);
//lcd_putchar (lcd_buffer[Vmin%10]);
//lcd_putchar ('B');//disp;
lcd_puts("  ");
}

// External Interrupt 0 service routine
interrupt [EXT_INT0] void ext_int0_isr(void)
{
// Place your code here
lcd_clear();
lcd_puts(Mes0);  /* [0] "Охрана ВКЛ." */
delay_ms(3000);
GICR |= (1<<INT1);       // Разрешаем внешние прерывания по вх.1
GICR &= ~(1<<INT0);       // Запрещаем внешние прерывания по вх.0
sleep_mk();
}

// External Interrupt 1 service routine
interrupt [EXT_INT1] void ext_int1_isr(void)
{
// Place your code here
PORTB |= (1<<PDJ); // вкл. пейджера
delay_ms(3000);
PORTB &= ~(1<<PDJ); // выкл. пейджера
sleep_mk();
}

                    // Timer 0 overflow interrupt service routine
                    interrupt [TIM0_OVF] void timer0_ovf_isr(void)
                    {
                    // Reinitialize Timer 0 value
                    TCNT0=0xDB;
                    // Place your code here

                    }

// Timer1 overflow interrupt service routine
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
// Reinitialize Timer1 value
TCNT1H=0x48E5 >> 8;
TCNT1L=0x48E5 & 0xff;
// Place your code here
ADCSR &= ~(1<<ADIE);   // запрещаем прерывание АЦП
    if (i<16)
    {
        i++;
        PORTB |= (1<<ZUM); //включение зуммера
        delay_ms(1000);
        PORTB &= ~(1<<ZUM); //выключение зуммера
    }
    else
    {
        eeprom_update_word(&_Vmin, Vmin); //запись в переменную
        GICR |= (1<<INT0);   // Разрешаем внешние прерывания по вх.0
        sleep_mk();
    };
ADCSR |= (1<<ADIE);  // разрешаем прерывание АЦП
}

                    // Timer2 overflow interrupt service routine
                    interrupt [TIM2_OVF] void timer2_ovf_isr(void)
                    {
                    // Reinitialize Timer2 value
                    TCNT2=0x64;
                    // Place your code here
                    }


// Voltage Reference: Int., cap. on AREF
#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))

// ADC interrupt service routine
interrupt [ADC_INT] void adc_isr(void)
{
//unsigned int adc_data;
// Read the AD conversion result
adc_data+=ADCW;

// Place your code here
adc_counter++; // Увеличиваем счетчик измерений
}



void main(void)
{
// Declare your local variables here
unsigned char z=0;
unsigned char butcount=0;
Vmin = _Vmin;
//Vmin = 1100;
// Input/Output Ports initialization
// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=Out Bit2=Out Bit1=Out Bit0=In
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (1<<DDB3) | (1<<DDB2) | (1<<DDB1) | (0<<DDB0);
// State: Bit7=P Bit6=P Bit5=T Bit4=T Bit3=0 Bit2=P Bit1=0 Bit0=T
PORTB=(1<<PORTB7) | (1<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (1<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

// Port C initialization
// Function: Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=Out Bit0=In
DDRC=(0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (1<<DDC1) | (0<<DDC0);
// State: Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T
PORTC=(1<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (1<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In
DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (1<<PORTD3) | (1<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

                            // Timer/Counter 0 initialization
                            // Clock source: System Clock
                            // Clock value: 125,000 kHz
                            TCCR0=(0<<CS02) | (1<<CS01) | (0<<CS00);
                            TCNT0=0xDB;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 0,977 kHz
// Mode: Normal top=0xFFFF
// OC1A output: Disconnected
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer Period: 20 s
// Timer1 Overflow Interrupt: On
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10);
TCNT1H=0x48;
TCNT1L=0xE5;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
//TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<TOIE0);

                            // Timer/Counter 2 initialization
                            // Clock source: System Clock
                            // Clock value: 7,813 kHz
                            // Mode: Normal top=0xFF
                            // OC2 output: Disconnected
                            // Timer Period: 19,968 ms
                            ASSR=0<<AS2;
                            TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (1<<CS22) | (0<<CS21) | (1<<CS20);
                            TCNT2=0x64;
                            OCR2=0x00;

// External Interrupt(s) initialization
// INT0: Off
// INT0 Mode: Low level
// INT1: Off
// INT1 Mode: Low level
//GICR|=(1<<INT1) | (1<<INT0);
MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
GIFR=(1<<INTF1) | (1<<INTF0);

// ADC initialization
// ADC Clock frequency: 125,000 kHz
// ADC Voltage Reference: Int., cap. on AREF
ADMUX=ADC_VREF_TYPE;
ADCSR=(1<<ADEN) | (1<<ADSC) | (1<<ADFR) | (0<<ADIF) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0);
SFIOR=(0<<ACME);

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTD Bit 5
// RD - PORTD Bit 1
// EN - PORTD Bit 0
// D4 - PORTС Bit 2
// D5 - PORTС Bit 3
// D6 - PORTС Bit 4
// D7 - PORTС Bit 5
// Characters/line: 16
lcd_init(16);

// Global enable interrupts
#asm("sei")

    while (1)
    {
        if (PINB.5==1)  //Проверка выключателя
        {
             /* Обработка нажатия кнопок */

             if (PINB.6==0)         //Если нажата кнопка "-"
             {
                if (butcount<5) {butcount++;}
                else
                {
                    ADCSR &= ~(1<<ADIE);   // запрещаем прерывание АЦП
                    do
                    {
                       if (Vmin>1000)
                       {
                           Vmin = Vmin - 10;
                       }
                       lcd_v();
                       delay_ms(500);
                    }
                    while (PINB.6==0);
                    ADCSR |= (1<<ADIE);  // разрешаем прерывание АЦП
                };
             }
             else
             {
                if (butcount>=0) {butcount--;}
             };
             if (PINB.7==0)         //Если нажата кнопка "+"
             {
                if (butcount<5) {butcount++;}
                else
                {
                    ADCSR &= ~(1<<ADIE);   // запрещаем прерывание АЦП
                    do
                    {
                       if (Vmin<1180)
                       {
                           Vmin = Vmin + 10;
                       };
                       lcd_v();
                       delay_ms(500);
                    }
                    while (PINB.7==0);
                    ADCSR |= (1<<ADIE);  // разрешаем прерывание АЦП
                };
             }
             else
             {
                if (butcount>=0) {butcount--;}
             };

             /* Замер напряжения АКБ */

             if(adc_counter == 250) // Если количество измерений АЦП равно 250
             {
                 display = (((adc_data*5*6*100)/1024)/adc_counter); // Вычисляем среднее значение АЦП

                 /* Сравнение текущего напряжения с минимальным */

                 if (display <= Vmin)
                 {
                    TIMSK |= (1<<TOIE1);  // разрешаем прерывание по счетчику 1

                    //lcd_clear();
                    lcd_gotoxy(0,0);
                    lcd_puts(O);
                    lcd_gotoxy(0,1);
                    lcd_puts("                ");
                    lcd_gotoxy(0,1); // Перемещаем курсор по первой строке

                    for (z=1; z<=i; z++)
                    {
                        lcd_putchar(0xFF); // Выводим на экран прямоугольник
                        //z++; // Увеличиваем значение счетчика на единицу
                        //delay_ms(500); // Пауза в 100 мс, чтобы мы успевали замечать перемещение прямоугольника
                        // Счетчик остановится когда мы пройдемся по всем ячейкам строки
                    }


                 }
                 else
                 {
                    TIMSK &= ~(1<<TOIE1);  // запрещаем прерывание по счетчику 1
                    i=0;

                    lcd_gotoxy(0,0);
                    lcd_puts("                ");
                    lcd_gotoxy(0,0);
                    lcd_puts("Vakb=");
                    //sprintf(lcd_buffer,"%0.2f",fdisplay);
                    if(display/1000==0)
                    {
                        lcd_putchar(' ');
                    }
                    else
                    {
                        lcd_putchar (lcd_buffer[display/1000]);
                        lcd_putchar (lcd_buffer[display/100%10]);
                        lcd_putchar('.');
                        lcd_putchar (lcd_buffer[display/10%10]);
                        lcd_putchar (lcd_buffer[display%10]);
                        lcd_v();
                    };

                    /* Температурный датчик */

                    ADCSR &= ~(1<<ADIE);   // запрещаем прерывание АЦП
                    if (w1_init()==0)
                    {
                        lcd_gotoxy(11,1);
                        lcd_puts("Error");
                    }
                    else
                    {
                        w1_write(0xCC);
                        w1_write(0x44);
                            //{
                               //lcd_gotoxy(11,1);
                               //lcd_puts("Error");
                            //}
                            //else
                            //{
                            //delay_us(5);
                        tempc = ds18b20_temperature(0);
                        if(tempc<0)
                        {
                            tempc=-tempc;
                            lcd_gotoxy(11,1);
                            lcd_putchar('-');
                        }
                        else
                        {
                            lcd_gotoxy(11,1);
                            lcd_putchar(' ');
                        };
                            /*if (tempc>1000)
                             { // если датчик выдаёт больше 1000
                                tempc=4096-tempc;
                                tempc=-tempc;
                                lcd_gotoxy(5,1);
                                lcd_putchar('-');
                             }

                                itoa(tempc, lcd_buffer_c);
                                lcd_gotoxy(11,1);
                                lcd_puts(lcd_buffer_c); */
                        lcd_gotoxy(12,1);
                        if(tempc/10==0)
                        lcd_putchar(' ');
                        lcd_putchar (lcd_buffer[tempc/10]);
                        lcd_putchar (lcd_buffer[tempc%10]);
                        lcd_gotoxy(14,1);
                        lcd_puts("\xdf\x43");     //вывод "*С"
                    };


                 };

             adc_counter = 0; // Обнуляем счетчик измерений
             adc_data = 0; // Обнуляем буфер АЦП
             ADCSR |= (1<<ADIE);  // разрешаем прерывание АЦП

             };
        }
        else
        {
            eeprom_update_word(&_Vmin, Vmin); //запись в переменную
            GICR |= (1<<INT0);   // Разрешаем внешние прерывания по вх.0
            sleep_mk();
            /*
            //Команда сон процессора
            MCUCR = 0xA0;     //sleep_enable();
            #asm("sleep");    //powerdown();
            */
        };
    }
}