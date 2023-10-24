#pragma once
#include<string>
#include<emscripten/fetch.h>

emscripten_fetch_t* download_file_to_FS(std::string path);

void init_filesystem();