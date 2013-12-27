//
//  Shader.fsh
//  HelloFBO
//
//  Created by nobu on 13/08/04.
//  Copyright (c) 2013年 Nobuhiro Kuroiwa. All rights reserved.
//

precision mediump float;

varying lowp vec2 f_texCoord;
varying lowp vec4 colorVarying;

uniform sampler2D texture0;

void main()
{
    vec4 col = vec4(1.0,1.0,0.0,1.0);
    gl_FragColor = col;
}

