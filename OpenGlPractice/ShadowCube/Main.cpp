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

static TCHAR szClassName[] = TEXT("ShadowCube");
static TCHAR szWindowName[] = TEXT("ShadowCube");

LRESULT CALLBACK WindowProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam);

static void OnCreate(HWND hWnd);
static void OnSize(HWND hWnd, int nWidth, int nHeight);
static void OnPaint(HWND hWnd);
static void OnDestroy(HWND hWnd);

static void Draw(bool shadow);

static GLuint g_depthVertexShader;
static GLuint g_depthShaderProgram;
static GLuint g_vertexShader;
static GLuint g_fragmentShader;
static GLuint g_shaderProgram;

static void LoadShaderSource(GLuint shader, const char* fileName);
static void DisplayCompileError(GLuint shader, HWND hWnd);
static void DisplayLinkError(GLuint program, HWND hWnd);

#define	CUBE_VERTEX_ARRAY	0
#define	FLOOR_VERTEX_ARRAY	1

#define	VERTEX_BUFFER	0
#define	NORMAL_BUFFER	1

static GLuint g_depthVertexArrayObject[2];
static GLuint g_vertexArrayObject[2];
static GLuint g_cubeBufferObject[2];
static GLuint g_floorBufferObject[2];

#define	TEXTURE_WIDTH	1024
#define	TEXTURE_HEIGHT	1024

