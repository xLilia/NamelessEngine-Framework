#include "MeshRenderer.h"

_NL::Component::MeshRenderer::MeshRenderer()
{

}

void _NL::Component::MeshRenderer::UnpackData() {
	///UNPACK VERTEX DATA
	
	VertsBuf.clear();
	IndicesBuf.clear();
	
	for each (_NL::Core::vIndices vI in Mesh->Indices)
	{
		if (vI.MTL_ID == -1)vI.MTL_ID = 0;
		///!!!THIS CAN BE OPTIMIZED!!!\\\ 
		IndicesBuf.push_back(vI.v[0] - 1);
		VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[0] - 1].Pos.x);
		VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[0] - 1].Pos.y);
		VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[0] - 1].Pos.z);
		VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[0] - 1].Norm.x);
		VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[0] - 1].Norm.y);
		VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[0] - 1].Norm.z);
		VertsBuf.push_back(Material->MTLData[vI.MTL_ID].Kd.r);
		VertsBuf.push_back(Material->MTLData[vI.MTL_ID].Kd.g);
		VertsBuf.push_back(Material->MTLData[vI.MTL_ID].Kd.b);
		VertsBuf.push_back(Mesh->MeshData.vTexC[vI.vt[0] - 1].TexCoord.s);
		VertsBuf.push_back(Mesh->MeshData.vTexC[vI.vt[0] - 1].TexCoord.t);
		IndicesBuf.push_back(vI.v[1] - 1);
		VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[1] - 1].Pos.x);
		VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[1] - 1].Pos.y);
		VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[1] - 1].Pos.z);
		VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[1] - 1].Norm.x);
		VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[1] - 1].Norm.y);
		VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[1] - 1].Norm.z);
		VertsBuf.push_back(Material->MTLData[vI.MTL_ID].Kd.r);
		VertsBuf.push_back(Material->MTLData[vI.MTL_ID].Kd.g);
		VertsBuf.push_back(Material->MTLData[vI.MTL_ID].Kd.b);
		VertsBuf.push_back(Mesh->MeshData.vTexC[vI.vt[1] - 1].TexCoord.s);
		VertsBuf.push_back(Mesh->MeshData.vTexC[vI.vt[1] - 1].TexCoord.t);
		IndicesBuf.push_back(vI.v[2] - 1);
		VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[2] - 1].Pos.x);
		VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[2] - 1].Pos.y);
		VertsBuf.push_back(Mesh->MeshData.vPos[vI.v[2] - 1].Pos.z);
		VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[2] - 1].Norm.x);
		VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[2] - 1].Norm.y);
		VertsBuf.push_back(Mesh->MeshData.vNorm[vI.vn[2] - 1].Norm.z);
		VertsBuf.push_back(Material->MTLData[vI.MTL_ID].Kd.r);
		VertsBuf.push_back(Material->MTLData[vI.MTL_ID].Kd.g);
		VertsBuf.push_back(Material->MTLData[vI.MTL_ID].Kd.b);
		VertsBuf.push_back(Mesh->MeshData.vTexC[vI.vt[2] - 1].TexCoord.s);
		VertsBuf.push_back(Mesh->MeshData.vTexC[vI.vt[2] - 1].TexCoord.t);
	}
	
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

	check_gl_error_full();

	///STATIC ELEMENT BUFFER OBJ 
	//glCreateBuffers(1, &ebo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*IndicesBuf.size(), &IndicesBuf[0], GL_STATIC_DRAW);

	check_gl_error_full();

	///GL BUFFERS && ARRAYS
	///Init Vertex Arrays
	glCreateVertexArrays(1, &vao);
	glEnableVertexArrayAttrib(vao, Pos_atrib);
	glEnableVertexArrayAttrib(vao, Norm_atrib);
	glEnableVertexArrayAttrib(vao, Col_atrib);
	glEnableVertexArrayAttrib(vao, TexC_atrib);
	check_gl_error_full();
	
	///Set Vertex Arrays Format
	glVertexArrayAttribBinding(vao, Pos_atrib, 0);
	glVertexArrayAttribFormat(vao, Pos_atrib, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 0);

	glVertexArrayAttribBinding(vao, Norm_atrib, 0);
	glVertexArrayAttribFormat(vao, Norm_atrib, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3);

	glVertexArrayAttribBinding(vao, Col_atrib, 0);
	glVertexArrayAttribFormat(vao, Col_atrib, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6);
	
	glVertexArrayAttribBinding(vao, TexC_atrib, 0);
	glVertexArrayAttribFormat(vao, TexC_atrib, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9);

	check_gl_error_full();

	///Confiugure Vertex Array and link Buffers
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(GLfloat) * 11); //11
	//glVertexArrayElementBuffer(vao, ebo);

	check_gl_error_full();
}

std::string _NL::Component::MeshRenderer::ClassName() const
{
	return "_NL::Component::MeshRenderer";
}

_NL::Component::MeshRenderer::~MeshRenderer()
{

}