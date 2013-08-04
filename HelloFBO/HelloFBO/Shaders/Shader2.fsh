//
//  Shader.fsh
//  HelloFBO
//
//  Created by nobu on 13/08/04.
//  Copyright (c) 2013年 Nobuhiro Kuroiwa. All rights reserved.
//

varying lowp vec2 f_texCoord;
varying lowp vec4 colorVarying;

uniform sampler2D texture0;

void main()
{
    gl_FragColor = texture2D(texture0, f_texCoord).rgba;
}