static GLuint g_textureObject;
static GLuint g_framebufferObject;


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

	const GLfloat cubeVertices[36][3] =
	{
		/* 前 */
		{-0.5f, -0.5f,  0.5f},
		{ 0.5f, -0.5f,  0.5f},
		{ 0.5f,  0.5f,  0.5f},

		{ 0.5f,  0.5f,  0.5f},
		{-0.5f,  0.5f,  0.5f},
		{-0.5f, -0.5f,  0.5f},

		/* 後 */
		{ 0.5f, -0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f},
		{-0.5f,  0.5f, -0.5f},

		{-0.5f,  0.5f, -0.5f},
		{ 0.5f,  0.5f, -0.5f},
		{ 0.5f, -0.5f, -0.5f},

		/* 右 */
		{ 0.5f, -0.5f,  0.5f},
		{ 0.5f, -0.5f, -0.5f},
		{ 0.5f,  0.5f, -0.5f},

		{ 0.5f,  0.5f, -0.5f},
		{ 0.5f,  0.5f,  0.5f},
		{ 0.5f, -0.5f,  0.5f},

		/* 左 */
		{-0.5f, -0.5f, -0.5f},
		{-0.5f, -0.5f,  0.5f},
		{-0.5f,  0.5f,  0.5f},

		{-0.5f,  0.5f,  0.5f},
		{-0.5f,  0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f},

		/* 上 */
		{-0.5f,  0.5f,  0.5f},
		{ 0.5f,  0.5f,  0.5f},
		{ 0.5f,  0.5f, -0.5f},

		{ 0.5f,  0.5f, -0.5f},
		{-0.5f,  0.5f, -0.5f},
		{-0.5f,  0.5f,  0.5f},

		/* 下 */
		{ 0.5f, -0.5f,  0.5f},
		{-0.5f, -0.5f,  0.5f},
		{-0.5f, -0.5f, -0.5f},

		{-0.5f, -0.5f, -0.5f},
		{ 0.5f, -0.5f, -0.5f},
		{ 0.5f, -0.5f,  0.5f}
	};

	const GLfloat cubeNormals[36][3] =
	{
		/* 前 */
		{ 0.0f,  0.0f,  1.0f},
		{ 0.0f,  0.0f,  1.0f},
		{ 0.0f,  0.0f,  1.0f},

		{ 0.0f,  0.0f,  1.0f},
		{ 0.0f,  0.0f,  1.0f},
		{ 0.0f,  0.0f,  1.0f},

		/* 後 */
		{ 0.0f,  0.0f, -1.0f},
		{ 0.0f,  0.0f, -1.0f},
		{ 0.0f,  0.0f, -1.0f},

		{ 0.0f,  0.0f, -1.0f},
		{ 0.0f,  0.0f, -1.0f},
		{ 0.0f,  0.0f, -1.0f},

		/* 右 */
		{ 1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},

		{ 1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},

		/* 左 */
		{-1.0f,  0.0f,  0.0f},
		{-1.0f,  0.0f,  0.0f},
		{-1.0f,  0.0f,  0.0f},

		{-1.0f,  0.0f,  0.0f},
		{-1.0f,  0.0f,  0.0f},
		{-1.0f,  0.0f,  0.0f},

		/* 上 */
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},

		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},

		/* 下 */
		{ 0.0f, -1.0f,  0.0f},
		{ 0.0f, -1.0f,  0.0f},
		{ 0.0f, -1.0f,  0.0f},

		{ 0.0f, -1.0f,  0.0f},
		{ 0.0f, -1.0f,  0.0f},
		{ 0.0f, -1.0f,  0.0f}
	};

	const GLfloat floorVertices[6][3] =
	{
		{-5.0f, -0.5f,  5.0f},
		{ 5.0f, -0.5f,  5.0f},
		{ 5.0f, -0.5f, -5.0f},

		{ 5.0f, -0.5f, -5.0f},
		{-5.0f, -0.5f, -5.0f},
		{-5.0f, -0.5f,  5.0f}
	};

	const GLfloat floorNormals[6][3] =
	{
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},

		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f}
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
		WGL_CONTEXT_MINOR_VERSION_ARB, 0, 
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	const GLenum drawBuffer[1] = {GL_NONE};

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

	g_depthVertexShader = glCreateShader(GL_VERTEX_SHADER);
	LoadShaderSource(g_depthVertexShader, "Simple.vert");
	glCompileShader(g_depthVertexShader);
   	DisplayCompileError(g_depthVertexShader, hWnd);

	g_depthShaderProgram = glCreateProgram();
	glAttachShader(g_depthShaderProgram, g_depthVertexShader);

	glDeleteShader(g_depthVertexShader);
	glLinkProgram(g_depthShaderProgram);
   	DisplayLinkError(g_depthShaderProgram, hWnd);

	g_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	LoadShaderSource(g_vertexShader, "Shadow.vert");
	glCompileShader(g_vertexShader);
   	DisplayCompileError(g_vertexShader, hWnd);

	g_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	LoadShaderSource(g_fragmentShader, "Shadow.frag");
	glCompileShader(g_fragmentShader);
   	DisplayCompileError(g_fragmentShader, hWnd);

	g_shaderProgram = glCreateProgram();
	glAttachShader(g_shaderProgram, g_vertexShader);
	glAttachShader(g_shaderProgram, g_fragmentShader);

	glDeleteShader(g_vertexShader);
	glDeleteShader(g_fragmentShader);
	glLinkProgram(g_shaderProgram);
   	DisplayLinkError(g_shaderProgram, hWnd);

	glGenBuffers(2, g_cubeBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, g_cubeBufferObject[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof (cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, g_cubeBufferObject[NORMAL_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof (cubeNormals), cubeNormals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(2, g_floorBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, g_floorBufferObject[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof (floorVertices), floorVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, g_floorBufferObject[NORMAL_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof (floorNormals), floorNormals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLint normalLocation = glGetAttribLocation(g_shaderProgram, "Normal");
	GLint vertexLocation = glGetAttribLocation(g_shaderProgram, "Vertex");

	glGenVertexArrays(2, g_vertexArrayObject);

	glBindVertexArray(g_vertexArrayObject[CUBE_VERTEX_ARRAY]);

	glBindBuffer(GL_ARRAY_BUFFER, g_cubeBufferObject[VERTEX_BUFFER]);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), 0);
	glEnableVertexAttribArray(vertexLocation);

	glBindBuffer(GL_ARRAY_BUFFER, g_cubeBufferObject[NORMAL_BUFFER]);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), 0);
	glEnableVertexAttribArray(normalLocation);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindVertexArray(g_vertexArrayObject[FLOOR_VERTEX_ARRAY]);

	glBindBuffer(GL_ARRAY_BUFFER, g_floorBufferObject[VERTEX_BUFFER]);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), 0);
	glEnableVertexAttribArray(vertexLocation);

	glBindBuffer(GL_ARRAY_BUFFER, g_floorBufferObject[NORMAL_BUFFER]);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), 0);
	glEnableVertexAttribArray(normalLocation);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	vertexLocation = glGetAttribLocation(g_depthShaderProgram, "Vertex");

	glGenVertexArrays(2, g_depthVertexArrayObject);

	glBindVertexArray(g_depthVertexArrayObject[CUBE_VERTEX_ARRAY]);

	glBindBuffer(GL_ARRAY_BUFFER, g_cubeBufferObject[VERTEX_BUFFER]);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), 0);
	glEnableVertexAttribArray(vertexLocation);

	glBindBuffer(GL_ARRAY_BUFFER, g_cubeBufferObject[NORMAL_BUFFER]);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindVertexArray(g_depthVertexArrayObject[FLOOR_VERTEX_ARRAY]);

	glBindBuffer(GL_ARRAY_BUFFER, g_floorBufferObject[VERTEX_BUFFER]);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), 0);
	glEnableVertexAttribArray(vertexLocation);

	glBindBuffer(GL_ARRAY_BUFFER, g_floorBufferObject[NORMAL_BUFFER]);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	const float borderColor[4]  = {1.0f, 1.0f, 1.0f, 1.0f};

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &g_textureObject);
	glBindTexture(GL_TEXTURE_2D, g_textureObject);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

	glGenFramebuffers(1, &g_framebufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, g_framebufferObject);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, g_textureObject, 0);
	glDrawBuffers(1, drawBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(g_shaderProgram);
	glUniform1i(glGetUniformLocation(g_shaderProgram, "shadowTexture"), 0);
	glUseProgram(0);

	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	wglMakeCurrent(NULL, NULL);

	ReleaseDC(hWnd, hDC);
}

