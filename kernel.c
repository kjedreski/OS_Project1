/* ACADEMIC INTEGRITY PLEDGE                                              */
/*                                                                        */
/* - I have not used source code obtained from another student nor        */
/*   any other unauthorized source, either modified or unmodified.        */
/*                                                                        */
/* - All source code and documentation used in my program is either       */
/*   my original work or was derived by me from the source code           */
/*   published in the textbook for this course or presented in            */
/*   class.                                                               */
/*                                                                        */
/* - I have not discussed coding details about this project with          */
/*   anyone other than my instructor. I understand that I may discuss     */
/*   the concepts of this program with other students and that another    */
/*   student may help me debug my program so long as neither of us        */
/*   writes anything during the discussion or modifies any computer       */
/*   file during the discussion.                                          */
/*                                                                        */
/* - I have violated neither the spirit nor letter of these restrictions. */
/*sd                                                                        */
/*a                                                                        */
/*                                                                        */
/* Signed:Kevin Jedreski Date:  Feb-11-2016 */
/*                                                                        */
/*                                                                        */
/* 3460:4/526 BlackDOS kernel, Version 1.02, Spring 2016.                 */
/*
Kevin Jedreski
Operating Systems Project 1
*/

/*void printString(char*);
void readString(char*);
void writeInt(int);
void ReadInt(int*);
void clearScreen(int,int);*/
void handleInterrupt21(int,int,int,int);
void main()
{
   char buffer[512];
   makeInterrupt21();
   interrupt(33,12,4,5,0);
   interrupt(33,0,"___.   .__                 __       .___           \r\n\0",0,0);
   interrupt(33,0,"\\_ |__ |  | _____    ____ |  | __ __| _/___  ______\r\n\0",0,0);
   interrupt(33,0," | __ \\|  | \\__  \\ _/ ___\\|  |/ // __ |/ _ \\/  ___\r\n\0",0,0);
   interrupt(33,0," | \\_\\ \\  |__/ /\\ \\\\  \\___|    </ /_/ ( <_> )___ \\ \r\n\0",0,0);
   interrupt(33,0," |___  /____(____  /\\___  >__|_ \\____ |\\___/____  >\r\n\0",0,0);
   interrupt(33,0,"     \\/          \\/     \\/     \\/    \\/         \\/ \r\n\0",0,0);
   interrupt(33,0," V. 1.02, C. 2016. Based on a project by M. Black. \r\n\0",0,0);
   interrupt(33,0," Author(s): Kevin Jedreski\r\n\0",0,0);
   interrupt(33,2,buffer,30,0);
   interrupt(33,0,buffer,0,0);
   while(1);
}

void printString(char* c)
{
  int index=0;
  while(c[index]!='\0'){
	char AL = c[index];
    /*Make system call to BIOS */
    interrupt(16,14*256+AL,0,0,0);
    index = index +1;
  }
   return;
}

void readString(char* c)
 {
	int index=0;
  /* Gather input*/
	while(c[index]!=13){
		/*input*/
		c[index] = interrupt(22,0,0,0,0);
    if (c[index]==13) break;
    /*handle backspace */
    if (c[index]==8 && index>=0){
      if (index!=0) {
      index = index-1;
      /*move cursor back, display space and then move cursor back again */
      interrupt(16,14*256+8,0,0,0);
      interrupt(16,14*256+' ',0,0,0);
      interrupt(16,14*256+8,0,0,0);
      }
    }
    /*display what user is typing*/
    else {
		interrupt(16,14*256+c[index],0,0,0);
		index = index +1;
		}
	}
  /*when enter is pressed, add 0x0 to end of char array */
  c[index]=0;
  printString("\r\n");
/*echo back string on next line */
  printString(c);
  	 return;
}

void clearScreen(int bx, int cx)
{
  int index =0;
  while (index!=23){
    interrupt(16,14*256+13,0,0,0);
    interrupt(16,14*256+'\n',0,0,0);
    index =index+1;
  }
/*manage colors */
  interrupt(16,512,0,0,0);
   if (bx>0 && cx>0){
    if (bx<=8 || cx<=16)
    interrupt(16,1536,4096*(bx-1)+256*(cx-1),0,6223);
  }
	return;
}

int mod(int a, int b)
{
   int x = a;
   while (x >= b) x = x - b;
   return x;
}

int div(int a, int b)
{
   int q = 0;
   while (q * b <= a) q++;
   return (q - 1);
}

void writeInt(int x)
{
   char number[6], *d;
   int q = x, r;
   int save;
   if (x < 1)
   {
      d = number; *d = 0x30; d++; *d = 0x0; d--;
   }
   else
   {
      d = number + 5;
      *d = 0x0; d--;
      while (q > 0)
      {
         r = mod(q,10);

         q = div(q,10);

         *d = r + 48;
         d--;
      }
      d++;
   }
   printString(d);
}


void ReadInt(int* number)
{
  int sum =0;
  int index=0;
  char input[20];
  int temp=10;
  /*Gather input */
  readString(input);
  while (input[index]!='\0'){
    sum = sum * 10 + (input[index]-48);
    index = index+1;
  }
  *number = sum;
}

/*
functions to add TODO: lab 2
*/

void readSector(char* buffer, int sector){
  int relSecNo = mod(sector,18)+1;
  int headNo = mod(div(sector,18),2);
  int trackNo = div(sector,36);
  int AX = 2*256+1;
  int CX = trackNo * 256 + relSecNo; 
  int DX = headNo*256 + 0; 

  interrupt(19,AX,buffer,CX,DX);
  //call interrupt 19 to read sector into buffer
  return;
}

void readFile(int bx, int cx, int dx){
  
  return;
}

void runProgram(int bx,int cx){
  return;
}

void stop() {
  return;
}

void error(int bx){

if (bx==0){
  printString("File is not found \r\n\0");
 }

else {
  printString("General DOS failure  \r\n\0");
}

}


// interrupt service routine to manage interrupt vector table
void handleInterrupt21(int ax, int bx, int cx, int dx){
  if (ax==0){
    printString(bx);
  }
  else if (ax==1){
    readString(bx);
  }
  else if (ax==2){
    readSector(bx,cx);
  }
  else if (ax==3){
    return;
  }
  else if (ax==4){
   return;
  }
  else if (ax==5){
   return;
  }
  else if (ax==12){
    clearScreen(bx,cx);
  }
  else if (ax==13){
    writeInt(bx);
  }
  else if (ax==14){
    ReadInt(bx);
  }
  else if (ax==15){
      error(bx);
  }
  else {
    printString("Incorrect service call\0");
  }
}
