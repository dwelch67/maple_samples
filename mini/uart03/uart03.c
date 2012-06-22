
//------------------------------------------------------------------------
//------------------------------------------------------------------------

//http://gitorious.org/~tormod/unofficial-clones/dfuse-dfu-util
//dfu-util -c 1 -i 0 -a 1 -D blinker02.gcc.thumb.bin

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );
void PUTGETSET ( unsigned int, unsigned int );
void PUTGETCLR ( unsigned int, unsigned int );
void PUTGETSETMASK ( unsigned int, unsigned int, unsigned int );

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
#define RCC_APB1ENR (RCC_BASE+0x1C)
#define RCC_CR      (RCC_BASE+0x00)
#define RCC_CFGR    (RCC_BASE+0x04)
#define FLASH_ACR   0x40022000

//------------------------------------------------------------------------
void clock_init ( void )
{
    //enable the external clock
    PUTGETSET(RCC_CR,1<<16); //HSEON
    //wait for HSE to settle
    while(1) if(GET32(RCC_CR)&(1<<17)) break; //HSERDY
    if(0)
    {
        //select HSE clock
        PUTGETSETMASK(RCC_CFGR,(0x3<<0),(0x1<<0));
        //wait for it
        while(1) if((GET32(RCC_CFGR)&0xF)==0x5) break;
        return;
    }
    //setup pll source settings
    PUTGETSETMASK(RCC_CFGR,(0xF<<18)|(1<<17)|(1<<16),(0x7<<18)|(0<<17)|(1<<16));
    //enable the pll
    PUTGETSET(RCC_CR,1<<24); //PLLON
    //wait for pll to lock
    while(1) if(GET32(RCC_CR)&(1<<25)) break; //HSERDY
    //USB div/1.5 AHB not divided, APB not divided, APB1 div/2, MCO, do you need this?
    PUTGETSETMASK(RCC_CFGR,(0x7<<24)|(0x1<<22)|(0x7<<11)|(0x7<<8)|(0xF<<4),(0x7<<24)|(0x0<<22)|(0x0<<11)|(0x4<<8)|(0x0<<4));
    //add wait states for the flash
    PUT32(FLASH_ACR,0x2);
    //switch to the pll as the source
    PUTGETSETMASK(RCC_CFGR,(0x3<<0),(0x2<<0));
    //wait for it
    while(1) if((GET32(RCC_CFGR)&0xF)==0xA) break;
}
//------------------------------------------------------------------------
void uart_init ( void )
{
    //assuming 8MHz clock, 115200 8N1
    unsigned int ra;

    ra=GET32(RCC_APB2ENR);
    ra|=1<<2;   //GPIOA
    ra|=1<<14;  //USART1
    PUT32(RCC_APB2ENR,ra);

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
    //39 1/16 0x27
    PUT32(USART1_BRR,0x0271);
    PUT32(USART1_CR1,0x200C);
}
//------------------------------------------------------------------------
unsigned int uart_getc ( void )
{
    while(1)
    {
        if(GET32(USART1_SR)&0x20) break;
    }
    return(GET32(USART1_DR)&0xFF);
}
//------------------------------------------------------------------------
void uart_putc ( unsigned int c )
{
    while(1)
    {
        if(GET32(USART1_SR)&0x80) break;
    }
    PUT32(USART1_DR,c);
}
//------------------------------------------------------------------------
void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_putc(rc);
        if(rb==0) break;
    }
    uart_putc(0x20);
}
//------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_putc(0x0D);
    uart_putc(0x0A);
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    clock_init();
    uart_init();
    hexstring(0x12345678);
    while(1)
    {
        ra=uart_getc();
        hexstring(ra);
    }
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
