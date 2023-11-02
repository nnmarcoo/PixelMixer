CXX = g++
CXXFLAGS_DEBUG = -g -O0 -DDEBUG $(shell wx-config --cxxflags)
CXXFLAGS_RELEASE = -O2 -DNDEBUG $(shell wx-config --cxxflags)
LDFLAGS = $(shell wx-config --libs gl,core,base) -lGLEW -lGL -lGLU -lglut
SOURCES = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard src/vendor/stb_image/*.cpp)
OBJ_DIR = obj
OBJECTS = $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
TARGET_DEBUG = PixelMixer_debug
TARGET_RELEASE = PixelMixer

all: release

debug: CXXFLAGS += $(CXXFLAGS_DEBUG)
debug: $(TARGET_DEBUG)

release: CXXFLAGS += $(CXXFLAGS_RELEASE)
release: $(TARGET_RELEASE)

$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: src/vendor/stb_image/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET_DEBUG) $(TARGET_RELEASE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET_DEBUG) $(TARGET_RELEASE)

