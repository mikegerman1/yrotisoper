default:
	echo "you need to either pick 'WIN' or 'LIN' option to compile"

WIN:
	x86_64-w64-mingw32-gcc main.c -D WIN_COMP -o prog.exe
	wine ./prog.exe

LIN:
	gcc main.c -D LIN_COMP -o prog.out
	./prog.out
