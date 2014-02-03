//
//  Shader.fsh
//  HelloFBO
//
//  Created by nobu on 13/08/04.
//  Copyright (c) 2013年 Nobuhiro Kuroiwa. All rights reserved.
//

precision mediump float;

varying lowp vec2 f_texCoord;

uniform sampler2D texture0;

void main()
{
    vec4 destColor = texture2D(texture0, f_texCoord);
    gl_FragColor = destColor;
}

