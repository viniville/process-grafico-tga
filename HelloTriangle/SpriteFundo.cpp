#include "SpriteFundo.h"



SpriteFundo::SpriteFundo()
{
}


SpriteFundo::~SpriteFundo()
{
}

void SpriteFundo::inicializar(float th, float tw)
{
	this->th = th;
	this->tw = tw;

	float vertices[] = {
		0.0f, 0.0f,  0.0f,  0.0f, 0.0f, // A
		0.0f,   th,  0.0f,  0.0f, 1.0f, // B
		  tw,   th,  0.0f,  1.0f, 1.0f, // C
		  tw, 0.0f,  0.0f,  1.0f, 0.0f  // D
	};
	int indices[] = {
		0, 1, 2,
		0, 2, 3,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void SpriteFundo::draw(glm::mat4 model)
{
	//shader->Use();
	//guarda posicao
	this->modelSprite = model;

	// Pegando a localização do uniform em que passaremos a matriz de transformação/modelo
	GLint modelLoc = glGetUniformLocation(shader->Program, "model");

	// Passando para o shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// vinculando a textura
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

	// Chama o shader
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
