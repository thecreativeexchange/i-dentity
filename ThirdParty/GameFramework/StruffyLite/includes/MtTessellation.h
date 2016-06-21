////////////////////////////////////////////////////////////////////////////////
// MtTesselation.h

#ifndef __MtTesselation_h__
#define __MtTesselation_h__

// Includes
#include "MtVector2.h"
#include "MtVector3.h"

struct MtTessellation
{
	static void ComputeBarycentricCoordinates( const MtVector2& rkV0,
		                                       const MtVector2& rkV1,
			                                   const MtVector2& rkV2,
				                               const MtVector2& rkP,
					                           BtFloat bary[3]
											 );

	static void ComputeBarycentricCoordinates( const MtVector3& rkV0,
		                                       const MtVector3& rkV1,
			                                   const MtVector3& rkV2,
				                               const MtVector3& rkP,
					                           BtFloat bary[3]
											 );
};

#endif // __MtTesselation