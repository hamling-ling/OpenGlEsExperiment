//
//  ViewController.m
//  HelloShadowMap
//
//  Created by Nobuhiro Kuroiwa on 2014/01/30.
//  Copyright (c) 2014年 Nobuhiro Kuroiwa. All rights reserved.
//

#import "ViewController.h"

//#define FBODEBUG

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// Uniform index.
enum
{
    UNIFORM_MVP_MATRIX,
    UNIFORM_NORMAL_MATRIX,
    UNIFORM_BIASEDSHADOW_MATRIX,
    UNIFORM_TEXTURE0,
    UNIFORM_TEXTURE1,
    NUM_UNIFORMS
};
GLint uniforms[NUM_UNIFORMS];
GLint uniforms2[NUM_UNIFORMS];
#ifdef FBODEBUG
GLint uniforms3[NUM_UNIFORMS];
#endif

// Attribute index.
enum
{
    ATTRIB_VERTEX,
    ATTRIB_NORMAL,
    ATTRIB_TEXCOORD,
    NUM_ATTRIBUTES
};

const GLfloat cubeVertices[][8] =
{
    // Front
    { -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f},
    {  0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f},
    {  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f},
    
    {  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f},
    { -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f},
    { -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f},
    
    // Back
    {  0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f},
    { -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f},
    { -0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 1.0f},
    
    { -0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 1.0f},
    {  0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f},
    {  0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f},
    
    // Right
    {  0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f},
    {  0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f},
    {  0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f},
    
    {  0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f},
    {  0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 1.0f},
    {  0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f},
    
    // Left
    { -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f},
    { -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f},
    { -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f},
    
    { -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f},
    { -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f},
    { -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f},
    
    // Top
    { -0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f},
    {  0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f},
    {  0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f},
    {  0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f},
    { -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f},
    { -0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f},
    
    // Bottom
    {  0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f},
    { -0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f},
    { -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f},
    
    { -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f},
    {  0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f},
    {  0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f}
};

const GLfloat floorVertices[][8] =
{
    {-5.0f, -0.5f,  5.0f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f},
    { 5.0f, -0.5f,  5.0f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f},
    { 5.0f, -0.5f, -5.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f},
    
    { 5.0f, -0.5f, -5.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f},
    {-5.0f, -0.5f, -5.0f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f},
    {-5.0f, -0.5f,  5.0f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f}
};

#ifdef FBODEBUG
const GLfloat screenVertices[][8] =
{
    {  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
    {  1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
    {  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
    
    {  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
    {  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
    {  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f}
};
#endif

@interface ViewController () {
    GLuint  _vertexArray1, _vertexArray2;
    GLuint  _vertexBuffer1, _vertexBuffer2;

    GLuint  _framebuffer;
    GLuint  _fbColTex, _fbDepTex;
    
    // shader programs
    GLuint _program1, _program2;
    
    //texture
    GLKTextureInfo  *_texInfo0;
    
    // rotation
    float           _rotation;

#ifdef FBODEBUG
    // for debug
    GLuint _vertexArray3, _vertexBuffer3, _program3;
#endif
}
@property (strong, nonatomic) EAGLContext *context;

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
        [self loadShadersWithProgram:&_program1 vshFileName:@"DepthShader" fshFileName:@"DepthShader"];
        
        uniforms[UNIFORM_MVP_MATRIX] = glGetUniformLocation(_program1, [@"modelViewProjectionMatrix" cStringUsingEncoding:NSUTF8StringEncoding]);
    }
    
    // create a program for path2
    {
        [self loadShadersWithProgram:&_program2 vshFileName:@"Shader" fshFileName:@"Shader"];

        uniforms2[UNIFORM_MVP_MATRIX] = glGetUniformLocation(_program2, [@"modelViewProjectionMatrix" cStringUsingEncoding:NSUTF8StringEncoding]);
        uniforms2[UNIFORM_BIASEDSHADOW_MATRIX] = glGetUniformLocation(_program2, [@"depthBiasMVP" cStringUsingEncoding:NSUTF8StringEncoding]);
        uniforms2[UNIFORM_TEXTURE0] = glGetUniformLocation(_program2, [@"texture0" cStringUsingEncoding:NSUTF8StringEncoding]);
        uniforms2[UNIFORM_TEXTURE1] = glGetUniformLocation(_program2, [@"shadowMap" cStringUsingEncoding:NSUTF8StringEncoding]);
    }

#ifdef FBODEBUG
    // for debug
    {
        [self loadShadersWithProgram:&_program3 vshFileName:@"SimpleShader" fshFileName:@"SimpleShader"];
        
        uniforms3[UNIFORM_MVP_MATRIX] = glGetUniformLocation(_program3, [@"modelViewProjectionMatrix" cStringUsingEncoding:NSUTF8StringEncoding]);
    }
#endif
    
    // setup cube vertex
    glGenVertexArraysOES(1, &_vertexArray1);
    glBindVertexArrayOES(_vertexArray1);
    
    glGenBuffers(1, &_vertexBuffer1);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, (void*)0);
    glEnableVertexAttribArray(ATTRIB_NORMAL);
    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, (void*)(sizeof(GLfloat)*3));
    glEnableVertexAttribArray(ATTRIB_TEXCOORD);
    glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, (void*)(sizeof(GLfloat)*6));
    
    glBindVertexArrayOES(0);
    
    // setup floor vertex
    glGenVertexArraysOES(1, &_vertexArray2);
    glBindVertexArrayOES(_vertexArray2);
    
    glGenBuffers(1, &_vertexBuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), &floorVertices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, (void*)0);
    glEnableVertexAttribArray(ATTRIB_NORMAL);
    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, (void*)(sizeof(GLfloat)*3));
    glEnableVertexAttribArray(ATTRIB_TEXCOORD);
    glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, (void*)(sizeof(GLfloat)*6));
    
    glBindVertexArrayOES(0);

