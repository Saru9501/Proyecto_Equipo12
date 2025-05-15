/*Autores:          Yesenia / Gabriela / Alan
* Num. Cuenta: 317248683 / 317313521 / 115002762
* Proyecto Equipo 12
* Fecha: 14 de mayo de 2025
*/
#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 1200, HEIGHT = 800;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 4.5f, 20.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0);
//Anim
glm::vec3 TipaPos(-2.93594, 3.42643, 40.3427f);
bool activateAnimation = false;
float rotationAngle = 0.0f;
float scaleOldDesks = 1.0f;
float newDesksYPos = 10.0f; // Posición inicial en el cielo
float animationTime = 0.0f;
const float animationDuration = 5.0f; // Duración total de la animación en segundos
float column_2 = 14.0f; //No quitar, es para el acomodo de modelos
float column_2n = 15.7f;
float altura = 0.5f;
float showComputersAndKeyboards = 0;
float velocidad = 0.01f; //velosidad para la tipa 

float tipaWalkDistance = 0.0f;
const float maxWalkDistance = 10.0f; // Límite de distancia para caminar
bool tipaWalking = false;
float walkCycleTime = 0.0f;
const float walkCycleSpeed = 4.0f; // Velocidad del ciclo de caminata

//movimiento de la tipa
float desplazamientoZ = 0.0f;      // Controla el movimiento hacia adelante
float rotBrazoD = 0.0f, rotBrazoI = 0.0f;  // Rotación brazos
float rotBrazoD2 = 0.0f, rotBrazoI2 = 0.0f;  // Rotación brazos para caminar
float rotPiernaD = 0.0f, rotPiernaI = 0.0f; // Rotación piernas
bool caminando = false;            // Estado de caminar (activado con C)

// Variables para animación de brazos extendidos
bool armsUp = false;
float armRaiseTime = 0.0f;
const float armRaiseDuration = 1.0f; // Duración de la animación de levantar brazos
float armRaiseAngle = 0.0f; // Ángulo actual de elevación de brazos

// Variables para la animación de la esfera
bool showParedes = false;
float Time = 1.0f;
float MaxTime = 3.0f;
float tiempo = 0.0f;

// Variables para la animación del suelo nuevo 
bool activateFloorAnimation = false;
float floorAnimationTime = 0.0f;
const float floorAnimationDuration = 5.0f; // Duración total de la animación

glm::vec3 suelo2FinalPos = glm::vec3(7.0f, 0.0f, 48.2758f);
glm::vec3 suelo3FinalPos = glm::vec3(7.0f, 0.0f, 48.2758f);
glm::vec3 suelo4FinalPos = glm::vec3(7.0f, 0.0f, 48.2758f);
glm::vec3 suelo5FinalPos = glm::vec3(7.0f, 0.0f, 48.2758f);
glm::vec3 suelo6FinalPos = glm::vec3(7.0f, 0.0f, 48.2758f);
glm::vec3 suelo7FinalPos = glm::vec3(7.0f, 0.0f, 48.2758f);
glm::vec3 suelo8FinalPos = glm::vec3(7.0f, 0.0f, 48.2758f);
glm::vec3 suelo9FinalPos = glm::vec3(7.0f, 0.0f, 48.2758f);

glm::vec3 suelo2InitialPos = glm::vec3(4.33657f, 0.0f, 29.6884f);
glm::vec3 suelo3InitialPos = glm::vec3(4.29459f, 0.0f, -17.9097f);
glm::vec3 suelo4InitialPos = glm::vec3(11.6496f, 0.0f, -11.2313f);
glm::vec3 suelo5InitialPos = glm::vec3(-4.26047f, 0.0f, 2.64633f);
glm::vec3 suelo6InitialPos = glm::vec3(6.91035f, 0.0f, 5.90945f);
glm::vec3 suelo7InitialPos = glm::vec3(12.0511f, 0.0f, 2.73778f);
glm::vec3 suelo8InitialPos = glm::vec3(-1.17046f, 0.0f, 20.507f);
glm::vec3 suelo9InitialPos = glm::vec3(11.8622f, 0.0f, 20.4255f);

