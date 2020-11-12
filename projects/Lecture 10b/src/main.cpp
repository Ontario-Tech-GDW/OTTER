#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream> //03
#include <string> //03

#include <GLM/glm.hpp> //04
#include <glm/gtc/matrix_transform.hpp> //04

#define STB_IMAGE_IMPLEMENTATION //07
#include <stb_image.h> //07

GLFWwindow* window;

unsigned char* image;
int width, height;

void loadImage() {
	int channels;
	stbi_set_flip_vertically_on_load(true);
	image = stbi_load("heightmap.bmp",
		&width,
		&height,
		&channels,
		0);
	if (image) {
		std::cout << "Image LOADED" << width << " " << height << std::endl;
	}
	else {
		std::cout << "Failed to load image!" << std::endl;
	}

}

bool initGLFW() {
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to Initialize GLFW" << std::endl;
		return false;
	}

	//Create a new GLFW window
	window = glfwCreateWindow(800, 800, "Window", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	return true;
}

bool initGLAD() {
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		std::cout << "Failed to initialize Glad" << std::endl;
		return false;
	}
}

GLuint shader_program;

bool loadShaders() {
	// Read Shaders from file
	std::string vert_shader_str;
	std::ifstream vs_stream("vertex_shader.glsl", std::ios::in);
	if (vs_stream.is_open()) {
		std::string Line = "";
		while (getline(vs_stream, Line))
			vert_shader_str += "\n" + Line;
		vs_stream.close();
	}
	else {
		printf("Could not open vertex shader!!\n");
		return false;
	}
	const char* vs_str = vert_shader_str.c_str();

	std::string frag_shader_str;
	std::ifstream fs_stream("frag_shader.glsl", std::ios::in);
	if (fs_stream.is_open()) {
		std::string Line = "";
		while (getline(fs_stream, Line))
			frag_shader_str += "\n" + Line;
		fs_stream.close();
	}
	else {
		printf("Could not open fragment shader!!\n");
		return false;
	}
	const char* fs_str = frag_shader_str.c_str();
	
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_str, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_str, NULL);
	glCompileShader(fs);

	shader_program = glCreateProgram();
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);

	return true;
}

//INPUT handling
float rx = 0.0f;
float ry = 0.0f;
float tz = 0.0f;
GLuint filter_mode = GL_NEAREST;

void keyboard() {
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		ry += 0.5;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		ry -= 0.5;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		rx -= 0.5;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		rx += 0.5;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		tz += 0.001;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		tz -= 0.001;
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		if (filter_mode == GL_LINEAR) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			filter_mode = GL_NEAREST;
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			filter_mode = GL_LINEAR;
		}
	}

	

}

GLfloat* points2;
GLfloat* uvs2;
int numVertices;