#ifdef FBODEBUG
    // setup screen vertex. for debug use
    glGenVertexArraysOES(1, &_vertexArray3);
    glBindVertexArrayOES(_vertexArray3);
    
    glGenBuffers(1, &_vertexBuffer3);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), &screenVertices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, (void*)0);
    glEnableVertexAttribArray(ATTRIB_NORMAL);
    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, (void*)(sizeof(GLfloat)*3));
    glEnableVertexAttribArray(ATTRIB_TEXCOORD);
    glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, (void*)(sizeof(GLfloat)*6));
    
    glBindVertexArrayOES(0);
#endif

    //// setup texture
    
    // load texture（image size doesn't have to be power of 2 in GLES2.0）
    NSURL *imageURL0 = [[NSBundle mainBundle] URLForResource:@"peko" withExtension:@"png"];
    _texInfo0 = [GLKTextureLoader textureWithContentsOfURL:imageURL0 options:nil error:NULL];
    
    // prepare for texture and rendering
    CGSize viewSize = self.view.bounds.size;
    CGFloat scale = self.view.contentScaleFactor;
    viewSize.width *= scale;
    viewSize.height *= scale;
    
    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    
    [self pepareTextureForFrameBuffer:_framebuffer
                       internalFormat:GL_RGBA
                               format:GL_RGBA
                                 type:GL_UNSIGNED_BYTE
                                texId:&_fbColTex
                           attachment:GL_COLOR_ATTACHMENT0
                           scaledSize:viewSize];
    [self pepareTextureForFrameBuffer:_framebuffer
                       internalFormat:GL_DEPTH_COMPONENT
                               format:GL_DEPTH_COMPONENT
                                 type:GL_UNSIGNED_SHORT
                                texId:&_fbDepTex
                           attachment:GL_DEPTH_ATTACHMENT
                           scaledSize:viewSize];
    //// initialize variables
    _rotation = 0.0;
}

- (void)pepareTextureForFrameBuffer:(GLuint)framebuffer
                     internalFormat:(GLint)internalFormat
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
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
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
    if (_fbDepTex != GL_INVALID_VALUE) {
        glDeleteTextures(1, &_fbDepTex);
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

#ifdef FBODEBUG
    glDeleteBuffers(1, &_vertexBuffer3);
    glDeleteVertexArraysOES(1, &_vertexArray3);
    if (_program3) {
        glDeleteProgram(_program3);
        _program3 = 0;
    }
#endif
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
    GLKMatrix4 depthMVP = [self drawPath1];
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);

#ifndef FBODEBUG
    // draw path2
    [view bindDrawable];
    glUseProgram(_program2);
    [self drawPath2WithDepthMVP:depthMVP];
#else
    [view bindDrawable];
    glUseProgram(_program3);
    [self drawDepthBufferForDebug];
#endif
}

#pragma mark - Drawing path

- (GLKMatrix4)drawPath1
{
    // set for 3D drawing
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // setup camera
    GLKVector3 lightPos = GLKVector3Make(1.0, 1.0, 1.0);
    GLKMatrix4 depthProjectionMatrix = GLKMatrix4MakeOrtho(-10, 10, -10, 10, -10, 20);
    GLKMatrix4 depthViewMatrix = GLKMatrix4MakeLookAt(lightPos.x, lightPos.y, lightPos.z,
                                                      0.0f, 0.0f, 0.0f,
                                                      0.0f, 1.0f, 0.0f);
    GLKMatrix4 depthMVP = GLKMatrix4Multiply( depthProjectionMatrix, depthViewMatrix);

    glUniformMatrix4fv(uniforms[UNIFORM_MVP_MATRIX], 1, 0, depthMVP.m);
    
    // clear
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // draw floor
    glBindVertexArrayOES(_vertexArray2);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(floorVertices)/sizeof(GLfloat)/8);
    
    // draw cube
    glBindVertexArrayOES(_vertexArray1);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices)/sizeof(GLfloat)/8);
    glBindVertexArrayOES(0);
    
    return depthMVP;
}

