#include "MaterialObj.h"
_NL::Element::MaterialObj::MaterialObj(const std::string name)
{
	this->name = name;
}

_NL::Element::MaterialObj::MaterialObj(const std::string name, const char * filePath)
{
	this->name = name;
	LoadMTLFile(filePath);
}

int _NL::Element::MaterialObj::LoadMTLFile(const char * filePath)
{
	if (MTLF.LOAD(filePath) != 0) {
		return -1;
	}
	MTLData = MTLF.MTLD;
	MTLTexIds = MTLF.TL.GLTexIDs;
	MTLF.RESET();
	return 0;
}

std::string _NL::Element::MaterialObj::ClassName() const
{
	return "_NL::Element::MaterialObj";
}


_NL::Element::MaterialObj::~MaterialObj()
{
}