static void OnSize(HWND hWnd, int nWidth, int nHeight)
{
	InvalidateRect(hWnd, NULL, NULL);
}

static void OnPaint(HWND hWnd)
{
	const GLfloat lightPosition[4] = {3.0f, 4.0f, 0.0f, 1.0f};
	const GLfloat lightDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	const GLfloat lightAmbient[4] = {0.25f, 0.25f, 0.25f, 1.0f};
	const GLfloat lightSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};

	PAINTSTRUCT ps;
	HDC hDC;
	RECT clientRect;

	GetClientRect(hWnd, &clientRect);
	int nWidth = clientRect.right - clientRect.left;
	int nHeight = clientRect.bottom - clientRect.top;;

	hDC = BeginPaint(hWnd, &ps);

	wglMakeCurrent(hDC, g_hGLRC);

	glBindFramebuffer(GL_FRAMEBUFFER, g_framebufferObject);

//	glCullFace(GL_FRONT);
	glPolygonOffset(1.0f, 1.0f);
	glEnable(GL_POLYGON_OFFSET_FILL);

	glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);

	CMatrix4x4f lightPerspective;

	float l, r, b, t, n = 0.1f, f = 100.0f;
	t = n * tanf(PI * (60.0f / 2.0f) / 180.0f);
	b = -t;
	r = t * (float)TEXTURE_WIDTH / (float)TEXTURE_HEIGHT;
	l = -r;
	lightPerspective.MakePerspective(l, r, b, t, n, f);

	CMatrix4x4f lightRotationX;
	CMatrix4x4f lightRotationY;
	CMatrix4x4f lightTranslation;
	CMatrix4x4f lightLookAt;

	float distance = sqrtf(3.0f * 3.0f + 4.0f * 4.0f);
	float angle = atan2f(4.0f, 3.0f) * 180.0f / PI;

	lightTranslation.MakeTranslation(CVector3f(0.0f, 0.0f, -distance));
	lightRotationX.MakeRotation(CVector3f(1.0f, 0.0f, 0.0f), angle);
	lightRotationY.MakeRotation(CVector3f(0.0f, 1.0f, 0.0f), -90.0f);
	lightLookAt = lightTranslation * lightRotationX * lightRotationY;

	CMatrix4x4f lightLookAtPerspective;
	lightLookAtPerspective = lightPerspective * lightLookAt;
	float lightModelViewProjectionMatrix[16];
	lightLookAtPerspective.GetGLMat(lightModelViewProjectionMatrix);

	glUseProgram(g_depthShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(g_depthShaderProgram, "modelViewProjectionMatrix"), 1, GL_FALSE, lightModelViewProjectionMatrix);

	glClear(GL_DEPTH_BUFFER_BIT);

	Draw(false);

	glUseProgram(0);

	glDisable(GL_POLYGON_OFFSET_FILL);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	CMatrix4x4f textureScaling;
	CMatrix4x4f textureTranslation;
	textureTranslation.MakeTranslation(CVector3f(0.5f, 0.5f, 0.5f));
	textureScaling.MakeScale(CVector3f(0.5f, 0.5f, 0.5f));

	CMatrix4x4f shadowLookAtPerspective;
	shadowLookAtPerspective = textureTranslation * textureScaling * lightLookAtPerspective;
	float shadowModelViewProjectionMatrix[16];
	shadowLookAtPerspective.GetGLMat(shadowModelViewProjectionMatrix);

	CMatrix4x4f perspective;;

	t = n * tanf(PI * (45.0f / 2.0f) / 180.0f);
	b = -t;
	r = t * (float)nWidth / (float)nHeight;
	l = -r;
	perspective.MakePerspective(l, r, b, t, n, f);

	CMatrix4x4f rotationX;
	CMatrix4x4f rotationY;
	CMatrix4x4f translation;
	CMatrix4x4f lookAt;

	distance = sqrtf(3.0f * 2.5f * 2.5f);
	angle = atan2f(1.0f, sqrtf(2.0f)) * 180.0f / PI;

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

	glViewport(0, 0, nWidth, nHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//	glCullFace(GL_BACK);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_textureObject);

	glUseProgram(g_shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(g_shaderProgram, "modelViewProjectionMatrix"), 1, GL_FALSE, modelViewProjectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(g_shaderProgram, "modelViewMatrix"), 1, GL_FALSE, modelViewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(g_shaderProgram, "viewMatrix"), 1, GL_FALSE, viewMatrix);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "lightPosition"), 1, lightPosition);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "Ld"), 1, lightDiffuse);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "La"), 1, lightAmbient);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "Ls"), 1, lightSpecular);
	glUniformMatrix4fv(glGetUniformLocation(g_shaderProgram, "shadowModelViewProjectionMatrix"), 1, GL_FALSE, shadowModelViewProjectionMatrix);

	Draw(true);

	glUseProgram(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glFlush();

	wglMakeCurrent(NULL, NULL);

	EndPaint(hWnd, &ps);
}

