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
    vec4 destColor = texture2D(texture0, f_texCoord.st * tFrag);

    destColor *= 0.36;
    destColor += texture2D(texture0, f_texCoord + vec2(-1.0,  1.0) * tFrag) * 0.04;
    destColor += texture2D(texture0, f_texCoord + vec2( 0.0,  1.0) * tFrag) * 0.04;
    destColor += texture2D(texture0, f_texCoord + vec2( 1.0,  1.0) * tFrag) * 0.04;
    destColor += texture2D(texture0, f_texCoord + vec2(-1.0,  0.0) * tFrag) * 0.04;
    destColor += texture2D(texture0, f_texCoord + vec2( 1.0,  0.0) * tFrag) * 0.04;
    destColor += texture2D(texture0, f_texCoord + vec2(-1.0, -1.0) * tFrag) * 0.04;
    destColor += texture2D(texture0, f_texCoord + vec2( 0.0, -1.0) * tFrag) * 0.04;
    destColor += texture2D(texture0, f_texCoord + vec2( 1.0, -1.0) * tFrag) * 0.04;
    destColor += texture2D(texture0, f_texCoord + vec2(-2.0,  2.0) * tFrag) * 0.02;
    destColor += texture2D(texture0, f_texCoord + vec2(-1.0,  2.0) * tFrag) * 0.02;
    destColor += texture2D(texture0, f_texCoord + vec2( 0.0,  2.0) * tFrag) * 0.02;
    destColor += texture2D(texture0, f_texCoord + vec2( 1.0,  2.0) * tFrag) * 0.02;
    destColor += texture2D(texture0, f_texCoord + vec2( 2.0,  2.0) * tFrag) * 0.02;
    destColor += texture2D(texture0, f_texCoord + vec2(-2.0,  1.0) * tFrag) * 0.02;
    destColor += texture2D(texture0, f_texCoord + vec2( 2.0,  1.0) * tFrag) * 0.02;
    destColor += texture2D(texture0, f_texCoord + vec2(-2.0,  0.0) * tFrag) * 0.02;
    destColor += texture2D(texture0, f_texCoord + vec2( 2.0,  0.0) * tFrag) * 0.02;
    destColor += texture2D(texture0, f_texCoord + vec2(-2.0, -1.0) * tFrag) * 0.02;
    destColor += texture2D(texture0, f_texCoord + vec2( 2.0, -1.0) * tFrag) * 0.02;
    destColor += texture2D(texture0, f_texCoord + vec2(-2.0, -2.0) * tFrag) * 0.02;
    destColor += texture2D(texture0, f_texCoord + vec2(-1.0, -2.0) * tFrag) * 0.02;
    destColor += texture2D(texture0, f_texCoord + vec2( 0.0, -2.0) * tFrag) * 0.02;
    destColor += texture2D(texture0, f_texCoord + vec2( 1.0, -2.0) * tFrag) * 0.02;
    destColor += texture2D(texture0, f_texCoord + vec2( 2.0, -2.0) * tFrag) * 0.02;

    gl_FragColor  = destColor;
}

