//
//  Shader.fsh
//  HelloOpenGl
//
//  Created by nobu on 13/04/24.
//  Copyright (c) 2013年 nobu. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
