#include "GLEW/glew.h"
#include "GL/freeglut.h"
#include "glm.hpp"
#include "ReadFile.h"
#include "Pipeline.h"
#include "PersProjInfo.h"
#include "Camera.h"


#define Window_Width  1920
#define Window_Height 1080

using namespace std;
using namespace glm;

GLuint VBO[2];
GLuint IBO[2];
GLuint VAO[2];
GLuint gWVPLocation;
GLuint gXAxisRot;
GLuint gYAxisRot;
GLuint gZAxisRot;
Camera* pCamera = NULL;
PersProjInfo persProj;

const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";

struct Vertex
{
	vec3 m_pos;
	vec4 m_color;
	int m_shader;

	Vertex() {}

	Vertex(vec3 pos, vec4 color, int shader)
	{
		m_pos = pos;
		m_color = color;
		m_shader = shader;
	}
};

static void RenderSceneCB()
{
	pCamera->OnRender();

	glClear(GL_COLOR_BUFFER_BIT); 

	static float Scale = 0.0;
	Scale += 0.001;

	Pipeline p;
	p.Scale(1.0f);
	p.Rotate(0.0f, 0.0f, 0.0f);
	p.WorldPos(0.0f, 0.0f, 0.0f);
	p.setCamera(*pCamera);
	p.setPersProj(persProj);

	glUniformMatrix4fv(gWVPLocation, 1, GL_FALSE, (const GLfloat*)p.getWVPTrans());
	glUniformMatrix4fv(gXAxisRot, 1, GL_FALSE, (const GLfloat*)p.getXAxisRot());
	glUniformMatrix4fv(gYAxisRot, 1, GL_FALSE, (const GLfloat*)p.getYAxisRot());
	glUniformMatrix4fv(gZAxisRot, 1, GL_FALSE, (const GLfloat*)p.getZAxisRot());

	glBindVertexArray(VAO[0]);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

	glBindVertexArray(VAO[1]);
	glDrawElements(GL_TRIANGLES, 120, GL_UNSIGNED_INT, 0);

	glutSwapBuffers();
}

static void SpecialKeyboardCB(int Key, int x, int y)
{
	pCamera->OnKeyboard(Key);
}

static void KeyboardCB(unsigned char Key, int x, int y)
{
	switch (Key) {
	case 'q':
		exit(0);
	}
}

static void PassiveMouseCB(int x, int y)
{
	pCamera->OnMouse(x, y);
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
	glutSpecialFunc(SpecialKeyboardCB);
	glutPassiveMotionFunc(PassiveMouseCB);
	glutKeyboardFunc(KeyboardCB);
}

static void CreateVertexBuffers()
{

	Vertex axisVert[12];

	float axisWidth = .005;

	axisVert[0] = { vec3(-1.0f, -.5*axisWidth, 0.0f), vec4(0.0, 0.0, 0.0, 1.0), 0 };
	axisVert[1] = { vec3(-1.0f, .5*axisWidth, 0.0f), vec4(0.0, 0.0, 0.0, 1.0), 0 };
	axisVert[2] = { vec3(1.0f, -.5*axisWidth, 0.0f), vec4(0.0, 0.0, 0.0, 1.0), 0 };
	axisVert[3] = { vec3(1.0f, .5*axisWidth, 0.0f), vec4(0.0, 0.0, 0.0, 1.0), 0 };
	axisVert[4] = { vec3(0.0f, -1.0f, -.5*axisWidth), vec4(0.0, 0.0, 0.0, 1.0), 0 };
	axisVert[5] = { vec3(0.0f, -1.0f, .5*axisWidth), vec4(0.0, 0.0, 0.0, 1.0), 0 };
	axisVert[6] = { vec3(0.0f, 1.0f, -.5*axisWidth), vec4(0.0, 0.0, 0.0, 1.0), 0 };
	axisVert[7] = { vec3(0.0f, 1.0f, .5*axisWidth), vec4(0.0, 0.0, 0.0, 1.0), 0 };
	axisVert[8] = { vec3(-.5*axisWidth, 0.0f, -1.0f), vec4(0.0, 0.0, 0.0, 1.0), 0 };
	axisVert[9] = { vec3(.5*axisWidth, 0.0f, -1.0f), vec4(0.0, 0.0, 0.0, 1.0), 0 };
	axisVert[10] = { vec3(-.5*axisWidth, 0.0f, 1.0f), vec4(0.0, 0.0, 0.0, 1.0), 0 };
	axisVert[11] = { vec3(.5*axisWidth, 0.0f, 1.0f), vec4(0.0, 0.0, 0.0, 1.0), 0 };

	glGenBuffers(1, &VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axisVert), axisVert, GL_STATIC_DRAW);

	Vertex spiralVert[42];

	for (int i = 0; i <= 20; i++) {
		spiralVert[2 * i] = { vec3(0.95 * cosf(.070710678 * i), 0.95 * sinf(.070710678 * i), 0.95 * .070710678 * i), vec4(0.0f, 0.0f, 1.0f, 1.0f), 1 };
		spiralVert[(2 * i) + 1] = { vec3(1.05 * cosf(.070710678 * i), 1.05 * sinf(.070710678 * i), 1.05 * .070710678 * i), vec4(0.0f, 0.0f, 1.0f, 1.0f), 1 };
	}

	glGenBuffers(1, &VBO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(spiralVert), spiralVert, GL_STATIC_DRAW);

}

