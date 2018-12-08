#pragma once

#include "Shader.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class TileIso
{
public:
	TileIso();
	~TileIso();
	void inicializar(float th, float tw);
	void setCor(float r, float g, float b);
	void draw(glm::mat4 model);
	void setShader(Shader *shader);
	void setTexture(unsigned int idxTexture);
	void setTextureCoord(vector<glm::vec2> coordenadas);
	void setCaminhavel(bool c) { caminhavel = c; }

	float getWidth() { return tw;  }
	float getHeight() { return th; }
	bool getCaminhavel() { return caminhavel; }
	glm::mat4 getModelTile() { return modelTile; }

protected:
	GLuint VBO, EBO, VAO; //ids dos buffers (na real só precisaria guardar o VAO)

	Shader *shader;

	//Texture index
	unsigned int texture;

	glm::mat4 modelTile;
	glm::vec4 cor;
	vector<glm::vec2> textureCoord; //array com 4 coordenadas x,y - p1, p2, p3, p4 - {(x,y), (x,y),(x,y),(x,y)}
	float tw, th;
	bool caminhavel;
};

