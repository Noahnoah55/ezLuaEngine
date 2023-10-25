#include"fetch.hpp"
#include"singletons.hpp"

#include<emscripten/fetch.h>
#include<string>
#include<vector>
#include<cstring>
#include<cstdio>

#include<filesystem>
#include<fstream>

void save_to_FS(emscripten_fetch_t *fetch) {
    printf("Finished downloading %llu bytes from URL %s.\n", fetch->numBytes, fetch->url);
    //printf("----FILE----\n%.*s\n----EOF----\n", (int)fetch->numBytes, fetch->data);

    auto path = std::filesystem::path(fetch->url);
    if (!std::filesystem::exists(path.parent_path())) {
        std::filesystem::create_directories(path.parent_path());
    }
    auto f = std::ofstream(path);
    f.write(fetch->data, fetch->numBytes);
    f.close();
    if (f.fail()) {
        printf("Failed to copy file %s to %s\n", fetch->url, path.string().c_str());
    }
    emscripten_fetch_close(fetch);
}

void report_download_error(emscripten_fetch_t *fetch) {
    printf("Download of %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);
    emscripten_fetch_close(fetch);
}

emscripten_fetch_t* download_file_to_FS(std::string path) {
    printf("Starting download of %s\n", path.c_str());
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = save_to_FS;
    attr.onerror = report_download_error;
    return emscripten_fetch(&attr, path.c_str());
}

void *_init_filesystem(void *a) {
    emscripten_fetch_attr_t includeAttr;
    emscripten_fetch_attr_init(&includeAttr);
    strcpy(includeAttr.requestMethod, "GET");
    includeAttr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY | EMSCRIPTEN_FETCH_SYNCHRONOUS;
    includeAttr.onerror = report_download_error;
    auto f = emscripten_fetch(&includeAttr, "include.txt");
    if (f->status != 200) {
        download_done = -1;
        pthread_exit(NULL);
    }

    std::string include;
    include.append(f->data, f->numBytes);

    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onerror = report_download_error;
    attr.onsuccess = save_to_FS;
    std::vector<std::string> lines;
    int pos = 0;
    int newpos = 0;
    do {
        newpos = include.find('\n', pos);
        auto s = include.substr(pos, newpos-pos);
        emscripten_fetch(&attr, s.c_str());
        pos = newpos+1;
    }
    while (pos < include.length());
    
    return NULL;
}

void init_filesystem() {
    pthread_t dl;
    pthread_create(&dl, NULL, _init_filesystem, NULL);
}