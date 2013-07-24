#include <windows.h>
#include <stdio.h>
#include <vector>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Chop.h"
#include "SimpleObject.h"

using namespace std;

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

const GLfloat normalsAndVertices[3][6] =
{
	{ 0.0f,  0.0f,  1.0f, -0.5f, -0.5f, 0.0f},
	{ 0.0f,  0.0f,  1.0f,  0.5f, -0.5f, 0.0f},
	{ 0.0f,  0.0f,  1.0f,  0.0f,  0.5f, 0.0f},
};

SimpleObject* pOrigObj;
vector<SimpleObject*> objects;

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

	GLint normalLocation = glGetAttribLocation(g_shaderProgram, "Normal");
	GLint vertexLocation = glGetAttribLocation(g_shaderProgram, "Vertex");

	pOrigObj = new SimpleObject();
	pOrigObj->BindBuffer(normalLocation, vertexLocation, &(normalsAndVertices[0][0]), 3);

	glDisableVertexAttribArray(glGetAttribLocation(g_shaderProgram, "Normal"));
	glDisableVertexAttribArray(glGetAttribLocation(g_shaderProgram, "Vertex"));

	glEnable(GL_CULL_FACE);

	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

	wglMakeCurrent(NULL, NULL);

	ReleaseDC(hWnd, hDC);

	//---------------
	// slice
	CVertex a(normalsAndVertices[0]);
	CVertex b(normalsAndVertices[1]);
	CVertex c(normalsAndVertices[2]);
	CTriangle3v tri(a,b,c);

	CVector3f p(-0.1f, 1.0f, 0.0f);
	CVector3f n(1.0f, 0.1f, 0.0f);
	CPlane plane(n, p);

	SliceResult3v sliceResult;

	GLfloat bufN[64][6] = {0.0f};
	GLfloat bufA[64][6] = {0.0f};

	if(ChopTriangle3v(tri, plane, sliceResult)) {

		for(int i = 0; i < sliceResult.NormalSideCount; i++) {
			sliceResult.NormalSides[i][CTriangle3v::A].GetValue(&(bufN[3*i+0][0]));
			sliceResult.NormalSides[i][CTriangle3v::B].GetValue(&(bufN[3*i+1][0]));
			sliceResult.NormalSides[i][CTriangle3v::C].GetValue(&(bufN[3*i+2][0]));
		}

		for(int i = 0; i < sliceResult.AntinormalSideCount; i++) {
			sliceResult.AntinormalSides[i][CTriangle3v::A].GetValue(&(bufA[3*i+0][0]));
			sliceResult.AntinormalSides[i][CTriangle3v::B].GetValue(&(bufA[3*i+1][0]));
			sliceResult.AntinormalSides[i][CTriangle3v::C].GetValue(&(bufA[3*i+2][0]));
		}
	}

	//-- crate another object
	hDC = GetDC(hWnd);
	wglMakeCurrent(hDC, g_hGLRC);

	normalLocation = glGetAttribLocation(g_shaderProgram, "Normal");
	vertexLocation = glGetAttribLocation(g_shaderProgram, "Vertex");

	if(sliceResult.NormalSideCount > 0) {
		SimpleObject *pObj = new SimpleObject();
		pObj->BindBuffer(normalLocation, vertexLocation, &(bufN[0][0]), sliceResult.NormalSideCount * 3);
		objects.push_back(pObj);
	}

	if(sliceResult.AntinormalSideCount > 0) {
		SimpleObject *pObj = new SimpleObject();
		pObj->BindBuffer(normalLocation, vertexLocation, &(bufA[0][0]), sliceResult.AntinormalSideCount * 3);
		objects.push_back(pObj);
	}

	glDisableVertexAttribArray(glGetAttribLocation(g_shaderProgram, "Normal"));
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

	//glBindVertexArray(pOrigObj->GetVertexArrayObject());
	//glDrawArrays(GL_TRIANGLES, 0, pOrigObj->GetVertexArrayLen());
	//glBindVertexArray(0);

	GLfloat color[3] = {1.0f, 0.0f, 0.0f};
	vector<SimpleObject*>::iterator it = objects.begin();
	while(it != objects.end()) {

		int index = it - objects.begin();
		color[0] = (float)((index) % 2);
		color[1] = (float)((index+1)%2);
		color[2] = (float)((index+2)%2);

		glUniform3fv(glGetUniformLocation(g_shaderProgram, "Color"), 1, color);

		glBindVertexArray((*it)->GetVertexArrayObject());
		glDrawArrays(GL_TRIANGLES, 0, (*it)->GetVertexArrayLen());
		glBindVertexArray(0);
		it++;
	}

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
	delete pOrigObj;;

	vector<SimpleObject*>::iterator it = objects.begin();
	while(it != objects.end()) {
		SimpleObject *p = (*it);
		objects.erase(it);
		delete p;
		it = objects.begin();
	}

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
