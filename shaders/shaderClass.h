#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
    public:
        unsigned int ID;
        unsigned int viewProjBlockIndex;
        Shader(const char* vertexPath, const char* fragmentPath){
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;

            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            try{
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);

                std::stringstream vShaderStream, fShaderStream;
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();

                vShaderFile.close();
                fShaderFile.close();
                
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }catch(std::ifstream::failure err){
                std::cout << "Failed to read shaders: " << std::endl;
            }
            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vShader, 1, &vShaderCode, NULL);
            glCompileShader(vShader);
            checkCompileErrors(vShader, "VERTEX");

            unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fShader, 1, &fShaderCode, NULL);
            glCompileShader(fShader);
            checkCompileErrors(fShader, "FRAGMENT");

            ID = glCreateProgram();
            glAttachShader(ID, vShader);
            glAttachShader(ID, fShader);
            glLinkProgram(ID);
            checkCompileErrors(ID, "PROGRAM");

            glDeleteShader(vShader);
            glDeleteShader(fShader);
            viewProjBlockIndex = glGetUniformBlockIndex(ID, "ViewProjMatrix");
        }

        void use(){
            glUseProgram(ID);

            glUniform3fv(glGetUniformLocation(ID, "material.ambient"), 1, glm::value_ptr(glm::vec3(1.0)));
            glUniform3fv(glGetUniformLocation(ID, "material.diffuse"), 1, glm::value_ptr(glm::vec3(1.0)));
            glUniform3fv(glGetUniformLocation(ID, "material.specular"), 1, glm::value_ptr(glm::vec3(1.0)));
            glUniform1f(glGetUniformLocation(ID, "material.shining"), 64.0);
        }

        void set1f(const char* name, float value) {
            glUniform1f(glGetUniformLocation(ID, name), value);
        }

        void set1b(const char* name, bool value) {
            glUniform1i(glGetUniformLocation(ID, name), value);
        }

        void set1i(const char* name, int value) {
            glUniform1i(glGetUniformLocation(ID, name), value);
        }

        void set3fv(const char* name, glm::vec3 value) {
            glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
        }

        void set4fv(const char* name, glm::vec4 value) {
            glUniform4fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
        }

        void setMatrix3fv(const char* name, glm::mat3 value) {
            glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
        }

        void setMatrix4fv(const char* name, glm::mat4 value) {
            glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
        }


    private:
        void checkCompileErrors(unsigned int shader, std::string type){
            int success;
            char infolog[1024];
            if(type != "PROGRAM"){
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if(!success){
		    glGetShaderInfoLog(shader, 1024, NULL, infolog);
                    std::cout << "Failed to compile shader: " << type << "\n" << infolog << std::endl;
                }
            }else{
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if(!success){
		    glGetProgramInfoLog(shader, 1024, NULL, infolog);
                    std::cout << "Shader program linking error: " << type << "\n" << infolog << std::endl;
                }
            }
        }
};
#endif