#include "MaterialInstance.h"

_NL::Element::MaterialInstance::MaterialInstance()
{

}

char* _NL::Element::MaterialInstance::getTypeName() 
{
	return "_NL::Element::MaterialInstance";
}

void _NL::Element::MaterialInstance::LoadTexture(TEXTURE_TYPE Texture_type, _NL::Element::TextureInstance* Texture, GLuint MTL_ID)
{
	if(Texture_type == TEXTURE_TYPE::AlbedoMap)
		MaterialInstanceData.AlbedoTexId = Texture->getTextureID();
	
	if (Texture_type == TEXTURE_TYPE::RoughnessMap)
		MaterialInstanceData.RoughnessTexId = Texture->getTextureID();
	
	if (Texture_type == TEXTURE_TYPE::MetalnessMap)
		MaterialInstanceData.MetalnessTexId = Texture->getTextureID();
	
	if (Texture_type == TEXTURE_TYPE::NormalMap)
		MaterialInstanceData.NormalTexId = Texture->getTextureID();
	
	if (Texture_type == TEXTURE_TYPE::AmbientOcclusionMap)
		MaterialInstanceData.AmbientOculusionTexId = Texture->getTextureID();
}

//void _NL::Element::MaterialInstance::LoadFlipbookTextureMap(TEXTURE_TYPE Texture_type, const char * filePath, GLuint MTL_ID, GLuint Rows, GLuint Columns, GLuint* AnimationPhase)
//{
//	//WIP
//}

_NL::Element::MaterialInstance::~MaterialInstance()
{
}

