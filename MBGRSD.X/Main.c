#include "mcc_generated_files/mcc.h"
#include "ff.h"
#include "lcd_lib.h"

FATFS FatFs;
FIL Fil;

void __delay_sec(char sec) {
    for(char i=0;i<=(50*sec);i++) {
        __delay_ms(20);
    }
}

/*
                         Main application
 */
void main(void)
{
    
    LATA = 0x00;
    TRISA = 0x00;
    ANSELA = 0x00;
    UINT bw;
   
    // Initialize the device
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable high priority global interrupts
    //INTERRUPT_GlobalInterruptHighEnable();

    // Enable low priority global interrupts.
    //INTERRUPT_GlobalInterruptLowEnable();

    // Disable high priority global interrupts
    //INTERRUPT_GlobalInterruptHighDisable();

    // Disable low priority global interrupts.
    //INTERRUPT_GlobalInterruptLowDisable();

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    Lcd_Init();
    Lcd_Clear();
    
    if (f_mount(&FatFs, "", 1) != FR_OK) {	/* Inicializa SD */
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("Inicio: ERROR");
        LATAbits.LATA0 ^= 1;
        LATAbits.LATA1 ^= 1;
        LATAbits.LATA2 ^= 1;
        LATAbits.LATA3 ^= 1;
        LATAbits.LATA4 ^= 1;
        LATAbits.LATA5 ^= 1;
        LATAbits.LATA6 ^= 1;
        LATAbits.LATA7 ^= 1;
        __delay_sec(2);
        
        while(f_mount(&FatFs, "", 1) != FR_OK) {
            ;
        }
    }    
    
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
         LATAbits.LATA0 ^= 0;
        LATAbits.LATA1 ^= 0;
        LATAbits.LATA2 ^= 0;
        LATAbits.LATA3 ^= 0;
        LATAbits.LATA4 ^= 0;
        LATAbits.LATA5 ^= 0;
        LATAbits.LATA6 ^= 0;
        LATAbits.LATA7 ^= 1;
    Lcd_Write_String("SD INICIALIZADA");
    __delay_sec(2);
        
    if (f_open(&Fil, "BeeDev.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK) {	/* Abre o crea el archivo */
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("Archivo CREADO");
        LATAbits.LATA1 ^= 1;
        __delay_sec(2);
            
			if ((Fil.fsize != 0) && (f_lseek(&Fil, Fil.fsize) != FR_OK)) goto endSD;	/* Salta al final del archivo */
                Lcd_Set_Cursor(1,1);
                LATAbits.LATA1 ^= 0;
                Lcd_Write_String("Archivo ABIERTO");
                __delay_sec(2);
                
                f_write(&Fil, "Este archivo TXT fue creado desde BeeDev_UG.\r\n", 46, &bw);	/* ]Escribe en el archivo */
                Lcd_Set_Cursor(1,1);
                LATAbits.LATA2 ^= 1;
                Lcd_Write_String("Archivo EDITADO");
                __delay_sec(2);
                
                endSD: f_close(&Fil);								/* Cierra el archivo */
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String("Archivo GUARDADO");
                LATAbits.LATA2 ^= 0;
                Lcd_Set_Cursor(2,1);
                Lcd_Write_String("Extraer SD");
                LATAbits.LATA3 ^= 1;
                __delay_sec(2);
     
	}
    else {
       Lcd_Clear();
       Lcd_Set_Cursor(1,1);
       Lcd_Write_String("Archivo: ERROR");
       LATAbits.LATA1 ^= 1;
       LATAbits.LATA2 ^= 1;
       LATAbits.LATA3 ^= 1;
    }
    
    while (1)
    {
        // Add your application code
    }
}
/**
 End of File
*/