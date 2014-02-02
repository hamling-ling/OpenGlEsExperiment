//
//  DepthShader.fsh
//  HelloShadow
//
//  Created by nobu on 13/08/04.
//  Copyright (c) 2013年 Nobuhiro Kuroiwa. All rights reserved.
//

precision mediump float;

void main()
{
    gl_FragColor = vec4(gl_FragCoord.z, 0.0, 0.0, 1.0);
}

