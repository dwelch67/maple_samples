//http://gitorious.org/~tormod/unofficial-clones/dfuse-dfu-util
//dfu-util -c 1 -i 0 -a 1 -D blinker02.gcc.thumb.bin

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );

#define RCC_BASE 0x40021000
#define RCC_APB2ENR (RCC_BASE+0x18)

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

int notmain ( void )
{
    unsigned int ra;

    ra=GET32(RCC_APB2ENR);
    ra|=1<<3;   //GPIOB
    PUT32(RCC_APB2ENR,ra);


    //pb1 output
    ra=GET32(GPIOB_CRL);
    ra&=~(0xF0);
    //mode 01  cnf 00
    //0001
    ra|=0x10;
    PUT32(GPIOB_CRL,ra);

    PUT32(STK_CTRL,0);
    PUT32(STK_LOAD,0x00FFFFFF);
    PUT32(STK_CTRL,1);

    while(1)
    {
        PUT32(GPIOB_BSRR,0x00000002);
        while(1)
        {
            if((GET32(STK_VAL)&0x800000)==0x800000) break;
        }
        PUT32(GPIOB_BSRR,0x00020000);
        while(1)
        {
            if((GET32(STK_VAL)&0x800000)==0x000000) break;
        }
    }
    return(0);
}
