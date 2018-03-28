#include "MaterialInstance.h"

_NL::Element::MaterialInstance::MaterialInstance()
{

}

_NL::Element::MaterialInstance::MaterialInstance(const char * filePath)
{
	//LoadMTLFile(filePath);
}

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

_NL::Element::MaterialInstance::~MaterialInstance()
{
}
