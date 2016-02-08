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
/*                                                                        */
/*                                                                        */
/*                                                                        */
/* Signed:_____________________________________ Date:_____________        */
/*                                                                        */
/*                                                                        */
/* 3460:4/526 BlackDOS kernel, Version 1.02, Spring 2016.                 */
/*
Kevin Jedreski
Operating Systems Project 1
*/

void printString(char*);
void readString(char*);
void writeInt(int);
void readInt(int*);
void clearScreen(int,int);

void main()
{
   char line[80];
   int x;

   clearScreen(2,16);

   printString("___.   .__                 __       .___           \r\n\0");
   printString("\\_ |__ |  | _____    ____ |  | __ __| _/___  ______\r\n\0");
   printString(" | __ \\|  | \\__  \\ _/ ___\\|  |/ // __ |/ _ \\/  ___/\r\n\0");
   printString(" | \\_\\ \\  |__/ /\\ \\\\  \\___|    </ /_/ ( <_> )___ \\ \r\n\0");
   printString(" |___  /____(____  /\\___  >__|_ \\____ |\\___/____  >\r\n\0");
   printString("     \\/          \\/     \\/     \\/    \\/         \\/ \r\n\0");
   printString(" Vll. 1.02, C. 2016. Based on a project by M. Black. \r\n\0");
   printString(" Author(s): Kevin Jedreski\r\n\r\n\0");
   printString("Hola mondo.\r\n\0");
   printString("Enter a line: \0");
   readString(line);
   printString("\r\nYou typed: \0");
   printString(line);
   printString("\r\n\0");
   printString("Enter a number: \0");
   readInt(&x);
   printString("Your number is \0");
   writeInt(x);
   printString("\r\n\0");
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
  c[index+1]=0;
  /*FIXME: bug where there is a tab when you press enter */
  printString("\n");
  printString(c);
  	 return;
}
/*TODO: clearScreen is under maintence. -kj */
void clearScreen(int bx, int cx)
{
  int index =0;
  while (index!=23){
    interrupt(16,14*256+13,0,0,0);
    interrupt(16,14*256+'\n',0,0,0);
    index =index+1;
  }
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
   if (x < 1)
   {
      d = number; *d = 0x30; d++; *d = 0x0; d--;
   }
   else
   {
      d = number + 5;
      *d = 0x0; d--;
      while (q > -1)
      {
         r = mod(q,10); q = div(q,10);
         *d = r + 48; d--;
      }
      d++;
   }
   printString(d);
}

void readInt(int* number)
{
  /*read number as a character string.*/
  char *input;
  /*first handle input for reading integers  */
  int index=0;
  while (number[index]!=13){
        /*get initial input */
        number[index] = interrupt(22,0,0,0,0);
        /*display input back to user*/
        interrupt(16,14*256+c[index],0,0,0);


  }


  int sum=0;
  /*conversion from ascii to integers. */
  while(number[index]!='\0'){
    /*to convert to from ascii to integer, subtract 48 */
    sum = (number[index]-48)+sum;
    index = index+1;
  }
  number = sum;
  /*use 1234%10 to get each digit right to left. */

   /* Fill this in as well. */
   return;
}
