Kevin Jedreski
kaj61@zips.uakron.edu
March/7/2016

Operating Systems Project 2:
Implemented:
*Read Disk Sector [readSector()]: which reads and writes secotrs from
  floppy disk into a buffer pointer
*Reading a File [readFile()]: which takes a file name in a string and
  reads the contents of the file into a buffer
*Error handling for invalid file names [error()]:
*starting a program [runProgram()]:
*stopping a program once it's finished executing [stop()]:

Additional notes:  all 4 files are loading into memory via compileOS.sh
In main I'm only launching "fib" file.

To RUN:
Xterm console:
		>./compileOS.sh
