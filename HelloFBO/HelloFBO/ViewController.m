//
//  ViewController.m
//  HelloFBO
//
//  Created by nobu on 13/08/04.
//  Copyright (c) 2013年 Nobuhiro Kuroiwa. All rights reserved.
//

#import "ViewController.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// Uniform index.
enum
{
    UNIFORM_MVP_MATRIX,
    UNIFORM_TEXTURE0,
    UNIFORM_NORMAL_MATRIX,
    UNIFORM_SCREENSIZE,
    NUM_UNIFORMS
};
GLint uniforms[NUM_UNIFORMS];
GLint uniforms2[NUM_UNIFORMS];

// Attribute index.
enum
{
    ATTRIB_VERTEX,
    ATTRIB_NORMAL,
    ATTRIB_TEXCOORD,
    NUM_ATTRIBUTES
};

const GLfloat gVertexData[][8] =
{
    {  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
    {  1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
    {  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
    
    {  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
    {  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
    {  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f}
};

const GLfloat gTriangleData[][8] =
{
	{ -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f},
	{  0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f},
	{  0.0f,  1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.5f, 1.0f},
};

@interface ViewController () {
    GLuint  _vertexArray1, _vertexArray2;
    GLuint  _vertexBuffer1, _vertexBuffer2;
    
    GLuint  _framebuffer;
    GLuint  _fbColTex;
    
    // shader programs
    GLuint _program1, _program2;
    
    //texture
    GLKTextureInfo  *_texInfo0;
    
    // rotation
    float           _rotation;
}
@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)tearDownGL;
- (void)pepareTextureWithInternalFormat:(GLint)internalFormat
                                 format:(GLenum)format
                                   type:(GLenum)type
                                  texId:(GLuint*)texId
                             attachment:(GLenum)attachment
                             scaledSize:(CGSize)viewSize;
- (void)drawPath1;
- (void)drawPath2;
- (BOOL)loadShadersWithProgram:(GLuint*)program vshFileName:(NSString*) vshFileName fshFileName:(NSString*) fshFileName;
- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type file:(NSString *)file;
- (BOOL)linkProgram:(GLuint)prog;
- (BOOL)validateProgram:(GLuint)prog;
@end

@implementation ViewController

#pragma mark - ViewController Manner

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    
    [EAGLContext setCurrentContext:self.context];
    [self setupGL];
}

- (void)dealloc
{    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

    // Dispose of any resources that can be recreated.
}

#pragma mark - OpenGL Setup and TearDown

- (void)setupGL
{
    // create a program for path1
    {
        [self loadShadersWithProgram:&_program1 vshFileName:@"Shader" fshFileName:@"Shader"];
        
        uniforms[UNIFORM_MVP_MATRIX] = glGetUniformLocation(_program1, [@"modelViewProjectionMatrix" cStringUsingEncoding:NSUTF8StringEncoding]);
        uniforms[UNIFORM_TEXTURE0] = glGetUniformLocation(_program1, [@"texture0" cStringUsingEncoding:NSUTF8StringEncoding]);
        uniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(_program1, [@"normalMatrix" cStringUsingEncoding:NSUTF8StringEncoding]);
    }
    
    // create a program for path2
    {
        [self loadShadersWithProgram:&_program2 vshFileName:@"Shader" fshFileName:@"Shader2"];
        
        uniforms2[UNIFORM_MVP_MATRIX] = glGetUniformLocation(_program2, [@"modelViewProjectionMatrix" cStringUsingEncoding:NSUTF8StringEncoding]);
        uniforms2[UNIFORM_TEXTURE0] = glGetUniformLocation(_program2, [@"texture0" cStringUsingEncoding:NSUTF8StringEncoding]);
        uniforms2[UNIFORM_SCREENSIZE] = glGetUniformLocation(_program2, [@"screensize" cStringUsingEncoding:NSUTF8StringEncoding]);
    }
    
    // setup triangle to be drawn in 1st path
    glGenVertexArraysOES(1, &_vertexArray1);
    glBindVertexArrayOES(_vertexArray1);
    
    glGenBuffers(1, &_vertexBuffer1);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gTriangleData), &gTriangleData[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)0);
    glEnableVertexAttribArray(ATTRIB_NORMAL);
    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(ATTRIB_TEXCOORD);
    glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*6));
    
    glBindVertexArrayOES(0);
    
    // setup plane to draw in 2nd path
    glGenVertexArraysOES(1, &_vertexArray2);
    glBindVertexArrayOES(_vertexArray2);
    
    glGenBuffers(1, &_vertexBuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexData), &gVertexData[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)0);
    glEnableVertexAttribArray(ATTRIB_NORMAL);
    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(ATTRIB_TEXCOORD);
    glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*6));
    
    glBindVertexArrayOES(0);
    
    //// setup texture
    
    // load texture（image size doesn't have to be power of 2 in GLES2.0）
    NSURL *imageURL0 = [[NSBundle mainBundle] URLForResource:@"shinden" withExtension:@"png"];
    _texInfo0 = [GLKTextureLoader textureWithContentsOfURL:imageURL0 options:nil error:NULL];
    
    // prepare for texture and rendering
    CGSize viewSize = self.view.bounds.size;
    CGFloat scale = self.view.contentScaleFactor;
    viewSize.width *= scale;
    viewSize.height *= scale;
    
    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    
    [self pepareTextureWithInternalFormat:GL_RGBA
                                   format:GL_RGBA
                                     type:GL_UNSIGNED_BYTE
                                    texId:&_fbColTex
                               attachment:GL_COLOR_ATTACHMENT0
                               scaledSize:viewSize];
    
    //// initialize variables
    _rotation = 0.0;
}

