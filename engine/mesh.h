//Clase MESH para assimp
#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "shader.h"
#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 textCoord;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};


struct Texture{
    unsigned int id;
    std::string type;
    std::string path;
};


class Mesh {
    public:
    // mesh data
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;
        unsigned int VAO;


        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
        {
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;

            setupMesh();
        }

        void Draw(Shader &shader) 
        {
                unsigned int diffuseNr = 1;
                unsigned int specularNr = 1;
                unsigned int normalNr = 1;
                unsigned int heightNr = 1;
                for(unsigned int i = 0; i < textures.size(); i++)
                {
                    glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
                    // retrieve texture number (the N in diffuse_textureN)
                    std::string number;
                    std::string name = textures[i].type;
                    if(name == "texture_diffuse")
                        number = std::to_string(diffuseNr++);
                    else if(name == "texture_specular")
                        number = std::to_string(specularNr++);
                    else if(name == "texture_normal")
                        number = std::to_string(normalNr++); // transfer unsigned int to string
                        else if(name == "texture_height")
                        number = std::to_string(heightNr++); // transfer unsigned int to string

                    shader.setFloat(("material." + name + number).c_str(), i);
                    glBindTexture(GL_TEXTURE_2D, textures[i].id);
                }
                //glActiveTexture(GL_TEXTURE0);

                // draw mesh
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);

                glActiveTexture(GL_TEXTURE0);
        } 

    private:

        unsigned int VBO, EBO;



        void setupMesh(){
            glGenVertexArrays(1,&VAO);
            glGenBuffers(1,&VBO); //los objetos en openGL tienen IDS correspondientes, dirs de mem, lo genereamos
            glGenBuffers(1,&EBO);
            glBindVertexArray(VAO); //bindea VBOS subsecuentes a un VAO

            glBindBuffer(GL_ARRAY_BUFFER,VBO); //este buffer VBO es del tipo array buffer
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW); // hace sitio en mem y guarda los datos, tipo, tam, puntero a datos y uso

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW); 

            //Each vertex attribute takes its data from memory managed by a VBO and which VBO it takes its data from (you can have multiple VBOs)
            // is determined by the VBO currently bound to GL_ARRAY_BUFFER when calling glVertexAttribPointer

            glEnableVertexAttribArray(0);  
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); 
            // 0, location=0 donde queremos que se guarde el vertex,   3 porque vec3 se compone de 3 datos, tipo de dato
            // si queremos que se normalize los datos, tamaño del vertice (antes era(3* float porque vec3 = 3 floats)), offset al inicio (0)
            
            glEnableVertexAttribArray(1);  
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)offsetof(Vertex, Normal)); 

            glEnableVertexAttribArray(2);  
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textCoord));

             // vertex tangent
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
            // vertex bitangent
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
            // ids
            glEnableVertexAttribArray(5);
            glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

            // weights
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
            glBindVertexArray(0);

            glBindVertexArray(0);
        }

 
};