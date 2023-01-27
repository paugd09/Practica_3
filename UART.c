#include "lib/include.h"

extern void Configurar_UART2(void)
{
    SYSCTL->RCGCUART |= (1<<2);   //Paso 1 (RCGCUART) pag.344 UART/modulo0 0->Disable 1->Enable
    SYSCTL->RCGCGPIO |= (1<<3);     //Paso 2 (RCGCGPIO) pag.340 Enable clock port A
    //(GPIOAFSEL) pag.671 Enable alternate function
    GPIOD-> LOCK= 0X4C4F434B;
    GPIOD-> CR = 0XC0; 
    GPIOD->AFSEL = (1<<6) | (1<<7);
    //GPIO Port Control (GPIOPCTL) PA0-> U0Rx PA1-> U0Tx pag.688
    GPIOD->PCTL = (GPIOD->PCTL&0xFFFFFF00) | 0x11000000;// (1<<0) | (1<<4);//0x00000011
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOD->DEN = (1<<6) | (1<<7);//PA1 PA0
    //UART0 UART Control (UARTCTL) pag.918 DISABLE!!
    UART2->CTL = (0<<0) | (0<<8) | (0<<09);
    

    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    BRD = 25,000,000 / (16*57600) = 27.1267
    UARTFBRD[DIVFRAC] = integer(.1267 * 64 + 0.5)
    */
    UART2->IBRD = 54;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART2->FBRD = 17;
    //  UART Line Control (UARTLCRH) pag.916
    UART2->LCRH = (0x3<<5)|(1<<4);
    //  UART Clock Configuration(UARTCC) pag.939
    UART2->CC =(0<<0);
    //Disable UART0 UART Control (UARTCTL) pag.918
    UART2->CTL = (1<<0) | (1<<8) | (1<<9);


extern void UART_2_CONFIG (void){
    //Inicializacion del UART
    // UART2 los PIN D6 Y D7 

    SYSCTL -> RCGCUART |= 0X4;
    SYSCTL -> RCGCGPIO |= 0x8;
    GPIOD -> LOCK = 0x4C4F434B;
    GPIOD -> CR = 0xC0; // los bit 6 y 7
    GPIOD -> AFSEL = (1 << 6) | (1 << 7);
    GPIOD -> PTCL |= (GPIOD -> PCTL & 0x00FFFFFF) | 0x11000000;
    GPIOD -> DEN =(1 << 6) | (1 << 7);
     //
    //
    UART2 -> CTL = (0 << 0 ) | (0 << 8) | ( 0 << 9);
    //Porcion entera de BRD
    UART2 -> IBRD =21;
    //Porcion fracionaria de BRD
    UART2 -> FBRD =45;
    //
    UART2 -> LCRH =(0x3 << 5) | (1<<4);
    UART2 -> CC = 0; 
    UART2 -> CTLC = (1 << 0) | (1 << 8) | (1 << 9);
    
}

}

extern char readChar(void)
{
    //UART FR flag pag 911
    //UART DR data 906
    int v;
    char c;
    while((UART2->FR & (1<<4)) != 0 );
    v = UART2->DR & 0xFF;
    c = v;
    return c;
}
extern void printChar(char c)
{
    while((UART2->FR & (1<<5)) != 0 );
    UART2->DR = c;
}
extern void printString(char* string)
{
    while(*string)
    {
        printChar(*(string++));
    }
}

extern char * readString(char delimitador)
{

   int i=0;
   char *string = (char *)calloc(10,sizeof(char));
   char c = readChar();
   while(c != delimitador)
   {
       *(string+i) = c;
       i++;
       if(i%10==0)
       {
           string = realloc(string,(i+10)*sizeof(char));
       }
       c = readChar();
   }

   return string;

}
extern void invertirString (char *string, int i)
{
    char aux;
   for (int j= 0; j < i/2; j++){
        aux = string [j];
        string[j]=string[i- 1 -j];
        string[i- 1 -j]=aux;
    }
}
extern void int number (char *string, char *num, int i){
    int i =0;
    int p =0;
    while (1<(i* 2)){
        num[1]=string[p];
        i++;
        num[1]=( unsigned char )(49 +p);
        p++;
        i++;
    }
   string = num;
}
//Experimento 2

//El envio es su nombre  (rave) 

// invertirlo y regresarlo con numeros consecutivos
// entre letras (e1v2a3r) 

