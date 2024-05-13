CXX = g++

CXXFLAGS = -std=c++11

EXECUTABLE = TearminalGame

SRCS = main.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXECUTABLE)