
CXX = g++
CXXFLAGS = -Wall -g

INCLUDES = -I.
LFLAGS = -L../abc
LIBS = -labc
FLAGS = -lm -ldl -rdynamic -lreadline -ltermcap -lpthread
SRCS = gate/gate.cpp circuit/circuit.cpp IFS/ifs.cpp IFS/myHash.cpp project/main.cpp
OBJS = $(SRCS:.cpp=.o)
MAIN = IF_Searcher
vpath gate/gate.% src/gate
vpath circuit/circuit.% src/circuit
vpath IFS/ifs.% src/IFS
vpath IFS/myHash.% src/IFS
vpath project/main.cpp src/project
vpath lib%.a abc

.PHONY: all clean

all: clean $(MAIN)

$(MAIN): $(OBJS)
	#$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(MAIN).o $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS) $(FLAGS)


#circuit.o: circuit.c

#gate.o: gate.c gate.h
	#$(CXX) $(CXXFLAGS) $(INCLUDES) -o 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	${RM} ${OBJS} ${MAIN}
