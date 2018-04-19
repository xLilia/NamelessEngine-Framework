#include "MaterialInstance.h"

_NL::Element::MaterialInstance::MaterialInstance()
{

}

//_NL::Element::MaterialInstance::MaterialInstance(const char * filePath)
//{
//	//LoadMTLFile(filePath);
//}

//int _NL::Element::MaterialInstance::LoadMTLFile(const char * filePath)
//{
//	if (MTLF.LOAD(filePath) != 0) {
//		return -1;
//	}
//	MaterialInstanceData = MTLF.MTLD;
//	MTLTexIds = MTLF.TL.GLTexIDs;
//	MTLF.RESET();
//	return 0;
//}

std::string _NL::Element::MaterialInstance::ClassName() const
{
	return "_NL::Element::MaterialInstance";
}

void _NL::Element::MaterialInstance::AddNew_Material()
{
	this->MaterialInstanceData.push_back(_NL::Core::MaterialInstanceData());
}

void _NL::Element::MaterialInstance::LoadTextureMap(TEXTURE_TYPE Texture_type, const char * filePath, GLuint MTL_ID)
{
	if(Texture_type == TEXTURE_TYPE::AlbedoMap)
		MaterialInstanceData[MTL_ID].AlbedoTexId = TL.GenerateTexure(filePath);
	
	if (Texture_type == TEXTURE_TYPE::RoughnessMap)
		MaterialInstanceData[MTL_ID].RoughnessTexId = TL.GenerateTexure(filePath);
	
	if (Texture_type == TEXTURE_TYPE::MetalnessMap)
		MaterialInstanceData[MTL_ID].MetalnessTexId = TL.GenerateTexure(filePath);
	
	if (Texture_type == TEXTURE_TYPE::NormalMap)
		MaterialInstanceData[MTL_ID].NormalTexId = TL.GenerateTexure(filePath);
	
	if (Texture_type == TEXTURE_TYPE::AmbientOcclusionMap)
		MaterialInstanceData[MTL_ID].AmbientOculusionTexId = TL.GenerateTexure(filePath);
}

void _NL::Element::MaterialInstance::LoadFlipbookTextureMap(TEXTURE_TYPE Texture_type, const char * filePath, GLuint MTL_ID, GLuint Rows, GLuint Columns, GLuint* AnimationPhase)
{
	//WIP
}

_NL::Element::MaterialInstance::~MaterialInstance()
{
}

