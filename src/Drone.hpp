#ifndef _DRONE_HPP_
#define _DRONE_HPP_

#include <Newton.h>
#include <stdlib.h>

#include "dMath/dVector.h"
#include "dMath/dMatrix.h"
#include "CVector.hpp"
#include "Scene.hpp"
#include "Cylindre.hpp"
#include "Motor.hpp"

#define INITIAL_HEIGHT 5
#define INITIAL_SIZE 3

class Drone
{
private:
	Motor frontMotor, rightMotor, backMotor, leftMotor;
	Boite *body;
	double size;
	void renderBodyAtPos();
public:
	Drone();
	~Drone();
	void initialize();
	void render();
	dVector torque;
	dVector force;
	static Drone * instance;
	
	void genMaxPower(double maxPower, double moreOrLess);
	void setMotorPower(float front, float right, float back, float left);
	void calculateForceAndTorque();
};

dVector CalculateToqueAtPoint (const NewtonBody* body, const dVector& point, const dVector& force);

#endif
