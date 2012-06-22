
//------------------------------------------------------------------------
//------------------------------------------------------------------------

//http://dfu-util.gnumonks.org/releases/
//dfu-util -c 1 -i 0 -a 1 -D myprogram.bin

void PUT32 ( unsigned int, unsigned int );
void dummy ( unsigned int );

#define IWDG_BASE 0x40003000
#define IWDG_KR   (IWDG_BASE+0x00)
#define IWDG_PR   (IWDG_BASE+0x04)
#define IWDG_RLR  (IWDG_BASE+0x08)
#define IWDG_SR   (IWDG_BASE+0x0C)

//------------------------------------------------------------------------
int notmain ( void )
{
    PUT32(IWDG_KR,0x5555); //enable access to IWDG_PR
    PUT32(IWDG_PR,0x3);
    PUT32(IWDG_KR,0xCCCC); //start IWDG
    while(1) dummy(0);
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