static void OnDestroy(HWND hWnd)
{
	HDC hDC;

	hDC = GetDC(hWnd);

	wglMakeCurrent(hDC, g_hGLRC);
	glDeleteFramebuffers(1, &g_framebufferObject);
	glDeleteTextures(1, &g_textureObject);
	glDeleteVertexArrays(2, g_depthVertexArrayObject);
	glDeleteVertexArrays(2, g_vertexArrayObject);
	glDeleteBuffers(2, g_cubeBufferObject);
	glDeleteBuffers(2, g_floorBufferObject);
	glDeleteProgram(g_depthShaderProgram);
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

	fp = fopen(fileName, "rb");
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

static void Draw(bool shadow)
{
	const GLfloat cubeDiffuse[4] = {0.75f, 0.0f, 1.0f, 1.0f};
	const GLfloat cubeAmbient[4] = {0.3f, 0.25f, 0.4f, 1.0f};
	const GLfloat cubeSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	const GLfloat cubeShininess[1] = {32.0f};
	const GLfloat floorDiffuse[4] = {0.75f, 0.75f, 0.75f, 1.0f};
	const GLfloat floorAmbient[4] = {0.25f, 0.25f, 0.25f, 1.0f};
	const GLfloat floorSpecular[4] = {0.0f, 0.0f, 0.0f, 1.0f};

	if (shadow) {
		glUniform4fv(glGetUniformLocation(g_shaderProgram, "Kd"), 1, cubeDiffuse);
		glUniform4fv(glGetUniformLocation(g_shaderProgram, "Ka"), 1, cubeAmbient);
		glUniform4fv(glGetUniformLocation(g_shaderProgram, "Ks"), 1, cubeSpecular);
		glUniform1fv(glGetUniformLocation(g_shaderProgram, "shininess"), 1, cubeShininess);
		glBindVertexArray(g_vertexArrayObject[CUBE_VERTEX_ARRAY]);
	}
	else {
		glBindVertexArray(g_depthVertexArrayObject[CUBE_VERTEX_ARRAY]);
	}

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);

	if (shadow) {
		glUniform4fv(glGetUniformLocation(g_shaderProgram, "Kd"), 1, floorDiffuse);
		glUniform4fv(glGetUniformLocation(g_shaderProgram, "Ka"), 1, floorAmbient);
		glUniform4fv(glGetUniformLocation(g_shaderProgram, "Ks"), 1, floorSpecular);
		glBindVertexArray(g_vertexArrayObject[FLOOR_VERTEX_ARRAY]);
	}
	else {
		glBindVertexArray(g_depthVertexArrayObject[FLOOR_VERTEX_ARRAY]);
	}

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
}

