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

//uniform sampler2D texture0;
uniform sampler2D shadowMap;

void main()
{
    vec2 tc = f_texCoord;
    vec4 destColor;
    //vec4 destColor = texture2D(texture0, f_texCoord);
    
    highp float mapval = texture2D(shadowMap, shadowCoord.xy).r;
    highp float shadowCoordz = shadowCoord.z;
    //if(mapval > shadowCoordz)
    {
        //destColor = vec4(0.0,0.0,0.0,0.5);
        //destColor = vec4(mapval,0.0,0.0,0.5);
        destColor = texture2D(shadowMap, shadowCoord.xy);
    }

    gl_FragColor = destColor;
}

