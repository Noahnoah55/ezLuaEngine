CXX = em++
CC = emcc
CXXFLAGS = -s USE_SDL=2
BUILD = build
SRC = src

SRCS = $(SRC)/*.cpp

LUA_SRC = $(shell ls ./lua/*.c | grep -v "luac.c" | grep -v "lua.c" | tr "\n" " ")
LUA_A = liblua.a

ENGINE = $(BUILD)/page.js
PAGE = $(BUILD)/index.html

all: $(ENGINE) $(PAGE) $(BUILD)/main.lua

run: all
	emrun $(PAGE)

$(PAGE): $(SRC)/index.html $(BUILD)
	cp $(SRC)/index.html $(PAGE)

$(ENGINE): $(SRCS) $(BUILD) $(LUA_A)
	$(CXX) $(LUA_A) $(SRCS) -o $(ENGINE) $(CXXFLAGS)
	rm *.o

$(LUA_A): $(LUA_SRC)
	$(CC) $(LUA_SRC) -c
	emar qc $(LUA_A) *.o
	emranlib $(LUA_A)
	rm *.o

$(BUILD)/main.lua: $(BUILD)
	cp testgame/main.lua $(BUILD)/main.lua

$(BUILD):
	mkdir $(BUILD)

clean:
	rm -rf build
	rm -rf *.o
	rm -rf *.a