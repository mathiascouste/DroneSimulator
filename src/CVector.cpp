#include "CVector.hpp"

CVector::CVector ()
   : x (0.0f), y (0.0f), z (0.0f)
{
   // Ne fait rien
}

CVector::CVector (const float fX, const float fY, const float fZ)
   : x (fX), y (fY), z (fZ)
{
   // Ne fait rien
}

CVector::~CVector()
{
   // Ne fait rien
}

void CVector::ReglerCoordonnees (const float fX, const float fY, const float fZ)
{
   x = fX;
   y = fY;
   z = fZ;
}

CVector  CVector::operator- (const CVector v)const{
	return CVector(x - v.x, y - v.y, z - v.z);
}

CVector  CVector::operator+ (const CVector v)const{
	return CVector(x + v.x, y + v.y, z + v.z);
}

CVector  CVector::operator* (const CVector v)const{
	return CVector(
		y*v.z - z*v.y,
		z*v.x - x*v.z,
		x*v.y - y*v.x);
}
