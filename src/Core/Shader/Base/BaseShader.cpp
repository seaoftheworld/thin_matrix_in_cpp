#include "BaseShader.h"

#include <string>   // for std::getLine() and strings
#include <fstream>  // for reading shader file, texture file
#include <iostream>

int BaseShader::initShader(const char *path, GLuint *id, GLenum type) {
    std::ifstream filePath;
    std::string source = "", line;
    GLuint shaderId;

    // Read Vertex Shader source code from the file and
    // create vsh-id if successs.
    filePath.open(path);
    if (!filePath.is_open()) {
        printf("Fail to find shader file: %s\n", path);
        return 0;
    }

    printf("found file: %s\n", path);

    while (std::getline(filePath, line)) {
        source += line + "\n";
    }
    filePath.close();

    shaderId = glCreateShader(type);
    if (!shaderId) {
        printf("glCreateShader(): Failed to generate shader id for \'%s\' file!\n", path);
        return 0;
    }

    // Compile the Shader and check the result
    const GLchar *sh_source = source.c_str();
    glShaderSource(shaderId, 1, &sh_source, NULL);

    glCompileShader(shaderId); 

    GLint Result; {
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &Result);

        if (!Result) {

            std::string log;
            GLint logSize;

            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
            log.resize(logSize);
            log.reserve(logSize);
            glGetShaderInfoLog(shaderId, logSize, NULL, (char *)log.data());

            printf("shader Error:\n%s\n", log.c_str());

            glDeleteShader(shaderId);
            return 0;
        }
    }

    if (id) {
        *id = shaderId;
    }
    return 1;
}

int BaseShader::initProg(GLuint vsh, GLuint fsh, GLuint *prog) {
    if (!prog) {
        return 0;
    }

    *prog = glCreateProgram();
    if(*prog == 0) {
        printf("glCreateProgram(): failed to create program !\n");
        return 0;
    }

    // Link and make sure the progrma linked correctly
    glAttachShader(*prog, vsh);
    glAttachShader(*prog, fsh);
    glLinkProgram(*prog);

    GLint linkStatus; {

        glGetProgramiv(*prog, GL_LINK_STATUS, &linkStatus);

        if (linkStatus == GL_FALSE) {
            std::string log;
            GLint logSize;

            glGetProgramiv(*prog, GL_INFO_LOG_LENGTH, &logSize);
            log.resize(logSize);
            log.reserve(logSize);

            glGetProgramInfoLog(*prog, logSize, NULL, (char *)log.data());
            printf("glLinkProgram() Failed: \n%s\n", log.c_str());
            
            // status = link_prog_failed;  // status assigned value according to returned value
            glDeleteProgram(*prog);
            return 0;
        }
    }

    printf("  gl prog generated, id: \'%d'\n", *prog);

    glDetachShader(*prog, vsh);
    glDetachShader(*prog, fsh);
    glDeleteShader(vsh);
    glDeleteShader(fsh);

    // status = link_prog_passed;  // status assigned value according to returned value
    return 1;
}

// unsigned int BaseShader::load(const char *shaderName) {
//     std::string name(shaderName);
//     GLuint vsh, fsh, prog;
    
//     std::string vshPath = "data/shaders/" + name + ".vsh";
//     std::string fshPath = "data/shaders/" + name + ".fsh";
//         // std::string vshPath = "./data/shaders";
//         // vshPath.append(shaderName);

//     if (!initShader(vshPath.c_str(), &vsh, GL_VERTEX_SHADER)) {
//         return -1;
//     }
//     if (!initShader(fshPath.c_str(), &fsh, GL_FRAGMENT_SHADER)) {
//         return -1;
//     }
//     if (!initProg(vsh, fsh, &prog)) {
//         return -1;
//     }

//     return (unsigned int)prog;
// }

void BaseShader::unloadProg() {
    if (progId >= 0) {
        printf("  gl prog: %d will be deleted.\n\n", progId);
        glDeleteProgram(progId);
        progId = -1;

        status = deleted_after_linked;
    }
    else {
        printf("  gl prog: %d < 0, dont have to deleted anymore.\n\n", progId);
        progId--;  // minus value indicates how many times unloadProg() has been called.
    }
}

// int BaseShader::loadProg(const char *vshPath, const char *fshPath) {
//     GLuint vsh, fsh, prog;

//     // TODO: Shall delete previous steps' result on failure !!!
//     //       not only returning !!!

//     // printf("11111111\n");
//     if (!initShader(vshPath, &vsh, GL_VERTEX_SHADER)) {
//         return -3;
//     }
//     // printf("22222222\n");
//     if (!initShader(fshPath, &fsh, GL_FRAGMENT_SHADER)) {
//         return -2;
//     }

//     {// This shall be called before linking shader-prog
//         // bindAllAttributeLocations();
//     }

//     // printf("3333333\n");
//     if (!initProg(vsh, fsh, &prog)) {
//         return -1;
//     }

//     {// This shall be called after linking shader-prog
//         // getAllUniformLocations();
//     }

//     return (int)prog;
// }

// int BaseShader::initShaders(const char *vshPath, const char *fshPath) {

//     // TODO: Shall delete previous steps' result on failure !!!
//     //       not only returning !!!

//     bindAllAttributeLocations();

//     // printf("11111111\n");
//     if (!initShader(vshPath, &vsh, GL_VERTEX_SHADER)) {
//         return -1;
//     }
//     // printf("22222222\n");
//     if (!initShader(fshPath, &fsh, GL_FRAGMENT_SHADER)) {
//         return -1;
//     }

//     return 1;
// }

// int BaseShader::linkProg() {
//     GLuint prog;

//     // TODO: Shall delete previous steps' result on failure !!!
//     //       not only returning !!!

//     // printf("3333333\n");
//     if (!initProg(vsh, fsh, &prog)) {
//         progId = -1;
//         return -1;
//     }
//     else {
//         progId = (int)prog;
//         return (int)prog;
//     }
// }


void BaseShader::start() {
    if (progId >= 0) {
        glUseProgram((GLuint)progId);
    }
}

void BaseShader::stop() {
    glUseProgram(0);
}

void BaseShader::cleanUp() {
    unloadProg();
}
