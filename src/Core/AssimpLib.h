// #pragma once // includeded in 1. terrrain_03 main.cpp, 2. DataInit.h

// #include "Common/data.h"
#include "Loader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>

// struct AssimpMesh {
//     std::vector<StaticTexture *> textures;
//     StaticModel_SingleVbo *model;
// };

struct Assimp_Material {
    std::string tex_path;
    StaticTexture *tex;
};

class AssimpLib {
    // std::vector<std::string> texture_paths;
    std::vector<Assimp_Material> loaded_material;
    std::string model_file_direct;

public:
    // std::vector<AssimpMesh> meshes;
    std::vector<Entity> entities;

    AssimpLib() {
        cleanUp();
    }

    void cleanUp() {
        // auto itr = entities.begin();
        // for (; itr != entities.end(); itr++) {
        //  delete (*itr);
        // }
        
        // meshes.clear();
        // texture_paths.clear();

        loaded_material.clear();
        entities.clear();
    }

    /*
    void loadModel(const std::string &path, EntityInfo &transform, Loader &loader) {
        Assimp::Importer importer;
        unsigned int flag = (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // unsigned int flag = (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

        const aiScene *scene = importer.ReadFile(path, flag);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            printf("  ASSIMP load error:: \n    %s\n\n", importer.GetErrorString());
            return;
        }
        
        printf("  ASSIMP load done:\n    %s\n", path.c_str());
        printf("    num of aiScene's mesh, material, textures are: %d, %d, %d\n\n", 
            scene->mNumMeshes, scene->mNumMaterials, scene->mNumTextures);

        model_file_direct = path.substr(0, path.find_last_of('/'));
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            printf("\n\n  process mesh: %d\n", i);
            assimp_Mesh_Material(scene, scene->mMeshes[i], transform, loader);
        }
        printf("\n\n\n");

            // debug info
            printf("    model direct: %s\n", model_file_direct.c_str());
            for (unsigned int i = 0; i < loaded_material.size(); i++) {
                // printf("  tex-%d, id-%d: %s\n", i, loaded_material[i].tex->getId(), loaded_material[i].tex_path.c_str());
                printf("  tex-%d: %s\n", i, loaded_material[i].tex_path.c_str());
            }
            printf("\n\n");
    }
    //*/

    void loadModel(const std::string &path, Loader &loader, Transform *transforms[], unsigned int transforms_num) {
        Assimp::Importer importer;
        unsigned int flag = (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // unsigned int flag = (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

        const aiScene *scene = importer.ReadFile(path, flag);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            printf("  ASSIMP load error:: \n    %s\n\n", importer.GetErrorString());
            return;
        }
        
        printf("  ASSIMP load done:\n    %s\n", path.c_str());
        printf("    num of aiScene's mesh, material, textures are: %d, %d, %d\n\n", 
            scene->mNumMeshes, scene->mNumMaterials, scene->mNumTextures);

        model_file_direct = path.substr(0, path.find_last_of('/'));
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            printf("\n\n  process mesh: %d\n", i);
            assimp_Mesh_Material(scene, scene->mMeshes[i], loader, transforms, transforms_num);
        }
        printf("\n\n\n");

            // debug info
            printf("    model direct: %s\n", model_file_direct.c_str());
            for (unsigned int i = 0; i < loaded_material.size(); i++) {
                // printf("  tex-%d, id-%d: %s\n", i, loaded_material[i].tex->getId(), loaded_material[i].tex_path.c_str());
                printf("  tex-%d: %s\n", i, loaded_material[i].tex_path.c_str());
            }
            printf("\n\n");
    }

