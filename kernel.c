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
Operating Systems Project 3
*/

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
   interrupt(33,0,"Author(s): Kevin Jedreski\r\n\0",0,0);
  // interrupt(33,7,"fib\0",2,0);
  // interrupt(33,7,"test1\0",2,0);
  // interrupt(33,7,"test2\0",2,0);
   //interrupt(33,7,"msg\0",2,0);
     interrupt(33,8,"NewIsh\0",2,0);
   //interrupt(33,4,"fib\0",2,0);
   //interrupt(33,0,"Error if this executes. \r\n\0",0,0);
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
  printString("\r\n\0");
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

void error(int bx){
  /*Error handling */
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
  /*Read/write sectors from floppy disk */
  interrupt(19,AX,buffer,CX,DX);
}

void readFile(char* fname, char* buffer, int* size){
  int matching = 0;
  /*fileLength count length of file string */
  int fileLength = 0;
  /*fileLength is used to find file name in sectors */
  int isMatch = 0;
  /*d and i are iterators used multiple times for arrays */
  int d = 0;
  int i = 0;
  /*lastFileChar finds the index number, for last char in file string */
  int lastFileChar = 0;
  /*copycat is used for storing data in memory from buffer, by adding 512
  each time. */
  int copycat =0;
  int Error=0;
  /* copy directory sector */
  char Arr[512];
  /*store all sector numbers here */
  char trackSector[512];
  /*calculate file length*/
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
    for ( d =0; d < 512; d ++) {
      //if (fname[i]!=Arr[d]){matching = 0;}
      if (fname[i]==Arr[d]){
        i++;
        isMatch++;
      //  matching = 1;
        if (isMatch == fileLength) { lastFileChar=d; break; }
     }
  }
  if (isMatch!=fileLength) {d=512;}

/*reach end of disk and no file?: "Throw error"
if d is 512, than it is garuenteed there is no file match
 */
if (d==512){
  error(0);
}
else {
  i=fileLength;
while (i < 7 && i!=6 ){
  lastFileChar++;
  i++;
}
/*set i to lastfilechar plus one, so we can start reading the sector*/
i=lastFileChar+1;
d=0;
/* read all sectors number, into each index of buffer */
while ( i < lastFileChar+26){
  trackSector[d] = Arr[i];
  d++;
  i++;
}
/* check to see if file name is in directory*/
/* read all sector numbers from trackSector into buffer*/
i=0;
while (trackSector[i]!='\0'){
  readSector(buffer+copycat,trackSector[i]);
  copycat = copycat + 512;
  i= i+1;
    }
  }
}

void runProgram(char *name, int segment){
  // load file into buffer
char buffer[13312];
int size;
int base_location = segment*4096;
int index=0;
readFile(name,buffer,&size);
/*loads in memory segment location and then loads in buffer info */
while (index!=13312){
  putInMemory(base_location,index,buffer[index]);
  ++index;
  }
  /*launches program */
launchProgram(base_location);
}

void stop() {
  while(1);
}

/*ADD skeletons for project 3 here */
/*NOTES:
bx = Address of character array storing sector to write
cx = number of sector being written
dx = Total number of sectors to be written
 */

 void writeSector(char* buffer, int sector){
   int relSecNo = mod(sector,18)+1;
   int headNo = mod(div(sector,18),2);
   int trackNo = div(sector,36);
   int AX = 769;
   int CX = trackNo * 256 + relSecNo;
   int DX = headNo*256 + 0;
   /*Read/write sectors from floppy disk */
   interrupt(19,AX,buffer,CX,DX);
 }
