#include "MeshRenderer.h"

_NL::Component::MeshRenderer::MeshRenderer()
{

}

glm::vec3 calculateTangent(glm::vec3 pos0, glm::vec3 pos1, glm::vec3 pos2, glm::vec2 UV0, glm::vec2 UV1, glm::vec2 UV2) {
	//TANGENT
	glm::vec3 tangent;

	glm::vec3 edge1 = pos1 - pos0;
	glm::vec3 edge2 = pos2 - pos0;
	glm::vec2 deltaUV1 = UV1 - UV0;
	glm::vec2 deltaUV2 = UV2 - UV0;

	GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	return glm::normalize(tangent);

	//bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	//bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	//bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	//bitangent = glm::normalize(bitangent);

	//average the vertex properties like normals and tangents/bitangents
	//for each vertex to get a more smooth result. 
	
}

void _NL::Component::MeshRenderer::UpdateGLSettings()
{
	if (GL_CullFace) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_CullFaceMode);
		glFrontFace(GL_CullFaceOrientation);
	}
	else {
		glDisable(GL_CULL_FACE);
	}
}

void _NL::Component::MeshRenderer::UnpackObjData() {
	///UNPACK VERTEX DATA

	//VertsBuf.resize(Mesh->MeshData.mesh_positions_array.size() * 11);
	//glm::vec3 LastTriPos[3];
	//glm::vec2 LastTriMap[3];
	//GLuint LastTriVbufferLoc[3];
	//GLuint TangentCount = 0;
	//GLuint It = 0;
	//for each (glm::vec3 I in Mesh->MeshData.mesh_triangles_array)
	//{
	//	GLuint V = It;
	//	IndicesBuf.push_back(I.x);
	//	VertsBuf[V] = Mesh->MeshData.mesh_positions_array[I.x].x;
	//	VertsBuf[V + 1] = Mesh->MeshData.mesh_positions_array[I.x].y;
	//	VertsBuf[V + 2] = Mesh->MeshData.mesh_positions_array[I.x].z;
	//	VertsBuf[V + 3] = Mesh->MeshData.mesh_normals_array[I.y].x;
	//	VertsBuf[V + 4] = Mesh->MeshData.mesh_normals_array[I.y].y;
	//	VertsBuf[V + 5] = Mesh->MeshData.mesh_normals_array[I.y].z;
	//
	//	//VertsBuf[V+6]= //TangentX
	//	//VertsBuf[V+7]= //TangentY
	//	//VertsBuf[V+8]= //TangentZ
	//
	//	VertsBuf[V + 9] = Mesh->MeshData.mesh_map_array[I.z].x;
	//	VertsBuf[V + 10] = Mesh->MeshData.mesh_map_array[I.z].y;
	//
	//	It += 11;
	//
	//	glm::vec3 Tangent;
	//
	//	switch (TangentCount)
	//	{
	//	case 0:
	//		LastTriPos[0] = glm::vec3(VertsBuf[V], VertsBuf[V + 1], VertsBuf[V + 2]);
	//		LastTriMap[0] = glm::vec2(VertsBuf[V + 9], VertsBuf[V + 10]);
	//		LastTriVbufferLoc[0] = V;
	//		break;
	//	case 1:
	//		LastTriPos[1] = glm::vec3(VertsBuf[V], VertsBuf[V + 1], VertsBuf[V + 2]);
	//		LastTriMap[1] = glm::vec2(VertsBuf[V + 9], VertsBuf[V + 10]);
	//		LastTriVbufferLoc[1] = V;
	//		break;
	//	case 2:
	//		LastTriPos[2] = glm::vec3(VertsBuf[V], VertsBuf[V + 1], VertsBuf[V + 2]);
	//		LastTriMap[2] = glm::vec2(VertsBuf[V + 9], VertsBuf[V + 10]);
	//		LastTriVbufferLoc[2] = V;
	//
	//		Tangent = calculateTangent(
	//			LastTriPos[0],
	//			LastTriPos[1],
	//			LastTriPos[2],
	//			LastTriMap[0],
	//			LastTriMap[1],
	//			LastTriMap[2]
	//		);
	//
	//		VertsBuf[LastTriVbufferLoc[0] + 6] = Tangent.x; //TangentX1
	//		VertsBuf[LastTriVbufferLoc[0] + 7] = Tangent.y; //TangentY1
	//		VertsBuf[LastTriVbufferLoc[0] + 8] = Tangent.z; //TangentZ1
	//
	//		VertsBuf[LastTriVbufferLoc[1] + 6] = Tangent.x; //TangentX2
	//		VertsBuf[LastTriVbufferLoc[1] + 7] = Tangent.y; //TangentY2
	//		VertsBuf[LastTriVbufferLoc[1] + 8] = Tangent.z; //TangentZ2
	//
	//		VertsBuf[LastTriVbufferLoc[2] + 6] = Tangent.x; //TangentX3
	//		VertsBuf[LastTriVbufferLoc[2] + 7] = Tangent.y; //TangentY3
	//		VertsBuf[LastTriVbufferLoc[2] + 8] = Tangent.z; //TangentZ3
	//														//	break;
	//														///*case QUAD*/ case 3:
	//		TangentCount = -1;
	//		break;
	//	}
	//	TangentCount++;
	//}

	//VBuffer: |Pos:v3|Norm:v3|Tangent:v3|Map:v2|

	for (GLuint I = 0; I < Mesh->MeshData.mesh_triangles_array.size(); I += 3) {

		glm::vec3 Iv1 = Mesh->MeshData.mesh_triangles_array[I+0];
		glm::vec3 Iv2 = Mesh->MeshData.mesh_triangles_array[I+1];
		glm::vec3 Iv3 = Mesh->MeshData.mesh_triangles_array[I+2];

		glm::vec3 Tangent;

		Tangent = calculateTangent(
			Mesh->MeshData.mesh_positions_array[Iv1.x],
			Mesh->MeshData.mesh_positions_array[Iv2.x],
			Mesh->MeshData.mesh_positions_array[Iv3.x],
			Mesh->MeshData.mesh_map_array[Iv1.z],
			Mesh->MeshData.mesh_map_array[Iv2.z],
			Mesh->MeshData.mesh_map_array[Iv3.z]
		);
		
		//V1 ======================

		VertsBuf.push_back(Mesh->MeshData.mesh_positions_array[Iv1.x].x);
		VertsBuf.push_back(Mesh->MeshData.mesh_positions_array[Iv1.x].y);
		VertsBuf.push_back(Mesh->MeshData.mesh_positions_array[Iv1.x].z);
		VertsBuf.push_back(Mesh->MeshData.mesh_normals_array[Iv1.y].x);
		VertsBuf.push_back(Mesh->MeshData.mesh_normals_array[Iv1.y].y);
		VertsBuf.push_back(Mesh->MeshData.mesh_normals_array[Iv1.y].z);
		VertsBuf.push_back(Tangent.x);
		VertsBuf.push_back(Tangent.y);
		VertsBuf.push_back(Tangent.z);
		VertsBuf.push_back(Mesh->MeshData.mesh_map_array[Iv1.z].s);
		VertsBuf.push_back(Mesh->MeshData.mesh_map_array[Iv1.z].t);

		//V2 ======================

		VertsBuf.push_back(Mesh->MeshData.mesh_positions_array[Iv2.x].x);
		VertsBuf.push_back(Mesh->MeshData.mesh_positions_array[Iv2.x].y);
		VertsBuf.push_back(Mesh->MeshData.mesh_positions_array[Iv2.x].z);
		VertsBuf.push_back(Mesh->MeshData.mesh_normals_array[Iv2.y].x);
		VertsBuf.push_back(Mesh->MeshData.mesh_normals_array[Iv2.y].y);
		VertsBuf.push_back(Mesh->MeshData.mesh_normals_array[Iv2.y].z);
		VertsBuf.push_back(Tangent.x);
		VertsBuf.push_back(Tangent.y);
		VertsBuf.push_back(Tangent.z);
		VertsBuf.push_back(Mesh->MeshData.mesh_map_array[Iv2.z].s);
		VertsBuf.push_back(Mesh->MeshData.mesh_map_array[Iv2.z].t);

		//V3 ======================

		VertsBuf.push_back(Mesh->MeshData.mesh_positions_array[Iv3.x].x);
		VertsBuf.push_back(Mesh->MeshData.mesh_positions_array[Iv3.x].y);
		VertsBuf.push_back(Mesh->MeshData.mesh_positions_array[Iv3.x].z);
		VertsBuf.push_back(Mesh->MeshData.mesh_normals_array[Iv3.y].x);
		VertsBuf.push_back(Mesh->MeshData.mesh_normals_array[Iv3.y].y);
		VertsBuf.push_back(Mesh->MeshData.mesh_normals_array[Iv3.y].z);
		VertsBuf.push_back(Tangent.x);
		VertsBuf.push_back(Tangent.y);
		VertsBuf.push_back(Tangent.z);
		VertsBuf.push_back(Mesh->MeshData.mesh_map_array[Iv3.z].s);
		VertsBuf.push_back(Mesh->MeshData.mesh_map_array[Iv3.z].t);

	}
	
	//MESH VERTEX SHARE OPTIMIZATION ALGORITHM

	//for (GLuint I = 0; I < Mesh->MeshData.mesh_positions_array.size(); I++) {
	//	GLfloat VertexSample[11];
	//	GLfloat VertexCompare[11];
	//	for (GLuint i = 0; i < 11; i++) {
	//		VertexSample[i] = VertsBuf[I*11 + i];
	//	}
	//	GLuint Vcount = 0;
	//	GLuint VI = 0;
	//	for (GLuint N = 0; N < VertsBuf.size(); N++) {
	//		Vcount = N % 11;
	//		VertexCompare[Vcount] = VertsBuf[N];
	//		if (Vcount == 0 && I * 11 != N) {
	//			bool bSame = true;
	//			for (GLuint k = 0; k < 11; k++) {
	//				if (VertexCompare[k] != VertexSample[k]) {
	//					bSame = false;
	//				}
	//			}
	//			if (bSame) {
	//				if (VI != I) {
	//					int a = 1;
	//				}
	//			}
	//			VI++;
	//		}
	//	}
	//}
	
	bIsUnpacked = true;
}

