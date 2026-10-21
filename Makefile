CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g

TARGET = calculator
OBJS = main.o calculator.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp calculator.h
	$(CXX) $(CXXFLAGS) -c main.cpp

calculator.o: calculator.cpp calculator.h
	$(CXX) $(CXXFLAGS) -c calculator.cpp

run: $(TARGET)
	./$(TARGET) tests.txt

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all run clean
