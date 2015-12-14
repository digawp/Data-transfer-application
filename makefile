CXX = g++
RM = rm -f
CXXFLAGS = -O3 -std=c++11
DFLAGS = -W -Wall -Wextra -Werror -Wunused -Wshadow -Weffc++ -ansi -pedantic -g -std=c++11
LFLAGS = -lpthread -lboost_system -lboost_filesystem
SOURCES1 = ./client/client.cpp
SOURCES2 = ./server/server.cpp
OBJECTS1 = $(SOURCES1:.cpp=.o)
OBJECTS2 = $(SOURCES2:.cpp=.o)
PATH1 = ./client
PATH2 = ./server
BIN1 = client.exe
BIN2 = server.exe

build: $(OBJECTS1) $(OBJECTS2)
	$(CXX) $(OBJECTS1) -o $(addprefix $(PATH1)/, $(BIN1)) $(CXXFLAGS) $(LFLAGS)
	$(CXX) $(OBJECTS2) -o $(addprefix $(PATH2)/, $(BIN2)) $(CXXFLAGS) $(LFLAGS)

debug: $(OBJECTS1) $(OBJECTS2)
	$(CXX) $(OBJECTS1) -o $(addprefix $(PATH1)/, $(BIN1)) $(DFLAGS) $(LFLAGS)
	$(CXX) $(OBJECTS2) -o $(addprefix $(PATH2)/, $(BIN2)) $(DFLAGS) $(LFLAGS)

clean:
	$(RM) $(OBJECTS1) $(OBJECTS2) $(addprefix $(PATH1)/, $(BIN1)) $(addprefix $(PATH2)/, $(BIN2)) *~

rebuild: clean build

.PHONY: build debug clean rebuild