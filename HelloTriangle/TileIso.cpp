#include "TileIso.h"


TileIso::TileIso()
{
	cor.r = rand() % 101 / 100.0; // 0 a 1, 1 casa decimal
	cor.g = rand() % 101 / 100.0;
	cor.b = rand() % 101 / 100.0;
	cor.a = 1.0; //por enquanto
}

TileIso::~TileIso()
{
}

void TileIso::inicializar(float th, float tw)
{
	this->th = th;
	this->tw = tw;
	/*
	    p2
	   /  \
	  p1   p3
	   \   /
	     p0
	*/
	////Inicializando com as coordenadas do losango
	float vertices[] = {
		   0.0f,  th/2.0f,  0.0f,  textureCoord[1].x, textureCoord[1].y, // A
		tw/2.0f,       th,  0.0f,  textureCoord[2].x, textureCoord[2].y, // B
		     tw,  th/2.0f,  0.0f,  textureCoord[3].x, textureCoord[3].y, // C
		tw/2.0f,     0.0f,  0.0f,  textureCoord[0].x, textureCoord[0].y  // D
	};

	int indices[] = {
		0, 1, 3,  //triangulo ABD
		1, 2, 3,  //triangulo BCD
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

void TileIso::setCor(float r, float g, float b)
{
	cor.r = r;
	cor.g = g;
	cor.b = b;
	cor.a = 1.0; //por enquanto
}

void TileIso::setTexture(unsigned int idxTexture) 
{
	texture = idxTexture;
}

void TileIso::setTextureCoord(vector<glm::vec2> coordenadas)
{
	textureCoord = coordenadas;
}

void TileIso::draw(glm::mat4 model)
{
	//shader->Use();
	this->modelTile = model;

	// Pegando a localização do uniform em que passaremos a matriz de transformação/modelo
	GLint modelLoc = glGetUniformLocation(shader->Program, "model");

	// Passando para o shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Pegando a localização do uniform em que passaremos a cor (provisório
	GLint colorLoc = glGetUniformLocation(shader->Program, "inputColor");
	glUniform4fv(colorLoc,1, glm::value_ptr(cor));

	// vinculando a textura
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

	// Chama o shader
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void TileIso::setShader(Shader * shader)
{
	this->shader = shader;
}
