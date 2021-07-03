#pragma once
#include "Core/Common/gl_header.h"

#include <vector>  // terrain-shader.h and multi-lights-shader.h will 
#include <string>  // need to include this for multi-lights

#include <stdio.h>

class BaseShader {

public:
    enum ShaderStatus {
        not_created = 0, 
        comp_vsh_failed, comp_vsh_passed, comp_vsh_passed_fsh_failed, comp_vsh_passed_fsh_passed,
        link_prog_failed, link_prog_passed,
        deleted_after_linked
    };

    BaseShader(const char *vshPath, const char *fshPath) : status(not_created) {
        printf("  baseclass constructor called.\n");

        if (!initShader(vshPath, &vsh, GL_VERTEX_SHADER)) {
            status = comp_vsh_failed;  // shall delete shader related mem ...
            return;
        }
        else {
            status = comp_vsh_passed;
        }

        if (!initShader(fshPath, &fsh, GL_FRAGMENT_SHADER)) {
            status = comp_vsh_passed_fsh_failed;  // shall delete shader related mem ...
            return;
        }
        else {
            status = comp_vsh_passed_fsh_passed;
        }
    }
    virtual ~BaseShader() {
    }

    // Tobe called by derived class in its constructor, after
    // virtual funcs are implemented according to
    // derived class' details
    void call_subclass_init_funcs() {
        if (status == comp_vsh_passed_fsh_passed) {
            bindAllAttributeLocations();

            GLuint prog;
            if (!initProg(vsh, fsh, &prog)) {
                status = link_prog_failed;
                return;
            }

            status = link_prog_passed;
            progId = prog;
        }

        if (status == link_prog_passed) {
            getAllUniformLocations();
        }
    }

    // This is supposed to be called in the rendering-loop, inside renderer() function for each entity
    void loadTransformMatrix(const float *input_data) {
        // if (status == link_prog_passed && progId > 0) {
        // }
        glUniformMatrix4fv(transform_matrix_loc, 1, false, input_data);
    }
    void setTransformMatLoc(int input_loc) {
        transform_matrix_loc = (GLint)input_loc;
    }
    int getTransformMatLoc() {
        return (int)transform_matrix_loc;
    }

    // This is supposed to be called in the rendering-loop
    void loadViewMatrix(const float *input_data) {
        // if (status == link_prog_passed && progId > 0) {
        // }
        glUniformMatrix4fv(view_matrix_loc, 1, false, input_data);
    }
    void setViewMatLoc(int input_loc) {
        view_matrix_loc = (GLint)input_loc;
    }
    int getViewMatLoc() {
        return (int)view_matrix_loc;
    }

    // This is Not-supposed to be called in the rendering-loop
    void loadProjMatrix(const float *input_data) {
        // if (status == link_prog_passed && progId > 0) {
        // }
        glUniformMatrix4fv(proj_matrix_loc, 1, false, input_data);
    }
    void setProjMatLoc(int input_loc) {
        proj_matrix_loc = (GLint)input_loc;
    }
    int getProjMatLoc() {
        return (int)proj_matrix_loc;
    }

    // Supposed tobe implemented by derived class according to
    // its details
    virtual void bindAllAttributeLocations() = 0;
    virtual void getAllUniformLocations() = 0;



    // // Tobe called by derived class in its constructor, after
    // // virtual funcs are implemented according to
    // // derived class' details
    // // Sequence:
    // //    base-class constructor: compile shaders
    // //    sub-class  bindAllAttributeLocations(): this shall be called before linking shader-prog
    // //    base-class linkProg()
    // //    sub-class  getAllUniformLocations(): this shall be called after linking shader-prog
    // virtual void bindAllAttributeLocations() = 0;
    // virtual void getAllUniformLocations() = 0;

        // Tobe used by derived class to implement 
        // its virtual functions
        void bindAttributeLocation(unsigned int index, const char *name) {
            if (status == link_prog_passed && progId > 0) {
                glBindAttribLocation((GLuint)progId, (GLuint)index, name);
            }
        }

        int getUniformLocation(const char *name) {
            if (status == link_prog_passed && progId > 0) {
                GLint result = glGetUniformLocation((GLuint)progId, name);
                return (int)result;
            }
            else {
                return -1;
            }
        }

        void uniform1i(int location, int input) {
            glUniform1i((GLint)location, (GLint)input);
        }

        void uniform1f(int location, float f0) {
            glUniform1f((GLint)location, (GLfloat)f0);
        }

        void uniformMatrix4fv(int location, const float *f0) {
            glUniformMatrix4fv((GLint)location, 1, false, (const GLfloat *)f0);
        }

        void uniform3fv(int location, unsigned short count, const float *p3f) {
            glUniform3fv( (GLint)location, count, (const GLfloat *)p3f);
        }

        void uniform4fv(int location, unsigned short count, const float *p4f) {
            glUniform4fv( (GLint)location, count, (const GLfloat *)p4f);
        }

    // int linkProg();

    int getProgId() {
        return progId;
    }

    enum ShaderStatus getStatus() {
        return status;
    }


    void start();
    void stop();

    void cleanUp();

private:
    enum ShaderStatus status;
    int progId;

    GLint transform_matrix_loc = -1,
               view_matrix_loc = -1,
               proj_matrix_loc = -1;
    GLuint vsh, fsh;

    int    initShader(const char *path, GLuint *id, GLenum type);
    int    initProg(GLuint vsh, GLuint fsh, GLuint *prog);

    void unloadProg();
};