glm::vec3 savedPosition = camera.GetPosition();
glm::vec3 savedFront = camera.GetFront();
// Variables para la animacion del techo y las sillas
bool activateAnimation2 = false;
float roofAngle = 0.0f;          //  ngulo de inclinaci n del techo
float oldChairsOffset = 0.0f;    // Desplazamiento de las sillas antiguas
float newChairsOffset = 30.0f;  // Desplazamiento inicial de las sillas nuevas
float animationTime2 = 0.0f;
const float animationDuration2 = 8.0f; // Duraci n total de la animaci n en segundos
int animationPhase = 0;          // Fase actual de la animaci n

bool showOldChairs = true;

//piso bool
bool aparecePiso = true;

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Equipo 12: Yesenia y Gabriela", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	//models
	Model Escritorios((char*)"Models/all_desk.obj");
	Model NewEscri((char*)"Models/all_new_desk.obj");
	Model Compu((char*)"Models/computadora.obj");
	Model Teclado((char*)"Models/teclado.obj");
	Model Silla((char*)"Models/silla.obj");
	Model SillaNueva((char*)"Models/chairNew.obj");
	Model Piso((char*)"Models/suelo.obj");
	Model Paredes((char*)"Models/Paredes.obj");
	Model ParedesNuevas((char*)"Models/ParedesNuevas.obj");
	Model Techo((char*)"Models/techo.obj");
	Model luz((char*)"Models/leds.obj");
	Model Ventana((char*)"Models/all_windows.obj");
	Model Pizza((char*)"Models/pizzaron.obj");
	Model bodega((char*)"Models/bodega.obj");
	Model emerg((char*)"Models/emerg.obj");
	Model cosoMadera((char*)"Models/cosoMadera.obj");
	Model puertaDer((char*)"Models/puertaDer.obj");
	Model puertaIzq((char*)"Models/puertaIzq.obj");
	Model VentanasNuevas((char*)"Models/VentanasNuevas.obj");
	Model sueloNuevo((char*)"Models/sueloNuevo.obj");
	Model columnaIzq((char*)"Models/columnaIzq.obj");
	Model columnaDer((char*)"Models/columnaDer.obj");

	//Piso Nuevo
	Model suelo1((char*)"Models/SueloN/1.obj");
	Model suelo2((char*)"Models/SueloN/2.obj");
	Model suelo3((char*)"Models/SueloN/3.obj");
	Model suelo4((char*)"Models/SueloN/4.obj");
	Model suelo5((char*)"Models/SueloN/5.obj");
	Model suelo6((char*)"Models/SueloN/6.obj");
	Model suelo7((char*)"Models/SueloN/7.obj");
	Model suelo8((char*)"Models/SueloN/8.obj");
	Model suelo9((char*)"Models/SueloN/9.obj");

	//Tipa 
	Model cuerpo((char*)"Models/Persona/cuerpo.obj");
	Model musloI((char*)"Models/Persona/MusloIzq.obj");
	Model musloD((char*)"Models/Persona/MusloDer.obj");
	Model BrazoI((char*)"Models/Persona/BrazoIzq.obj");
	Model BrazoD((char*)"Models/Persona/BrazoDer.obj");



	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model(1);

		//Cargas de modelos 
		//Lamparas
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		luz.Draw(lightingShader);

		// Dibujar el techo con inclinacion
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 31.9f));
		model = glm::rotate(model, glm::radians(roofAngle), glm::vec3(1.0f, 0.0f, 0.0f)); // Inclinar en el eje X
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Techo.Draw(lightingShader);

		if (scaleOldDesks > 0.0f) {
			model = glm::mat4(1);
			model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(scaleOldDesks));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Escritorios.Draw(lightingShader);
		}

		// Dibujar nuevos escritorios (si la animación está activa o completada)
		if (activateAnimation || scaleOldDesks == 0.0f) {
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, newDesksYPos, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			NewEscri.Draw(lightingShader);
		}

		//Pizzaron
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pizza.Draw(lightingShader);

		if (showComputersAndKeyboards == 0) {
			//Compus
			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			//Segunda columna
			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Compu.Draw(lightingShader);
			//Teclados
			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			//Segunda columna
			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Teclado.Draw(lightingShader);



		}
		else {
			if (showComputersAndKeyboards == 1)
			{
				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				////Segunda columna
				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Compu.Draw(lightingShader);

				//Teclados nuevos

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f + altura, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f + altura, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f + altura, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f + altura, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f + altura, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f + altura, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f + altura, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f + altura, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f + altura, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f + altura, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f + altura, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f + altura, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f + altura, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f + altura, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f + altura, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f + altura, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f + altura, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f + altura, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f + altura, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f + altura, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f + altura, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f + altura, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f + altura, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f + altura, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f + altura, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f + altura, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f + altura, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f + altura, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f, 0.0f + altura, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f, 0.0f + altura, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f, 0.0f + altura, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f, 0.0f + altura, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				////Segunda columna
				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f + altura, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f + altura, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f + altura, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f + altura, 0.1f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f + altura, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f + altura, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f + altura, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f + altura, 6.2f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f + altura, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f + altura, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f + altura, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f + altura, 12.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f + altura, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f + altura, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f + altura, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f + altura, 18.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f + altura, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f + altura, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f + altura, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f + altura, 24.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f + altura, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f + altura, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f + altura, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f + altura, -6.6f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f + altura, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f + altura, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f + altura, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f + altura, -12.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1.3f + column_2n, 0.0f + altura, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-2.15f + column_2n, 0.0f + altura, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-5.6f + column_2n, 0.0f + altura, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(-8.90f + column_2n, 0.0f + altura, -18.4f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Teclado.Draw(lightingShader);
			}
		}


		//Sillaaaaaa
		if (showOldChairs) {
			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f + oldChairsOffset, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f + oldChairsOffset, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f + oldChairsOffset, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f + oldChairsOffset, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f + oldChairsOffset, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f + oldChairsOffset, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f + oldChairsOffset, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f + oldChairsOffset, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f + oldChairsOffset, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f + oldChairsOffset, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f + oldChairsOffset, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f + oldChairsOffset, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f + oldChairsOffset, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f + oldChairsOffset, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f + oldChairsOffset, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f + oldChairsOffset, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f + oldChairsOffset, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f + oldChairsOffset, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f + oldChairsOffset, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f + oldChairsOffset, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f + oldChairsOffset, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f + oldChairsOffset, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f + oldChairsOffset, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f + oldChairsOffset, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f + oldChairsOffset, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f + oldChairsOffset, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f + oldChairsOffset, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f + oldChairsOffset, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f + oldChairsOffset, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f, 0.0f + oldChairsOffset, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f, 0.0f + oldChairsOffset, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f, 0.0f + oldChairsOffset, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			//Segunda columna
			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f + oldChairsOffset, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f + oldChairsOffset, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f + oldChairsOffset, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f + oldChairsOffset, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f + oldChairsOffset, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f + oldChairsOffset, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f + oldChairsOffset, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f + oldChairsOffset, 6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f + oldChairsOffset, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f + oldChairsOffset, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f + oldChairsOffset, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f + oldChairsOffset, 12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f + oldChairsOffset, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f + oldChairsOffset, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f + oldChairsOffset, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f + oldChairsOffset, 18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f + oldChairsOffset, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f + oldChairsOffset, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f + oldChairsOffset, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f + oldChairsOffset, 24.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f + oldChairsOffset, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f + oldChairsOffset, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f + oldChairsOffset, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f + oldChairsOffset, -6.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f + oldChairsOffset, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f + oldChairsOffset, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f + oldChairsOffset, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f + oldChairsOffset, -12.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f + column_2, 0.0f + oldChairsOffset, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-2.15f + column_2, 0.0f + oldChairsOffset, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-4.3f + column_2, 0.0f + oldChairsOffset, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-6.45f + column_2, 0.0f + oldChairsOffset, -18.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);
		}
		//SillaNuevaaaaaa
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f + newChairsOffset, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.4f, 0.0f + newChairsOffset, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.7f, 0.0f + newChairsOffset, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f + newChairsOffset, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);


		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f + newChairsOffset, 6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.4f, 0.0f + newChairsOffset, 6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.7f, 0.0f + newChairsOffset, 6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f + newChairsOffset, 6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);


		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f + newChairsOffset, 12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.4f, 0.0f + newChairsOffset, 12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.7f, 0.0f + newChairsOffset, 12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f + newChairsOffset, 12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);


		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f + newChairsOffset, 18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.4f, 0.0f + newChairsOffset, 18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.7f, 0.0f + newChairsOffset, 18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f + newChairsOffset, 18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);


		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f + newChairsOffset, 24.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.4f, 0.0f + newChairsOffset, 24.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.7f, 0.0f + newChairsOffset, 24.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f + newChairsOffset, 24.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);


		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f + newChairsOffset, -6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.4f, 0.0f + newChairsOffset, -6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.7f, 0.0f + newChairsOffset, -6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f + newChairsOffset, -6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);


		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f + newChairsOffset, -12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.4f, 0.0f + newChairsOffset, -12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.7f, 0.0f + newChairsOffset, -12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f + newChairsOffset, -12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);


		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f + newChairsOffset, -18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.4f, 0.0f + newChairsOffset, -18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.7f, 0.0f + newChairsOffset, -18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f + newChairsOffset, -18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		//Segunda columna


		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f + column_2n, 0.0f + newChairsOffset, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.4f + column_2n, 0.0f + newChairsOffset, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.7f + column_2n, 0.0f + newChairsOffset, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f + column_2n, 0.0f + newChairsOffset, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);


		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f + column_2n, 0.0f + newChairsOffset, 6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.4f + column_2n, 0.0f + newChairsOffset, 6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.7f + column_2n, 0.0f + newChairsOffset, 6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f + column_2n, 0.0f + newChairsOffset, 6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);


		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f + column_2n, 0.0f + newChairsOffset, 12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.4f + column_2n, 0.0f + newChairsOffset, 12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.7f + column_2n, 0.0f + newChairsOffset, 12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f + column_2n, 0.0f + newChairsOffset, 12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);


		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f + column_2n, 0.0f + newChairsOffset, 18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.4f + column_2n, 0.0f + newChairsOffset, 18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.7f + column_2n, 0.0f + newChairsOffset, 18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f + column_2n, 0.0f + newChairsOffset, 18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);


		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f + column_2n, 0.0f + newChairsOffset, 24.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.4f + column_2n, 0.0f + newChairsOffset, 24.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.7f + column_2n, 0.0f + newChairsOffset, 24.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f + column_2n, 0.0f + newChairsOffset, 24.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);


		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f + column_2n, 0.0f + newChairsOffset, -6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.4f + column_2n, 0.0f + newChairsOffset, -6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.7f + column_2n, 0.0f + newChairsOffset, -6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f + column_2n, 0.0f + newChairsOffset, -6.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);


		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f + column_2n, 0.0f + newChairsOffset, -12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.4f + column_2n, 0.0f + newChairsOffset, -12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.7f + column_2n, 0.0f + newChairsOffset, -12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f + column_2n, 0.0f + newChairsOffset, -12.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);


		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f + column_2n, 0.0f + newChairsOffset, -18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.4f + column_2n, 0.0f + newChairsOffset, -18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.7f + column_2n, 0.0f + newChairsOffset, -18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f + column_2n, 0.0f + newChairsOffset, -18.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaNueva.Draw(lightingShader);

		//bodega
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		bodega.Draw(lightingShader);

		//emergencia
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		emerg.Draw(lightingShader);

		//coso madera
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cosoMadera.Draw(lightingShader);

		//puerta Izquierda
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		puertaIzq.Draw(lightingShader);

		//puerta Dere
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		puertaDer.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		if (aparecePiso == true) {
			//Paredes
			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Paredes.Draw(lightingShader);

			//Ventanas
			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Ventana.Draw(lightingShader);
		}
		else {
			//Paredes Nuevas
			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			ParedesNuevas.Draw(lightingShader);

			//Ventanas nuevas
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-14.0f + column_2, 0.0f, -0.02f));
			//model = glm::scale(model, glm::vec3(1.25f, 0.75f, 0.0f)); 
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			VentanasNuevas.Draw(lightingShader);
		}
		//Suelo nuevo
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(23.0705f, 0.0f, 3.59264f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		suelo1.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, suelo2FinalPos);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		suelo2.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, suelo3FinalPos);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		suelo3.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, suelo4FinalPos);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		suelo4.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, suelo5FinalPos);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		suelo5.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, suelo6FinalPos);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		suelo6.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, suelo7FinalPos);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		suelo7.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, suelo8FinalPos);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		suelo8.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, suelo9FinalPos);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		suelo9.Draw(lightingShader);

		//Modelo de la tipa
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Body
		modelTemp = model = glm::translate(model, TipaPos);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cuerpo.Draw(lightingShader);

		//BrazoD
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.368652f, -0.31328f, 0.114035f));
		model = glm::rotate(model, glm::radians(rotBrazoD), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación en Z
		model = glm::rotate(model, glm::radians(rotBrazoD2), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BrazoD.Draw(lightingShader);

		//BrazoI
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.382797f, -0.31328f, 0.114035f));
		model = glm::rotate(model, glm::radians(rotBrazoI), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(rotBrazoI2), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BrazoI.Draw(lightingShader);

		// Muslo Derecho (rotación para caminar)
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.193549f, -1.217093f, 0.026334f));
		model = glm::rotate(model, glm::radians(rotPiernaD), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		musloD.Draw(lightingShader);

		// Muslo Izquierdo (rotación opuesta)
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.193549f, -1.217093f, 0.026334f));
		model = glm::rotate(model, glm::radians(rotPiernaI), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		musloI.Draw(lightingShader);
		
		//Columna derecha 
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-14.0f + column_2, 0.0f, -0.02f));
		//model = glm::scale(model, glm::vec3(1.25f, 0.75f, 0.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		columnaDer.Draw(lightingShader);

		//Columna izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-14.0f + column_2, 0.0f, -0.02f));
		//model = glm::scale(model, glm::vec3(1.25f, 0.75f, 0.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		columnaIzq.Draw(lightingShader);

		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);

		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
	if (keys[GLFW_KEY_R]) {
		activateAnimation = true;
		animationTime = 0.0f;
	}
	if (keys[GLFW_KEY_T]) {
		aparecePiso = !aparecePiso;
	}
	if (keys[GLFW_KEY_Y]) {
		activateAnimation2 = true;
		animationTime2 = 0.0f;
		showOldChairs = true;
	}
	if (keys[GLFW_KEY_C]) {
		if (!tipaWalking) {
			tipaWalking = true;
			tipaWalkDistance = 0.0f; // Resetear distancia
			walkCycleTime = 0.0f;    // Resetear ciclo
			armsUp = false;          // Desactivar brazos levantados
		}
		else {
			tipaWalking = false;
			// No resetear rotaciones aquí, dejar que DoMovement() maneje la transición
		}
	}
	if (keys[GLFW_KEY_F]) {
		activateFloorAnimation = true;
		floorAnimationTime = 0.0f;
	}
}

