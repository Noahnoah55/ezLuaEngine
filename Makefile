CXX = em++
COMPILEFLAGS = -sNO_DISABLE_EXCEPTION_CATCHING -std=c++17
LINKFLAGS = --use-preload-plugins -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -Ilua -Isol2/include --preload-file $(GAME_FILES)
CXXFLAGS = $(LINKFLAGS) $(COMPILEFLAGS)
BUILD = build
SRC = src

SRCS = $(SRC)/*.cpp

LUA_SRC = $(shell ls ./lua/*.c | grep -v "luac.c" | grep -v "lua.c" | tr "\n" " ")
LUA_A = liblua.a

ENGINE = $(BUILD)/page.js
PAGE = $(BUILD)/index.html

GAME_FILES = game-files

all: $(ENGINE) $(PAGE)

run: all
	emrun $(PAGE)

$(PAGE): $(SRC)/index.html $(BUILD)
	cp $(SRC)/index.html $(PAGE)

$(ENGINE): $(SRCS) $(BUILD) $(LUA_A) $(GAME_CODE)/*
	$(CXX) $(LUA_A) $(SRCS) -o $(ENGINE) $(CXXFLAGS)

$(LUA_A): $(LUA_SRC)
	$(CXX) $(LUA_SRC) -c $(CXXFLAGS)
	emar qc $(LUA_A) *.o
	emranlib $(LUA_A)
	rm *.o


$(BUILD):
	mkdir $(BUILD)

clean:
	rm -rf build
	rm -rf *.o
	rm -rf *.a