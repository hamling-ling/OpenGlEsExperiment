#include <windows.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Slice.h"

static HGLRC g_hGLRC;

static TCHAR szClassName[] = TEXT("SimpleTrianle");
static TCHAR szWindowName[] = TEXT("SimpleTrianle");

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
static GLuint g_bufferObject0;
static GLuint g_bufferObject1;
static GLuint g_bufferObject2;
static GLuint g_vertexArrayObject;
static GLuint g_vertexArrayObject0;
static GLuint g_vertexArrayObject1;
static GLuint g_vertexArrayObject2;

const GLfloat colorsAndVertices[3][5] =
{
	{ 1.0f,  1.0f,  0.0f, -0.5f, -0.5f},
	{ 1.0f,  1.0f,  0.0f,  0.5f, -0.5f},
	{ 1.0f,  1.0f,  0.0f,  0.0f,  0.5f},
};

GLfloat colorsAndVerticesSliced0[3][5] = {0.0};


GLfloat colorsAndVerticesSliced1[3][5] = {0.0};
GLfloat colorsAndVerticesSliced2[3][5] = {0.0};

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

static void foo(int &i) { i = 3;};

static void OnCreate(HWND hWnd)
{
	HDC hDC;
	int nPfdID;
	BOOL bResult;
	HGLRC hGLRC;

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
	LoadShaderSource(g_vertexShader, "Simple.vert");
	glCompileShader(g_vertexShader);
	DisplayCompileError(g_vertexShader, hWnd);

	g_shaderProgram = glCreateProgram();
	glAttachShader(g_shaderProgram, g_vertexShader);

	glDeleteShader(g_vertexShader);
	glLinkProgram(g_shaderProgram);
	DisplayLinkError(g_shaderProgram, hWnd);

	GLint colorLocation = glGetAttribLocation(g_shaderProgram, "Color");
	GLint vertexLocation = glGetAttribLocation(g_shaderProgram, "Vertex");

	glGenBuffers(1, &g_bufferObject0);
	glBindBuffer(GL_ARRAY_BUFFER, g_bufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof (colorsAndVertices), colorsAndVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &g_vertexArrayObject);
	glBindVertexArray(g_vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, g_bufferObject);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), 0);
	glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), (GLvoid *)(3 * sizeof (GLfloat)));
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(vertexLocation);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDisableVertexAttribArray(glGetAttribLocation(g_shaderProgram, "Color"));
	glDisableVertexAttribArray(glGetAttribLocation(g_shaderProgram, "Vertex"));

	glEnable(GL_CULL_FACE);

	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

	wglMakeCurrent(NULL, NULL);

	ReleaseDC(hWnd, hDC);

	//---------------
	// slice
	CVector3f a(colorsAndVertices[0][3],colorsAndVertices[0][4],0.0);
	CVector3f b(colorsAndVertices[1][3],colorsAndVertices[1][4],0.0);
	CVector3f c(colorsAndVertices[2][3],colorsAndVertices[2][4],0.0);
	CTriangle3f tri0(a,b,c);

	CVector3f s0(-0.1f, 1.0f, 0.0f);
	CVector3f s1(0.1f, -1.0f, 0.0f);
	CTriangle3f sliced[3];
	if(SliceTriangle(tri0, s0, s1, sliced)) {
		colorsAndVerticesSliced0[0][3] = sliced[0][CTriangle3f::A][CVector3f::X];
		colorsAndVerticesSliced0[0][4] = sliced[0][CTriangle3f::A][CVector3f::Y];
		colorsAndVerticesSliced0[1][3] = sliced[0][CTriangle3f::B][CVector3f::X];
		colorsAndVerticesSliced0[1][4] = sliced[0][CTriangle3f::B][CVector3f::Y];
		colorsAndVerticesSliced0[2][3] = sliced[0][CTriangle3f::C][CVector3f::X];
		colorsAndVerticesSliced0[2][4] = sliced[0][CTriangle3f::C][CVector3f::Y];
		for(int i = 0; i < 3; i++)
		{
			colorsAndVerticesSliced0[i][0] = 1.0;
			colorsAndVerticesSliced0[i][1] = 1.0;
			colorsAndVerticesSliced0[i][2] = 0.0;
		}

		colorsAndVerticesSliced1[0][3] = sliced[1][CTriangle3f::A][CVector3f::X];
		colorsAndVerticesSliced1[0][4] = sliced[1][CTriangle3f::A][CVector3f::Y];
		colorsAndVerticesSliced1[1][3] = sliced[1][CTriangle3f::B][CVector3f::X];
		colorsAndVerticesSliced1[1][4] = sliced[1][CTriangle3f::B][CVector3f::Y];
		colorsAndVerticesSliced1[2][3] = sliced[1][CTriangle3f::C][CVector3f::X];
		colorsAndVerticesSliced1[2][4] = sliced[1][CTriangle3f::C][CVector3f::Y];
		for(int i = 0; i < 3; i++)
		{
			colorsAndVerticesSliced1[i][0] = 0.0;
			colorsAndVerticesSliced1[i][1] = 1.0;
			colorsAndVerticesSliced1[i][2] = 1.0;
		}

		colorsAndVerticesSliced2[0][3] = sliced[2][CTriangle3f::A][CVector3f::X];
		colorsAndVerticesSliced2[0][4] = sliced[2][CTriangle3f::A][CVector3f::Y];
		colorsAndVerticesSliced2[1][3] = sliced[2][CTriangle3f::B][CVector3f::X];
		colorsAndVerticesSliced2[1][4] = sliced[2][CTriangle3f::B][CVector3f::Y];
		colorsAndVerticesSliced2[2][3] = sliced[2][CTriangle3f::C][CVector3f::X];
		colorsAndVerticesSliced2[2][4] = sliced[2][CTriangle3f::C][CVector3f::Y];
		for(int i = 0; i < 3; i++)
		{
			colorsAndVerticesSliced2[i][0] = 1.0;
			colorsAndVerticesSliced2[i][1] = 0.0;
			colorsAndVerticesSliced2[i][2] = 1.0;
		}
	}

	//-- crate another object
	hDC = GetDC(hWnd);
	wglMakeCurrent(hDC, g_hGLRC);

	colorLocation = glGetAttribLocation(g_shaderProgram, "Color");
	vertexLocation = glGetAttribLocation(g_shaderProgram, "Vertex");

	glGenBuffers(1, &g_bufferObject0);
	glBindBuffer(GL_ARRAY_BUFFER, g_bufferObject0);
	glBufferData(GL_ARRAY_BUFFER, sizeof (colorsAndVerticesSliced0), colorsAndVerticesSliced0, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &g_vertexArrayObject0);
	glBindVertexArray(g_vertexArrayObject0);
	glBindBuffer(GL_ARRAY_BUFFER, g_bufferObject0);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), 0);
	glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), (GLvoid *)(3 * sizeof (GLfloat)));
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(vertexLocation);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenBuffers(1, &g_bufferObject1);
	glBindBuffer(GL_ARRAY_BUFFER, g_bufferObject1);
	glBufferData(GL_ARRAY_BUFFER, sizeof (colorsAndVerticesSliced1), colorsAndVerticesSliced1, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &g_vertexArrayObject1);
	glBindVertexArray(g_vertexArrayObject1);
	glBindBuffer(GL_ARRAY_BUFFER, g_bufferObject1);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), 0);
	glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), (GLvoid *)(3 * sizeof (GLfloat)));
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(vertexLocation);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenBuffers(1, &g_bufferObject2);
	glBindBuffer(GL_ARRAY_BUFFER, g_bufferObject2);
	glBufferData(GL_ARRAY_BUFFER, sizeof (colorsAndVerticesSliced2), colorsAndVerticesSliced2, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &g_vertexArrayObject2);
	glBindVertexArray(g_vertexArrayObject2);
	glBindBuffer(GL_ARRAY_BUFFER, g_bufferObject2);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), 0);
	glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), (GLvoid *)(3 * sizeof (GLfloat)));
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(vertexLocation);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDisableVertexAttribArray(glGetAttribLocation(g_shaderProgram, "Color"));
	glDisableVertexAttribArray(glGetAttribLocation(g_shaderProgram, "Vertex"));

	wglMakeCurrent(NULL, NULL);

	ReleaseDC(hWnd, hDC);


}