- (void)pepareTextureWithInternalFormat:(GLint)internalFormat
                                 format:(GLenum)format
                                   type:(GLenum)type
                                  texId:(GLuint*)texId
                             attachment:(GLenum)attachment
                             scaledSize:(CGSize)viewSize
{
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, texId);
    
    if (*texId == GL_INVALID_OPERATION) {
        NSLog(@"[Error] GLMRenderTarget: Failed to creaete a texture as a rendering target (INVALID_OPERATION).");
        return;
    } else if (*texId == GL_INVALID_VALUE) {
        NSLog(@"[Error] GLMRenderTarget: Failed to creaete a texture as a rendering target (INVALID_VALUE).");
        return;
    } else if (*texId == GL_INVALID_ENUM) {
        NSLog(@"[Error] GLMRenderTarget: Failed to creaete a texture as a rendering target (INVALID_ENUM).");
        return;
    }
    
    glBindTexture(GL_TEXTURE_2D, *texId);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D,     // Specifies the target texture
                 0,                 // Specifies the level-of-detail number
                 internalFormat,    // Specifies the number of color components in the texture
                 (GLsizei)viewSize.width,   // Specifies the height of the texture image
                 (GLsizei)viewSize.height,  // Specifies the width of the texture image
                 0,                 // This value must be 0
                 format,            // Specifies the format of the pixel data
                 type,              // Specifies the data type of the pixel data
                 0);                // Specifies a pointer to the image data in memory
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, *texId, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
    glDeleteBuffers(1, &_vertexBuffer1);
    glDeleteBuffers(1, &_vertexBuffer2);
    glDeleteVertexArraysOES(1, &_vertexArray1);
    glDeleteVertexArraysOES(1, &_vertexArray2);
    
    if (_fbColTex != GL_INVALID_VALUE) {
        glDeleteTextures(1, &_fbColTex);
    }

    glDeleteFramebuffers(1, &_framebuffer);
    
    if (_program1) {
        glDeleteProgram(_program1);
        _program1 = 0;
    }
    
    if (_program2) {
        glDeleteProgram(_program2);
        _program2 = 0;
    }
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    _rotation += self.timeSinceLastUpdate * 0.5f;
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    // draw path1
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glUseProgram(_program1);
    [self drawPath1];
    
    // draw path2
    [view bindDrawable];
    glUseProgram(_program2);
    [self drawPath2];
}

#pragma mark - Drawing path

