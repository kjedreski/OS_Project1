#include "blackdos.h"

void deleteFile(char cmdLine[]);
int stringCompare(char a[], char b[]);
void copyFile(char cmdLine[]);


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
  else if (stringCompare(x,"copy")==1){
    copyFile(x);
    // copy file1 file2
    //TODO: second parse file1 and file2
    }
  //else if (stringCompare(x,"run"))
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
   filename[3];
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
