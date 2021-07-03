#include "Core/Renderer.h"

#include "WaterFrameBuffers.h"
#include "WaterShader.h"
#include "WaterTile.h"

#include "Core/Common/gl_math.h"

class WaterRenderer : public BaseRenderer {

private:
    WaterShader *waterShader = NULL;

    // contains values to generate Transform matris for each water tile
    std::vector<WaterTile *> waters;

    float moveSpeed = 0.0f, moveSpeedFactor = 0.0005f;

public:
    void allocShadersData() override;
    void freeShadersData() override;
    bool ready() override;

    void addWaterTile(WaterTile *water) {
        waters.push_back(water);
    }

    void run(WaterFrameBuffers *fbos, unsigned int dudv, unsigned int normal) {
        waterShader->start();
        waterShader->loadViewMatrix(getViewMatrix());
        // gl_math::mat4 ident_mat = gl_math::mat4(1.0f);

        moveSpeed += moveSpeedFactor; if (moveSpeed > 1.0f) { moveSpeed = 0.0f; }
        waterShader->loadMoveSpeed(moveSpeed);
            // render(fbos, dudv, normal);
            for (int i = 0; i < waters.size(); i++) {
                auto water00 = waters.begin();
                auto water = waters.begin() + i;

                // (*water)->getFbo()->bind();
                (*water00)->getFbo()->bind();

                // reset veiw / perspective matrices
                // waterShader->loadViewMatrix(&ident_mat[0][0]);
                // waterShader->loadProjMatrix(&ident_mat[0][0]);
                render((WaterTile *)NULL, dudv, normal);

                (*water)->getFbo()->unbind();
                // restore view / perspective matrices
                // waterShader->loadViewMatrix(getViewMatrix());
                // waterShader->loadProjMatrix(getProjMatrix());
                render((*water), dudv, normal);
            }
    }

    void render(WaterTile *water, unsigned int dudv, unsigned int normal) {

        unsigned int vertices_count = 0, stride_in_float = 0;
        if (WaterTile::rect) {
            stride_in_float = WaterTile::rect->getVerticesStride();
            vertices_count = WaterTile::rect->getVerticesCount();

            glBindBuffer(GL_ARRAY_BUFFER, WaterTile::rect->getVboID());
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(
                0,
                stride_in_float, GL_FLOAT,
                GL_FALSE,
                stride_in_float << 2,  // N-floats * 4 ==> stride in bytes (N = 3, 2, ...)
                0);

        }

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, dudv);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, normal);

        if (water) {
            gl_math::mat4 transform_matrix = gl_math::mat4(1.0f); {
                gl_math::create_transform_matrix(water->getTransform(), WaterTile::SIZE, WaterTile::SIZE, &transform_matrix);
                waterShader->loadTransformMatrix(&transform_matrix[0][0]);
            }
        }
        else {
            gl_math::mat4 transform_matrix = gl_math::mat4(1.0f); {
                // float dummy_translate[] = {0.0f, 0.0f, 0.0f};
                // gl_math::create_transform_matrix(&dummy_translate, WaterTile::SIZE, WaterTile::SIZE, &transform_matrix);
                // waterShader->loadTransformMatrix(&transform_matrix[0][0]);
            }
        }

        glDrawArrays(GL_TRIANGLES, 0, vertices_count);
    }

    void render(WaterFrameBuffers *fbos, unsigned int dudv, unsigned int normal) {

        unsigned int vertices_count = 0, stride_in_float = 0;
        if (WaterTile::rect) {
            stride_in_float = WaterTile::rect->getVerticesStride();
            vertices_count = WaterTile::rect->getVerticesCount();

            glBindBuffer(GL_ARRAY_BUFFER, WaterTile::rect->getVboID());
            glEnableVertexAttribArray(0);            
            glVertexAttribPointer(
                0, 
                stride_in_float, GL_FLOAT,
                GL_FALSE, 
                stride_in_float << 2,  // N-floats * 4 ==> stride in bytes (N = 3, 2, ...)
                0);

        }

        for (unsigned int i = 0; i < waters.size(); i++) {

            auto water = waters.begin() + i;

            // glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, fbos->getReflectionTexture());
            // glActiveTexture(GL_TEXTURE1);
            // glBindTexture(GL_TEXTURE_2D, fbos->getRefractionTexture());
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, dudv);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, normal);

            gl_math::mat4 transform_matrix = gl_math::mat4(1.0f); {
                // gl_math::create_transform_matrix((*gui)->getPosition(), (*gui)->getScale(), &transform_matrix);
                // guiShader->loadTransformMatrix(&transform_matrix[0][0]);

                gl_math::create_transform_matrix((*water)->getTransform(), WaterTile::SIZE, WaterTile::SIZE, &transform_matrix);
                waterShader->loadTransformMatrix(&transform_matrix[0][0]);
            }

            if ( water == waters.begin() ) {
                glDrawArrays(GL_TRIANGLES, 0, vertices_count);

                (*water)->getFbo()->bind();
                unsigned int vertices_count = 0, stride_in_float = 0;
                if (WaterTile::rect) {
                    stride_in_float = WaterTile::rect->getVerticesStride();
                    vertices_count = WaterTile::rect->getVerticesCount();

                    glBindBuffer(GL_ARRAY_BUFFER, WaterTile::rect->getVboID());
                    glEnableVertexAttribArray(0);
                    glVertexAttribPointer(
                        0,
                        stride_in_float, GL_FLOAT,
                        GL_FALSE,
                        stride_in_float << 2,  // N-floats * 4 ==> stride in bytes (N = 3, 2, ...)
                        0);

                }

                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, dudv);
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, normal);

                gl_math::mat4 transform_matrix = gl_math::mat4(1.0f); {
                    gl_math::create_transform_matrix((*water)->getTransform(), WaterTile::SIZE, WaterTile::SIZE, &transform_matrix);
                    waterShader->loadTransformMatrix(&transform_matrix[0][0]);
                }

                glDrawArrays(GL_TRIANGLES, 0, vertices_count);
                (*water)->getFbo()->unbind();
            }
            else {
                glDrawArrays(GL_TRIANGLES, 0, vertices_count);
            }

            // printf("water-renderer vbo: %d.\n", WaterTile::rect->getVboID());
            // printf("water-renderer: %d vertices.\n", vertices_count);
        }
    }

public:
    WaterRenderer() {
        printf("  __ water-renderer constructor called.\n");
        freeShadersData();
        allocShadersData();
    }

    ~WaterRenderer() {
        printf("  __ water-renderer destructor called.\n");
        freeShadersData();
    }
};
