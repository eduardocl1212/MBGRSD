#include "mcc_generated_files/mcc.h"
#include "ff.h"
//#include "lcd_lib.h"
#include "Errores.h"

char data[];
FATFS FatFs;
FIL Fil;

void __delay_sec(char sec) {
    for(char i=0;i<=(50*sec);i++) {
        __delay_ms(20);
    }
}

void guardar(char* data){
    UINT bw;   
    SYSTEM_Initialize();
    if (f_mount(&FatFs, "", 1) != FR_OK) {	/* Inicializa SD */
        
        Error(33);
        __delay_sec(2);
        while(f_mount(&FatFs, "", 1) != FR_OK) {
            ;
        }
    }    
    
    Error(1);
    __delay_sec(2);
        
    if (f_open(&Fil, "BeeDev.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK) {	/* Abre o crea el archivo */
        Error(3);
        
       
        __delay_sec(2);
         Error(0);
            
			if ((Fil.fsize != 0) && (f_lseek(&Fil, Fil.fsize) != FR_OK)) goto endSD;	/* Salta al final del archivo */
                Error(4);
                __delay_sec(2);
                
                f_write(&Fil, data , 28 , &bw);	// DATA ARRAY, NUMERO DE CHAR
                //f_printf(&fil, "%s", "String");  
                Error(5);
                __delay_sec(2);
                endSD: f_close(&Fil);								/* Cierra el archivo */
                Error(6);
                __delay_sec(1);
                
                Error(7);
                __delay_sec(2);
     
	}
    else {
        Error(9);
    }
}

void main(void)
{
    
   
    guardar("Prueba por ARRAY.\r\n");
    __delay_sec(5);
    while (1)
    {
	 //asm("reset"); //Reset Programado, no se para que ponerlo pero por si las moscas aqui esta :V
    }
}
/**
 End of File
*/
