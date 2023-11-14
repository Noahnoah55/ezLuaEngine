#pragma once

#include<functional>
#include<string>

/// @brief Fetch a .txt manfiest file listing all the files to preload
/// @param path path to manifest.txt
/// @param callback function to call when download is finished
void fetch_by_manifest(std::string path, std::function<void()> callback);