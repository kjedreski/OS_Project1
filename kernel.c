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
   char buffer[13312];
   int size;
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
   interrupt(33,3,"msg\0",buffer,&size);
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

/*Helper function*/
void printChar(char c){
interrupt(16,14*256+c,0,0,0);
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
void error(int bx){

if (bx==0){
  printString("File is not found \r\n\0");
 }

else {
  printString("General DOS failure  \r\n\0");
}

}

void readSector(char* buffer, int sector){
  int relSecNo = mod(sector,18)+1;
  int headNo = mod(div(sector,18),2);
  int trackNo = div(sector,36);
  int AX = 2*256+1;
  int CX = trackNo * 256 + relSecNo;
  int DX = headNo*256 + 0;
  //printString("readSector value: \r\n\0");
  //writeInt(sector);

  interrupt(19,AX,buffer,CX,DX);
  /*call interrupt 19 to read sector into buffer*/
  return;
}

void readFile(char* fname, char* buffer, int* size){
  int notFound = 0;
  int fnameCount = 0;
  int fileLength = 0;
  int isMatch = 0;
  int d = 0;
  int i = 0;
  int lastFileChar = 0;
  int sectorLength=0;
  int copycat =0;
  /* for part 1*/
  char Arr[512];
  char trackSector[512];
  /* track indices of Arr in here*/


  /*count how mayn chars file has*/
  while (fname[i]!='\0'){
   i = i +1;
   fileLength = fileLength+1;
  }

 /* Load the directory sector into a 512-byte chars array called Arr*/
  readSector(Arr,2);
/*sector now read into buffer[]512-byte */
/* iterate through directory array and match filename*/
  /*for each char of filename, go through buffer if there's a match
  increment isMatch by 1, and at the end compare size of isMatch to fnameCount*/
  /*track sector number using index */
  i=0;
  while (notFound == 0) {
    for ( d =0; d < 512; d ++) {
      if (fname[i]==Arr[d]){
        i++;
        isMatch = isMatch+1;
        if (isMatch == fileLength) {notFound = 1; lastFileChar=d;}
     }
  }
  //if filename not in first sector, add 512 and check next spot
  if (notFound!=1) readSector(Arr+512,2);
}
printString("File found!\r\n\0");
//TODO: error handling if file is not in system


// first 6 bytes are file name
i=fileLength;
while (i < 7 && i!=6 ){
  lastFileChar++;
  i++;
}

//set i to lastfilechar plus one, so we can start reading the sector
i=lastFileChar+1;
d=0;
//read all sectors number, into each index of buffer

while ( i < lastFileChar+26){
  //this prints sector numbers
  //writeInt(Arr[i]);
  // i'm thinking we call readsector for each number in Arr[i]
  //readsector(buffer[d],Arr[i])
  trackSector[d] = Arr[i];
  d++;
  i++;
}

  /* check to see if file name is in directory*/
  /*if (isMatch==fnameCount){*/
     /* read all sector numbers from trackSector into buffer*/
    i=0;
    while (trackSector[i]!='\0'){
      //TODO: the issue: buffer[i] I believe is passing char to a int
      // parameter
      //we may need to convert this char to a integer
      readSector(buffer+copycat,trackSector[i]);
      copycat = copycat + 512;
      i= i+1;
     }




/*  } */
/* if file not found, throw error*/
  /*else { */
    /*error(0);
  }   */

}

void runProgram(int bx,int cx){
  return;
}

void stop() {
  return;
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
    readFile(bx,cx,dx);
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