static void CreateIndexBuffers()
{

	unsigned int axisIndex[] = { 0, 1, 3,
								 3, 2, 0,
								 4, 6, 7,
								 7, 5, 4,
								 8, 9, 11,
								 11, 10, 8
	};

	glGenBuffers(1, &IBO[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(axisIndex), axisIndex, GL_STATIC_DRAW);

	unsigned int spiralIndex[120];

	for (int i = 0; i < 20; i++) {
		spiralIndex[6 * i + 0] = 2 * i + 0;	spiralIndex[6 * i + 1] = 2 * i + 2;		spiralIndex[6 * i + 2] = 2 * i + 3;
		spiralIndex[6 * i + 3] = 2 * i + 3;	spiralIndex[6 * i + 4] = 2 * i + 1;		spiralIndex[6 * i + 5] = 2 * i + 0;
	}

	glGenBuffers(1, &IBO[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(spiralIndex), spiralIndex, GL_STATIC_DRAW);
}

static void CreateVertexArrays()
{
	
	glGenVertexArrays(1, &VAO[0]);
	glBindVertexArray(VAO[0]);


	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, sizeof(Vertex), (const GLvoid*)28);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[0]);


	glGenVertexArrays(1, &VAO[1]);
	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, sizeof(Vertex), (const GLvoid*)28);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[1]);

}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
	}

	glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
	GLuint ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	string vs, fs;

	if (!ReadFile(pVSFileName, vs)) {
		exit(1);
	}

	if (!ReadFile(pFSFileName, fs)) {
		exit(1);
	}

	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glUseProgram(ShaderProgram);

	gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
	assert(gWVPLocation != 0xFFFFFFFF);
	gXAxisRot = glGetUniformLocation(ShaderProgram, "xRotate");
	assert(gXAxisRot != 0xFFFFFFFF);
	gYAxisRot = glGetUniformLocation(ShaderProgram, "yRotate");
	assert(gYAxisRot != 0xFFFFFFFF);
	gZAxisRot = glGetUniformLocation(ShaderProgram, "zRotate");
	assert(gZAxisRot != 0xFFFFFFFF);
}

int main(int argc, char** argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(Window_Width, Window_Height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Grapher");
	glutFullScreenToggle();

	InitializeGlutCallbacks();

	pCamera = new Camera(Window_Width, Window_Height);

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	CreateVertexBuffers();
	CreateIndexBuffers();
	CreateVertexArrays();

	CompileShaders();

	persProj.FOV = 60.0f;
	persProj.Height = Window_Height;
	persProj.Width = Window_Width;
	persProj.zNear = 0.0f;
	persProj.zFar = 100.0f;

	glutMainLoop();

	return 0;

}