#pragma once

#ifndef PI
#define PI  3.14159265358979323846f
#endif
#define EPSILON		(1e-5f)
#define FEQ(x,y)	(abs((x)-(y))<EPSILON)
#define ISPOSITIVE(x)	((x) > 0.0f)

