/************************************************************************/
/*							Create by LYujia				 			*/
/*								2019/7/17		 						*/
/*			Copyright 2019 by LYujia, All Rights Reserved.		 		*/
/************************************************************************/

#pragma once

#include <string>
#include <iostream>

using namespace std;

class Vector3f
{
public:
	Vector3f();
	Vector3f(float _x, float _y, float _z);

	~Vector3f();

private:
	float x, y, z;
};