CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17
LDFLAGS  = -lncurses

SRC_DIR  = src
BUILD_DIR = build

# Ejecutables
BINARIES = $(BUILD_DIR)/win $(BUILD_DIR)/file_details $(BUILD_DIR)/make_path

.PHONY: all clean run lib

all: $(BUILD_DIR)/win

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/win: $(SRC_DIR)/UI/win.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

$(BUILD_DIR)/file_details: $(SRC_DIR)/file_details.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

$(BUILD_DIR)/make_path: $(SRC_DIR)/make_path.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)

run: $(BUILD_DIR)/win
	./$(BUILD_DIR)/win

lib: $(BUILD_DIR)/file_details $(BUILD_DIR)/make_path
