//#include "primitive.h"
//
//nerv::BVHBound nerv::primitive::triangleBoundingInfo(triangle tris, int index)
//{
//	float sx1 = tris.v1[0];
//	float sx2 = tris.v2[1];
//	float sx3 = tris.v3[2];
//
//	float sy1 = tris.v1[0];
//	float sy2 = tris.v2[1];
//	float sy3 = tris.v3[2];
//
//	float sz1 = tris.v1[0];
//	float sz2 = tris.v2[1];
//	float sz3 = tris.v3[2];
//
//	float xmax = sx1 > sx2 ? (sx1 > sx3 ? sx1 : sx3) : (sx2 > sx3 ? sx2 : sx3);
//	float ymax = sy1 > sy2 ? (sy1 > sy3 ? sy1 : sy3) : (sy2 > sy3 ? sy2 : sy3);
//	float zmax = sz1 > sz2 ? (sz1 > sz3 ? sz1 : sz3) : (sz2 > sz3 ? sz2 : sz3);
//
//	float xmin = sx1 < sx2 ? (sx1 < sx3 ? sx1 : sx3) : (sx2 < sx3 ? sx2 : sx3);
//	float ymin = sy1 < sy2 ? (sy1 < sy3 ? sy1 : sy3) : (sy2 < sy3 ? sy2 : sy3);
//	float zmin = sz1 < sz2 ? (sz1 < sz3 ? sz1 : sz3) : (sz2 < sz3 ? sz2 : sz3);
//	
//	glm::vec3 LLC(xmin, ymin, zmin);
//	glm::vec3 URC(xmax, ymax, zmax);
//
//	BVHBound info{
//		index,
//		LLC,
//		URC,
//		glm::vec3(0.5f * LLC + 0.5f * URC)
//	};
//
//	return info;
//}
