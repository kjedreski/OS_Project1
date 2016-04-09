#include "blackdos.h"

void deleteFile(char cmdLine[]);
int stringCompare(char a[], char b[]);
void copyFile(char cmdLine[]);
void echoText(char cmdLine[]);
void runFile(char cmdLine[]);
void tweet(char cmdLine[]);
void type(char cmdLine[]);
void dir(char cmdLine[]);


void main(){
  int input;
  char arg1[];
  char arg2[];
  char x[];

   CLS();
  PRINTS("Welcome\r\n\0");

while(1){
  PRINTS("\r\ncxxxx][===blackdos===>");
  //user input
  SCANS(x);
  if (stringCompare(x,"cls")==1)
    CLS();
  else if (stringCompare(x,"boot")==1)
    BOOT();
  else if (stringCompare(x,"del")==1)
    deleteFile(x);
  else if (stringCompare(x,"copy")==1)
    copyFile(x);
  else if (stringCompare(x,"echo"))
    echoText(x);
  else if (stringCompare(x,"run")==1)
    runFile(x);
  else if (stringCompare(x,"tweet")==1)
    tweet(x);
  else if (stringCompare(x,"type")==1)
    type(x);
  else if (stringCompare(x,"dir")==1)
    dir(x);

  //else if (stringCompare(x,"run"))
  }
}

void dir (char cmdLine[]){
  // list disk directory contents
  // first read dir sector
  char directory[512];
  int i=0;
  int d=0;
  int ascii=0;
  int sectorCount=0;
  interrupt(33,2,&directory,2,0);

  // im thinking nested loops
  // outer goes through entire directory
  // inner goes through first 6 chars

  PRINTS("\r\n\0");
  for (i=0 ; i < 512; i ++ ){
    //check that it's multiple of 32 and there's stuff in file
    // validate the first letter isn't capitalized.
    ascii = directory[i];
   if (i %32 == 0 && directory[i] != 0 && (ascii>=90 && ascii<=122) ){
     //PRINT first 6 letters of name
     //PRINT sector number and then newline
     // TODO: Hide capital letters
     // TODO: print total space used by files and total free space remaining
     for (d=0; d < 6; d++)
       PRINTC(directory[i+d]);

     for (d=6; d < 32; d++){
       PRINTN(directory[i+d]);
       PRINTC(' ');
       if (directory[i+d] != 0)
         sectorCount++;
     }

     PRINTS("\r\n\0");
    }
  }

  PRINTS("Space used by files: \0");
  PRINTN(sectorCount*32);

}

void type (char cmdLine[]){
  // first parse filename
  int i=0;
  int d=0;
  char filename[512];
  char buffer[512];
  while (cmdLine[i] != ' ')
    i++;
  i++;
  PRINTS("\n\r\0");
  while (cmdLine[i] != '\0'){
    filename[d] = cmdLine[i];
    i++;
    d++;
  }
  // read file into buffer
  interrupt(33,3,&filename,&buffer,1);
  // print contents of buffer
  interrupt(33,0,&buffer,0,0);
}

void tweet(char cmdLine[]){
  // first parse first file name
  int i=0;
  int d=0;
  char filename[512];
  while (cmdLine[i] != ' ')
    i++;
  i++;
  PRINTS("\n\r\0");
  while (cmdLine[i] != '\0'){
    filename[d] = cmdLine[i];
    i++;
    d++;
  }
  PRINTC(filename[0]);
  PRINTC(filename[1]);
  PRINTS("*******\n\0");
  interrupt(33,8,"tweet\0",&filename,1);
  //interrupt(33,8,&file2,&buffer,size);

}

void runFile(char cmdLine[]){
  int i=0;
  int d=0;
  char file1[512];
  while (cmdLine[i] != ' ')
    i++;
  i++;
  PRINTS("\n\r\0");
  while (cmdLine[i] != '\0'){
    file1[d] = cmdLine[i];
    i++;
    d++;
  }
  //run program at segment 4
  interrupt(33,4,&file1,4,0);

}

void echoText(char cmdLine[]){
  int i=0;
  int d=0;
  char file1[512];

  while (cmdLine[i] != ' ')
    i++;
  i++;
  PRINTS("\n\r\0");
  while (cmdLine[i] != '\0'){
    PRINTC(cmdLine[i]);
    i++;
  }
}

void copyFile(char cmdLine[]){
  //TODO: parse file1 and file2
  char file1[512];
  char file2[512];
  char buffer[13312];
  int size;
  // iterators
  int i=0;
  int d=0;
  // first parse file1
  // iterate till first space
  while (cmdLine[i] != ' ')
   i++;
  i++;
  while (cmdLine[i] !=  ' '){
    file1[d]=cmdLine[i];
    d++;
    i++;
  }
  // iterate till null term
  i++;
  d=0;
  while (cmdLine[i] != '\0'){
    file2[d] = cmdLine[i];
    d++;
    i++;
  }
  // first read contents of file1 into a buffer
  interrupt(33,3,&file1,buffer,&size);
  // add new filename to system and copy over buffer contents to file2
  interrupt(33,8,&file2,&buffer,size);
  // copy over contents of buffer into file1
}

void deleteFile(char cmdLine[]){
  //first parse file
  int i=0;
  int d=0;
  char filename[512];
  // parse file name
  while (cmdLine[i]!=' ')
    i++;
  i++;
  d=0;
  while (cmdLine[i]!='\0'){
    filename[d]=cmdLine[i];
    d++;
    i++;
  }
   interrupt(33,7,&filename,2,0);
  //filename contains file to be delted

}





int stringCompare(char a[], char b[]){
  /*calculate string len of a and b*/
  int i=0;
  int d=0;
  int stringLength1 =0;
  int stringLength2 =0;

  while (a[i]!='\0'){
   if (a[i]== ' '){
     stringLength1 = i;
     break;
   }
   i = i +1;
   stringLength1 = stringLength1+1;
  }
  i=0;
  while (b[i]!='\0'){
   i = i +1;
   stringLength2 = stringLength2+1;
  }
  // now check to see if lengths are same for a and b
  if (stringLength1 != stringLength2)
    return 0;
  else
    for (i=0; i < stringLength1; i ++){
      if (a[i] != b[i])
        return 0;
    }
    return 1;
}
