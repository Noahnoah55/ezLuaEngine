#include"fetch.hpp"
#include<emscripten/fetch.h>
#include<string>
#include<vector>
#include<cstring>
#include<cstdio>

#include<filesystem>
#include<fstream>
#include<set>
#include<mutex>

std::set<emscripten_fetch_t*> fset;
std::mutex fset_mut;

void save_to_FS(emscripten_fetch_t *fetch) {
    printf("Finished downloading %llu bytes from URL %s.\n", fetch->numBytes, fetch->url);

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
    else {
        printf("Copied file %s to %s\n", fetch->url, path.string().c_str());
    }
    std::lock_guard<std::mutex> lock(fset_mut);
    fset.erase(fetch);
    emscripten_fetch_close(fetch);
}

void report_download_error(emscripten_fetch_t *fetch) {
    printf("Download of %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);
    emscripten_fetch_close(fetch);
}

void _init_filesystem(emscripten_fetch_t *fetch) {

    std::string include;
    include.append(fetch->data, fetch->numBytes);

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
        std::lock_guard l(fset_mut);
        fset.emplace(emscripten_fetch(&attr, s.c_str()));
        pos = newpos+1;
    }
    while (pos < include.length());
    return;
}

void init_filesystem() {
    emscripten_fetch_attr_t includeAttr;
    emscripten_fetch_attr_init(&includeAttr);
    strcpy(includeAttr.requestMethod, "GET");
    includeAttr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    includeAttr.onerror = report_download_error;
    includeAttr.onsuccess = _init_filesystem;
    emscripten_fetch(&includeAttr, "include.txt");
}