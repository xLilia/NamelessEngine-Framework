#include "MaterialObj.h"

_NL::Element::MaterialObj::MaterialObj()
{
}

_NL::Element::MaterialObj::MaterialObj(const char * filePath)
{
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
