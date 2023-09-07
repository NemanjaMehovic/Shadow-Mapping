#include<Minimap.h>
#include<Utils.h>
#include<glad/glad.h>
#include<vector>
#include<iostream>

const float Minimap::data[] = { 
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  1.0f,  1.0f, 1.0f
};

Minimap::Minimap()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

Minimap::~Minimap()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Minimap::activate() const
{
    glBindVertexArray(VAO);
}