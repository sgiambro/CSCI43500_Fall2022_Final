Final: Final.o
	g++ -g Final.o -o Final
Final.o: Final.cpp
	g++ -c Final.cpp
clean:
	rm -f *.o
run: Final
	./Final RawImages/??.??.??.?.????copy.raw