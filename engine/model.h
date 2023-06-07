
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//#include <assimp/version.h>
#include "stb_image.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include "animdata.h"

#include "AssimpGLMHelper.h"

/*

                    MATRIX 4x4 DEL NODO

   .x           .y               .z            .w          

{Escala.X       XXXX             XXXXXX         XXXX        }       [0]
{XXXXX          Escala.Y         XXXXXX         XXXX        }       [1]
{XXXXX          XXXXXXX          Escala.Z       XXXX        }       [2]
{Posicion.X     Posicion.Y       Posicion.Z     Posicion.W  }       [3]


*/

struct Model 
{
    public:
        //esta matriz contiene la posicion, rotacion y escala del objeto
        glm::mat4 nodeMatrix;

        std::vector<Texture> textures_loaded; 
        std::vector<Mesh> meshes;
        std::string directory;
        std::string modelname;
        Model()
        {

            //nodeMatrix = glm::mat4(1.0f);
            //loadModel("assets/backpack.obj");
        }

        Model(const char *path, glm::vec3 posiciones)
        {   
            nodeMatrix = glm::mat4(1.0f);
            setPosition(posiciones.x,posiciones.y,posiciones.z);
            loadModel(path);
        }

        Model(const Model& obj){
            nodeMatrix = obj.nodeMatrix;

            directory = obj.directory;
            meshes = obj.meshes;
            textures_loaded = obj.textures_loaded;
        }
        
        void Draw(Shader& sha)
        {
            sha.use();
            sha.setMat4("model", nodeMatrix);
            for(unsigned int i = 0; i < meshes.size(); i++)
                meshes[i].Draw(sha);
        } 

        

        void setPosition(float x,float y, float z)
        {
            nodeMatrix[3].x = x;
            nodeMatrix[3].y = y;
            nodeMatrix[3].z = z;

        }
        glm::vec3 getPosition()
        {
            return glm::vec3(nodeMatrix[3].x,nodeMatrix[3].y,nodeMatrix[3].z);
        }

        void Translate(float x,float y, float z) 
        {
            nodeMatrix = glm::translate(nodeMatrix, glm::vec3(x, y, z));
        }

        void Scale(float x,float y, float z) 
        {
            nodeMatrix = glm::scale(nodeMatrix, glm::vec3(x, y, z));
        }

        void Rotate(float angle,float x,float y, float z) 
        {
            
            nodeMatrix = glm::rotate(nodeMatrix,glm::radians(angle), glm::vec3(x, y, z));
        }
        
        void useLoadModel(std::string path)
        {
            std::cout << path << "\n";
            loadModel(path);
        }
        

        static unsigned int loadImageFromFile(const char *path, const std::string &directory)
        {
            std::string filename = std::string(path);
            filename = directory + '/' + filename;
            
            unsigned int textureID;
            glGenTextures(1, &textureID);
            int width, height, nrComponents;
            
            unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
            if (data)
            {
                
                GLenum format = GL_RED;
                if (nrComponents == 1)
                    format = GL_RED;
                else if (nrComponents == 3)
                    format = GL_RGB;
                else if (nrComponents == 4)
                    format = GL_RGBA;

                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Texture failed to load at path: " << path << std::endl;
                stbi_image_free(data);
            }

            return textureID;
        }


        auto& GetBoneInfoMap() { return m_BoneInfoMap; }
        int& GetBoneCount() { return m_BoneCounter; } 

    private:
        // model data
        std::map<std::string, BoneInfo> m_BoneInfoMap;
        int m_BoneCounter = 0;

