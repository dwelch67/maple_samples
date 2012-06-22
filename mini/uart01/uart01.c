//http://dfu-util.gnumonks.org/releases/
//dfu-util -c 1 -i 0 -a 1 -D myprogram.bin

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );

#define GPIOB_BASE 0x40010C00
#define GPIOB_CRL  (GPIOB_BASE+0x00)
#define GPIOB_CRH  (GPIOB_BASE+0x04)
#define GPIOB_IDR  (GPIOB_BASE+0x08)
#define GPIOB_ODR  (GPIOB_BASE+0x0C)
#define GPIOB_BSRR (GPIOB_BASE+0x10)
#define GPIOB_BRR  (GPIOB_BASE+0x14)
#define GPIOB_LCKR (GPIOB_BASE+0x18)

#define STK_CTRL (0xE000E010+0x00)
#define STK_LOAD (0xE000E010+0x04)
#define STK_VAL  (0xE000E010+0x08)


#define USART1_BASE 0x40013800
#define USART1_SR   (USART1_BASE+0x00)
#define USART1_DR   (USART1_BASE+0x04)
#define USART1_BRR  (USART1_BASE+0x08)
#define USART1_CR1  (USART1_BASE+0x0C)
#define USART1_CR2  (USART1_BASE+0x10)
#define USART1_CR3  (USART1_BASE+0x14)
#define USART1_GTPR (USART1_BASE+0x18)


#define GPIOA_BASE 0x40010800
#define GPIOA_CRL  (GPIOA_BASE+0x00)
#define GPIOA_CRH  (GPIOA_BASE+0x04)
#define GPIOA_IDR  (GPIOA_BASE+0x08)
#define GPIOA_ODR  (GPIOA_BASE+0x0C)
#define GPIOA_BSRR (GPIOA_BASE+0x10)
#define GPIOA_BRR  (GPIOA_BASE+0x14)
#define GPIOA_LCKR (GPIOA_BASE+0x18)


#define RCC_BASE 0x40021000
#define RCC_APB2ENR (RCC_BASE+0x18)

int notmain ( void )
{
    unsigned int ra;

    ra=GET32(RCC_APB2ENR);
    ra|=1<<2;   //GPIOA
    ra|=1<<3;   //GPIOB
    ra|=1<<14;  //USART1
    PUT32(RCC_APB2ENR,ra);

    //LED output
    //pb1 output
    ra=GET32(GPIOB_CRL);
    ra&=~(0xF0);
    //mode 01  cnf 00
    //0001
    ra|=0x10;
    PUT32(GPIOB_CRL,ra);

    //pa9 TX  alternate function output push-pull
    //pa10 RX configure as input floating
    ra=GET32(GPIOA_CRH);
    ra&=~(0xFF0);
    ra|=0x490;
    PUT32(GPIOA_CRH,ra);



    PUT32(USART1_CR1,0x2000);
    PUT32(USART1_CR2,0x0000);
    PUT32(USART1_CR3,0x0000);
    //8000000/16 = 500000
    //500000/115200 = 4.34
    //4 and 5/16 = 4.3125
    //4.3125 * 16 * 115200 = 7948800
    PUT32(USART1_BRR,0x0045);
    PUT32(USART1_CR1,0x200C);

    PUT32(STK_CTRL,4);
    PUT32(STK_LOAD,0x00FFFFFF);
    PUT32(STK_CTRL,5);

    for(ra=0;ra<3;ra++)
    {
        PUT32(GPIOB_BSRR,0x00000002);
        PUT32(USART1_DR,0x55);
        while(1)
        {
            if((GET32(STK_VAL)&0x800000)==0x800000) break;
        }
        PUT32(GPIOB_BSRR,0x00020000);
        PUT32(USART1_DR,0x56);
        while(1)
        {
            if((GET32(STK_VAL)&0x800000)==0x000000) break;
        }
    }
    for(ra=0;ra<100;ra++)
    {
        while(1)
        {
            if(GET32(USART1_SR)&0x80) break;
        }
        PUT32(USART1_DR,0x30|(ra&0x7));
    }
    while(1)
    {
        while(1)
        {
            if(GET32(USART1_SR)&0x20) break;
        }
        ra=GET32(USART1_DR)&0xFF;
        while(1)
        {
            if(GET32(USART1_SR)&0x80) break;
        }
        PUT32(USART1_DR,ra);
    }


    return(0);
}
