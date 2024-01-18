CC = g++
MAIN = ./src/main.cpp
QVM_SOURCE = ./src/sources/QVM.cpp ./src/sources/Registers.cpp ./src/sources/Memory.cpp ./src/sources/QPU.cpp ./src/sources/Qubit.cpp
NAME = qvm

make:
	$(CC) $(MAIN) $(QVM_SOURCE) -o $(NAME)

test:
	$(CC) $(MAIN) $(QVM_SOURCE) -o $(NAME)
	./$(NAME) -i ./test.qasm
	rm -f $(NAME)

clean:
	rm -f $(NAME)