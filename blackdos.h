#define PRINTS(x) interrupt(33,0,x,0,0)
#define PRINTN(x) interrupt(33,13,x,0,0)
#define SCANS(x)  interrupt(33,1,x,0,0)
#define SCANN(x)  interrupt(33,14,&x,0,0)
#define CLS()     interrupt(33,12,4,5,0)
#define END()     interrupt(33,5,0,0,0)
#define BOOT()    interrupt(33,12,0,0,0)
#define PRINTC(x) interrupt(33,66,x,0,0);