void Animation() {
	//Animacion de escritorios
	if (activateAnimation) {
		animationTime += deltaTime;
		float progress = animationTime / animationDuration;
		showComputersAndKeyboards = 2;

		if (progress < 0.5f) {
			// Fase 1: Rotacion y reduccion de escala de los escritorios antiguos
			rotationAngle += 20.0f; // Giro r pido
			scaleOldDesks = 1.0f - progress * 2.0f; // Reduce la escala hasta 0
		}
		else if (progress < 1.0f) {
			// Fase 2: Ca da de los nuevos escritorios
			newDesksYPos = 10.0f - (progress - 0.5f) * 20.0f; // Cae desde Y=10 a Y=0
		}
		else {
			// Finalizar animaci n
			activateAnimation = false;
			rotationAngle = 0.0f;
			scaleOldDesks = 0.0f;
			newDesksYPos = 0.0f;
			showComputersAndKeyboards = 1;
		}
	}
	//Animacion de sillas
	if (activateAnimation2) {
		animationTime2 += deltaTime;
		float progress = animationTime2 / animationDuration2;

		if (progress < 0.25f) {
			// Fase 1: Inclinacion del techo (0% - 25% del tiempo)
			animationPhase = 1;
			roofAngle = glm::mix(0.0f, 30.0f, progress / 0.25f); // Inclinar hasta 15 grados
		}
		else if (progress < 0.5f) {
			// Fase 2: Salida de las sillas antiguas (25% - 50% del tiempo)
			animationPhase = 2;
			oldChairsOffset = glm::mix(0.0f, 30.0f, (progress - 0.25f) / 0.25f); // Mover sillas hacia afuera
		}
		else if (progress < 0.75f) {
			// Fase 3: Entrada de las sillas nuevas (50% - 75% del tiempo)
			animationPhase = 3;
			newChairsOffset = glm::mix(30.0f, 0.0f, (progress - 0.5f) / 0.25f); // Mover sillas hacia adentro
		}
		else if (progress < 1.0f) {
			// Fase 4: Regreso del techo a su posici n original (75% - 100% del tiempo)
			animationPhase = 4;
			roofAngle = glm::mix(30.0f, 0.0f, (progress - 0.75f) / 0.25f); // Regresar a 0 grados
		}
		else {
			// Finalizar animaci n
			activateAnimation2 = false;
			showOldChairs = false;
			animationTime2 = 0.0f;
			roofAngle = 0.0f;
			oldChairsOffset = 0.0f;
			newChairsOffset = 0.0f;
			animationPhase = 0;
		}
	}
	//Animacion de las paredes
	// Animación de caminar
	if (tipaWalking && tipaWalkDistance < maxWalkDistance) {
		float walkSpeed = 1.2f * deltaTime; // Velocidad de desplazamiento

		// Actualizar posición
		TipaPos.z -= walkSpeed;
		tipaWalkDistance += walkSpeed;

		// Animación cíclica de caminata
		walkCycleTime += deltaTime * walkCycleSpeed;

		// Movimiento de brazos (alternados)
		rotBrazoD2 = sin(walkCycleTime) * 35.0f; // Oscila entre -30 y 30 grados
		rotBrazoI2 = -rotBrazoD2; // Brazo izquierdo hace lo contrario

		// Movimiento de piernas (alternados con mayor amplitud)
		rotPiernaD = sin(walkCycleTime) * 25.0f; // Oscila entre -45 y 45 grados
		rotPiernaI = -rotPiernaD;

		// Pequeño movimiento vertical para simular pasos
		TipaPos.y = 3.42643f + sin(walkCycleTime * 2.0f) * 0.05f;

		// Resetear animación de brazos levantados si estaba activa
		armsUp = false;
		armRaiseAngle = 0.0f;
	}
	else if (tipaWalking) {
		// Detener la animación cuando se alcanza el límite
		tipaWalking = false;
		rotBrazoD2 = rotBrazoI2 = rotPiernaD = rotPiernaI = 0.0f;
		TipaPos.y = 3.42643f; // Restablecer altura

		// Activar animación de brazos levantados
		armsUp = true;
		armRaiseTime = 0.0f;
	}
	// Animación de brazos levantados
	if (armsUp && armRaiseTime < armRaiseDuration) {
		armRaiseTime += deltaTime;
		float progress = armRaiseTime / armRaiseDuration;

		// Suavizar la animación con función de ease-out
		float t = 1.0f - (1.0f - progress) * (1.0f - progress);
		armRaiseAngle = t * 120.0f; // Levantar hasta 120 grados

		// Aplicar a ambos brazos
		rotBrazoD = armRaiseAngle;
		rotBrazoI = -armRaiseAngle;
		showParedes = true;
	}
	if (showParedes) {
		tiempo += Time * deltaTime;

		// Cuando alcanza el tamaño máximo, cambiar las paredes
		if (tiempo >= MaxTime) {
			aparecePiso = false; // Esto cambiará las paredes por las nuevas
		}
	}
	//Animacion del suelo 
	if (activateFloorAnimation && floorAnimationTime < floorAnimationDuration) {
		floorAnimationTime += deltaTime;
		float progress = floorAnimationTime / floorAnimationDuration;

		// Suavizar la animación (ease-in-out)
		float t = pow(progress, 7) * (3.0f - 2.0f * progress);
		// Mover cada pieza desde su posición inicial a la final
		if (progress > 0.1f) suelo3FinalPos = glm::mix(suelo3InitialPos, suelo3FinalPos, t);
		if (progress > 0.2f) suelo4FinalPos = glm::mix(suelo4InitialPos, suelo4FinalPos, t);
		if (progress > 0.3f) suelo5FinalPos = glm::mix(suelo5InitialPos, suelo5FinalPos, t);
		if (progress > 0.4f) suelo6FinalPos = glm::mix(suelo6InitialPos, suelo6FinalPos, t);
		if (progress > 0.5f) suelo7FinalPos = glm::mix(suelo7InitialPos, suelo7FinalPos, t);
		if (progress > 0.6f) suelo8FinalPos = glm::mix(suelo8InitialPos, suelo8FinalPos, t);
		if (progress > 0.7f) suelo9FinalPos = glm::mix(suelo9InitialPos, suelo9FinalPos, t);
		if (progress > 0.8f) suelo2FinalPos = glm::mix(suelo2InitialPos, suelo2FinalPos, t);
	}
}
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}