static void OnSize(HWND hWnd, int nWidth, int nHeight)
{
	HDC hDC;

	hDC = GetDC(hWnd);

	wglMakeCurrent(hDC, g_hGLRC);

	glViewport(0, 0, nWidth, nHeight);

	wglMakeCurrent(NULL, NULL);

	ReleaseDC(hWnd, hDC);

	InvalidateRect(hWnd, NULL, NULL);
}

static void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hDC;

	hDC = BeginPaint(hWnd, &ps);

	wglMakeCurrent(hDC, g_hGLRC);

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(g_shaderProgram);

	//glBindVertexArray(g_vertexArrayObject);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glBindVertexArray(0);

	glBindVertexArray(g_vertexArrayObject0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	glBindVertexArray(g_vertexArrayObject1);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	glBindVertexArray(g_vertexArrayObject2);
	glDrawArrays(GL_TRIANGLES, 0, 3);
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
	glDeleteVertexArrays(1, &g_vertexArrayObject0);
	glDeleteVertexArrays(1, &g_vertexArrayObject1);
	glDeleteVertexArrays(1, &g_vertexArrayObject2);
	glDeleteBuffers(1, &g_bufferObject);
	glDeleteBuffers(1, &g_bufferObject0);
	glDeleteBuffers(1, &g_bufferObject1);
	glDeleteBuffers(1, &g_bufferObject2);
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

	errno_t err = fopen_s(&fp, fileName, "rb");
	if(err != 0)
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
