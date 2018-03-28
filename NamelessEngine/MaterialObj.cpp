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

void _NL::Element::MaterialObj::Add_NewMaterial()
{
	this->MTLData.push_back(_NL::Core::MTLdata());
}

void _NL::Element::MaterialObj::Add_NewTexture(const char* filePath, int mtlID) {
	TextureLoader aTL;
	aTL.GenerateTexure(filePath);

	if (mtlID != -1) {
		this->MTLTexIds[mtlID] = aTL.GLTexIDs[0];
		aTL.RESET();
	}
	else {
		int pos = MTLTexIds.size();
		this->MTLTexIds.push_back(MTLTexIds.size());
		this->MTLTexIds[pos] = aTL.GLTexIDs[0];
		aTL.RESET();
	}
		
}

_NL::Element::MaterialObj::~MaterialObj()
{
}
