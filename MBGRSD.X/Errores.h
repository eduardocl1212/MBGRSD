/* 
 * File:   Errores.h
 * Author: eduar
 *
 * Created on 15 de mayo de 2019, 04:47 PM
 */

#ifndef ERRORES_H
#define	ERRORES_H

void Error(char a){
    if (a == 1){         //Error Inicializar
        LATAbits.LATA0 ^= 1;
        LATAbits.LATA1 ^= 1;
        LATAbits.LATA2 ^= 1;
        LATAbits.LATA3 ^= 1;
        LATAbits.LATA4 ^= 1;
        LATAbits.LATA5 ^= 1;
        LATAbits.LATA6 ^= 1;
        LATAbits.LATA7 ^= 1;
    }
    if (a == 2){        //SD INIT
        LATAbits.LATA0 ^= 0;
        LATAbits.LATA1 ^= 0;
        LATAbits.LATA2 ^= 0;
        LATAbits.LATA3 ^= 0;
        LATAbits.LATA4 ^= 0;
        LATAbits.LATA5 ^= 0;
        LATAbits.LATA6 ^= 0;
        LATAbits.LATA7 ^= 1;
    }
    if (a == 3){        //Archivo Creado
        LATAbits.LATA0 ^= 0;
        LATAbits.LATA1 ^= 0;
        LATAbits.LATA2 ^= 0;
        LATAbits.LATA3 ^= 0;
        LATAbits.LATA4 ^= 0;
        LATAbits.LATA5 ^= 0;
        LATAbits.LATA6 ^= 1;
        LATAbits.LATA7 ^= 1;
    }
    if (a == 4){        //Archivo Abierto
        LATAbits.LATA0 ^= 0;
        LATAbits.LATA1 ^= 0;
        LATAbits.LATA2 ^= 0;
        LATAbits.LATA3 ^= 0;
        LATAbits.LATA4 ^= 0;
        LATAbits.LATA5 ^= 1;
        LATAbits.LATA6 ^= 1;
        LATAbits.LATA7 ^= 1;
    }
    if (a == 5){        //Archivo Editado
        LATAbits.LATA0 ^= 0;
        LATAbits.LATA1 ^= 0;
        LATAbits.LATA2 ^= 0;
        LATAbits.LATA3 ^= 0;
        LATAbits.LATA4 ^= 1;
        LATAbits.LATA5 ^= 1;
        LATAbits.LATA6 ^= 1;
        LATAbits.LATA7 ^= 1;
    }
    if (a == 6){        //Archivo Guardado
        LATAbits.LATA0 ^= 0;
        LATAbits.LATA1 ^= 0;
        LATAbits.LATA2 ^= 0;
        LATAbits.LATA3 ^= 1;
        LATAbits.LATA4 ^= 1;
        LATAbits.LATA5 ^= 1;
        LATAbits.LATA6 ^= 1;
        LATAbits.LATA7 ^= 1;
    }
    if (a == 7){        //Extraer SD
        LATAbits.LATA0 ^= 0;
        LATAbits.LATA1 ^= 0;
        LATAbits.LATA2 ^= 1;
        LATAbits.LATA3 ^= 1;
        LATAbits.LATA4 ^= 1;
        LATAbits.LATA5 ^= 1;
        LATAbits.LATA6 ^= 1;
        LATAbits.LATA7 ^= 1;
    }
    if (a == 11){
        LATAbits.LATA0 ^= 0;
        LATAbits.LATA1 ^= 1;
        LATAbits.LATA2 ^= 0;
        LATAbits.LATA3 ^= 0;
        LATAbits.LATA4 ^= 0;
        LATAbits.LATA5 ^= 1;
        LATAbits.LATA6 ^= 0;
        LATAbits.LATA7 ^= 1;
    }
    if(a == 15) { //EN ESPERA    
        LATAbits.LATA0 ^= 0;
        LATAbits.LATA1 ^= 0;
        LATAbits.LATA2 ^= 0;
        LATAbits.LATA3 ^= 0;
        LATAbits.LATA4 ^= 0;
        LATAbits.LATA5 ^= 0;
        LATAbits.LATA6 ^= 0;
        LATAbits.LATA7 ^= 0;
    
    }
}


#endif