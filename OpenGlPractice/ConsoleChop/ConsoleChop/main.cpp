//
//  main.cpp
//  ConsoleChop
//
//  Created by Nobuhiro Kuroiwa on 2014/03/31.
//  Copyright (c) 2014年 All rights reserved.
//

#include <iostream>
#include <OpenGL/gl.h>
#include <chrono>
#include "Vertex_Zak.h"
#include "Chop.h"

using namespace std;

int main(int argc, const char * argv[])
{
	float bufN[MAX_CHOP_BUF][8] = {0.0f};
	float bufA[MAX_CHOP_BUF][8] = {0.0f};
	int bufNCount = 0;
	int bufACount = 0;
	
	auto start = std::chrono::system_clock::now();
	
	for(float x = 0.0; x < 1.0; x+=0.001) {
		MODELVEC3D pos = {1.0f, 1.0f, 0.0f};
		MODELVEC3D normal = {0.0f, x, 0.0f};
		MODELPLANE plane = { pos, normal};

		Chop(plane,
			 &(ZakVertexData[0].vertex.x),
			 sizeof(ZakVertexData)/sizeof(vertexData),
			 bufN,
			 bufA,
			 bufNCount,
			 bufACount);
		
		if(bufNCount == 0 && bufACount == 0)
		{
			cout << "not worked" << endl;
		}
	}
	
	auto end = std::chrono::system_clock::now();
	
	auto diff = end - start;
    std::cout << "elapsed time = "
	<< std::chrono::duration_cast<std::chrono::milliseconds>(diff).count()
	<< " msec."
	<< std::endl;
	
    return 0;
}

