#include "mcc_generated_files/mcc.h"
#include "ff.h"
//#include "lcd_lib.h"
#include "Errores.h"

FATFS FatFs;
FIL Fil;

void __delay_sec(char sec) {
    for(char i=0;i<=(50*sec);i++) {
        __delay_ms(20);
    }
}

void guardar(){
    UINT bw;   
    SYSTEM_Initialize();
    if (f_mount(&FatFs, "", 1) != FR_OK) {	/* Inicializa SD */
        Error(1);
        __delay_sec(2);
        Error(99);
        while(f_mount(&FatFs, "", 1) != FR_OK) {
            ;
        }
    }    
    
    Error(2);
    __delay_sec(2);
    Error(99);
        
    if (f_open(&Fil, "BeeDev.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK) {	/* Abre o crea el archivo */
        Error(3);
        
       
        __delay_sec(2);
         Error(99);
            
			if ((Fil.fsize != 0) && (f_lseek(&Fil, Fil.fsize) != FR_OK)) goto endSD;	/* Salta al final del archivo */
                Error(4);
                __delay_sec(2);
                
                Error(99);
                f_write(&Fil, "CREADO POR MI :V .\r\n", 46, &bw);	/* ]Escribe en el archivo */
                Error(5);
                __delay_sec(2);
                 Error(99);
                endSD: f_close(&Fil);								/* Cierra el archivo */
                Error(6);
                __delay_sec(1);
                
                Error(7);
                __delay_sec(2);
                 Error(99);
     
	}
    else {
        Error(11);
    }
}

void main(void)
{
    
    LATA = 0x00;
    TRISA = 0x00;
    ANSELA = 0x00;
    
    guardar();

    while (1)
    {
        Error(1);
        __delay_sec(5);
        Error(15);
        __delay_sec(2);
	 //asm("reset"); //Reset Programado, no se para que ponerlo pero por si las moscas aqui esta :V
    }
}
/**
 End of File
*/
