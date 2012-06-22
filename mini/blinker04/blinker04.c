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
    //0111: PLL input clock x 9 PLLMUL 21:18
    PUTGETSETMASK(RCC_CFGR,(0xF<<18)|(1<<17)|(1<<16),(0x7<<18)|(0<<17)|(1<<16));
    //enable the pll
    PUTGETSET(RCC_CR,1<<24); //PLLON
    //wait for pll to lock
    while(1) if(GET32(RCC_CR)&(1<<25)) break; //HSERDY
    //AHB not divided, APB not divided, APB1 cant exceed 36, MCO, do you need this?
    PUTGETSETMASK(RCC_CFGR,(0x7<<24)|(0x7<<11)|(0x7<<8)|(0xF<<4),(0x7<<24)|(0x0<<11)|(0x4<<8)|(0x0<<4));
    //FLASH ACR
    //Bits 2:0 LATENCY: Latency
    //These bits represent the ratio of the SYSCLK (system clock) period to the Flash access
    //time.
    //000 Zero wait state, if 0 < SYSCLK≤ 24 MHz
    //001 One wait state, if 24 MHz < SYSCLK ≤ 48 MHz
    //010 Two wait states, if 48 MHz < SYSCLK ≤ 72 MHz
    PUT32(FLASH_ACR,0x2);
    //switch to the pll as the source
    PUTGETSETMASK(RCC_CFGR,(0x3<<0),(0x2<<0));
    //wait for it
    while(1) if((GET32(RCC_CFGR)&0xF)==0xA) break;
}

int notmain ( void )
{
    unsigned int ra;


    PUTGETSET(RCC_APB2ENR,1<<3); //enable GPIOB
    PUTGETSETMASK(GPIOB_CRL,0xF<<4,0x1<<4);

    clock_init();

    //Need to enable the timer before we can use it.
    //1<<0; //enable timer 2
    //1<<1; //enable timer 3
    //1<<2; //enable timer 4
    //1<<3; //enable timer 5
    PUTGETSET(RCC_APB1ENR,1<<0);

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
