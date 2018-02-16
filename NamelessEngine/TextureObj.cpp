#include "TextureObj.h"

_NL::Element::TextureObj::TextureObj(const std::string filePath) {
	Texture.loadFromFile(filePath);
}

_NL::Element::TextureObj::~TextureObj() {

}