#pragma once

#include<string>
using namespace std;

namespace ezlua {
    class shader_program {
        public:
            unsigned int id;

            shader_program(const char* vertex_path, const char* fragment_path);
            void use();

            void setBool(string name, bool value);
            void setInt(string name, int value);
            void setFloat(string name, float value);
    };
}