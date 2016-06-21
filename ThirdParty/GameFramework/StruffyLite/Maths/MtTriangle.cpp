// Local includes /////////////////////////////////////////////////////////////
#include "MtMath.h"
#include "MtVector3.h"
#include "MtVector4.h"
#include "MtLine.h"
#include "MtTriangle.h"
#include "MtPlane.h"

// Class definition ///////////////////////////////////////////////////////////
BtBool MtTriangle::SphericalTest(const MtTriangle &other)
{
  BtFloat temp_dist;

  BtFloat t = (radius + other.radius);
  temp_dist = t * t;

  if(( ((other.center.x-center.x) * (other.center.x-center.x))+
       ((other.center.y-center.y) * (other.center.y-center.y))+
       ((other.center.z-center.z) * (other.center.z-center.z))
     ) < temp_dist )
  {
      return BtTrue;
  }

	return false;
}

void MtTriangle::FindPolygonCenter()
{
     BtFloat minx,miny,minz,maxx,maxy,maxz;
     minx = maxx = a.x;
     miny = maxy = a.y;
     minz = maxz = a.z;

     if (b.x < minx)
       minx = b.x;

     if (c.x < minx)
       minx = c.x;

     if (b.y < miny)
       miny = b.y;

     if (c.y < miny)
       miny = c.y;

     if (b.z < minz)
       minz = b.z;

     if (c.z < minz)
       minz = c.z;

     if (b.x > maxx)
       maxx = b.x;

     if (c.x > maxx)
       maxx = c.x;

     if (b.y > maxy)
       maxy = b.y;

     if (c.y > maxy)
       maxy = c.y;

     if (b.z > maxz)
       maxz = b.z;

     if (c.z > maxz)
       maxz = c.z;

     center.x = ( minx + maxx ) * 0.5f;
     center.y = ( miny + maxy ) * 0.5f;
     center.z = ( minz + maxz ) * 0.5f;
}

void MtTriangle::FindPolygonRadius()
{
     BtFloat temp_radius;

     temp_radius = (BtFloat) sqrt(((center.x-a.x)*(center.x-a.x))+
				                ((center.y-a.y)*(center.y-a.y))+
				                ((center.z-a.z)*(center.z-a.z)));

     if ((sqrt(((center.x-b.x)*(center.x-b.x))+
	           ((center.y-b.y)*(center.y-b.y))+
	           ((center.z-b.z)*(center.z-b.z)))) > temp_radius)

     temp_radius = (BtFloat) sqrt(((center.x-b.x)*(center.x-b.x))+
				                ((center.y-b.y)*(center.y-b.y))+
				                ((center.z-b.z)*(center.z-b.z)));

     if (((BtFloat) sqrt(((center.x-c.x)*(center.x-c.x))+
		               ((center.y-c.y)*(center.y-c.y))+
		               ((center.z-c.z)*(center.z-c.z))) > temp_radius))

     temp_radius = (BtFloat) sqrt(((center.x-c.x)*(center.x-c.x))+
				                ((center.y-c.y)*(center.y-c.y))+
				                ((center.z-c.z)*(center.z-c.z)));

     radius = temp_radius;
}

void MtTriangle::FindNormal()
{
	normal = MtVector3::CrossProduct( b - a, c - a );
	normal.Normalise();
}

MtTriangle& MtTriangle::operator *= ( const MtMatrix4& matrix )
{
	a = a * matrix;
	b = b * matrix;
	c = c * matrix;
	return *this;
}

BtU32 MtTriangle::Collide(MtTriangle &tri2, MtVector3 intersections[6] )
{
   MtLine line1 = MtLine( a, b );
   MtLine line2 = MtLine( b, c );
   MtLine line3 = MtLine( a, c );

   MtLine line4 = MtLine( tri2.a, tri2.b );
   MtLine line5 = MtLine( tri2.b, tri2.c );
   MtLine line6 = MtLine( tri2.a, tri2.c );

   BtU32 count = 0;

   if( line1.Collide( tri2, intersections[count] ) )
   {
		count++;
   }

   if( line2.Collide( tri2, intersections[count] ) )
   {
		count++;
   }

   if( line3.Collide( tri2, intersections[count] ) )
   {
		count++;
   }

   if( line4.Collide( *this, intersections[count] ) )
   {
		count++;
   }

   if( line5.Collide( *this, intersections[count] ) )
   {
		count++;
   }

   if( line6.Collide( *this, intersections[count] ) )
   {
		count++;
   }

   return count;
}






