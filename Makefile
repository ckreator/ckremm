# make dummy 'all' target
all:
	

# build headers
headers: src/headers/headers.c src/headers/headers.h
	gcc -o bin/headers src/headers/headers.c

# build tables
tables: ./src/tables/tables.c ./src/tables/tables.h
	gcc -o bin/tables src/tables/tables.c

# testing headers
test_headers: tmp/headers.o tmp/headers_test.o
	gcc -o bin/test_headers tmp/headers_test.o tmp/headers.o

# testing tables
test_tables: tmp/tables.o tmp/tables_test.o
	gcc -o bin/test_tables tmp/tables_test.o tmp/tables.o

######################
# build object files
######################
tmp/headers.o: src/headers/headers.c src/headers/headers.h
	gcc -c src/headers/headers.c -o tmp/headers.o

tmp/tables.o: src/tables/tables.c src/tables/tables.h
	gcc -c src/tables/tables.c -o tmp/tables.o

tmp/headers_test.o: test/headers/test.c src/headers/headers.h
	gcc -c test/headers/test.c -o tmp/headers_test.o

tmp/tables_test.o: test/tables/test.c src/tables/tables.h
	gcc -c test/tables/test.c -o tmp/tables_test.o

clean:
	rm -rf *.o
