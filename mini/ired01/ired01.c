//http://dfu-util.gnumonks.org/releases/
//dfu-util -c 1 -i 0 -a 1 -D myprogram.bin

void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
void PUTGETSET ( unsigned int, unsigned int );
void PUTGETCLR ( unsigned int, unsigned int );
void PUTGETSETMASK ( unsigned int, unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );

#define RCC_BASE 0x40021000
#define RCC_APB2ENR (RCC_BASE+0x18)
#define RCC_APB1ENR (RCC_BASE+0x1C)
#define RCC_CR      (RCC_BASE+0x00)
#define RCC_CFGR    (RCC_BASE+0x04)
#define FLASH_ACR   0x40022000

#define GPIOB_BASE 0x40010C00
#define GPIOB_CRL  (GPIOB_BASE+0x00)
#define GPIOB_CRH  (GPIOB_BASE+0x04)
#define GPIOB_IDR  (GPIOB_BASE+0x08)
#define GPIOB_ODR  (GPIOB_BASE+0x0C)
#define GPIOB_BSRR (GPIOB_BASE+0x10)
#define GPIOB_BRR  (GPIOB_BASE+0x14)
#define GPIOB_LCKR (GPIOB_BASE+0x18)

int notmain ( void )
{
    unsigned int ra;

    PUTGETSET(RCC_APB2ENR,1<<3); //enable GPIOB
    PUTGETSETMASK(GPIOB_CRL,0xF<<4,0x1<<4); //enable LED GPIO
    PUTGETSETMASK(GPIOB_CRL,0xF<<28,0x4<<28); //enable input gpio
    while(1)
    {
        if(GET32(GPIOB_IDR)&(1<<7))  PUT32(GPIOB_BSRR,0x00020000);
        else                         PUT32(GPIOB_BSRR,0x00000002);
    }
    return(0);
}
