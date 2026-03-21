CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O0 -g -MMD -MP
TARGET   = ray_tracing

SRCS_CC  = $(wildcard *.cc)
SRCS_CPP = $(wildcard *.cpp)

OBJS     = $(SRCS_CC:.cc=.o) $(SRCS_CPP:.cpp=.o)

DEPS     = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET) *.ppm *.png

re: clean all

-include $(DEPS)

.PHONY: all clean re
