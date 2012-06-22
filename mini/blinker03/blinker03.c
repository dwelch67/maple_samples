//http://dfu-util.gnumonks.org/releases/
//dfu-util -c 1 -i 0 -a 1 -D myprogram.bin

void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );

#define RCC_BASE 0x40021000
#define RCC_APB2ENR (RCC_BASE+0x18)
#define RCC_APB1ENR (RCC_BASE+0x1C)

#define GPIOB_BASE 0x40010C00
#define GPIOB_CRL  (GPIOB_BASE+0x00)
#define GPIOB_CRH  (GPIOB_BASE+0x04)
#define GPIOB_IDR  (GPIOB_BASE+0x08)
#define GPIOB_ODR  (GPIOB_BASE+0x0C)
#define GPIOB_BSRR (GPIOB_BASE+0x10)
#define GPIOB_BRR  (GPIOB_BASE+0x14)
#define GPIOB_LCKR (GPIOB_BASE+0x18)

//#define TIM5_BASE 0x40000C00
//#define TIM4_BASE 0x40000800
//#define TIM3_BASE 0x40000400
#define TIM2_BASE 0x40000000
#define TIM_BASE TIM2_BASE
#define TIM_CR1 (TIM_BASE+0x00)
#define TIM_SR  (TIM_BASE+0x10)
#define TIM_CNT (TIM_BASE+0x24)
#define TIM_PSC (TIM_BASE+0x28)
#define TIM_ARR (TIM_BASE+0x2C)

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

    //Need to enable the timer before we can use it.
    ra=GET32(RCC_APB1ENR);
    ra|=1<<0; //enable timer 2
    //ra|=1<<1; //enable timer 3
    //ra|=1<<2; //enable timer 4
    //ra|=1<<3; //enable timer 5
    PUT32(RCC_APB1ENR,ra);

    //make sure the timer is stopped, set the reload value and
    //the prescaler.
    //0xF424*0x200 = 0x1E84800 = 32000000 which is 4 seconds at
    //8Mhz per clock tick.
    //so the led should change state every 4 seconds
    PUT16(TIM_CR1,0x0000);
    PUT16(TIM_ARR,0xF423);
    PUT16(TIM_PSC,0x0200);
    PUT16(TIM_CR1,0x0001);

    //The status register is used to look for a timer event (the timer
    //reaching zero and reloading from the reload register).
    //have to write a zero to that bit to clear it. (read about rc_w0)
    //in the manual also notice that the status register bits are rc_w0.
    while(1)
    {
        PUT32(GPIOB_BSRR,0x00000002);
        while(1) if(GET16(TIM_SR)&1) break;
        PUT16(TIM_SR,0xFFFE);
        PUT32(GPIOB_BSRR,0x00020000);
        while(1) if(GET16(TIM_SR)&1) break;
        PUT16(TIM_SR,0xFFFE);
    }
    return(0);
}
