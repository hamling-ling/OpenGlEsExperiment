//
//  Shader.fsh
//  HelloFBO
//
//  Created by nobu on 13/08/04.
//  Copyright (c) 2013年 Nobuhiro Kuroiwa. All rights reserved.
//

precision highp float;

varying lowp vec2 f_texCoord;
varying lowp vec4 shadowCoord;

uniform sampler2D texture0;
uniform sampler2D shadowMap;

void multiTexExperiment()
{
    vec2 tc = f_texCoord;
    vec4 destColor = texture2D(texture0, f_texCoord);
    
    vec4 biasedDepthCoord = shadowCoord * 0.5;
    biasedDepthCoord = biasedDepthCoord + 0.5;
    float depth = texture2D(shadowMap, biasedDepthCoord.xy).r;

    gl_FragColor = destColor * depth;
}

void main()
{
    //worked();
    //experiment();
    multiTexExperiment();
}

