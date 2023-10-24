#include"fetch.hpp"

#include<emscripten/fetch.h>
#include<string>
#include<vector>
#include<cstring>
#include<cstdio>
#include"singletons.hpp"

void save_to_FS(emscripten_fetch_t *fetch) {
    printf("Finished downloading %llu bytes from URL %s.\n", fetch->numBytes, fetch->url);
    //printf("----FILE----\n%.*s\n----EOF----\n", (int)fetch->numBytes, fetch->data);
    auto file = fopen("/main.lua", "w");
    if (file != NULL) {
        fwrite(fetch->data, sizeof(char), fetch->numBytes, file);
        fclose(file);
    }
    else {
        printf("Failed to open %s\n", fetch->url);
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
    printf("%s",include.c_str());

    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onerror = report_download_error;
    attr.onsuccess = save_to_FS;
    std::vector<std::string> lines;
    int pos = 0;
    int newpos = include.find('\n', pos);

    printf("%d, %d, %d\n", pos, newpos, (int)include.length());
    
    pthread_exit(NULL);
}

void init_filesystem() {
    pthread_t dl;
    pthread_create(&dl, NULL, _init_filesystem, NULL);
}