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

	//VBuffer: |Pos:v3|Norm:v3|Tangent:v3|Map:v2|
	
	VertsBuf.resize(Mesh->MeshData.mesh_positions_array.size() * 11);
	glm::vec3 LastTriPos[3];
	glm::vec2 LastTriMap[3];
	GLuint LastTriVbufferLoc[3];
	GLuint TangentCount = 0;
	GLuint It = 0;
	for each (auto I in Mesh->MeshData.mesh_triangles_array)
	{
		GLuint V = I.x * 11;
		IndicesBuf.push_back(I.x);
		VertsBuf[V]=Mesh->MeshData.mesh_positions_array[I.x].x;
		VertsBuf[V+1]=Mesh->MeshData.mesh_positions_array[I.x].y;
		VertsBuf[V+2]=Mesh->MeshData.mesh_positions_array[I.x].z;
		VertsBuf[V+3]=Mesh->MeshData.mesh_normals_array[I.y].x;
		VertsBuf[V+4]=Mesh->MeshData.mesh_normals_array[I.y].y;
		VertsBuf[V+5]=Mesh->MeshData.mesh_normals_array[I.y].z;
		
		//VertsBuf[V+6]= //TangentX
		//VertsBuf[V+7]= //TangentY
		//VertsBuf[V+8]= //TangentZ

		VertsBuf[V + 9] = Mesh->MeshData.mesh_map_array[I.z].x;
		VertsBuf[V + 10] = Mesh->MeshData.mesh_map_array[I.z].y;
		
		glm::vec3 Tangent;

		switch (TangentCount)
		{
		case 0:
			LastTriPos[0] = glm::vec3(VertsBuf[V], VertsBuf[V+1], VertsBuf[V+2]);
			LastTriMap[0] = glm::vec2(VertsBuf[V + 9], VertsBuf[V + 10]);
			LastTriVbufferLoc[0] = V;
			break;
		case 1:
			LastTriPos[1] = glm::vec3(VertsBuf[V], VertsBuf[V + 1], VertsBuf[V + 2]);
			LastTriMap[1] = glm::vec2(VertsBuf[V + 9], VertsBuf[V + 10]);
			LastTriVbufferLoc[1] = V;
			break;
		case 2: 
			LastTriPos[2] = glm::vec3(VertsBuf[V], VertsBuf[V + 1], VertsBuf[V + 2]);
			LastTriMap[2] = glm::vec2(VertsBuf[V + 9], VertsBuf[V + 10]);
			LastTriVbufferLoc[2] = V;
			
			Tangent = calculateTangent(
				LastTriPos[0],
				LastTriPos[1],
				LastTriPos[2],
				LastTriMap[0],
				LastTriMap[1],
				LastTriMap[2]
			);

			VertsBuf[LastTriVbufferLoc[0] + 6] = Tangent.x; //TangentX1
			VertsBuf[LastTriVbufferLoc[0] + 7] = Tangent.y; //TangentY1
			VertsBuf[LastTriVbufferLoc[0] + 8] = Tangent.z; //TangentZ1
					
			VertsBuf[LastTriVbufferLoc[1] + 6] = Tangent.x; //TangentX2
			VertsBuf[LastTriVbufferLoc[1] + 7] = Tangent.y; //TangentY2
			VertsBuf[LastTriVbufferLoc[1] + 8] = Tangent.z; //TangentZ2
				
			VertsBuf[LastTriVbufferLoc[2] + 6] = Tangent.x; //TangentX3
			VertsBuf[LastTriVbufferLoc[2] + 7] = Tangent.y; //TangentY3
			VertsBuf[LastTriVbufferLoc[2] + 8] = Tangent.z; //TangentZ3
		//	break;
		///*case QUAD*/ case 3:
			TangentCount = -1;
			break;
		}
		TangentCount++;
		}
	
	//for each (_NL::Core::vIndices vI in Mesh->Indices)
	//{
	//	glm::vec3 Tangent;
	//
	//	Tangent = calculateTangent(
	//		Mesh->MeshData.vPos[vI.v[0] - 1].Pos,
	//		Mesh->MeshData.vPos[vI.v[1] - 1].Pos,
	//		Mesh->MeshData.vPos[vI.v[2] - 1].Pos,
	//		Mesh->MeshData.vTexC[vI.vt[0] - 1].TexCoord,
	//		Mesh->MeshData.vTexC[vI.vt[1] - 1].TexCoord,
	//		Mesh->MeshData.vTexC[vI.vt[2] - 1].TexCoord
	//	);
	//
	//	if (vI.MTL_ID == -1)vI.MTL_ID = 0;
	//	///!!!THIS CAN BE OPTIMIZED!!!\\\(not using indices) 
	//	//V1 ======================
	//	IndicesBuf.push_back(vI.v[0] - 1);
	//	VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[0] - 1].Pos.x);
	//	VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[0] - 1].Pos.y);
	//	VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[0] - 1].Pos.z);
	//	VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[0] - 1].Norm.x);
	//	VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[0] - 1].Norm.y);
	//	VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[0] - 1].Norm.z);
	//	VertsBuf.push_back(Tangent.x);
	//	VertsBuf.push_back(Tangent.y);
	//	VertsBuf.push_back(Tangent.z);
	//	VertsBuf.push_back(Mesh->MeshData.vTexC[vI.vt[0] - 1].TexCoord.s);
	//	VertsBuf.push_back(Mesh->MeshData.vTexC[vI.vt[0] - 1].TexCoord.t);
	//	//V2 ======================
	//	IndicesBuf.push_back(vI.v[1] - 1);
	//	VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[1] - 1].Pos.x);
	//	VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[1] - 1].Pos.y);
	//	VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[1] - 1].Pos.z);
	//	VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[1] - 1].Norm.x);
	//	VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[1] - 1].Norm.y);
	//	VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[1] - 1].Norm.z);
	//	VertsBuf.push_back(Tangent.x);
	//	VertsBuf.push_back(Tangent.y);
	//	VertsBuf.push_back(Tangent.z);
	//	VertsBuf.push_back(Mesh->MeshData.vTexC[vI.vt[1] - 1].TexCoord.s);
	//	VertsBuf.push_back(Mesh->MeshData.vTexC[vI.vt[1] - 1].TexCoord.t);
	//	//V3 ======================
	//	IndicesBuf.push_back(vI.v[2] - 1);
	//	VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[2] - 1].Pos.x);
	//	VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[2] - 1].Pos.y);
	//	VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[2] - 1].Pos.z);
	//	VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[2] - 1].Norm.x);
	//	VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[2] - 1].Norm.y);
	//	VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[2] - 1].Norm.z);
	//	VertsBuf.push_back(Tangent.x);
	//	VertsBuf.push_back(Tangent.y);
	//	VertsBuf.push_back(Tangent.z);
	//	VertsBuf.push_back(Mesh->MeshData.vTexC[vI.vt[2] - 1].TexCoord.s);
	//	VertsBuf.push_back(Mesh->MeshData.vTexC[vI.vt[2] - 1].TexCoord.t);
	//	//---------------------------------------------------------------------------------
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
	glCreateBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*IndicesBuf.size(), &IndicesBuf[0], GL_STATIC_DRAW);

	check_gl_error();

	///GL BUFFERS && ARRAYS
	///Init Vertex Arrays
	glCreateVertexArrays(1, &vao);
	glEnableVertexArrayAttrib(vao, _NL::Core::Pos_atrib);
	glEnableVertexArrayAttrib(vao, _NL::Core::Norm_atrib);
	glEnableVertexArrayAttrib(vao, _NL::Core::Tangent_atrib);
	glEnableVertexArrayAttrib(vao, _NL::Core::TexC_atrib);
	check_gl_error();
	
	//Set Vertex Arrays Format

	GLsizei foatSize = sizeof(GLfloat);
	GLsizei lastvaoAtrib = (foatSize * 11);
	
	glVertexArrayAttribBinding(vao, _NL::Core::Pos_atrib, 0);
	glVertexArrayAttribFormat(vao, _NL::Core::Pos_atrib, 3, GL_FLOAT, GL_FALSE, foatSize * 0);

	glVertexArrayAttribBinding(vao, _NL::Core::Norm_atrib, 0);
	glVertexArrayAttribFormat(vao, _NL::Core::Norm_atrib, 3, GL_FLOAT, GL_FALSE, foatSize * 3);

	glVertexArrayAttribBinding(vao, _NL::Core::Tangent_atrib, 0);
	glVertexArrayAttribFormat(vao, _NL::Core::Tangent_atrib, 3, GL_FLOAT, GL_FALSE, foatSize * 6);
	
	glVertexArrayAttribBinding(vao, _NL::Core::TexC_atrib, 0);
	glVertexArrayAttribFormat(vao, _NL::Core::TexC_atrib, 2, GL_FLOAT, GL_FALSE, foatSize * 9);

	check_gl_error();

	///Confiugure Vertex Array and link Buffers
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, lastvaoAtrib);
	glVertexArrayElementBuffer(vao, ebo);

	check_gl_error();
}

char* _NL::Component::MeshRenderer::ClassName() const
{
	return "_NL::Component::MeshRenderer";
}

_NL::Component::MeshRenderer::~MeshRenderer()
{

}