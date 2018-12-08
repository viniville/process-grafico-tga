#pragma once

#include "Shader.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class SpriteFundo
{
public:
	SpriteFundo();
	~SpriteFundo();
	void inicializar(float th, float tw);
	void draw(glm::mat4 model);
	void setShader(Shader *shader) { this->shader = shader; };
	void setTexture(unsigned int idxTexture) { this->texture = idxTexture; }

	float getWidth() { return tw; }
	float getHeight() { return th; }
	glm::mat4 getModelSprite() { return modelSprite; }
protected:
	GLuint VBO, EBO, VAO; //ids dos buffers (na real só precisaria guardar o VAO)

	Shader *shader;

	//Texture index
	unsigned int texture;
	glm::mat4 modelSprite;

	float tw, th;
};
