run: output
	./output

output: rspsa.o
	g++ rspsa.o -o output

rspsa.o: rspsa.cpp rspsa.h
	g++ -c rspsa.cpp

clean: 
	-rm *.o output
