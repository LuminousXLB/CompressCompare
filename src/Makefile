# compile macro
CPPFLAGS    = -O2 -Wall

COMPILER 	= g++
LINKER 		= g++

TARGET		= compress.exe
OBJECTS		= Compress.o BitSet.o BitStream.o LZCoder.o



$(TARGET): $(SRC_OBJECTS)
	$(LINKER) $(CPPFLAGS) $(SRC_OBJECTS) -o $(TARGET)


{src\}.cpp{src\}.o:
	$(COMPILER) $(CFLAGS) -c $< -o $@


# $(SRC_OBJECTS):
# 	$(COMPILER) $(CPPFLAGS) -c $(@B).cpp -o $@




.PHONY: clean
clean: cleansubdirs
	del $(TARGET) $(OBJECTS)

cleansubdirs:
	del $(SRC_OBJECTS)



# %.o: %.cpp
# 	$(COMPILER) $(CFLAGS) -c $< -o $@

# %.o: %.cc
# 	$(COMPILER) $(CFLAGS) -c $< -o $@


