/*
 * File:   main.c
 * Author: big_m
 *
 * Created on 25 de mayo de 2019, 06:32 PM
 */

// CONFIG1L
#pragma config PLLSEL = PLL4X   // PLL Selection (4x clock multiplier)
#pragma config CFGPLLEN = OFF   // PLL Enable Configuration bit (PLL Disabled (firmware controlled))
#pragma config CPUDIV = NOCLKDIV// CPU System Clock Postscaler (CPU uses system clock (no divide))
#pragma config LS48MHZ = SYS24X4// Low Speed USB mode with 48 MHz system clock (System clock at 24 MHz, USB clock divider is set to 4)

// CONFIG1H
#pragma config FOSC = INTOSCIO  // Oscillator Selection (Internal oscillator)
#pragma config PCLKEN = ON     // Primary Oscillator Shutdown (Primary oscillator shutdown firmware controlled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF      // Internal/External Oscillator Switchover (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config nPWRTEN = OFF    // Power-up Timer Enable (Power up timer disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable (BOR enabled in hardware (SBOREN is ignored))
#pragma config BORV = 190       // Brown-out Reset Voltage (BOR set to 1.9V nominal)
#pragma config nLPBOR = OFF     // Low-Power Brown-out Reset (Low-Power Brown-out Reset disabled)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (WDT disabled in hardware (SWDTEN ignored))
#pragma config WDTPS = 32768    // Watchdog Timer Postscaler (1:32768)

// CONFIG3H
#pragma config CCP2MX = RC1     // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF      // PORTB A/D Enable bit (PORTB<5:0> pins are configured as analog input channels on Reset)
#pragma config T3CMX = RC0      // Timer3 Clock Input MUX bit (T3CKI function is on RC0)
#pragma config SDOMX = RB3      // SDO Output MUX bit (SDO function is on RB3)
#pragma config MCLRE = ON       // Master Clear Reset Pin Enable (MCLR pin enabled; RE3 input disabled)

// CONFIG4L
#pragma config STVREN = ON   // Stack Full/Underflow Reset (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled if MCLRE is also 1)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port Enable (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled)

// CONFIG5L
#pragma config CP0 = OFF        // Block 0 Code Protect (Block 0 is not code-protected)
#pragma config CP1 = OFF        // Block 1 Code Protect (Block 1 is not code-protected)
#pragma config CP2 = OFF        // Block 2 Code Protect (Block 2 is not code-protected)
#pragma config CP3 = OFF        // Block 3 Code Protect (Block 3 is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protect (Boot block is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protect (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Block 0 Write Protect (Block 0 (0800-1FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Block 1 Write Protect (Block 1 (2000-3FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Block 2 Write Protect (Block 2 (04000-5FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Block 3 Write Protect (Block 3 (06000-7FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Registers Write Protect (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protect (Boot block (0000-7FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protect (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Block 0 Table Read Protect (Block 0 is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Block 1 Table Read Protect (Block 1 is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Block 2 Table Read Protect (Block 2 is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Block 3 Table Read Protect (Block 3 is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protect (Boot block is not protected from table reads executed in other blocks)


#include "mcc_generated_files/mcc.h"
#include "ff.h"
#include "Errores.h"

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>


#define _mSec 1
#define _vin 5.0
#define _base 1023.0

float volt;
unsigned int adcin;

UINT bw;
UINT br;
int a = 0;
char ND[2];
int b = 0;
int push;

char data;
void send_string(const char *x){
    while(*x){
        __delay_ms(1);
        EUSART1_Write(*x++);
    }
}
FATFS FatFs;
FIL Fil;

void Init8LEDs(void);           //Función Inicializa puerto A
int interruptadc(int);
void grabador(void);
void reproductor(void);
void guardar(void);
void __delay_sec(char sec);
void reproducir(void);


