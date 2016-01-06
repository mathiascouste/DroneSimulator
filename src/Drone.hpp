#ifndef _DRONE_HPP_
#define _DRONE_HPP_

#include <Newton.h>
#include <stdlib.h>
#include <stdlib.h>

#include "dMath/dVector.h"
#include "dMath/dMatrix.h"
#include "CVector.hpp"
#include "Scene.hpp"
#include "Cylindre.hpp"

#define INITIAL_HEIGHT -4
#define INITIAL_SIZE 3

class Drone
{
private:
	double maxPowerNE, maxPowerSE, maxPowerSO, maxPowerNO;
	Boite *motorNE, *motorSE, *motorSO, *motorNO;
	Boite *body;
	double size;
	void renderBodyAtPos();
	void renderMotorAtPos(float,float,float);
public:
	Drone();
	~Drone();
	void initialize();
	void render();
	dVector torque;
	dVector force;
	static Drone * instance;
	
	void genMaxPower(double maxPower, double moreOrLess);
	void setMotorPower(float ne, float se, float so, float no);
};

dVector CalculateToqueAtPoint (const NewtonBody* body, const dVector& point, const dVector& force);

#endif
