#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our
{

#define ATTRIB_LOC_POSITION 0
#define ATTRIB_LOC_COLOR 1
#define ATTRIB_LOC_TEXCOORD 2
#define ATTRIB_LOC_NORMAL 3

    class Mesh
    {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements
        GLsizei elementCount;

    public:
        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering
        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &elements)
        {
            //  DONE: (Req 1) Write this function
            //  remember to store the number of elements in "elementCount" since you will need it for drawing
            //  For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc

            elementCount = elements.size();

            // Create and bind vertix array object
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            // Create and bind the buffer to store vertex data
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            // Add data to the vertex buffer with the following params:
            // - target: the target to which the buffer object is bound fo
            // - size: the size in bytes of the buffer object's new data store
            // - data: a pointer to data that will be copied into the buffer
            // - usage:  the expected usage pattern of the buffer
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

            // Enable the location attribute
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);
            // Define the array of data for the location attribute with the following params
            // - index: the index of the attribute
            // - size: the number of components for every attribute ex: pos has 3 components (x, y, z)
            // - type: the data type of each component
            // - normalized: when true it will normalize the data (from 0 to 1)
            // - stride: the byte offset between consecutive attributes
            // - pointer: the offset of the first component of the first attribute
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, false, sizeof(Vertex), (void *)0);

            // Enable the color attribute and difine its data
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 3, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void *)offsetof(Vertex, color));

            // Enable the texture coordinates attribute and difine its data
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, tex_coord));

            // Enable the surface normal attribute and difine its data
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, normal));

            // Create and bind the buffer to store elements data
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            // Add data to the elements buffer
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(uint32_t), elements.data(), GL_STATIC_DRAW);
        }

        // this function should render the mesh
        void draw()
        {
            // DONE: (Req 1) Write this function

            // Bind VAO to start drawing
            glBindVertexArray(VAO);
            // Draw elements in EBO with the folowing params:
            // - mode: what kind of primitives to render
            // - count: the number of elements to be rendered
            // - type:  the type of the values in indices. Must be unsigned type
            // - indices: an offset of the first index in EBO
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (void *)0);
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh()
        {
            // DONE: (Req 1) Write this function
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}