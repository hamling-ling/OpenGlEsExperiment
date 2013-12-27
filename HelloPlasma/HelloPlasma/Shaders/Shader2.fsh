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

    int N = 10;
    for(int i=-N; i<=N; i++) {
        for(int j=-N; j<=N; j++) {
            destColor += texture2D(texture0, f_texCoord + vec2( float(i), float(j)) * tFrag);
        }
    }

    float loopNum = 2.0*float(N)+1.0;
    destColor /= (loopNum * loopNum);

    gl_FragColor  = destColor;

}