void genPlane(int d) {
	numVertices = 6 * (d * d);
	points2 = new GLfloat[3*numVertices];
	uvs2 = new GLfloat[2*numVertices];
	GLfloat x, y, z;
	int i = 0;
	x = -1.0;
	y = -0.5;
	z = 1.0;

	int uvcounter = 0;

	for (int col = 0; col < d; col++)
	for (int row = 0; row < d; row++) {

		points2[i++] = x + row * (2.0 / d);
		//std::cout << "Px[" << i - 1 << "] = " << points2[i - 1] << "\n";
		points2[i++] = y;
		//std::cout << "Py[" << i - 1 << "] = " << points2[i - 1] << "\n";
		points2[i++] = z - col * (2.0 / d);
		//std::cout << "Pz[" << i - 1 << "] = " << points2[i - 1] << "\n";

		uvs2[uvcounter++] = 0.0 + row*(1.0/d);
		uvs2[uvcounter++] = 0.0 + col*(1.0/d);

		points2[i++] = points2[i-3] + (2.0 / d); //std::cout << points2[i-4] << " " << 2.0/d << "\n";
		//std::cout << "Px[" << i - 1 << "] = " << points2[i - 1] << "\n";
		points2[i++] = y;
		//std::cout << "Py[" << i - 1 << "] = " << points2[i - 1] << "\n";
		points2[i++] = points2[i-3];
		//std::cout << "Pz[" << i - 1 << "] = " << points2[i - 1] << "\n";

		uvs2[uvcounter++] = uvs2[uvcounter-2] + 1.0/d;
		uvs2[uvcounter++] = uvs2[uvcounter-2];

		points2[i++] = points2[i-6];
		//std::cout << "Px[" << i - 1 << "] = " << points2[i - 1] << "\n";
		points2[i++] = y;
		//std::cout << "Py[" << i - 1 << "] = " << points2[i - 1] << "\n";
		points2[i++] = points2[i-3] - (2.0 / d);
		//std::cout << "Pz[" << i - 1 << "] = " << points2[i - 1] << "\n";

		uvs2[uvcounter++] = uvs2[uvcounter - 4];
		uvs2[uvcounter++] = uvs2[uvcounter - 2] + 1.0/d;


		points2[i++] = points2[i - 6];
		//std::cout << "Px[" << i - 1 << "] = " << points2[i - 1] << "\n";
		points2[i++] = y;
		//std::cout << "Py[" << i - 1 << "] = " << points2[i - 1] << "\n";
		points2[i++] = points2[i - 6];
		//std::cout << "Pz[" << i - 1 << "] = " << points2[i - 1] << "\n";

		uvs2[uvcounter++] = uvs2[uvcounter - 4];
		uvs2[uvcounter++] = uvs2[uvcounter - 4];

		points2[i++] = points2[i - 3];
		//std::cout << "Px[" << i - 1 << "] = " << points2[i - 1] << "\n";
		points2[i++] = y;
		//std::cout << "Py[" << i - 1 << "] = " << points2[i - 1] << "\n";
		points2[i++] = points2[i - 3] - (2.0 / d);
		//std::cout << "Pz[" << i - 1 << "] = " << points2[i - 1] << "\n";

		uvs2[uvcounter++] = uvs2[uvcounter - 2];
		uvs2[uvcounter++] = uvs2[uvcounter - 4];


		points2[i++] = points2[i - 9];
		//std::cout << "Px[" << i - 1 << "] = " << points2[i - 1] << "\n";
		points2[i++] = y;
		//std::cout << "Py[" << i - 1 << "] = " << points2[i - 1] << "\n";
		points2[i++] = points2[i - 3];
		//std::cout << "Pz[" << i - 1 << "] = " << points2[i - 1] << "\n";

		uvs2[uvcounter++] = uvs2[uvcounter - 6];
		uvs2[uvcounter++] = uvs2[uvcounter - 6];

	}
}



int main() {
	
	//Initialize GLFW
	if (!initGLFW())
		return 1;
	
	//Initialize GLAD
	if (!initGLAD())
		return 1;

	// Mesh data
	genPlane(100);

	// Color data
	static const GLfloat colors[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f
	};
	
	//VBO
	GLuint pos_vbo = 0;
	glGenBuffers(1, &pos_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glBufferData(GL_ARRAY_BUFFER, (3*numVertices)*sizeof(GLfloat), points2, GL_STATIC_DRAW);

	GLuint color_vbo = 1;
	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	// VAO
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	
	//			(index, size, type, normalized, stride, pointer)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	//			(stride: byte offset between consecutive values)
	//			(pointer: offset of the first component of the 
	//			first attribute in the array - initial value is 0)
	
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	   
	GLuint uv_vbo = 2;
	glGenBuffers(1, &uv_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
	glBufferData(GL_ARRAY_BUFFER, (2*numVertices)*sizeof(GLfloat), uvs2, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	loadImage();
	
	GLuint textureHandle;
	
	glGenTextures(1, &textureHandle);
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	// Release the space used for your image once you're done
	stbi_image_free(image);
	

	// Load your shaders
	if (!loadShaders())
		return 1;

	// Lecture 04
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glm::mat4 Projection = 
		glm::perspective(glm::radians(45.0f), 
		(float)width / (float)height, 0.0001f, 100.0f);
	
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 2, 4), // Camera is at (0,0,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// create individual matrices glm::mat4 T R and S, then multiply them
	Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, 0.0f));


	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

		// T * R * S usual order
	// Or if you want to rotate the object around a certain point
	// T * R * point T * S
	
	// Get a handle for our "MVP" uniform
// Only during the initialisation
	GLuint MatrixID = 
		glGetUniformLocation(shader_program, "MVP");

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);

	/////// TEXTURE
	glUniform1i(glGetUniformLocation(shader_program, "myTextureSampler"), 0);
	
	
	
	////// LECTURE 10
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	//LECTURE 10B
	GLuint loc = glGetUniformLocation(shader_program, "time");
	GLfloat time = 0.0;
	
	
	///// Game loop /////
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader_program);

		keyboard();

		Model = glm::rotate(Model, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
		Model = glm::rotate(Model, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
		Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, tz));
		mvp = Projection * View * Model;
		rx = ry = tz = 0;
		
		glUniformMatrix4fv(MatrixID, 1, 
			GL_FALSE, &mvp[0][0]);

		//LECTURE 10B
		glUniform1f(loc, time);
		time += 0.1;

		
		glDrawArrays(GL_TRIANGLES, 0, numVertices);

		glfwSwapBuffers(window);
	}
	return 0;

}