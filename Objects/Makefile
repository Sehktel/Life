CC = g++

ABC_SRC = abc
ABC_BIN = abc

.PHONNY: all
# , abc1, abc2, abc3, abc4, abc5

clear: clean

clean:
	rm abc1 abc2 abc3 abc4 abc5

all: abc1 abc2 abc3 abc4 abc5

abc1:
	$(CC) ./$(ABC_SRC)1.cpp -o ./$(ABC_BIN)1
	./$(ABC_BIN)1
abc2:
	$(CC) ./$(ABC_SRC)2.cpp -o ./$(ABC_BIN)2
	./$(ABC_BIN)2
abc3:
	$(CC) ./$(ABC_SRC)3.cpp -o ./$(ABC_BIN)3
	./$(ABC_BIN)3
abc4:
	$(CC) ./$(ABC_SRC)4.cpp s_A.cpp s_B.cpp -o ./$(ABC_BIN)4
	./$(ABC_BIN)4
abc5:
	$(CC)  -c ./s_A.cpp
	$(CC)  -c ./s_B.cpp
	$(CC) -o ./$(ABC_BIN)5 ./$(ABC_SRC)5.cpp   s_A.o    s_B.o
	./$(ABC_BIN)5
