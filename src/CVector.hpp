#ifndef _CVECTOR_H_
#define _CVECTOR_H_

class CVector
{
   public:
      CVector ();
      CVector (const float fX, const float fY, const float fZ);
      virtual ~CVector();

      void ReglerCoordonnees (const float fX, const float fY, const float fZ);

		CVector  operator- (CVector) const;
		CVector  operator+ (CVector) const;
		CVector  operator* (CVector) const;

      float x, y, z;
};

#endif // CVECTOR_H
