CXX      = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O3 -g -MMD -MP -fopenmp
LDFLAGS  = -fopenmp
TARGET   = ray_tracing

SRCS_CC  = $(shell find src/ -name "*.cc")

OBJS     = $(SRCS_CC:.cc=.o)
DEPS     = $(OBJS:.o=.d)

INC_DIRS = $(shell find src/ -type d)
INCLUDES = $(addprefix -I, $(INC_DIRS))

CXXFLAGS += $(INCLUDES)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET) *.ppm *.png

re: clean all

-include $(DEPS)

.PHONY: all clean re
