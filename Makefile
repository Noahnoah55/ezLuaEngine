BUILD = build
SRC = src
SRCS = $(wildcard $(SRC)/*.cpp)
HEADERS = $(wildcard $(SRC)/*.hpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
HTML = $(SRC)/shell.html

CXX = em++
CC = emcc
C_COMPILEFLAGS = -sNO_DISABLE_EXCEPTION_CATCHING -g3
CXX_COMPILEFLAGS = -sNO_DISABLE_EXCEPTION_CATCHING -std=c++17 -g -Ilua -Isol2/include -g3
LINKFLAGS = --use-preload-plugins -sFETCH -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS='["bmp", "png"]' -sUSE_SDL_TTF=2 -sUSE_SDL_MIXER=2 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 --emrun --shell-file $(HTML) -g3 --preload-file examples/opengl-simple@/


LUA_SRC = $(shell ls ./lua/*.c | grep -v "luac.c" | grep -v "lua.c" | tr "\n" " ")
LUA_A = liblua.a

ENGINE = $(BUILD)/game.html

GAME_FILES = game-files

all: $(ENGINE) $(COPY_DEST)

run: all
	cp -r examples/hello-world/* build/
	cd build; \
		find assets/ src/ -type f > include.txt
	emrun $(ENGINE)

$(ENGINE): $(OBJS) $(LUA_A) $(HTML)
	mkdir -p $(BUILD)
	$(CXX) $(LUA_A) $(OBJS) -o $(ENGINE) $(LINKFLAGS)

$(OBJS): %.o : %.cpp $(HEADERS)
	$(CXX) -c $(CXX_COMPILEFLAGS) $< -o $@

$(LUA_A): $(LUA_SRC)
	$(CC) $(LUA_SRC) -c $(C_COMPILEFLAGS)
	emar qc $(LUA_A) *.o
	emranlib $(LUA_A)
	rm *.o

clean:
	rm -rf build
	rm -rf *.o
	rm -rf $(SRC)/*.o
	rm -rf *.a