#include "objectuv.h"

ObjectUV::ObjectUV(const MString& name)
:m_objectname(name)
{

}
//
ObjectUV::~ObjectUV()
{

}
//
void ObjectUV::testUV()
{

}

void ObjectUV::getUV()
{
/*		//
		MStatus status;
		MFnMesh fnMesh(mesh->objDagPath, &status);
		IfMErrorMsgWarn(status, ribNode__->name);

		MIntArray triangleCounts,triangleVertices;
		IfMErrorMsgWarn(fnMesh.getTriangles(triangleCounts, triangleVertices), ribNode__->name);


		//get position from liquid-cooked values
		const liqTokenPointer *token = _exportVertexFromNodePlug(ribNode__, sample_first);

		const liqFloat *P = token->getTokenFloatArray();
		const liqFloat *P_mb = ( sample_first != sample_last )?//has motion?
								_exportVertexFromNodePlug(ribNode__, sample_last)->getTokenFloatArray()
								: NULL;
		//uv
		MString currentUVsetName;
		IfMErrorMsgWarn(fnMesh.getCurrentUVSetName(currentUVsetName), ribNode__->name);
		const bool hasUV = currentUVsetName.length()>0 && fnMesh.numUVs(currentUVsetName)>0;

		//normal
		const bool hasSmoothNormal = (fnMesh.numVertices() == fnMesh.numNormals());


		//
		std::vector<MVector> POSITION;
		std::vector<MVector> POSITION_mb;//motion blur position
		std::vector<std::size_t> INDEX;//global vertex index
		std::vector<MVector> NORMAL;
		std::vector<MVector> UV;

		POSITION.reserve(token->getTokenFloatArraySize());
		POSITION_mb.reserve(token->getTokenFloatArraySize());
		INDEX.reserve(token->getTokenFloatArraySize());
		NORMAL.reserve(token->getTokenFloatArraySize());
		UV.reserve(token->getTokenFloatArraySize());

		int numPolygons = fnMesh.numPolygons();
		std::size_t PBufferSize = token->getTokenFloatArraySize();

		int gpi = 0;
		int ti  = 0;
		try{
			for(gpi = 0; gpi< numPolygons; ++gpi)//gpi: global polygon index
			{
				//  for one polygon

				MIntArray vertexList;
				IfMErrorMsgWarn(fnMesh.getPolygonVertices(gpi, vertexList), ribNode__->name);
				assert( vertexList.length() == fnMesh.polygonVertexCount( gpi ) );
				// vertex index in polygon: i  <---> global vertex index: vertexList[i]

				//int vertexCountInPolygon   = fnMesh.polygonVertexCount( gpi );
				int triangleCountInPolygon = triangleCounts[ gpi ];
				for(ti = 0; ti<triangleCountInPolygon; ++ti)//ti: triangle index in a polygon
				{
					//  for one triangle

					int gvi[3]={-1, -1, -1};//global vertex index
					IfMErrorMsgWarn(fnMesh.getPolygonTriangleVertices(gpi, ti, gvi), ribNode__->name);

					//position/triangle index list
					//vertex0
					std::size_t i_v0 = 3*gvi[0];// index of vertex0
					INDEX.push_back(POSITION.size());
					POSITION.push_back(MVector(P[i_v0+0], P[i_v0+1], P[i_v0+2]));//vertex0.x, vertex0.y, vertex0.z
					//vertex1
					std::size_t i_v1 = 3*gvi[1];// index of vertex1
					INDEX.push_back(POSITION.size());
					POSITION.push_back(MVector(P[i_v1+0], P[i_v1+1], P[i_v1+2]));//vertex1.x, vertex1.y, vertex1.z
					//vertex2
					std::size_t i_v2 = 3*gvi[2];// index of vertex2
					INDEX.push_back(POSITION.size());
					POSITION.push_back(MVector(P[i_v2+0], P[i_v2+1], P[i_v2+2]));//vertex2.x, vertex2.y, vertex2.z

					//position motion blur
					if( sample_first != sample_last )
					{
						POSITION_mb.push_back(MVector(P_mb[i_v0+0], P_mb[i_v0+1], P_mb[i_v0+2]));
						POSITION_mb.push_back(MVector(P_mb[i_v1+0], P_mb[i_v1+1], P_mb[i_v1+2]));
						POSITION_mb.push_back(MVector(P_mb[i_v2+0], P_mb[i_v2+1], P_mb[i_v2+2]));
					}


					//normal
					if( hasSmoothNormal )
					{
						MVector normal0(0.0f,0.0f,0.0f);
						MVector normal1(0.0f,0.0f,0.0f);
						MVector normal2(0.0f,0.0f,0.0f);
						IfMErrorMsgWarn(fnMesh.getVertexNormal(gvi[0], false, normal0), ribNode__->name);
						IfMErrorMsgWarn(fnMesh.getVertexNormal(gvi[1], false, normal1), ribNode__->name);
						IfMErrorMsgWarn(fnMesh.getVertexNormal(gvi[2], false, normal2), ribNode__->name);

						NORMAL.push_back(normal0);
						NORMAL.push_back(normal1);
						NORMAL.push_back(normal2);
					}else{
						//let ER calculate the normal internally.
					}


					//uv
					if( hasUV )
					{
						float u0=0.0f, v0=0.0f;
						float u1=0.0f, v1=0.0f;
						float u2=0.0f, v2=0.0f;
						//vi0, vi1, vi2;// vertex index in polygon
						int vi0 = getVertexInexInPolygon( gvi[0], vertexList);
						int vi1 = getVertexInexInPolygon( gvi[1], vertexList);
						int vi2 = getVertexInexInPolygon( gvi[2], vertexList);
						IfMErrorMsgWarn(fnMesh.getPolygonUV(gpi, vi0, u0, v0, &currentUVsetName), ribNode__->name);
						IfMErrorMsgWarn(fnMesh.getPolygonUV(gpi, vi1, u1, v1, &currentUVsetName), ribNode__->name);
						IfMErrorMsgWarn(fnMesh.getPolygonUV(gpi, vi2, u2, v2, &currentUVsetName), ribNode__->name);
						UV.push_back(MVector(u0, v0));
						UV.push_back(MVector(u1, v1));
						UV.push_back(MVector(u2, v2));
					}

				}//for(int ti = 0; ti<triangleCountInPolygon; ++ti)
			}//for(int gpi = 0; gpi< numPolygons; ++gpi)
		}
		catch(std::bad_alloc& e){
			liquidMessage2(messageError, "er_writeMeshData.cpp::_write1()> bad_alloc caught: [%s]", e.what());
		}
		catch(...){
			liquidMessage2(messageError, "ERROR, er mesh write, gpi=%d, ti=%d", gpi, ti);
			CM_TRACE_FUNC("gpi="<<gpi<<" ti="<<ti);
		}
*/
}

