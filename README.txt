Kevin Jedreski
kaj61@zips.uakron.edu
March/7/2016

Operating Systems Project 2

Goal: Read from disk via I/O

-----------------------
Implemented:
*Read Disk Sector [readSector()]: which reads and writes sectors from
  floppy disk into a buffer pointer

*Reading a File [readFile()]: which takes a file name in a string and
  reads the contents of the file into a buffer

*Error handling for invalid file names [error()]: if parameter is 1,
file loading error, else a BIOS problem.

*starting a program [runProgram()]:  loads buffer into segmented memory and
runs launchProgram function provided to us.

*stopping a program once it's finished executing [stop()]:  Implicitly
when 'END' in a c-program is called.  This is tied to the stop function
and stop will end the program.

---------------------------
Additional notes:  all 4 files are loading into memory via compileOS.sh
In main I'm only launching "fib" file.

To RUN
Xterm console:
		>./compileOS.sh
