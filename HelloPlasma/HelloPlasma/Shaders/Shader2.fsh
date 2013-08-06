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
uniform vec2 screensize;

void main()
{
    vec2 tFrag = vec2(1.0 / screensize.x, 1.0/screensize.y);
    vec4 destColor = vec4(0.0);

    destColor += texture2D(texture0, f_texCoord + vec2(-9.0,  0.0) * tFrag);
    destColor += texture2D(texture0, f_texCoord + vec2(-8.0,  0.0) * tFrag);
    destColor += texture2D(texture0, f_texCoord + vec2(-7.0,  0.0) * tFrag);
    destColor += texture2D(texture0, f_texCoord + vec2(-6.0,  0.0) * tFrag);
    destColor += texture2D(texture0, f_texCoord + vec2(-5.0,  0.0) * tFrag);
    destColor += texture2D(texture0, f_texCoord + vec2(-4.0,  0.0) * tFrag);
    destColor += texture2D(texture0, f_texCoord + vec2(-3.0,  0.0) * tFrag);
    destColor += texture2D(texture0, f_texCoord + vec2(-2.0,  0.0) * tFrag);
    destColor += texture2D(texture0, f_texCoord + vec2(-1.0,  0.0) * tFrag);
    destColor += texture2D(texture0, f_texCoord + vec2( 0.0,  0.0) * tFrag);
    destColor += texture2D(texture0, f_texCoord + vec2( 1.0,  0.0) * tFrag);
    destColor += texture2D(texture0, f_texCoord + vec2( 2.0,  0.0) * tFrag);
    destColor += texture2D(texture0, f_texCoord + vec2( 3.0,  0.0) * tFrag);
    destColor += texture2D(texture0, f_texCoord + vec2( 4.0,  0.0) * tFrag);
    destColor += texture2D(texture0, f_texCoord + vec2( 5.0,  0.0) * tFrag);
    destColor += texture2D(texture0, f_texCoord + vec2( 6.0,  0.0) * tFrag);
    destColor += texture2D(texture0, f_texCoord + vec2( 7.0,  0.0) * tFrag);
    destColor += texture2D(texture0, f_texCoord + vec2( 8.0,  0.0) * tFrag);
    destColor += texture2D(texture0, f_texCoord + vec2( 9.0,  0.0) * tFrag);

    vec4 finalColor = destColor / 19.0;
    
    gl_FragColor  = finalColor;
}

