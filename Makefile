CC = em++
CFLAGS = -s USE_SDL=2
BUILD = build
SRC = src

JS = $(BUILD)/page.js
PAGE = $(BUILD)/index.html
FENG = $(BUILD)/fengari-web.js
FENG_URL = https://github.com/fengari-lua/fengari-web/releases/download/v0.1.4/fengari-web.js 

all: $(JS) $(PAGE) $(BUILD)/luastate.js

run: all
	emrun $(PAGE)

$(BUILD)/luastate.js: $(SRC)/luastate.js
	cp $(SRC)/luastate.js $(BUILD)/luastate.js

$(PAGE): $(SRC)/index.html
	cp $(SRC)/index.html $(PAGE)

$(JS): $(SRC)/main.cpp $(FENG)
	em++ $(SRC)/main.cpp -o $(JS) $(CFLAGS)

$(FENG):
	curl -L -o $(FENG) $(FENG_URL)

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm build/*