void deleteFile(char* name) {
  // load disk directory(disk sector 2)
  // load map (disk sector 2)
  // into both 512 byte char arrays
  char disk_Dir[512];
  char map[512];
  int i = 0;
  int d = 0;
  int fileLength = 0;
  int isMatch = 0;
  int matching = 0;
  int lastFileChar = 0;
  int holder[512];
  readSector(disk_Dir,2);
  readSector(map,1);
  //search through directory and find file name
  /*calculate file length*/
  while (name[i]!='\0'){
    i = i +1;
    fileLength = fileLength+1;
    }
    /*iterate through directory array and match filename*/
    /*for each char of filename, go through buffer if there's a match
      increment isMatch by 1, and at the end compare size of isMatch to fnameCount*/
      /*track sector number using index */
  i=0;
  for ( d =0; d < 512; d ++) {
    if (name[i]== disk_Dir[d]){
      i++;
      isMatch++;
      if (isMatch == fileLength) { lastFileChar = d; break; }
      }
    }
  if (isMatch!=fileLength) {d=512;}
  /*reach end of disk and no file?: "Throw error"
  if d is 512, than it is garuenteed there is no file match
   */
  if (d==512){
    error(0);
  }
  else {
    i=fileLength;
  while (i < 7 && i!=6 ){
    lastFileChar++;
    i++;
  }
  printString("lastFileChar is: \r\n\0");
  writeInt(lastFileChar);
  /*set i to lastfilechar plus one, so we can start reading the sector*/
    //lastFileChar=lastFileChar+1;
    //minus length from d and we get first byte
    d = d-(fileLength-1);
    /*HERE -> we set first byte of file name to zero */
    disk_Dir[d]=0;
    /*Now step through the sectors numbers listed as belonging to file */
    /*for each sector, set the corresponding map byte to zero */
    /*iterate over map sectors, after first 6 in directory */
    for (i = lastFileChar-25; i < lastFileChar; i++){
     map[i+1]=0;
    }
    /*Write the character arrays holding the directory and map back to their
    appropiate sectors */
    writeSector(map,1);
    writeSector(disk_Dir,2);
  }
}

void writeFile(char* name,char* buffer, int numberOfSectors) {
  //puprose: writes a file to the disk
  char map[512];
  char dir[512];
  int i=0;
  int isMatch =0;
  int fileLength =0;
  int lastFileChar = 0;
  int d=0;
  int FreeEntry=0;
  int helper=0;
  int ctrl =1;
  //Load map and directory sectors into buffers
  readSector(map,1);
  readSector(dir,2);
  // search through the driectory, doing 2 things
    //* if file name exists, call error
    //* find and note afree directory enettry
    while (name[i]!='\0'){
      i = i +1;
      fileLength = fileLength+1;
      }
      /*iterate through directory array and match filename*/
      /*for each char of filename, go through buffer if there's a match
        increment isMatch by 1, and at the end compare size of isMatch to fnameCount*/
        /*track sector number using index */
    i=0;
    for ( d =0; d < 512; d ++) {
      if (name[i]== dir[d]){
        i++;
        isMatch++;
        if (isMatch == fileLength) { lastFileChar = d; break; }
        }
      //modulus 32, if 0 then theres a free entry at that sector
      //ctrl enforces mechanism memory is contiguously stored
      if (d%32==0 && ctrl==1 && dir[d]==0){
        writeInt(d);
        ctrl = 0;
        FreeEntry = d;
      }
    }
    if (isMatch==fileLength) {d=512;}
    // file is already in system, throw error
    if (d!=512){
      error(0);
    }
    //FreeEntry is where we store the file name
    //calc name length
    while (name[i]!='\0'){
      i = i +1;
      fileLength = fileLength+1;
      }
    //copy name to that directory entry
    // if length not 6, pad zeros till 6
    //FreeENtry is the first spot in memory where it is free
    // and can be used for storage
    //helper will be used to validate fileLength
    for (i = 0;i < fileLength; i++){
      dir[FreeEntry+i] = name[i];
      helper++;
    }
    // helper validates that is at least 6
    for (i = fileLength; i < fileLength+6; i ++){
      if (helper < 6){
        helper++;
        dir[i] = 0;
      }
      else break;
    }

//for each sector making up that file:
  //a. find a free sector by searching through the map for zero
















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
    runProgram(bx,cx);
  }
  else if (ax==5){
   stop();
  }
  else if (ax==6){
    writeSector(bx,cx);
  }
  else if (ax==7) {
    deleteFile(bx);
  }
  else if (ax==8){
    writeFile(bx,cx,dx);
  }
  else if (ax==11){
    interrupt(25,0,0,0,0);
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
