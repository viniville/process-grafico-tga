#include "SceneManager.h"

#include <queue>

//static controllers for mouse and keyboard
static bool keys[1024];
static bool resized;
static GLuint width, height;
static queue<int> filaMovimentos;
static bool inicializacao;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint w, GLuint h)
{
	width = w;
	height = h;
	inicializacao = true;
	
	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();
}

void SceneManager::initializeGraphics()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "Hello Transform", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	//Setando a callback de redimensionamento da janela
	glfwSetWindowSizeCallback(window, resize);
	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Build and compile our shader program
	loadShaders();

	// CONFIGURAÇÃO
	setupScene();

	resized = true; //para entrar no setup da câmera na 1a vez
	
	setupCamera2D();
}

void SceneManager::loadShaders()
{
	string vFilename = "../shaders/transformations.vs";
	string fFilename = "../shaders/transformations.frag";
	shader = new Shader(vFilename.c_str(), fFilename.c_str());

	vFilename = "../shaders/spriteShader.vs";
	fFilename = "../shaders/spriteShader.frag";
	shaderSprite = new Shader(vFilename.c_str(), fFilename.c_str());
}

void SceneManager::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) {
			keys[key] = true;
			//cout << endl << "pressionou tecla : " << key;
			//if (key >= 262 && key <= 265) {
			//if ((key >= 321 && key <= 324) || (key >= 326 && key <= 329)) {
				//adiciona em uma fila para ser consumida ao renderizar
				//filaMovimentos.push(key); 
			//}
		}
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

glm::mat4 SceneManager::atualizarPosicaoPersonagem()
{
	//consome a fila de movimentos para atualizar a posicao do personagem
	glm::mat4 newModelSprite;

	while (!filaMovimentos.empty()) {
		switch (filaMovimentos.front())
		{
			default:
				break;
		}
		filaMovimentos.pop();
	}
	return newModelSprite;
}

void SceneManager::resize(GLFWwindow * window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	// Define the viewport dimensions
	glViewport(0, 0, width, height);
}


void SceneManager::do_movement()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void SceneManager::render()
{
	// Clear the colorbuffer
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render scene
	shader->Use();
	
	if (resized) //se houve redimensionamento na janela, redefine a projection matrix
	{
		setupCamera2D();
		resized = false;
	}

	glm::mat4 modelIniSprite;
	sprite.draw(modelIniSprite);
	spriteFundo.draw(modelIniSprite);
}

void SceneManager::run()
{
	//GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Update method(s)
		do_movement();

		//Render scene
		render();
		
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
}

void SceneManager::finish()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}


void SceneManager::setupScene()
{
	//Carrega texturas
	setupTexture();
	
	spriteFundo.setShader(shader);
	spriteFundo.setTexture(textureSprite);
	spriteFundo.inicializar(800, 600);

	//cria e inicializa sprite do personagem
	sprite.setShader(shader);
	sprite.setTexture(textureSprite);
	sprite.inicializar(64, 128);	
}

void SceneManager::setupCamera2D()
{
	glViewport(0, 0, width, height);

	//corrigindo o aspecto
	float ratio;
	float xMin = 0.0, xMax = 800, yMin = 600, yMax = 0.0, zNear = -1.0, zFar = 1.0;
	if (width >= height)
	{
		ratio = 1;//width / (float)height;
		projection = glm::ortho(xMin*ratio, xMax*ratio, yMin, yMax, zNear, zFar);
	}
	else
	{
		ratio = height / (float)width;
		projection = glm::ortho(xMin, xMax, yMin*ratio, yMax*ratio, zNear, zFar);
	}

	// Get their uniform location
	GLint projLoc = glGetUniformLocation(shader->Program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void SceneManager::setupTexture()
{
	//**********************************/
	/*    CONFIGURA TEXTURA DO TILE    */
	/***********************************/
	// load and create a texture 
	// -------------------------
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int t_width, t_height, nrChannels;
	//unsigned char *data = SOIL_load_image("../textures/wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	unsigned char *data = stbi_load("../textures/tileset2.png", &t_width, &t_height, &nrChannels, 0);
	
	cout << "Nro de canais: " << nrChannels << endl;
	cout << "largura x altura: " << t_width << " x " << t_height << endl;

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//**********************************/
	/*    CONFIGURA TEXTURA DO SPRITE  */
	/***********************************/
	glGenTextures(1, &textureSprite);
	glBindTexture(GL_TEXTURE_2D, textureSprite); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//unsigned char *data = SOIL_load_image("../textures/wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	unsigned char *data2 = stbi_load("../textures/mario4.png", &t_width, &t_height, &nrChannels, 0);

	cout << "Nro de canais: " << nrChannels << endl;
	cout << "largura x altura: " << t_width << " x " << t_height << endl;

	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data2);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}