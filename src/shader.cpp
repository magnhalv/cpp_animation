#define _CRT_SECURE_NO_WARNINGS
#include "shader.h"
#include "glad.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() {
	m_handle = glCreateProgram();
}

Shader::Shader(const std::string& vertex, const std::string& fragment) {
	m_handle = glCreateProgram();
   load(vertex, fragment);
}

Shader::~Shader() {
	glDeleteProgram(m_handle);
}

std::string Shader::read_file(const std::string& path) {
	std::ifstream file;
	file.open(path);
	std::stringstream contents;
	contents << file.rdbuf();
	file.close();
	return contents.str();
}

unsigned int Shader::compile_vertex_shader(const std::string& vertex) {
	unsigned int v_shader = glCreateShader(GL_VERTEX_SHADER);
	const char* v_source = vertex.c_str();
	glShaderSource(v_shader, 1, &v_source, NULL);
	glCompileShader(v_shader);
	int success = 0;
	glGetShaderiv(v_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(v_shader, 512, NULL, infoLog);
		std::cout << "ERROR: Vertex compilation failed.\n";
		std::cout << "\t" << infoLog << "\n";
		glDeleteShader(v_shader);
		return 0;
	};
	return v_shader;
}

unsigned int Shader::compile_fragment_shader(const std::string& fragment) {
	unsigned int f_shader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* f_source = fragment.c_str();
	glShaderSource(f_shader, 1, &f_source, NULL);
	glCompileShader(f_shader);
	int success = 0;
	glGetShaderiv(f_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(f_shader, 512, NULL, infoLog);
		std::cout << "ERROR: Fragment compilation failed.\n";
		std::cout << "\t" << infoLog << "\n";
		glDeleteShader(f_shader);
		return 0;
	};
	return f_shader;
}

bool Shader::link_shaders(unsigned int vertex, unsigned int fragment) {
	glAttachShader(m_handle, vertex);
	glAttachShader(m_handle, fragment);
	glLinkProgram(m_handle);
	int success = 0;
	glGetProgramiv(m_handle, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(m_handle, 512, NULL, infoLog);
		std::cout << "ERROR: Shader linking failed.\n";
		std::cout << "\t" << infoLog << "\n";
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		return false;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return true;
}

void Shader::populate_attributes() {
	int count = -1;
	int length;
	char name[128];
	int size;
	GLenum type;

	glUseProgram(m_handle);
	glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTES, &count);

	for (int i = 0; i < count; ++i) {
		memset(name, 0, sizeof(char) * 128);
		glGetActiveAttrib(m_handle, (GLuint)i, 128, &length, &size, &type, name);
		int attrib = glGetAttribLocation(m_handle, name);
		if (attrib >= 0) {
			m_attributes[name] = attrib;
		}
	}

	glUseProgram(0);
}

void Shader::populate_uniforms() {
	int count = -1;
	int length;
	char name[128];
	int size;
	GLenum type;
	char testName[256];

	glUseProgram(m_handle);
	glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &count);

	for (int i = 0; i < count; ++i) {
		memset(name, 0, sizeof(char) * 128);
		glGetActiveUniform(m_handle, (GLuint)i, 128, &length, &size, &type, name);

		int uniform = glGetUniformLocation(m_handle, name);
		if (uniform >= 0) {
			std::string uniformName = name;
			std::size_t found = uniformName.find('[');
			if (found != std::string::npos) {
				uniformName.erase(uniformName.begin() + found, uniformName.end());
				// Populate subscripted names too
				unsigned int uniformIndex = 0;
				while (true) {
					memset(testName, 0, sizeof(char) * 256);
					sprintf(testName, "%s[%d]", uniformName.c_str(), uniformIndex++);
					int uniformLocation = glGetUniformLocation(m_handle, testName);
					if (uniformLocation < 0) {
						break;
					}
					m_uniforms[testName] = uniformLocation;
				}
			}
			m_uniforms[uniformName] = uniform;
		}
	}

	glUseProgram(0);
}

void Shader::load(const std::string& vertex, const std::string& fragment) {
	std::ifstream f(vertex.c_str());
	bool vertFile = f.good();
	f.close();

	f = std::ifstream(vertex.c_str());
	bool fragFile = f.good();
	f.close();

	std::string v_source = vertex;
	if (vertFile) {
		v_source = read_file(vertex);
	}

	std::string f_source = fragment;
	if (fragFile) {
		f_source = read_file(fragment);
	}

	unsigned int v_shader = compile_vertex_shader(v_source);
	unsigned int f_shader = compile_fragment_shader(f_source);
	if (link_shaders(v_shader, f_shader)) {
		populate_attributes();
		populate_uniforms();
	}
}

void Shader::bind() {
	glUseProgram(m_handle);
}

void Shader::unbind() {
	glUseProgram(0);
}

unsigned int Shader::get_handle() {
	return m_handle;
}

unsigned int Shader::get_attribute(const std::string& name) {
	std::map<std::string, unsigned int>::iterator it = m_attributes.find(name);
	if (it == m_attributes.end()) {
		std::cout << "Retrieving bad attribute index: " << name << "\n";
		return 0;
	}
	return it->second;
}

unsigned int Shader::get_uniform(const std::string& name) {
	std::map<std::string, unsigned int>::iterator it = m_uniforms.find(name);
	if (it == m_uniforms.end()) {
		std::cout << "Retrieving bad uniform index: " << name << "\n";
		return 0;
	}
	return it->second;
}
