#include <windows.h>
#include <stdio.h>
#include <math.h>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Vector3f.h"
#include "Matrix4x4f.h"


static HGLRC g_hGLRC;

static TCHAR szClassName[] = TEXT("DirectionalLightCube");
static TCHAR szWindowName[] = TEXT("DirectionalLightCube");

LRESULT CALLBACK WindowProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam);

static void OnCreate(HWND hWnd);
static void OnSize(HWND hWnd, int nWidth, int nHeight);
static void OnPaint(HWND hWnd);
static void OnDestroy(HWND hWnd);

static GLuint g_vertexShader;
static GLuint g_shaderProgram;

static void LoadShaderSource(GLuint shader, const char* fileName);
static void DisplayCompileError(GLuint shader, HWND hWnd);
static void DisplayLinkError(GLuint program, HWND hWnd);

static GLuint g_bufferObject;
static GLuint g_vertexArrayObject;


int WINAPI WinMain(HINSTANCE hCurrInstance, HINSTANCE hPrevInstance, LPSTR szArgs, int nWinMode)
{
	HWND hWnd;
	MSG msg;
	WNDCLASSEX wc;
	int nResult;

	wc.cbSize = sizeof (WNDCLASSEX);
	wc.hInstance = hCurrInstance;
	wc.lpszClassName = szClassName;
	wc.lpfnWndProc = WindowProc;
	wc.style = 0;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	if (RegisterClassEx(&wc) == 0) {
		return 0;
	}

	hWnd = CreateWindow(szClassName,
			szWindowName,
			WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL,
			NULL,
			hCurrInstance,
			NULL);

	if (hWnd == NULL) {
		return 0;
	}

	ShowWindow(hWnd, nWinMode);
	UpdateWindow(hWnd);

	while (TRUE) {
		nResult = GetMessage(&msg, NULL, 0, 0);
		if (nResult == -1 || nResult == 0) {
			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	switch (nMessage) {
		case WM_CREATE:
			OnCreate(hWnd);
			break;
		case WM_SIZE:
			OnSize(hWnd, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_PAINT:
			OnPaint(hWnd);
			break;
		case WM_DESTROY:
			OnDestroy(hWnd);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, nMessage, wParam, lParam);
	}

	return 0;
}


static void OnCreate(HWND hWnd)
{
	HDC hDC;
	int nPfdID;
	BOOL bResult;
	HGLRC hGLRC;

	const GLfloat normalsAndVertices[36][6] =
	{
		/* ‘O */
		{ 0.0f,  0.0f,  1.0f, -0.5f, -0.5f,  0.5f},
		{ 0.0f,  0.0f,  1.0f,  0.5f, -0.5f,  0.5f},
		{ 0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f},

		{ 0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f},
		{ 0.0f,  0.0f,  1.0f, -0.5f,  0.5f,  0.5f},
		{ 0.0f,  0.0f,  1.0f, -0.5f, -0.5f,  0.5f},

		/* Œã */
		{ 0.0f,  0.0f, -1.0f,  0.5f, -0.5f, -0.5f},
		{ 0.0f,  0.0f, -1.0f, -0.5f, -0.5f, -0.5f},
		{ 0.0f,  0.0f, -1.0f, -0.5f,  0.5f, -0.5f},

		{ 0.0f,  0.0f, -1.0f, -0.5f,  0.5f, -0.5f},
		{ 0.0f,  0.0f, -1.0f,  0.5f,  0.5f, -0.5f},
		{ 0.0f,  0.0f, -1.0f,  0.5f, -0.5f, -0.5f},

		/* ‰E */
		{ 1.0f,  0.0f,  0.0f,  0.5f, -0.5f,  0.5f},
		{ 1.0f,  0.0f,  0.0f,  0.5f, -0.5f, -0.5f},
		{ 1.0f,  0.0f,  0.0f,  0.5f,  0.5f, -0.5f},

		{ 1.0f,  0.0f,  0.0f,  0.5f,  0.5f, -0.5f},
		{ 1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f},
		{ 1.0f,  0.0f,  0.0f,  0.5f, -0.5f,  0.5f},

		/* ¶ */
		{-1.0f,  0.0f,  0.0f, -0.5f, -0.5f, -0.5f},
		{-1.0f,  0.0f,  0.0f, -0.5f, -0.5f,  0.5f},
		{-1.0f,  0.0f,  0.0f, -0.5f,  0.5f,  0.5f},

		{-1.0f,  0.0f,  0.0f, -0.5f,  0.5f,  0.5f},
		{-1.0f,  0.0f,  0.0f, -0.5f,  0.5f, -0.5f},
		{-1.0f,  0.0f,  0.0f, -0.5f, -0.5f, -0.5f},

		/* ã */
		{ 0.0f,  1.0f,  0.0f, -0.5f,  0.5f,  0.5f},
		{ 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f},
		{ 0.0f,  1.0f,  0.0f,  0.5f,  0.5f, -0.5f},

		{ 0.0f,  1.0f,  0.0f,  0.5f,  0.5f, -0.5f},
		{ 0.0f,  1.0f,  0.0f, -0.5f,  0.5f, -0.5f},
		{ 0.0f,  1.0f,  0.0f, -0.5f,  0.5f,  0.5f},

		/* ‰º */
		{ 0.0f, -1.0f,  0.0f,  0.5f, -0.5f,  0.5f},
		{ 0.0f, -1.0f,  0.0f, -0.5f, -0.5f,  0.5f},
		{ 0.0f, -1.0f,  0.0f, -0.5f, -0.5f, -0.5f},

		{ 0.0f, -1.0f,  0.0f, -0.5f, -0.5f, -0.5f},
		{ 0.0f, -1.0f,  0.0f,  0.5f, -0.5f, -0.5f},
		{ 0.0f, -1.0f,  0.0f,  0.5f, -0.5f,  0.5f}
	};

	const PIXELFORMATDESCRIPTOR pfd = {
		sizeof (PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0,
		0, 0, 0,
		0, 0,
		0, 0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0,
		0,
		0
	};

	const int attributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3, 
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	hDC = GetDC(hWnd);

	nPfdID = ChoosePixelFormat(hDC, &pfd);	
	if (nPfdID == 0) {
		return;
	}

	bResult = SetPixelFormat(hDC, nPfdID, &pfd);
	if (bResult == FALSE) {
		return;
	}

	g_hGLRC = wglCreateContext(hDC);
	if (g_hGLRC == NULL) {
		return;
	}

	wglMakeCurrent(hDC, g_hGLRC);

	glewInit();

	hGLRC = wglCreateContextAttribsARB(hDC, 0, attributes);

	wglMakeCurrent(NULL, NULL);

	wglDeleteContext(g_hGLRC);

	g_hGLRC = hGLRC;

	wglMakeCurrent(hDC, g_hGLRC);

	g_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	LoadShaderSource(g_vertexShader, "Lighting.vert");
	glCompileShader(g_vertexShader);
   	DisplayCompileError(g_vertexShader, hWnd);

	g_shaderProgram = glCreateProgram();
	glAttachShader(g_shaderProgram, g_vertexShader);

	glDeleteShader(g_vertexShader);
	glLinkProgram(g_shaderProgram);
   	DisplayLinkError(g_shaderProgram, hWnd);

	glGenBuffers(1, &g_bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, g_bufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof (normalsAndVertices), normalsAndVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &g_vertexArrayObject);
	glBindVertexArray(g_vertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, g_bufferObject);

	GLint normalLocation = glGetAttribLocation(g_shaderProgram, "Normal");
	GLint vertexLocation = glGetAttribLocation(g_shaderProgram, "Vertex");
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof (GLfloat), 0);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof (GLfloat), (GLvoid *)(3 * sizeof (GLfloat)));
	glEnableVertexAttribArray(normalLocation);
	glEnableVertexAttribArray(vertexLocation);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

	wglMakeCurrent(NULL, NULL);

	ReleaseDC(hWnd, hDC);
}

static void OnSize(HWND hWnd, int nWidth, int nHeight)
{
	HDC hDC;

	hDC = GetDC(hWnd);

	wglMakeCurrent(hDC, g_hGLRC);

	glViewport(0, 0, nWidth, nHeight);

	CMatrix4x4f perspective;;

	float l, r, b, t, n = 0.1f, f = 10.0f;
	t = n * tanf(PI * (45.0f / 2.0f) / 180.0f);
	b = -t;
	r = t * (float)nWidth / (float)nHeight;
	l = -r;
	perspective.MakePerspective(l, r, b, t, n, f);

	CMatrix4x4f rotationX;
	CMatrix4x4f rotationY;
	CMatrix4x4f translation;
	CMatrix4x4f lookAt;

	float distance = sqrtf(3.0f * 2.5f * 2.5f);
	float angle = atan2f(1.0f, sqrtf(2.0f)) * 180.0f / PI;

	translation.MakeTranslation(CVector3f(0.0f, 0.0f, -distance));
	rotationX.MakeRotation(CVector3f(1.0f, 0.0f, 0.0f), angle);
	rotationY.MakeRotation(CVector3f(0.0f, 1.0f, 0.0f), -45.0f);
	lookAt = translation * rotationX * rotationY;
	float modelViewMatrix[16];
	float viewMatrix[16];
	lookAt.GetGLMat(modelViewMatrix);
	lookAt.GetGLMat(viewMatrix);

	CMatrix4x4f lookAtPerspective;
	lookAtPerspective = perspective * lookAt;
	float modelViewProjectionMatrix[16];
	lookAtPerspective.GetGLMat(modelViewProjectionMatrix);

	glUseProgram(g_shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(g_shaderProgram, "modelViewProjectionMatrix"), 1, GL_FALSE, modelViewProjectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(g_shaderProgram, "modelViewMatrix"), 1, GL_FALSE, modelViewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(g_shaderProgram, "viewMatrix"), 1, GL_FALSE, viewMatrix);
	glUseProgram(0);

	wglMakeCurrent(NULL, NULL);

	ReleaseDC(hWnd, hDC);

	InvalidateRect(hWnd, NULL, NULL);
}

static void OnPaint(HWND hWnd)
{
	const GLfloat lightPosition[4] = {3.0f, 4.0f, 0.0f, 0.0f};
	const GLfloat lightDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	const GLfloat lightAmbient[4] = {0.25f, 0.25f, 0.25f, 1.0f};
	const GLfloat lightSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};

	const GLfloat cubeDiffuse[4] = {0.75f, 0.0f, 1.0f, 1.0f};
	const GLfloat cubeAmbient[4] = {0.3f, 0.25f, 0.4f, 1.0f};
	const GLfloat cubeSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	const GLfloat cubeShininess[1] = {32.0f};

	PAINTSTRUCT ps;
	HDC hDC;

	hDC = BeginPaint(hWnd, &ps);

	wglMakeCurrent(hDC, g_hGLRC);

	glUseProgram(g_shaderProgram);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "lightPosition"), 1, lightPosition);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "Ld"), 1, lightDiffuse);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "La"), 1, lightAmbient);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "Ls"), 1, lightSpecular);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "Kd"), 1, cubeDiffuse);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "Ka"), 1, cubeAmbient);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "Ks"), 1, cubeSpecular);
	glUniform1fv(glGetUniformLocation(g_shaderProgram, "shininess"), 1, cubeShininess);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(g_vertexArrayObject);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);

	glFlush();

	glUseProgram(0);

	wglMakeCurrent(NULL, NULL);

	EndPaint(hWnd, &ps);
}

