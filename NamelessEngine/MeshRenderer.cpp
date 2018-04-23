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

void _NL::Component::MeshRenderer::UnpackData() {
	///UNPACK VERTEX DATA
	
	for each (_NL::Core::vIndices vI in Mesh->Indices)
	{
		glm::vec3 Tangent;

		Tangent = calculateTangent(
			Mesh->MeshData.vPos[vI.v[0] - 1].Pos,
			Mesh->MeshData.vPos[vI.v[1] - 1].Pos,
			Mesh->MeshData.vPos[vI.v[2] - 1].Pos,
			Mesh->MeshData.vTexC[vI.vt[0] - 1].TexCoord,
			Mesh->MeshData.vTexC[vI.vt[1] - 1].TexCoord,
			Mesh->MeshData.vTexC[vI.vt[2] - 1].TexCoord
		);

		if (vI.MTL_ID == -1)vI.MTL_ID = 0;
		///!!!THIS CAN BE OPTIMIZED!!!\\\(not using indices) 
		//V1 ======================
		IndicesBuf.push_back(vI.v[0] - 1);
		VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[0] - 1].Pos.x);
		VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[0] - 1].Pos.y);
		VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[0] - 1].Pos.z);
		VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[0] - 1].Norm.x);
		VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[0] - 1].Norm.y);
		VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[0] - 1].Norm.z);
		VertsBuf.push_back(Tangent.x);
		VertsBuf.push_back(Tangent.y);
		VertsBuf.push_back(Tangent.z);
		VertsBuf.push_back(Mesh->MeshData.vTexC[vI.vt[0] - 1].TexCoord.s);
		VertsBuf.push_back(Mesh->MeshData.vTexC[vI.vt[0] - 1].TexCoord.t);
		//V2 ======================
		IndicesBuf.push_back(vI.v[1] - 1);
		VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[1] - 1].Pos.x);
		VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[1] - 1].Pos.y);
		VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[1] - 1].Pos.z);
		VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[1] - 1].Norm.x);
		VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[1] - 1].Norm.y);
		VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[1] - 1].Norm.z);
		VertsBuf.push_back(Tangent.x);
		VertsBuf.push_back(Tangent.y);
		VertsBuf.push_back(Tangent.z);
		VertsBuf.push_back(Mesh->MeshData.vTexC[vI.vt[1] - 1].TexCoord.s);
		VertsBuf.push_back(Mesh->MeshData.vTexC[vI.vt[1] - 1].TexCoord.t);
		//V3 ======================
		IndicesBuf.push_back(vI.v[2] - 1);
		VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[2] - 1].Pos.x);
		VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[2] - 1].Pos.y);
		VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[2] - 1].Pos.z);
		VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[2] - 1].Norm.x);
		VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[2] - 1].Norm.y);
		VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[2] - 1].Norm.z);
		VertsBuf.push_back(Tangent.x);
		VertsBuf.push_back(Tangent.y);
		VertsBuf.push_back(Tangent.z);
		VertsBuf.push_back(Mesh->MeshData.vTexC[vI.vt[2] - 1].TexCoord.s);
		VertsBuf.push_back(Mesh->MeshData.vTexC[vI.vt[2] - 1].TexCoord.t);
		//---------------------------------------------------------------------------------
	}
	
	bIsUnpacked = true;

	//VertsBuf.push_back(Material->MaterialInstanceData[vI.MTL_ID].Kd.r);
	//VertsBuf.push_back(Material->MaterialInstanceData[vI.MTL_ID].Kd.g);
	//VertsBuf.push_back(Material->MaterialInstanceData[vI.MTL_ID].Kd.b);
	//VertsBuf.push_back(Material->MaterialInstanceData[vI.MTL_ID].Kd.r);
	//VertsBuf.push_back(Material->MaterialInstanceData[vI.MTL_ID].Kd.g);
	//VertsBuf.push_back(Material->MaterialInstanceData[vI.MTL_ID].Kd.b);
	//VertsBuf.push_back(Material->MaterialInstanceData[vI.MTL_ID].Kd.r);
	//VertsBuf.push_back(Material->MaterialInstanceData[vI.MTL_ID].Kd.g);
	//VertsBuf.push_back(Material->MaterialInstanceData[vI.MTL_ID].Kd.b);

	//for each(_NL::Core::VertexPos P in Mesh->MeshData.vPos) {
	//	VertsBuf.push_back(P.Pos.x);
	//	VertsBuf.push_back(P.Pos.y);
	//	VertsBuf.push_back(P.Pos.z);
	//}
	//for each(_NL::Core::VertexCol C in Mesh->MeshData.vCol) {
	//	VertsBuf.push_back(C.Col.r);
	//	VertsBuf.push_back(C.Col.g);
	//	VertsBuf.push_back(C.Col.b);
	//}
	//for each(_NL::Core::VertexNorm N in Mesh->MeshData.vNorm) {
	//	VertsBuf.push_back(N.Norm.x);
	//	VertsBuf.push_back(N.Norm.y);
	//	VertsBuf.push_back(N.Norm.z);
	//}
	//for each(_NL::Core::VertexTexCoord T in Mesh->MeshData.vTexC) {
	//	VertsBuf.push_back(T.TexCoord.s);
	//	VertsBuf.push_back(T.TexCoord.t);
	//}

	///UNPACK INDEX DATA

	//for each (_NL::Core::vIndices I in Mesh->Indices)
	//{
	//	
	//}

}

void _NL::Component::MeshRenderer::initGLObj()
{
	UnpackData();

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
	//glVertexArrayElementBuffer(vao, ebo);

	check_gl_error();
}

std::string _NL::Component::MeshRenderer::ClassName() const
{
	return "_NL::Component::MeshRenderer";
}

_NL::Component::MeshRenderer::~MeshRenderer()
{

}