private:
    /*
    void assimp_Mesh_Material(const aiScene *scene, aiMesh *mesh, EntityInfo &transform, Loader &loader) {

        // get data from mesh and generate an entity with these data
        // std::vector<StaticTexture *> temp_textures; 
        // temp_textures.clear(); {

        StaticTexture *temp_texture = NULL; {

            aiMaterial *mesh_material = scene->mMaterials[mesh->mMaterialIndex];
            unsigned int mesh_tex_num = mesh_material->GetTextureCount(aiTextureType_DIFFUSE);

            // only supports up to 1-texture for each material now
            // for (unsigned int i = 0; i < mesh_tex_num; i++ ) {

            for (unsigned int i = 0; i < 1; i++) {
                // StaticTexture *texture = NULL; {
                //     // alloc texture according to aiMesh's material info
                //     texture = assimp_Texture(mesh_mat, aiTextureType_DIFFUSE, i);
                // }

                // if (texture) {
                //     temp_textures.push_back(texture);
                // }

                temp_texture = assimp_Texture(mesh_material, aiTextureType_DIFFUSE, i, loader);
            }
        }

        StaticModel_SingleVbo *temp_model = NULL; {

            // alloc static-model according to aiMesh's data
            std::vector<float> input_attr_data;
            input_attr_data.clear();
            unsigned int vertex_count; {

                for (unsigned int i = 0; i < mesh->mNumVertices; i++) {

                    input_attr_data.push_back(mesh->mVertices[i].x);
                    input_attr_data.push_back(mesh->mVertices[i].y);
                    input_attr_data.push_back(mesh->mVertices[i].z);

                    if ( mesh->mTextureCoords[0] ) {
                        // xxx *mTextureCoords[8];
                        // a vertex can contain up to 8 different texture coordinates. We assum we won't 
                        // use models where a vertex can have multiple texture coordinates, 
                        // always take the first set (0).

                        input_attr_data.push_back(mesh->mTextureCoords[0][i].x); 
                        input_attr_data.push_back(mesh->mTextureCoords[0][i].y); 
                    }
                    else {
                        input_attr_data.push_back(0.0f);
                        input_attr_data.push_back(0.0f);
                    }

                    if (mesh->HasNormals()) {
                        input_attr_data.push_back(mesh->mNormals[i].x);
                        input_attr_data.push_back(mesh->mNormals[i].y);
                        input_attr_data.push_back(mesh->mNormals[i].z);
                    }
                    else {
                        input_attr_data.push_back(0.0f);
                        input_attr_data.push_back(0.0f);
                        input_attr_data.push_back(0.0f);
                    }
                }

                vertex_count = mesh->mNumVertices;
            }

            std::vector<unsigned short> input_indices_data;
            input_indices_data.clear();
            unsigned int indices_count; {

                for (unsigned int i = 0; i < mesh->mNumFaces; i++) {

                    aiFace face = mesh->mFaces[i];

                    // should have been trianglated, face shall be triangle
                    for (unsigned int i = 0; i < face.mNumIndices; i++) {
                        input_indices_data.push_back(face.mIndices[i]);
                    }
                }

                indices_count = input_indices_data.size();
            }

            temp_model = loader.loadSingleVboModel(
                &input_attr_data[0], vertex_count, &input_indices_data[0], indices_count
            );

            input_indices_data.clear();
            input_attr_data.clear();
        }

        // push the address of the generated entity into the container
        // AssimpMesh assimp_mesh; {
        //     assimp_mesh.textures = temp_textures;
        //     assimp_mesh.model = temp_model;
        // }
        // meshes.push_back(assimp_mesh);

        Entity entity; {
            // only supports up to 1-texture for each material now
            entity.setTextureModel(temp_texture, temp_model);
            entity.addInfo(transform);

            // for debugging misa
            //
            // {
            //     float info_00[Entity::transform::max] = {
            //             0.0f, 0.0f, 0.0f,
            //             0.0f, 0.0f, 3.140034f,
            //             1.0f
            //     };
            //     EntityInfo transform_00(&info_00);
            //     entity.addInfo(transform_00);

            //     float info_01[Entity::transform::max] = {
            //             0.0f, 0.0f, 0.0f,
            //             0.0f, 0.0f, -3.140035f,
            //             1.0f
            //     };
            //     EntityInfo transform_01(&info_01);
            //     entity.addInfo(transform_01);
            // }
        }

        entities.push_back(entity);
    }
    //*/

    void assimp_Mesh_Material(const aiScene *scene, aiMesh *mesh, Loader &loader, Transform *transforms[], unsigned int transforms_num) {

        // get data from mesh and generate an entity with these data
        // std::vector<StaticTexture *> temp_textures; 
        // temp_textures.clear(); {

        StaticTexture *temp_texture = NULL; {

            aiMaterial *mesh_material = scene->mMaterials[mesh->mMaterialIndex];
            unsigned int mesh_tex_num = mesh_material->GetTextureCount(aiTextureType_DIFFUSE);

            // only supports up to 1-texture for each material now
            // for (unsigned int i = 0; i < mesh_tex_num; i++ ) {

            for (unsigned int i = 0; i < 1; i++) {
                // StaticTexture *texture = NULL; {
                //     // alloc texture according to aiMesh's material info
                //     texture = assimp_Texture(mesh_mat, aiTextureType_DIFFUSE, i);
                // }

                // if (texture) {
                //     temp_textures.push_back(texture);
                // }

                temp_texture = assimp_Texture(mesh_material, aiTextureType_DIFFUSE, i, loader);
            }
        }

        StaticModel_SingleVbo *temp_model = NULL; {

            // alloc static-model according to aiMesh's data
            std::vector<float> input_attr_data;
            input_attr_data.clear();
            unsigned int vertex_count; {

                for (unsigned int i = 0; i < mesh->mNumVertices; i++) {

                    input_attr_data.push_back(mesh->mVertices[i].x);
                    input_attr_data.push_back(mesh->mVertices[i].y);
                    input_attr_data.push_back(mesh->mVertices[i].z);

                    if ( mesh->mTextureCoords[0] ) {
                        // xxx *mTextureCoords[8];
                        // a vertex can contain up to 8 different texture coordinates. We assum we won't 
                        // use models where a vertex can have multiple texture coordinates, 
                        // always take the first set (0).

                        input_attr_data.push_back(mesh->mTextureCoords[0][i].x); 
                        input_attr_data.push_back(mesh->mTextureCoords[0][i].y); 
                    }
                    else {
                        input_attr_data.push_back(0.0f);
                        input_attr_data.push_back(0.0f);
                    }

                    if (mesh->HasNormals()) {
                        input_attr_data.push_back(mesh->mNormals[i].x);
                        input_attr_data.push_back(mesh->mNormals[i].y);
                        input_attr_data.push_back(mesh->mNormals[i].z);
                    }
                    else {
                        input_attr_data.push_back(0.0f);
                        input_attr_data.push_back(0.0f);
                        input_attr_data.push_back(0.0f);
                    }
                }

                vertex_count = mesh->mNumVertices;
            }

            std::vector<unsigned short> input_indices_data;
            input_indices_data.clear();
            unsigned int indices_count; {

                for (unsigned int i = 0; i < mesh->mNumFaces; i++) {

                    aiFace face = mesh->mFaces[i];

                    // should have been trianglated, face shall be triangle
                    for (unsigned int i = 0; i < face.mNumIndices; i++) {
                        input_indices_data.push_back(face.mIndices[i]);
                    }
                }

                indices_count = input_indices_data.size();
            }

            temp_model = loader.loadSingleVboModel(
                &input_attr_data[0], vertex_count, &input_indices_data[0], indices_count
            );

            input_indices_data.clear();
            input_attr_data.clear();
        }

        // push the address of the generated entity into the container
        // AssimpMesh assimp_mesh; {
        //     assimp_mesh.textures = temp_textures;
        //     assimp_mesh.model = temp_model;
        // }
        // meshes.push_back(assimp_mesh);

        Entity entity; {
            // only supports up to 1-texture for each material now
            entity.setTextureModel(temp_texture, temp_model);

            // entity.addInfo(transform);
            for (unsigned int i = 0; i < transforms_num; i++) {
                
                // float test = (*transforms)[i].values[0];
                // test = transforms[i]->values[0];  // ???
                // test = (*transforms + i)->values[0];
                float temp_data[Entity::transform::max];
                for (int j = 0; j < Entity::transform::max; j++) {
                    if (!transforms[j]) {
                        continue;
                    }
                    
                    temp_data[j] = (transforms[i])->values[j];
                }
                Transform temp_transform(&temp_data);

                entity.addTransform( temp_transform );
            }

            // for debugging misa
            //
            // {
            //     float info_00[Entity::transform::max] = {
            //             0.0f, 0.0f, 0.0f,
            //             0.0f, 0.0f, 3.140034f,
            //             1.0f
            //     };
            //     EntityInfo transform_00(&info_00);
            //     entity.addInfo(transform_00);

            //     float info_01[Entity::transform::max] = {
            //             0.0f, 0.0f, 0.0f,
            //             0.0f, 0.0f, -3.140035f,
            //             1.0f
            //     };
            //     EntityInfo transform_01(&info_01);
            //     entity.addInfo(transform_01);
            // }
        }

        entities.push_back(entity);
    }

    StaticTexture *assimp_Texture(aiMaterial *mat, aiTextureType type, unsigned int i, Loader &loader) {
        aiString str;
        mat->GetTexture(type, i, &str);

        for (unsigned int i = 0; i < loaded_material.size(); i++) {
            if (str.C_Str() == loaded_material[i].tex_path) {
                return loaded_material[i].tex;
            }
        }

        std::string texture_path = model_file_direct + '/' + str.C_Str(); {

            printf("    mat-tex path: %s\n", texture_path.c_str());
            StaticTexture *tex = NULL;
            loader.loadStaticTextures(&texture_path, 1, &tex);

            struct Assimp_Material material; {
                material.tex_path = str.C_Str();
                material.tex = tex;
            }
            loaded_material.push_back(material);

            return tex;
        }
    }
};
