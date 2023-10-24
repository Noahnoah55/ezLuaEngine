BUILD = build
SRC = src
SRCS = $(SRC)/*.cpp
HTML = $(SRC)/shell.html

CXX = em++
CC = emcc
C_COMPILEFLAGS = -sNO_DISABLE_EXCEPTION_CATCHING -g
CXX_COMPILEFLAGS = -sNO_DISABLE_EXCEPTION_CATCHING -std=c++17 -g
LINKFLAGS = --use-preload-plugins -sFETCH -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2 -sUSE_SDL_MIXER=2 -Ilua -Isol2/include --emrun --shell-file $(HTML)
CXXFLAGS = $(LINKFLAGS) $(CXX_COMPILEFLAGS)


LUA_SRC = $(shell ls ./lua/*.c | grep -v "luac.c" | grep -v "lua.c" | tr "\n" " ")
LUA_A = liblua.a

ENGINE = $(BUILD)/game.html

GAME_FILES = game-files

all: $(ENGINE)

run-sample: all
	cp -r examples/hello-world/* build/
	emrun $(ENGINE)

$(ENGINE): $(SRCS) $(LUA_A) $(BUILD) $(HTML)
	$(CXX) $(LUA_A) $(SRCS) -o $(ENGINE) $(CXXFLAGS)

$(LUA_A): $(LUA_SRC)
	$(CC) $(LUA_SRC) -c $(C_COMPILEFLAGS)
	emar qc $(LUA_A) *.o
	emranlib $(LUA_A)
	rm *.o


$(BUILD):
	mkdir $(BUILD)

clean:
	rm -rf build
	rm -rf *.o
	rm -rf *.a