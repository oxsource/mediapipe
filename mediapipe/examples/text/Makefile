CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -pthread

INCLUDES := -I./gcc/include

LIB_DIR := ./gcc/lib
LIBS := -L$(LIB_DIR) -ltext_service_api

TARGET := string_join_service
SRCS := string_join_service.cc

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LIBS)

run:
	chmod +x ./$(TARGET)
	LD_LIBRARY_PATH=$(LIB_DIR) ./$(TARGET)

sync:
	cp ./service/text_service.h ./gcc/include/mediapipe/examples/text/service/
	cp //bazel-bin/mediapipe/examples/text/service/libtext_service_api.so ./gcc/lib/

clean:
	rm -f $(TARGET)
