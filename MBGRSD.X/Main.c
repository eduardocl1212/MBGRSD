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

    int a = 0;
    int out;
    char ND[2];

char data[2] = "da";
FATFS FatFs;
FIL Fil;
int i = -5000;


void Init8LEDs(void);           //Función Inicializa puerto A
int interruptadc(int,int);
void grabador(void);
void reproductor(void);
void guardar(void);
void __delay_sec(char sec);


void main(void)
{

    SYSTEM_Initialize();
    Init8LEDs();        //Inicializa puerto A
      
    OSCCON = 0x53;      //Oscilador interno 4 MHz
    ADCON2 = 0x94;      //ACQT = 4TAD, TAD de 1microS, Justificado derecha 
    ADCON1 = 0x00;      //Vref- = GND, Vref+ = +5V
    LATA = 0x00;        //Limpiamos A
    TRISA = 0x00;       //bit RA1 como salida 
    ANSELA = 0x00;      //RA1 como analógico
    TRISC = 0x04;       //bit RC2 como entrada
    ANSELC = 0x04;      //RC2 como analógico
    ADCON0 = 0x39;      //Enciendo ADC, canal AN14
    GIE = 1;            //Habilita interrupciones globales
    PEIE = 1;           //Habilita interrupciones de periféricos
    ADIE = 1;           //Habilita interrupción ADC
       guardar();
    while(1){
        Error(55);
    }
    return;
}

int interruptadc(int adcin, int a) {                //Interrupción conversión                       
    if(ADIF==1) {                     //verifica bandera conversión ADC
        ADIE=0;                       //Deshabilita interrupción ADC
        adcin = (ADRESH<<8)+ADRESL;   //Asigna valor ADRESH y ADRESL a adcin
    }
    ADIE=1;                           //Habilita interrupción ADC
    ADIF=0; //Limpia bandera conversión ADC
    //data[a] = adcin/4;
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
                   //Inicia conversión ADC
    do{
        i++;        
        ADCON0bits.GO=1;
        adcin = interruptadc(adcin, i);
        PORTA = adcin/4;            //Asigna valor adcin a puerto A
        volt = (adcin*_vin)/_base;  //Conversión a flotante
        ND[0] = (char) adcin;
        //ND[1] = (char) adcin;
        f_write(&Fil, ND , 2 , &bw);
        }while(i!=5000);
       return; 
}

void reproductor(void){
        
        //Display7Seg(volt);          //Llama a función desplegar en displays
        //LA1=out;
         //IO_RA1_SetLow();
}


void __delay_sec(char sec) {
    for(char i=0;i<=(50*sec);i++) {
        __delay_ms(20);
    }
}

void guardar(void){
    Error(5);
    if (f_mount(&FatFs, "", 1) != FR_OK) {	/* Inicializa SD */
          
        while(f_mount(&FatFs, "", 1) != FR_OK) {
            ;
        }
    }    
    
    Error(1);

        
    if (f_open(&Fil, "BeeDev.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK) {	/* Abre o crea el archivo */

            
			if ((Fil.fsize != 0) && (f_lseek(&Fil, Fil.fsize) != FR_OK)) goto endSD;	/* Salta al final del archivo */
                
                grabador();
                endSD: f_close(&Fil);	
                Error(55);/* Cierra el archivo */
	}
    else {
        Error(0);
    }
}