//
//  Shader.vsh
//  HelloFBO
//
//  Created by nobu on 13/08/04.
//  Copyright (c) 2013年 Nobuhiro Kuroiwa. All rights reserved.
//

attribute vec4 position;
attribute vec3 normal;
attribute vec2 texCoord;

varying lowp vec2 f_texCoord;

uniform mat4 modelViewProjectionMatrix;

void main()
{
    vec3 n = normal;// not used
    
    gl_Position = modelViewProjectionMatrix * position;
    f_texCoord = texCoord;
}

