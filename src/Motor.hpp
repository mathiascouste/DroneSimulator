#ifndef _MOTOR_HPP_
#define _MOTOR_HPP_


#include "dMath/dVector.h"
#include "dMath/dMatrix.h"
#include "Scene.hpp"
#include "Boite.hpp"

class Motor {
private:
	double maxPower;
	double powerOrder;
	double size;
	Boite *body;
public:
	dVector force;
	dVector torque;
	
	Motor() = default;
	Motor(double size);
	
	void initialize(double xPosition, double yPosition, double zPosition);
	void Render();
	void setMaxPower(double maxPower);
	void setPowerOrder(float order);
	void calculateForceAndTorque();
	
};

#endif
