CCFLAGS=/nologo /W4 /ZI /MD /Od
CCLINK=Gdi32.lib opengl32.lib User32.lib Shell32.lib
EXENAME=pacman


all:
	IF NOT EXIST obj MKDIR obj
	IF NOT EXIST bin MKDIR bin
	cl /Fe$(EXENAME).exe $(CCFLAGS) src\*.cpp src\glad\glad.c /Fo.\obj\ $(CCLINK)
	IF EXIST *.exe MOVE *.exe bin
	IF EXIST *.idb MOVE *.idb bin
	IF EXIST *.ilk MOVE *.ilk bin
	IF EXIST *.pdb MOVE *.pdb bin
