//
//  Shader.fsh
//  HelloFBO
//
//  Created by nobu on 13/08/04.
//  Copyright (c) 2013年 Nobuhiro Kuroiwa. All rights reserved.
//

precision mediump float;

varying lowp vec2 f_texCoord;
//varying lowp vec4 shadowCoord;

uniform sampler2D texture0;
//uniform sampler2D shadowMap;

void main()
{
    vec4 destColor = texture2D(texture0, f_texCoord);
    
    //if(texture2D(shadowMap, shadowCoord.xy).r < shadowCoord.z)
    {
        //destColor = vec4(0,0,0,0);
    }

    gl_FragColor = destColor;
}

