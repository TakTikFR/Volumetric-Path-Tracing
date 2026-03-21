CXX      = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O3 -g -MMD -MP -fopenmp
LDFLAGS  = -fopenmp
TARGET   = ray_tracing

SRCS_CC  = $(wildcard *.cc)
SRCS_CPP = $(wildcard *.cpp)

OBJS     = $(SRCS_CC:.cc=.o) $(SRCS_CPP:.cpp=.o)

DEPS     = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET) *.ppm *.png

re: clean all

-include $(DEPS)

.PHONY: all clean re
