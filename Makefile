CXX := g++
CXXFLAGS := -std=gnu++17 -O3 -pedantic -Wall -Wextra
DCXXFLAGS := $(CXXFLAGS) -g -O0 -DDEBUG
CXXFLAGS += -O3

TARGET := reflexview

SRCS := $(wildcard *.cpp)
builddir := build
debugdir := build/debug
OBJS := $(SRCS:%.cpp=$(builddir)/%.o)
DOBJS := $(SRCS:%.cpp=$(debugdir)/%.o)
DTARGET := $(TARGET)-debug

all: $(TARGET)

debug: $(DTARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(builddir)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DTARGET): $(DOBJS)
	$(CXX) $(DCXXFLAGS) -o $@ $^

$(debugdir)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(DCXXFLAGS) -c $< -o $@


clean:
	rm -rfv $(TARGET) $(DTARGET) $(debugdir) $(builddir)

