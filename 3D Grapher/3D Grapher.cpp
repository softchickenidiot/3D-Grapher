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
#include "VectorFunction.h"


#define Window_Width  1920
#define Window_Height 1080

using namespace std;
using namespace glm;

GLuint VBO[4];
GLuint IBO[4];
GLuint VAO[4];
GLuint gWVPLocation;
GLuint gXAxisRot;
GLuint gYAxisRot;
GLuint gZAxisRot;
Camera* pCamera = NULL;
PersProjInfo persProj;

Curve* curve1;
Curve* curve2;

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

	glBindVertexArray(VAO[1]);
	glDrawElements(GL_TRIANGLES, 99462, GL_UNSIGNED_INT, 0);


	glBindVertexArray(VAO[2]);
	glDrawElements(GL_TRIANGLES, curve1->IBOsize(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(VAO[3]);
	glDrawElements(GL_TRIANGLES, curve2->IBOsize(), GL_UNSIGNED_INT, 0);

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

	Vertex surface[5*65*65];

	float s;
	float t;
	vec3 pos;
	vec3 partialS;
	vec3 partialT;
	vec4 color;

	for (int i = 0; i < samples + 1; i++) {
		for (int j = 0; j < samples + 1; j++) {
			s = 4.0f * pi<float>() * float(i) / float(samples) - 2*pi<float>();
			t = 4.0f * pi<float>() * float(j) / float(samples) - 2*pi<float>();
			pos = vec3(s, sinf(t)*sinf(s), t);
			partialS = vec3(1, sin(t)*cos(s), 0);
			partialT = vec3(0, cos(t)*sin(s), cosf(t));
			color = vec4(float(j) / samples, float(i) / samples, 0.0f, 1.0f);
			surface[5 * ((samples + 1) * i + j) + 0] = { pos, color, 1 };
			surface[5 * ((samples + 1) * i + j) + 1] = { pos - .005f * partialS, vec4(0.0f, 0.0f, 0.0f, 1.0f), 1 };
			surface[5 * ((samples + 1) * i + j) + 2] = { pos + .005f * partialS, vec4(0.0f, 0.0f, 0.0f, 1.0f), 1 };
			surface[5 * ((samples + 1) * i + j) + 3] = { pos - .005f * partialT, vec4(0.0f, 0.0f, 0.0f, 1.0f), 1 };
			surface[5 * ((samples + 1) * i + j) + 4] = { pos + .005f * partialT, vec4(0.0f, 0.0f, 0.0f, 1.0f), 1 };

		}
	}

	glGenBuffers(1, &VBO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(surface), surface, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*curve1->VBOsize(), curve1->curveVert(), GL_STATIC_DRAW);

	glGenBuffers(1, &VBO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * curve2->VBOsize(), curve2->curveVert(), GL_STATIC_DRAW);
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

	unsigned int surface[99462];

	for (int i = 0; i < samples; i++) {
		for (int j = 0; j < samples; j++) {
			surface[6 * (samples * i + j) + 0] = 5 * ((samples + 1) * (i + 0) + j + 0);	surface[6 * (samples * i + j) + 1] = 5 * ((samples + 1) * (i + 0) + j + 1);	surface[6 * (samples * i + j) + 2] = 5 * ((samples + 1) * (i + 1) + j + 1);
			surface[6 * (samples * i + j) + 3] = 5 * ((samples + 1) * (i + 1) + j + 1);	surface[6 * (samples * i + j) + 4] = 5 * ((samples + 1) * (i + 1) + j + 0);	surface[6 * (samples * i + j) + 5] = 5 * ((samples + 1) * (i + 0) + j + 0);

		}
	}

	for (int i = 0; i < samples + 1; i++) {
		for (int j = 0; j < samples; j++) {
			surface[6 * samples * samples + 12 * (samples * i + j) + 0] = 5 * ((samples + 1) * i + j + 0) + 1;		surface[6 * samples * samples + 12 * (samples * i + j) + 1] = 5 * ((samples + 1) * i + j + 0) + 2;		surface[6 * samples * samples + 12 * (samples * i + j) + 2] = 5 * ((samples + 1) * i + j + 1) + 2;
			surface[6 * samples * samples + 12 * (samples * i + j) + 3] = 5 * ((samples + 1) * i + j + 1) + 2;		surface[6 * samples * samples + 12 * (samples * i + j) + 4] = 5 * ((samples + 1) * i + j + 1) + 1;		surface[6 * samples * samples + 12 * (samples * i + j) + 5] = 5 * ((samples + 1) * i + j + 0) + 1;
			surface[6 * samples * samples + 12 * (samples * i + j) + 6] = 5 * ((samples + 1) * (j + 0) + i) + 3;	surface[6 * samples * samples + 12 * (samples * i + j) + 7] = 5 * ((samples + 1) * (j + 0) + i) + 4;	surface[6 * samples * samples + 12 * (samples * i + j) + 8] = 5 * ((samples + 1) * (j + 1) + i) + 4;
			surface[6 * samples * samples + 12 * (samples * i + j) + 9] = 5 * ((samples + 1) * (j + 1) + i) + 4;	surface[6 * samples * samples + 12 * (samples * i + j) + 10] = 5 * ((samples + 1) * (j + 1) + i) + 3;	surface[6 * samples * samples + 12 * (samples * i + j) + 11] = 5 * ((samples + 1) * (j + 0) + i + 0) + 3;
		}
	}

	for (int i = 0; i < samples; i++) {
		for (int j = 0; j < samples; j++) {
			surface[6 * samples * (3 * samples + 2) + 6 * (samples * i + j) + 0] = 5 * ((samples + 1) * (i + 0) + j + 0) + 1;	surface[6 * samples * (3 * samples + 2) + 6 * (samples * i + j) + 1] = 5 * ((samples + 1) * (i + 1) + j + 1) + 4;	surface[6 * samples * (3 * samples + 2) + 6 * (samples * i + j) + 2] = 5 * ((samples + 1) * (i + 1) + j + 1) + 2;
			surface[6 * samples * (3 * samples + 2) + 6 * (samples * i + j) + 3] = 5 * ((samples + 1) * (i + 1) + j + 1) + 2;	surface[6 * samples * (3 * samples + 2) + 6 * (samples * i + j) + 4] = 5 * ((samples + 1) * (i + 0) + j + 0) + 3;	surface[6 * samples * (3 * samples + 2) + 6 * (samples * i + j) + 5] = 5 * ((samples + 1) * (i + 0) + j + 0) + 1;
		}
	}
	
	glGenBuffers(1, &IBO[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(surface), surface, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO[2]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*curve1->IBOsize(), curve1->curveIndex(), GL_STATIC_DRAW);
	
	glGenBuffers(1, &IBO[3]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * curve2->IBOsize(), curve2->curveIndex(), GL_STATIC_DRAW);
}

static void CreateVertexArrays()
{	
	for (int i = 0; i < size(VAO); i++) {
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

	VectorFunction trefoil("(2+cos(3*t))*cos(2*t)", "sin(3*t)+3", "(2+cos(3*t))*sin(2*t)");

	curve1 = new Curve(&trefoil, 0, 1*pi<float>(), 64);
	curve2 = new Curve(&trefoil, 1 * pi<float>(), 2 * pi<float>(), 64, 0.1, vec4(0.0f, 1.0f, 0.0f, 1.0f));

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