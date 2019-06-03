/*
 * File:   main.c
 * Author: big_m
 *
 * Created on 25 de mayo de 2019, 06:32 PM
 */


#include "mcc_generated_files/mcc.h"
#include "ff.h"
#include "Errores.h"
#include "init.h"

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

void initSD(void);
int interruptadc(int);
void grabadorSD(void);
void reproductorSD(void);
void guardado(void);
void __delay_sec(char sec);
void lectura(void);


void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    
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
   
    //initSD(); //Inicializa MicroSD
    while(1){
       Error(0);
       push =  PORTC;
       push = 0x03 & push;
       if(push == 2) guardado();   //RC1 GUARDA 
       if(push == 1) lectura(); //RC0 REPRODUCE
    }
    
    return;
}

int interruptadc(int adcin) {               //Comienza conversion ANALOGICA DIGITAL                    
    if(ADIF==1) {                     
        ADIE=0;                       
        adcin = (ADRESH<<8)+ADRESL;   
    }
    ADIE=1;                           
    ADIF=0; 
    return adcin;
}

void grabadorSD(void){
    Error(9); //Inicia Grabacion
    int j = 0;
    int a = 0;    
        j = 0;
        do{            
            ADCON0bits.GO=1;
            a = ADRESH; 
            ND[0] = (char) a;
            ND[1] = (char) a;
            f_write(&Fil, ND , 2 , &bw);
            //send_string(ND);
            j++;
        }while(j!=30000);   
         Error(4);
    return; 
}

void reproductorSD(void){
    Error(8); //Inicia conversión ADC
    int j = 0;
    char c;    
    do{
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
        }while(j!=30000);
        Error(4);
    return; 
}


void __delay_sec(char sec) {
    for(char i=0;i<=(50*sec);i++) {
        __delay_ms(20);
    }
}

void initSD(void){
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
    	}
    else {
        Error(0);
    }
            
}

void guardado(void){
    initSD();
    if ((Fil.fsize != 0) && (f_lseek(&Fil, Fil.fsize) != FR_OK))	/* Salta al final del archivo */
    Error(3);
    __delay_sec(1);
    //Error(999);
    grabadorSD();	// DATA ARRAY, NUMERO DE CHAR
    //f_printf(&fil, "%s", "String");  
    Error(4);
    __delay_sec(1);
    Error(5);
    __delay_sec(1);
    f_close(&Fil);								/* Cierra el archivo */
    Error(6);
    __delay_sec(1);


}

void lectura(void){
    initSD();
    reproductorSD();	// DATA ARRAY, NUMERO DE CHAR
    //f_printf(&fil, "%s", "String");  
    Error(5);
    __delay_sec(1);
    f_close(&Fil);								/* Cierra el archivo */
    Error(6);
    __delay_sec(1);
    LATA= 0x00;
}