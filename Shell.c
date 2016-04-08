#include "blackdos.h"

void deleteFile(char cmdLine[]);
int stringCompare(char a[], char b[]);


void main(){
  int input;
  char arg1[];
  char arg2[];
  char x[];

   CLS();
  PRINTS("plural, bitch");

while(1){
  PRINTS("cxxxx][===blackdos===>");
  //user input
  SCANS(x);
  if (stringCompare(x,"cls")==1)
    CLS();
  else if (stringCompare(x,"boot")==1)
    BOOT();
  else if (stringCompare(x,"del")==1) {
    PRINTS("Inside,\r\n\0");
    deleteFile(x);
   }
  }
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