        void processNode(aiNode *node, const aiScene *scene) //Se comprueban los indices de cada nodo de la mesh y luego se obtienen las meshes correspondientes con un indice a mMeshes
        {   
            //std::cout << "num meshes " << node->mNumMeshes << "\n";
            //std::cout << "num hijos " << node->mNumChildren << "\n";
            // process all the node's meshes (if any)
            for(unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
                meshes.push_back(processMesh(mesh, scene));
                	
            }
            // then do the same for each of its children
            for(unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene);
            }
            
        } 
        //Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        //std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
        //                                   std::string typeName);

        void loadModel(std::string path)
        {
                Assimp::Importer import;
                /*
                int i = aiGetVersionMajor();

                int z = aiGetVersionMinor();
                unsigned int j = aiGetVersionPatch(); */
                //std::cout << "version patch "  << j << " y minor major " << i << " . " << z << "\n";
            
                const aiScene *scene = import.ReadFile(path,aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);	
                
                if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
                {
                    
                    std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
                    return;
                }

                directory = path.substr(0, path.find_last_of('/'));
                modelname = path.substr(path.find_last_of('/')+1,path.size()-path.find_last_of('/'));
                
                processNode(scene->mRootNode, scene);

        }

        Mesh processMesh(aiMesh *mesh, const aiScene *scene)
        {
            
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<Texture> textures;

            for(unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                
                Vertex vertex;
                glm::vec3 vector3;
                SetVertexBoneDataToDefault(vertex);
			    vertex.Position = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
			    vertex.Normal = AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);

                vector3.x = mesh->mVertices[i].x;
                vector3.y = mesh->mVertices[i].y;
                vector3.z = mesh->mVertices[i].z;
                vertex.Position = vector3;
                // process vertex positions, normals and texture coordinates

                if(mesh->HasNormals())
                {
                    vector3.x = mesh->mNormals[i].x;
                    vector3.y = mesh->mNormals[i].y;
                    vector3.z = mesh->mNormals[i].z;
                    vertex.Normal = vector3;
                }
                

                if(mesh->mTextureCoords[0])
                {
                glm::vec2 vec;
                    // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    vec.x = mesh->mTextureCoords[0][i].x; 
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.textCoord = vec;
                    
                    // tangent
                    if(mesh->HasTangentsAndBitangents()) {
                        vector3.x = mesh->mTangents[i].x;
                        vector3.y = mesh->mTangents[i].y;
                        vector3.z = mesh->mTangents[i].z;
                        vertex.Tangent = vector3;
                        
                        // bitangent
                        vector3.x = mesh->mBitangents[i].x;
                        vector3.y = mesh->mBitangents[i].y;
                        vector3.z = mesh->mBitangents[i].z;
                        vertex.Bitangent = vector3;
                    }
                }
                else
                {
                    vertex.textCoord = glm::vec2(0.0f,0.0f);
                }
                vertices.push_back(vertex);
            }
            
            // process indices
            for(unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for(unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }
             
            // process material

            if(mesh->mMaterialIndex >= 0)
            {
                aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
                
                std::vector<Texture> diffuseMaps = loadMaterialTextures(material, 
                                                    aiTextureType_DIFFUSE, "texture_diffuse");
                textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
                std::vector<Texture> specularMaps = loadMaterialTextures(material, 
                                                    aiTextureType_SPECULAR, "texture_specular");
                textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            }
            ExtractBoneWeightForVertices(vertices,mesh,scene);
            return Mesh(vertices, indices, textures);
        }
        
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
        {   
            std::vector<Texture> textures;
            
            for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);
                bool skip = false;
                for(unsigned int j = 0; j < textures_loaded.size(); j++)
                {
                    if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                    {
                        textures.push_back(textures_loaded[j]);
                        skip = true; 
                        break;
                    }
                }
                
                if(!skip)
                {   // if texture hasn't been loaded already, load it
                    Texture texture;
                    texture.id = TextureFromFile(str.C_Str(), directory);
                    
                    texture.type = typeName;
                    texture.path = str.C_Str();
                    textures.push_back(texture);
                    textures_loaded.push_back(texture); // add to loaded textures
                }
            }
            return textures;
        };

        unsigned int TextureFromFile(const char *path, const std::string &directory)
        {
            
            std::string filename = std::string(path);
            filename = directory + '/' + filename;
            
            unsigned int textureID;
            glGenTextures(1, &textureID);
            int width, height, nrComponents;
            
            unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
            if (data)
            {
                
                GLenum format = GL_RED;
                if (nrComponents == 1)
                    format = GL_RED;
                else if (nrComponents == 3)
                    format = GL_RGB;
                else if (nrComponents == 4)
                    format = GL_RGBA;

                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Texture failed to load at path: " << path << std::endl;
                stbi_image_free(data);
            }

            return textureID;
        }

        void SetVertexBoneDataToDefault(Vertex& vertex)
        {
            for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
            {
                vertex.m_BoneIDs[i] = -1;
                vertex.m_Weights[i] = 0.0f;
            }
        }

        void SetVertexBoneData(Vertex& vertex, int boneID, float weight)
	    {
	    	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	    	{
	    		if (vertex.m_BoneIDs[i] < 0)
	    		{
	    			vertex.m_Weights[i] = weight;
	    			vertex.m_BoneIDs[i] = boneID;
	    			break;
	    		}
	    	}
	    }

        void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
        {
            for (int boneIndex = 0; boneIndex < (int)mesh->mNumBones; ++boneIndex)
            {
                int boneID = -1;
                std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
                if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
                {
                    BoneInfo newBoneInfo;
                    newBoneInfo.id = m_BoneCounter;
                    newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
                    m_BoneInfoMap[boneName] = newBoneInfo;
                    boneID = m_BoneCounter;
                    m_BoneCounter++;
                }
                else
                {
                    boneID = m_BoneInfoMap[boneName].id;
                }
                assert(boneID != -1);
                auto weights = mesh->mBones[boneIndex]->mWeights;
                
                int numWeights = mesh->mBones[boneIndex]->mNumWeights;

                for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
                {
                    int vertexId = weights[weightIndex].mVertexId;
                    float weight = weights[weightIndex].mWeight;
                    //std::cout << "bone weights "<< weight << " num bones "<< boneIndex << "\n";
                    assert(vertexId <= (int)vertices.size());
                    SetVertexBoneData(vertices[vertexId], boneID, weight);
                }
            }
        }
};