#pragma once

#include<string>
using namespace std;

namespace ezlua {
    class shader_program {
        public:
            unsigned int id;

            shader_program();
            shader_program(string vertex_path, string fragment_path);
            void use();

            void setBool(string name, bool value);
            void setInt(string name, int value);
            void setFloat(string name, float value);
    };
}