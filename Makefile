CXX = g++-5
CXXFLAGS = -std=c++14 -g
EXEC = watan
OBJECTS = main.o Board.o Die.o FileIO.o HexagonBoard.o Player.o Tile.o UserIO.o PlayerManager.o Subject.o Goose.o

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

.PHONY: clean

clean: 
	rm ${OBJECTS} ${EXEC}
