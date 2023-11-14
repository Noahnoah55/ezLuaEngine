#include"fetch.hpp"

#include<spdlog/spdlog.h>

#include<emscripten.h>
#include<emscripten/fetch.h>

#include<string>
#include<string.h>
#include<vector>
#include<filesystem>
#include<stdio.h>

void manifest_recieved(emscripten_fetch_t *fetch_result);
void manifest_not_recieved(emscripten_fetch_t *fetch_result);
void fetch_to_fs(emscripten_fetch_t *fetch_result);

std::function<void()> _callback;
int totalFiles = -1;

void fetch_by_manifest(std::string path, std::function<void()> callback) {
    emscripten_fetch_attr_t manifest_attr;
    emscripten_fetch_attr_init(&manifest_attr);

    strcpy(manifest_attr.requestMethod, "GET");
    spdlog::info("Sent fetch request");

    manifest_attr.onsuccess = manifest_recieved;
    manifest_attr.onerror = manifest_not_recieved;

    manifest_attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;

    emscripten_fetch(&manifest_attr, path.c_str());
    _callback = callback;
}

void manifest_recieved(emscripten_fetch_t *fetch_result) {
    std::string manifest(fetch_result->data, fetch_result->numBytes);
    spdlog::info("Manifest Recieved ({} bytes):\n****\n{}\n****", fetch_result->numBytes, manifest);
    emscripten_fetch_attr_t file_attr;
    emscripten_fetch_attr_init(&file_attr);
    file_attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    file_attr.onsuccess = fetch_to_fs;

    int tries = 10;
    std::vector<std::string> files;
    while (!manifest.empty()) {
        tries--;
        if (tries < 0) {
            spdlog::error("Bigtime string processing error in fetch.cpp::manfiest_recieved()");
            break;
        }

        std::string f = manifest.substr(0, manifest.find("\n"));
        if (manifest.find("\n") != manifest.npos) {
            manifest = manifest.substr(manifest.find("\n")+1);
        }
        else {
            manifest = "";
        }
        files.push_back(f);

    }
    for (auto f : files) {
        spdlog::info("Downloading file: ({})", f);
        emscripten_fetch(&file_attr, f.c_str());
    }
    totalFiles = files.size();
}

void manifest_not_recieved(emscripten_fetch_t *fetch_result){
    spdlog::error("Failed to get manifest from {}", fetch_result->url);
}

void report_file() {
    totalFiles--;
    if (totalFiles == 0) _callback();
}

void fetch_to_fs(emscripten_fetch_t *fetch_result) {
    spdlog::info("File ({}) recieved, {} bytes", fetch_result->url, fetch_result->numBytes);
    std::filesystem::path path(fetch_result->url);
    std::filesystem::create_directories(path.parent_path());
    FILE *f = fopen(fetch_result->url, "w");
    if (f == NULL) {
        spdlog::error("Failed to open file ({}), errno == {}", fetch_result->url, errno);
    }
    else {
        int n = fwrite(fetch_result->data, 1, fetch_result->numBytes, f);
        if (n != fetch_result->numBytes) {
            spdlog::error("Short write to ({}), {}/{} bytes", fetch_result->url, n, fetch_result->numBytes);
        }
    }
    fclose(f);
    report_file();
}

void mark_failure(emscripten_fetch_t *fetch_result) {
    spdlog::error("File ({}) failed to fetch, error {}", fetch_result->url, fetch_result->statusText);
    report_file();
}
