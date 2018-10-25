# compile macro
CPPFLAGS    = -O2 -Wall

COMPILER 	= g++
LINKER 		= g++

TARGET		= compress.exe
OBJECTS		= src\Compress.o src\BitSet.o src\BitStream.o src\LZCoder.o


$(TARGET): $(OBJECTS)
	$(LINKER) $(CPPFLAGS) $(OBJECTS) -o $(TARGET)


$(OBJECTS): $$(@R).cpp
	$(COMPILER) $(CPPFLAGS) -c $(@R).cpp -o $@


.PHONY: clean
clean:
	-del $(TARGET)
	-del $(OBJECTS)
