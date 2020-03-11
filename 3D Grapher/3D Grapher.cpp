#include <iostream>
#include <string>
#include "GLEW/glew.h"
#include "GL/freeglut.h"
#include "glm.hpp"
#include "ReadFile.h"
#include "Pipeline.h"
#include "PersProjInfo.h"
#include "Camera.h"
#include "Vertex.h"
#include "Curve.h"
#include "Surface.h"
#include "VectorFunction.h"


#define Window_Width  1920
#define Window_Height 1080

using namespace std;
using namespace glm;

GLuint gWVPLocation;
GLuint gXAxisRot;
GLuint gYAxisRot;
GLuint gZAxisRot;
Camera* pCamera = NULL;
PersProjInfo persProj;

GLuint* VBO;
GLuint* IBO;
GLuint* VAO;

Curve* curve[2];
Surface* surface[1];

unsigned int samples = pow(2, 6);

const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";


static void RenderSceneCB()
{
	pCamera->OnRender();

	glClear(GL_COLOR_BUFFER_BIT);

	static float Scale = 0.0;
	Scale += 0.001f;

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

	for (int i = 0; i < size(curve); i++) {
		glBindVertexArray(VAO[i + 1]);
		glDrawElements(GL_TRIANGLES, curve[i]->IBOsize(), GL_UNSIGNED_INT, 0);
	}

	for (int i = 0; i < size(surface); i++) {
		glBindVertexArray(VAO[i + size(curve) + 1]);
		glDrawElements(GL_TRIANGLES, surface[i]->IBOsize(), GL_UNSIGNED_INT, 0);
	}

	glutSwapBuffers();
}

static void KeyboardCB(unsigned char Key, int x, int y)
{
	switch (Key) {
	case 'q':
		exit(0);
	}
	pCamera->OnKeyboard(Key);
}

static void PassiveMouseCB(int x, int y)
{
	pCamera->OnMouse(x, y);
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
	glutPassiveMotionFunc(PassiveMouseCB);
	glutKeyboardFunc(KeyboardCB);
}

static void CreateVertexBuffers()
{

	Vertex axisVert[12];

	float axisWidth = .005f;

	axisVert[0] = { vec3(-1.0f, -.5*axisWidth, 0.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f), 0 };
	axisVert[1] = { vec3(-1.0f, .5*axisWidth, 0.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f), 0 };
	axisVert[2] = { vec3(1.0f, -.5*axisWidth, 0.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f), 0 };
	axisVert[3] = { vec3(1.0f, .5*axisWidth, 0.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f), 0 };
	axisVert[4] = { vec3(0.0f, -1.0f, -.5*axisWidth), vec4(0.0f, 0.0f, 0.0f, 1.0f), 0 };
	axisVert[5] = { vec3(0.0f, -1.0f, .5*axisWidth), vec4(0.0f, 0.0f, 0.0f, 1.0f), 0 };
	axisVert[6] = { vec3(0.0f, 1.0f, -.5*axisWidth), vec4(0.0f, 0.0f, 0.0f, 1.0f), 0 };
	axisVert[7] = { vec3(0.0f, 1.0f, .5*axisWidth), vec4(0.0f, 0.0f, 0.0f, 1.0f), 0 };
	axisVert[8] = { vec3(-.5*axisWidth, 0.0f, -1.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f), 0 };
	axisVert[9] = { vec3(.5*axisWidth, 0.0f, -1.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f), 0 };
	axisVert[10] = { vec3(-.5*axisWidth, 0.0f, 1.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f), 0 };
	axisVert[11] = { vec3(.5*axisWidth, 0.0f, 1.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f), 0 };

	glGenBuffers(1, &VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axisVert), axisVert, GL_STATIC_DRAW);

	for (int i = 0; i < size(curve); i++) {
		glGenBuffers(1, &VBO[i + 1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i + 1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * curve[i]->VBOsize(), curve[i]->curveVert(), GL_STATIC_DRAW);
	}

	for (int i = 0; i < size(surface); i++) {
		glGenBuffers(1, &VBO[i + size(curve) + 1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i + size(curve) + 1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * surface[i]->VBOsize(), surface[i]->surfaceVert(), GL_STATIC_DRAW);
	}
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
	
	for (int i = 0; i < size(curve); i++) {
		glGenBuffers(1, &IBO[i + 1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[i + 1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * curve[i]->IBOsize(), curve[i]->curveIndex(), GL_STATIC_DRAW);
	}

	for (int i = 0; i < size(surface); i++) {
		glGenBuffers(1, &IBO[i + size(curve) + 1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[i + size(curve) + 1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * surface[i]->IBOsize(), surface[i]->surfaceIndex(), GL_STATIC_DRAW);
	}
}

static void CreateVertexArrays()
{	
	for (int i = 0; i < 1 + size(curve) + size(surface); i++) {
		glGenVertexArrays(1, &VAO[i]);
		glBindVertexArray(VAO[i]);


		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
		glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, sizeof(Vertex), (const GLvoid*)28);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[i]);
	}
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

	VBO = new GLuint[1 + size(curve) + size(surface)];
	IBO = new GLuint[1 + size(curve) + size(surface)];
	VAO = new GLuint[1 + size(curve) + size(surface)];

	VectorFunction point("t", "t", "t");
	VectorFunction bound("s", "3*s+2*t", "t");

	curve[0] = new Curve(&point, -1, 1);
	surface[0] = new Surface(&bound, 0, 1, 0, 1, 16);

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