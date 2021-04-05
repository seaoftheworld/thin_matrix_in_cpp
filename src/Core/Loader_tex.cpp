#include "Loader.h"
#include "Common/gl_header.h"

#include <stdlib.h>  // for NULL to be used

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// TODO: use vector to replace the following global var
// StaticTexture *test_global_texture[MAX_TEXTURE_NUM_ONCE] = {NULL};

// unsigned int textureIds[MAX_TEXTURE_NUM_ONCE] = {0};
// unsigned int num_allocated_texture = 0;

    static int load_rgb_alpha_image(const std::string &path, bool alpha)
    {
        int width, height, imgComp;
        // stbi_set_flip_vertically_on_load(1);

        // req_comp has to be 3, since 'GL_RGB' is used as parameter 
        // for generating texture now, despite the original image comp.
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &imgComp, (alpha) ? STBI_rgb_alpha : STBI_rgb);
        if (!data) {
            printf("load_rgb_alpha_image(): failed to read file %s!\n\n", path.c_str());
            return 0;
        }
        if (width <= 0 || height <= 0) {
            printf("load_rgb_alpha_image(): width or height is 0!\n\n");
            return 0;
        }
        // if (imgComp != 3) {
        //     printf("load_rgb_alpha_image(): comp is: %d\n", imgComp);
        // }

        // Should have bind with the target textureId before the following operation
        glTexImage2D(GL_TEXTURE_2D, 0, 
            (alpha) ? GL_RGBA : GL_RGB, 
            width, height, 0, 
            (alpha) ? GL_RGBA : GL_RGB, 
            GL_UNSIGNED_BYTE, data);
            // unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
            // unsigned int size = ((imgWidth + 3) / 4) * ((imgHeight + 3) / 4) * blockSize; 
            // glCompressedTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, size, data); 

        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Use linear interpolation for the texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);
        printf("  gl rgb-%s texture2d ready, w-h: %d-%d\n", (alpha) ? ("alpha") : ("non_alpha"), width, height);
        printf("  path: %s\n\n", path.c_str());
        return 1;
    }

    static int load_grayscale_alpha_image(const std::string &path, bool alpha)
    {
        int width, height, imgComp;
        // stbi_set_flip_vertically_on_load(1);

        // How to load 1/2 channels image properly, instead of using RGBA format ???
        // unsigned char *data = stbi_load(path.c_str(), &width, &height, &imgComp, (alpha) ? STBI_grey_alpha : STBI_grey);
            unsigned char *data = stbi_load(path.c_str(), &width, &height, &imgComp, (alpha) ? STBI_rgb_alpha : STBI_rgb);
        
        if (!data) {
            printf("load_grayscale_alpha_image(): failed to read file %s!\n\n", path.c_str());
            return 0;
        }
        if (width <= 0 || height <= 0) {
            printf("load_grayscale_alpha_image(): width or height is 0!\n\n");
            return 0;
        }

        // How to load 1/2 channels image properly, instead of using RGBA format ???
        //
        // Should have bind with the target textureId before the following operation
        // glTexImage2D(GL_TEXTURE_2D, 0, 
        //     (alpha) ? GL_RG16 : GL_RED, 
        //     width, height, 0, 
        //     (alpha) ? GL_RG16 : GL_RED, 
        //     GL_UNSIGNED_BYTE, data);
        // if (alpha) {
        //     GLint swizzleMask[] = {GL_GREEN, GL_GREEN, GL_GREEN, GL_RED};
        //     glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
        // }
        // else {
        //     GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
        //     glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
        // }
            glTexImage2D(GL_TEXTURE_2D, 0, 
                (alpha) ? GL_RGBA : GL_RGB, 
                width, height, 0, 
                (alpha) ? GL_RGBA : GL_RGB, 
                GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Use linear interpolation for the texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        stbi_image_free(data);
        printf("  gl grayscale-%s texture2d ready, w-h: %d-%d\n", (alpha) ? ("alpha") : ("non_alpha"), width, height);
        printf("  path: %s\n\n", path.c_str());
        return 1;
    }

    void destroyTex(unsigned int num, unsigned int *texIds) {
        glDeleteTextures(num, texIds);
    }

    void bindTex(GLenum textureUnit, unsigned int texId) {
        // if (textureUnit >= GL_TEXTURE0 && textureUnit <= GL_TEXTURE31 && tex < TMAX) {
        if (textureUnit >= GL_TEXTURE0 && textureUnit <= GL_TEXTURE31) {
            glActiveTexture(textureUnit);
            glBindTexture(GL_TEXTURE_2D, texId);
        }
        else {
            printf("bindTex() failed.\n");
        }
    }

    unsigned int createTex(std::string *imgPaths, unsigned int num, unsigned int *texIds) {

        glGenTextures(num, texIds);
        for (unsigned int i = 0; i < num; i++) {
            if (texIds[i] == 0) {
                printf("  createTex(): tex id-%d is 0 !\n", i);

                destroyTex(num, texIds);
                return 0;
            }
        }

        for (unsigned int i = 0; i < num; i++) {

            // printf("  i: %d, bind tex-id: %d \n", i, texIds[i]);
            glBindTexture(GL_TEXTURE_2D, texIds[i]);

            int x, y, comp = 0;
            if ( !stbi_info(imgPaths[i].c_str(), &x, &y, &comp) ) {
                printf("  createTex(): failed to read info for texture file: %s\n", imgPaths[i].c_str());

                destroyTex(num, texIds);
                return 0;
            }

            if (comp == 3) {
                // TODO: For some png pictures taken by cellphone (no transparency), comp is still 4, 
                // this could be displayed correctly with alpha set;
                // but for some other png, comp is 3, and, if alpha is set to 0 in this case,
                // the displayed color is not correct,
                // set alpha to 1 for all situations ???
                // 
                // if (!load_rgb_alpha_image(imgPaths[i], 0)) {  
                if (!load_rgb_alpha_image(imgPaths[i], 1)) {
                    destroyTex(num, texIds);
                    return 0;
                }
            }
            else if (comp == 4) {
                if (!load_rgb_alpha_image(imgPaths[i], 1)) {
                // if (!load_rgb_alpha_image(imgPaths[i], 0)) {
                    destroyTex(num, texIds);
                    return 0;
                }
            }
            else if (comp == 2) {
                // grayscale-alpha character map for texts
                if (!load_grayscale_alpha_image(imgPaths[i], 1)) {
                    destroyTex(num, texIds);
                    return 0;
                }
            }
            else {
                printf("  createTex(): tex file:%s not rgb/rgba/text-char-map format, comp is:%d\n", imgPaths[i].c_str(), comp);
                destroyTex(num, texIds);
                return 0;
            }
        }

        return 1;
    }



// 'textureIds' as input 
void Loader::allocStaticTextureFromBuffers(unsigned int* input_textureIds, unsigned int num, StaticTexture** output_result) {
    if (!input_textureIds || !output_result) {
        return;
    }

    for (unsigned int i = 0; i < num; i++) {
        if (input_textureIds[i] == 0) {
            destroyTex(num, input_textureIds);
            return;
        }
    }

    size_t size_of_tex_vec_before_this_alloc = pStaticTextures.size();
    for (unsigned int i = 0; i < num; i++) {

        StaticTexture *p = NULL;
        p = new StaticTexture( input_textureIds[i] );

        if (!p) {
            printf("  __ mem alloc for tex id-%d failed, shall quit!\n", i);
            
            // remove the generated texture buffers in this call
            destroyTex(num, input_textureIds);

            // delete the allocated class' objs
            for (unsigned int j = 0; j < i; j++) {
                delete pStaticTextures[j + size_of_tex_vec_before_this_alloc];
            }

            // remove the elements pushed into the vector in this call
            if (i > 0) {
                vector<StaticTexture*>::iterator start = pStaticTextures.end() - i;
                vector<StaticTexture*>::iterator end = pStaticTextures.end();
                pStaticTextures.erase(start, end);
            }

            return;
        }

        pStaticTextures.push_back(p);
        output_result[i] = p;
    }
}

// 'texture_ids' as output for result
unsigned int Loader::createStaticTextureBuffers(std::string *imgPaths, unsigned int num, unsigned int texture_ids[][MAX_TEXTURE_NUM_ONCE]) {
    unsigned int _num = (num > MAX_TEXTURE_NUM_ONCE) ? (MAX_TEXTURE_NUM_ONCE) : (num);
    return createTex(imgPaths, _num, *texture_ids);
}

void Loader::loadStaticTextures(std::string *imgPaths, unsigned int num, StaticTexture **output_result) {
    
    unsigned int textureIds[MAX_TEXTURE_NUM_ONCE] = { 0 };

    if ( !createStaticTextureBuffers(imgPaths, num, &textureIds) ) {
        return;
    }
    
    allocStaticTextureFromBuffers(textureIds, num, output_result);
}

StaticTexture *Loader::loadStaticTextureCube(string imgPaths[][6]) {
    if (!imgPaths) {
        return NULL;
    }

    for (unsigned int i = 0; i < 6; i++) {
        int x, y, comp = 0;
        if ( !stbi_info((*imgPaths)[i].c_str(), &x, &y, &comp) ) {
            printf("  loadCubeStaticTexture(): failed to read info for texture file: %s\n", \
                (*imgPaths)[i].c_str());
            return NULL;
        }
        if (comp != 4) {
            printf("  loadCubeStaticTexture(): texture file: %s, comp is %d, not 4 !!!\n", \
                (*imgPaths)[i].c_str(), comp);
            printf("  compatible with the \'STBI_rgb_alpha\' parameter tobe used later ???\n");
        }
    }

    unsigned int texId;
    glGenTextures(1, &texId);
    // if (!texId) {
    //     glDeleteTextures(1, &texId);
    //     return 0;
    // }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

    stbi_set_flip_vertically_on_load(1);

    for (unsigned int i = 0; i < 6; i++) {

        int width, height, imgComp;

        unsigned char *data = stbi_load((*imgPaths)[i].c_str(), &width, &height, &imgComp, STBI_rgb_alpha);
        
        if (!data) {
            printf("loadCubeStaticTexture(): failed to read file %s!\n\n", (*imgPaths)[i].c_str());
            continue;
        }

        if (width <= 0 || height <= 0) {
            printf("loadCubeStaticTexture(): width or height is 0!\n\n");
            stbi_image_free(data);
            continue;
        }

        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 
            GL_RGBA, 
            width, height, 0, 
            GL_RGBA, 
            GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Use linear interpolation for the texture
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
        // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        stbi_image_free(data);
        // printf("  gl rgb-%s texture2d ready, w-h: %d-%d\n", (alpha) ? ("alpha") : ("non_alpha"), width, height);
        // printf("  path: %s\n\n", path.c_str());
        printf("  gl texture2d ready, w-h: %d-%d\n", width, height);
        printf("  path: %s\n", (*imgPaths)[i].c_str());
    }
    printf("\n\n");

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    stbi_set_flip_vertically_on_load(0);

    StaticTexture *result = NULL;
    result = new StaticTexture(texId);
    if (!result) {
        printf("  __ mem alloc for tex id-%d failed!\n", texId);
        glDeleteTextures(1, &texId);
        return NULL;
    }

    pStaticTextures.push_back(result);
    return result;
}