void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    
    Init8LEDs();        //Inicializa puerto A
      
    OSCCON = 0x53;      //Oscilador interno 4 MHz
    ADCON2 = 0x14;      //ACQT = 4TAD, TAD de 1microS, Justificado izquierda 
    ADCON1 = 0x00;      //Vref- = GND, Vref+ = +5V
    LATA = 0x00;        //Limpiamos A
    TRISA = 0x00;       //bit RA1 como salida 
    ANSELA = 0x00;      //RA1 como analógico
    TRISC = 0x07;       //bit RC2, RC1, RC0 como entrada
    ANSELC = 0x04;      //RC2 como analógico
    ADCON0 = 0x39;      //Enciendo ADC, canal AN14
    
    GIE = 1;            //Habilita interrupciones globales
    PEIE = 1;           //Habilita interrupciones de periféricos
    ADIE = 1;           //Habilita interrupción ADC
   
        
    while(1){
       Error(0);
       push =  PORTC;
       push = 0x03 & push;
       if(push == 2) guardar();
       if(push == 1) reproducir();
    }
  
    return;
}

int interruptadc(int adcin) {                //Interrupción conversión                       
    if(ADIF==1) {                     //verifica bandera conversión ADC
        ADIE=0;                       //Deshabilita interrupción ADC
        adcin = (ADRESH<<8)+ADRESL;   //Asigna valor ADRESH y ADRESL a adcin
    }
    ADIE=1;                           //Habilita interrupción ADC
    ADIF=0; //Limpia bandera conversión ADC
    return adcin;
}


void Init8LEDs(void)
{
    LATA = 0x00;
    TRISA = 0x00;       //Todos como salida digital
    ANSELA = 0x00;
    
    return;
}

void grabador(void){
    Error(4); //Inicia conversión ADC
    int i = 0;
    int j = 0;
    int a = 0;
    
    //while(i!=256){
        j = 0;
        while(j!=30000){
            
            ADCON0bits.GO=1;
            //adcin = interruptadc(adcin);
            a = ADRESH; 
            //PORTA = a;          //Asigna valor adcin a puerto A
            ND[0] = (char) a;
            ND[1] = (char) a;
            f_write(&Fil, ND , 2 , &bw);
            //send_string(ND);
            j++;
        }
        
       // i++;
    //}
    return; 
}

void reproductor(void){
    Error(4); //Inicia conversión ADC
    int j = 0;
    char c;
    
    //while(i!=256){
        j = 0;
        while(j!=30000){

            f_read(&Fil, ND , 2 , &br);
            c = ND[0];
            b = (int) c;
            PORTA = b;
            for(int i=0;i<3;i++);
            c = ND[1];
            b = (int) c;
            PORTA = b;
            for(int i=0;i<2;i++);
            j++;
        }
        
       // i++;
    //}
    return; 
}


void __delay_sec(char sec) {
    for(char i=0;i<=(50*sec);i++) {
        __delay_ms(20);
    }
}

void guardar(void){
    SYSTEM_Initialize();
    if (f_mount(&FatFs, "", 1) != FR_OK) {	/* Inicializa SD */
        Error(33);
        __delay_sec(1);
        while(f_mount(&FatFs, "", 1) != FR_OK) {
            ;
        }
    }    
    
    Error(1);
    __delay_sec(1);
        
    if (f_open(&Fil, "BeeDev.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK) {	/* Abre o crea el archivo */
        Error(2);
        __delay_sec(1);

        if ((Fil.fsize != 0) && (f_lseek(&Fil, Fil.fsize) != FR_OK)) goto endSD;	/* Salta al final del archivo */
            Error(3);
            __delay_sec(1);
            grabador();	// DATA ARRAY, NUMERO DE CHAR
            //f_printf(&fil, "%s", "String");  
            Error(5);
            __delay_sec(1);
            endSD: f_close(&Fil);								/* Cierra el archivo */
            Error(6);
            __delay_sec(1);

	}
    else {
        Error(0);
    }
}

void reproducir(void){
    SYSTEM_Initialize();
    if (f_mount(&FatFs, "", 1) != FR_OK) {	/* Inicializa SD */
        Error(33);
        __delay_sec(1);
        while(f_mount(&FatFs, "", 1) != FR_OK) {
            ;
        }
    }    
    
    Error(1);
    __delay_sec(1);
        
    if (f_open(&Fil, "BeeDev.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK) {	/* Abre o crea el archivo */
        Error(2);
        __delay_sec(1);

            reproductor();	// DATA ARRAY, NUMERO DE CHAR
            //f_printf(&fil, "%s", "String");  
            Error(5);
            __delay_sec(1);
            endSD: f_close(&Fil);								/* Cierra el archivo */
            Error(6);
            __delay_sec(1);

	}
    else {
        Error(0);
    }
}