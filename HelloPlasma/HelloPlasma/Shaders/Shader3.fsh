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
    vec4 col = texture2D(texture0, f_texCoord).rgba;
    //if(f_texCoord.x > 0.5) {
        //col = vec4(1.0,1.0,0.0,0.0);
    //    discard;
    //}
    if(col.a == 0.0) {
        col = vec4(1.0,1.0,0.0,0.0);
        //col = vec4(0.0);
        discard;
    }
    gl_FragColor = col;
}

