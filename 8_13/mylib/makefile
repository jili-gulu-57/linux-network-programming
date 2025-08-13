libmyc.so:mymath.o mystdio.o 	
	gcc -o $@ $^ -shared

%.o:%.c     
	gcc -fPIC -c $< 

.PHONY:clean
clean:
	rm -rf *.so *.o mylibc *.tgz

.PHONY:output
output:
	mkdir mylibc
	mkdir -p mylibc/include
	mkdir -p mylibc/lib 
	cp *.h mylibc/include
	cp *.so mylibc/lib 
	tar czf mylibc.tgz mylibc