- (void)drawPath2WithDepthMVP:(GLKMatrix4)depthMVP
{
    // set for 2D drawing
    glEnable(GL_DEPTH_TEST);
    
    // binding texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texInfo0.name);
    glUniform1i(uniforms2[UNIFORM_TEXTURE0], 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _fbColTex);
    glUniform1i(uniforms2[UNIFORM_TEXTURE1], 1);
    
    // setup camera
    float aspect = fabsf(self.view.bounds.size.width / self.view.bounds.size.height);
    GLKMatrix4 projectionMatrix = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(65.0f), aspect, 0.1f, 100.0f);
    GLKMatrix4 viewMatrix = GLKMatrix4MakeLookAt(0.0f, 3.0f, 3.0f,
                                                 0.0f, 0.0f, 0.0f,
                                                 0.0f, 1.0f, 0.0f);
    GLKMatrix4 modelMatrix = GLKMatrix4Identity;
    modelMatrix = GLKMatrix4Rotate(modelMatrix, _rotation, 0.0f, 1.0f, 0.0f);
    
    // compute matrices
    GLKMatrix4 modelViewMatrix = GLKMatrix4Multiply(viewMatrix, modelMatrix);
    GLKMatrix4 mvpMatrix = GLKMatrix4Multiply(projectionMatrix, modelViewMatrix);
    
    glUniformMatrix4fv(uniforms2[UNIFORM_MVP_MATRIX], 1, 0, mvpMatrix.m);
    
    // pass depthMVP
    glUniformMatrix4fv(uniforms2[UNIFORM_BIASEDSHADOW_MATRIX], 1, 0, depthMVP.m);

    // clear
    glClearColor(0.392*0.5, 0.584*0.7, 0.929, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw floor
    glBindVertexArrayOES(_vertexArray2);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(floorVertices)/sizeof(GLfloat)/8);
    
    // draw cube
    glBindVertexArrayOES(_vertexArray1);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices)/sizeof(GLfloat)/8);
}

#ifdef FBODEBUG
- (void)drawDepthBufferForDebug
{
    // set for 2D drawing
    glDisable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);// why GL_BACK doesn't work ?
    
    // binding texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _fbColTex);
    
    // setup camera(2D)
    GLKMatrix4 projectionMatrix = GLKMatrix4MakeOrtho(0.0, 320, 0, 480, 0.001, 100.0);
    GLKMatrix4 viewMatrix = GLKMatrix4MakeLookAt(0.0f, 0.0f, 1.0f,
                                                 0.0f, 0.0f, 0.0f,
                                                 0.0f, 1.0f, 0.0f);
    GLKMatrix4 modelMatrix = GLKMatrix4Identity;
    modelMatrix = GLKMatrix4Scale(modelMatrix, 320, 480, 0.0);
    
    // compute matrices
    GLKMatrix4 modelViewMatrix = GLKMatrix4Multiply(viewMatrix, modelMatrix);
    GLKMatrix4 mvpMatrix = GLKMatrix4Multiply(projectionMatrix, modelViewMatrix);
    
    glUniformMatrix4fv(uniforms3[UNIFORM_MVP_MATRIX], 1, 0, mvpMatrix.m);
    
    // draw. no need to clear since drawing entire screen.
    glBindVertexArrayOES(_vertexArray3);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(screenVertices)/sizeof(GLfloat)/8);
}
#endif

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
        NSLog(@"Failed to compile vertex shader %@", vshFileName);
        return NO;
    }
    
    // Create and compile fragment shader.
    fragShaderPathname = [[NSBundle mainBundle] pathForResource:fshFileName ofType:@"fsh"];
    if (![self compileShader:&fragShader type:GL_FRAGMENT_SHADER file:fragShaderPathname]) {
        NSLog(@"Failed to compile fragment shader %@", fshFileName);
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
    glBindAttribLocation(*program, ATTRIB_TEXCOORD, "texCoord");
    
    // Link program.
    BOOL linked = [self linkProgram:*program];
    
    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(*program, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(*program, fragShader);
        glDeleteShader(fragShader);
    }
    
    if(!linked) {
        NSLog(@"Failed to link program: %d for %@ and %@", *program, fshFileName, vshFileName);

        if (*program) {
            glDeleteProgram(*program);
            *program = 0;
        }
    }
    
    return linked;
}

- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type file:(NSString *)file
{
    GLint status;
    const GLchar *source;
    
    source = (GLchar *)[[NSString stringWithContentsOfFile:file encoding:NSUTF8StringEncoding error:nil] UTF8String];
    if (!source) {
        NSLog(@"Failed to load vertex shader %@", file);
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
        NSLog(@"Shader compile log for %@ :\n%s", file, log);
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

