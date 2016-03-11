#make image file of a floppy disk
dd if=/dev/zero of=floppya.img bs=512 count=2880
#copy bootload to beginning of floppy.img
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
#load map and directory for file system to kernel.
dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc
#compile kernel
bcc -ansi -c -o kernel.o kernel.c
#assemby kernel.asm and link all files together to produce a file kernel
as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel.o kernel_asm.o
#CMDs below link and make exe fib
#compile and link with fib
#the only assembly function we need is interrupt
#bcc -ansi -c -o fib.o fib.c
#as86 lib.asm -o lib_asm.o
#ld86 -o fib -d fib.o lib_asm.o
#make fib
#sector floppya.img at sector 3
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3
#reac Sector 30
dd if=msg of=floppya.img bs=512 count=1 seek=30 conv=notrunc