void _NL::Component::MeshRenderer::initGLObj()
{
	UnpackObjData();

	///STATIC VERTEX BUFFER OBJ
	glCreateBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*VertsBuf.size(), &VertsBuf[0], GL_STATIC_DRAW);
	
	check_gl_error();

	///STATIC ELEMENT BUFFER OBJ 
	//glCreateBuffers(1, &ebo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*IndicesBuf.size(), &IndicesBuf[0], GL_STATIC_DRAW);

	check_gl_error();

	///GL BUFFERS && ARRAYS
	///Init Vertex Arrays
	glCreateVertexArrays(1, &vao);
	glEnableVertexArrayAttrib(vao, _NL::Core::GLSL_AU::Pos_atrib);
	glEnableVertexArrayAttrib(vao, _NL::Core::GLSL_AU::Norm_atrib);
	glEnableVertexArrayAttrib(vao, _NL::Core::GLSL_AU::Tangent_atrib);
	glEnableVertexArrayAttrib(vao, _NL::Core::GLSL_AU::TexC_atrib);
	check_gl_error();
	
	//Set Vertex Arrays Format

	GLsizei foatSize = sizeof(GLfloat);
	GLsizei lastvaoAtrib = (foatSize * 11);
	
	glVertexArrayAttribBinding(vao, _NL::Core::GLSL_AU::Pos_atrib, 0);
	glVertexArrayAttribFormat(vao, _NL::Core::GLSL_AU::Pos_atrib, 3, GL_FLOAT, GL_FALSE, foatSize * 0);

	glVertexArrayAttribBinding(vao, _NL::Core::GLSL_AU::Norm_atrib, 0);
	glVertexArrayAttribFormat(vao, _NL::Core::GLSL_AU::Norm_atrib, 3, GL_FLOAT, GL_FALSE, foatSize * 3);

	glVertexArrayAttribBinding(vao, _NL::Core::GLSL_AU::Tangent_atrib, 0);
	glVertexArrayAttribFormat(vao, _NL::Core::GLSL_AU::Tangent_atrib, 3, GL_FLOAT, GL_FALSE, foatSize * 6);
	
	glVertexArrayAttribBinding(vao, _NL::Core::GLSL_AU::TexC_atrib, 0);
	glVertexArrayAttribFormat(vao, _NL::Core::GLSL_AU::TexC_atrib, 2, GL_FLOAT, GL_FALSE, foatSize * 9);

	check_gl_error();

	///Confiugure Vertex Array and link Buffers
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, lastvaoAtrib);
	//glVertexArrayElementBuffer(vao, ebo);

	check_gl_error();
}

char* _NL::Component::MeshRenderer::getTypeName() 
{
	return "_NL::Component::MeshRenderer";
}

_NL::Component::MeshRenderer::~MeshRenderer()
{

}