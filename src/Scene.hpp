#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include <Newton.h>
#include <stdlib.h>

#include "CVector.hpp"
#include "Boite.hpp"

class Scene
{
private:
	Boite *sol;
	void initGround();
public:
	~Scene();
	static NewtonWorld * nWorld;
	void initialize();
	void render();
};

#endif
