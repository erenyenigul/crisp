#discuss
CXX       := gcc

SRC     := .
INCLUDE := include

LIBRARIES   :=
EXECUTABLE  := main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.c
	$(CXX) -g crisp.c parser.c list.c scanner.c -o crisp

clean:
	-rm $(BIN)/*