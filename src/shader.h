#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <string>

class Shader {
    private:
        unsigned int m_handle;

        std::map<std::string, unsigned int> m_attributes;
        std::map<std::string, unsigned int> m_uniforms;

        Shader(const Shader&);
        Shader& operator=(const Shader&);

        std::string read_file(const std::string& path);
        unsigned int compile_vertex_shader(const std::string& vertex);
        unsigned int compile_fragment_shader(const std::string& fragment);

        bool link_shaders(unsigned int vertex, unsigned int fragment);

        void populate_attributes();
        void populate_uniforms();

    public:
        Shader();
        Shader(const std::string& vertex, const std::string& fragment);
        ~Shader();

        void load(const std::string& vertex, const std::string& fragment);
        void bind();
        void unbind();

        unsigned int get_attribute(const std::string& name);
        unsigned int get_uniform(const std::string& name);
        unsigned int get_handle();
};

#endif