static void OnDestroy(HWND hWnd)
{
	HDC hDC;

	hDC = GetDC(hWnd);

	wglMakeCurrent(hDC, g_hGLRC);
	glDeleteVertexArrays(1, &g_vertexArrayObject);
	glDeleteBuffers(1, &g_bufferObject);
	glDeleteProgram(g_shaderProgram);
	wglMakeCurrent(NULL, NULL);

	if (g_hGLRC != NULL) {
		wglDeleteContext(g_hGLRC);
	}

	ReleaseDC(hWnd, hDC);
}


static void LoadShaderSource(GLuint shader, const char* fileName)
{
	FILE* fp = NULL;
	int size;
	char* buf;

	if(fopen_s(&fp,fileName, "rb") != 0)
		return;

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);

	buf = (char *)malloc(size);
	fseek(fp, 0, SEEK_SET);

	fread(buf, 1, size, fp);

	glShaderSource(shader, 1, (const GLchar **)&buf, &size);

	free(buf);
	fclose(fp);
}

static void DisplayCompileError(GLuint shader, HWND hWnd)
{
	GLint compiled;
	int size;
	int len;
	char* buf;

   	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
		if (size > 0) {
			buf = (char *)malloc(size);
			glGetShaderInfoLog(shader, size, &len, buf);
			MessageBoxA(hWnd, buf, "Compile error !", MB_OK);
			free(buf);
		}
	}
}

static void DisplayLinkError(GLuint program, HWND hWnd)
{
	GLint linked;
	int size;
	int len;
	char* buf;

	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (linked == GL_FALSE) {
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &size);
		if (size > 0) {
			buf = (char *)malloc(size);
			glGetProgramInfoLog(program, size, &len, buf);
			MessageBoxA(hWnd, buf, "Link error !", MB_OK);
			free(buf);
		}
	}
}