- (void)drawPath1
{
    // set for 3D drawing
    glEnable(GL_DEPTH_TEST);
    
    // binding texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(_texInfo0.target, _texInfo0.name);
    glUniform1i(uniforms[UNIFORM_TEXTURE0], 0);
    
    // setup camera
    float aspect = fabsf(self.view.bounds.size.width / self.view.bounds.size.height);
    GLKMatrix4 projectionMatrix = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(65.0f), aspect, 0.1f, 100.0f);
    GLKMatrix4 viewMatrix = GLKMatrix4MakeLookAt(0.0f, 0.0f, -0.8f,
                                                 0.0f, 0.0f, 0.0f,
                                                 0.0f, 1.0f, 0.0f);
    GLKMatrix4 modelMatrix = GLKMatrix4MakeTranslation(0.0f, 0.0f, 1.5f);
    modelMatrix = GLKMatrix4Rotate(modelMatrix, -M_PI/6, 1.0f, 0.0f, 0.0f);
    modelMatrix = GLKMatrix4Rotate(modelMatrix, _rotation, 0.0f, 1.0f, 0.0f);
    
    // compute matrices
    GLKMatrix4 modelViewMatrix = GLKMatrix4Multiply(viewMatrix, modelMatrix);
    GLKMatrix4 mvpMatrix = GLKMatrix4Multiply(projectionMatrix, modelViewMatrix);
    GLKMatrix3 normalMatrix = GLKMatrix3InvertAndTranspose(GLKMatrix4GetMatrix3(modelViewMatrix), NULL);
    
    glUniformMatrix4fv(uniforms[UNIFORM_MVP_MATRIX], 1, 0, mvpMatrix.m);
    glUniformMatrix3fv(uniforms[UNIFORM_NORMAL_MATRIX], 1, 0, normalMatrix.m);
    
    // draw
    glClearColor(0.392*0.5, 0.584*0.7, 0.929, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArrayOES(_vertexArray1);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

- (void)drawPath2
{
    // set for 2D drawing
    glDisable(GL_DEPTH_TEST);
    
    // binding texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _fbColTex);
    glUniform1i(uniforms2[UNIFORM_TEXTURE0], 0);
    
    // setup camera(2D)
    GLKMatrix4 projectionMatrix = GLKMatrix4MakeOrtho(0.0, 320, 480, 0.0, 0.001, 100.0);
    GLKMatrix4 viewMatrix = GLKMatrix4MakeLookAt(0.0f, 0.0f, 1.0f,
                                                 0.0f, 0.0f, 0.0f,
                                                 0.0f, 1.0f, 0.0f);
    GLKMatrix4 modelMatrix = GLKMatrix4Identity;
    modelMatrix = GLKMatrix4Scale(modelMatrix, 320, 480, 0.0);
    
    // compute matrices
    GLKMatrix4 modelViewMatrix = GLKMatrix4Multiply(viewMatrix, modelMatrix);
    GLKMatrix4 mvpMatrix = GLKMatrix4Multiply(projectionMatrix, modelViewMatrix);
    
    glUniformMatrix4fv(uniforms2[UNIFORM_MVP_MATRIX], 1, 0, mvpMatrix.m);
    
    // pass screen size to shader
    GLKVector2 screensize = {0.0};
    screensize.x = self.view.bounds.size.width * self.view.contentScaleFactor;
    screensize.y = self.view.bounds.size.height * self.view.contentScaleFactor;
    glUniform2fv(uniforms2[UNIFORM_SCREENSIZE], 1, &(screensize.v[0]));
    
    // draw. no need to clear since drawing entire screen.
    glBindVertexArrayOES(_vertexArray2);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

#pragma mark -  OpenGL ES 2 shader compilation

- (BOOL)loadShadersWithProgram:(GLuint*)program vshFileName:(NSString*) vshFileName fshFileName:(NSString*) fshFileName
{
    GLuint vertShader, fragShader;
    NSString *vertShaderPathname, *fragShaderPathname;
    
    // Create shader program.
    *program = glCreateProgram();
    
    // Create and compile vertex shader.
    vertShaderPathname = [[NSBundle mainBundle] pathForResource:vshFileName ofType:@"vsh"];
    if (![self compileShader:&vertShader type:GL_VERTEX_SHADER file:vertShaderPathname]) {
        NSLog(@"Failed to compile vertex shader");
        return NO;
    }
    
    // Create and compile fragment shader.
    fragShaderPathname = [[NSBundle mainBundle] pathForResource:fshFileName ofType:@"fsh"];
    if (![self compileShader:&fragShader type:GL_FRAGMENT_SHADER file:fragShaderPathname]) {
        NSLog(@"Failed to compile fragment shader");
        return NO;
    }
    
    // Attach vertex shader to program.
    glAttachShader(*program, vertShader);
    
    // Attach fragment shader to program.
    glAttachShader(*program, fragShader);
    
    // Bind attribute locations.
    // This needs to be done prior to linking.
    glBindAttribLocation(*program, ATTRIB_VERTEX, "position");
    glBindAttribLocation(*program, ATTRIB_NORMAL, "normal");
    //glBindAttribLocation(*program, ATTRIB_TEXCOORD, "texCoord");
    
    // Link program.
    if (![self linkProgram:*program]) {
        NSLog(@"Failed to link program: %d", *program);
        
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (*program) {
            glDeleteProgram(*program);
            *program = 0;
        }
        
        return NO;
    }
    
    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(*program, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(*program, fragShader);
        glDeleteShader(fragShader);
    }
    
    return YES;
}


- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type file:(NSString *)file
{
    GLint status;
    const GLchar *source;
    
    source = (GLchar *)[[NSString stringWithContentsOfFile:file encoding:NSUTF8StringEncoding error:nil] UTF8String];
    if (!source) {
        NSLog(@"Failed to load vertex shader");
        return NO;
    }
    
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
    
#if defined(DEBUG)
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        NSLog(@"Shader compile log:\n%s", log);
        free(log);
    }
#endif
    
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        glDeleteShader(*shader);
        return NO;
    }
    
    return YES;
}

- (BOOL)linkProgram:(GLuint)prog
{
    GLint status;
    glLinkProgram(prog);
    
#if defined(DEBUG)
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        NSLog(@"Program link log:\n%s", log);
        free(log);
    }
#endif
    
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0) {
        return NO;
    }
    
    return YES;
}

- (BOOL)validateProgram:(GLuint)prog
{
    GLint logLength, status;
    
    glValidateProgram(prog);
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        NSLog(@"Program validate log:\n%s", log);
        free(log);
    }
    
    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
    if (status == 0) {
        return NO;
    }
    
    return YES;
}

@end
