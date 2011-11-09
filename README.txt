I tried to build again the executable using Cygwin and mingw and it was working, miracle, the code is 10 years old ! [I had to install mingw-gcc-g++ for Cygwin (apt-cyg install mingw-gcc-g++), fixing Allegro includes (version 4.4.2) for mingw (./fix.bat mingw32) and add them to the sources, build liballeg.a and alleg42.dll (modified for palette fix under Windows seven see: http://www.allegro.cc/forums/thread/603939/863411#target), add some mingw dll].

You can find in the exemple Makefile what you need to build the game, all you need is to change the libs and include paths to fit your system:

LIBS =  -L"C:\Users\tony\C++\Mayhem2" -lalleg -luser32 -lgdi32 -lopengl32 -lglu32 -mwindows
CXXINCS =  -I"C:\cygwin\usr\i686-pc-mingw32\sys-root\mingw\include" -I"C:\Users\tony\C++\Mayhem2\allegro442_include"

Then:

$ make
$ ./Mayhem2.exe

Enjoy !