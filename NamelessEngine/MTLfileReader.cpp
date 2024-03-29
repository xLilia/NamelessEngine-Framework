#include "MTLfileReader.h"

_NL::Tools::MTLfileReader::MTLfileReader()
{
}

void _NL::Tools::MTLfileReader::RESET() {
	this->MTLD.clear();
	this->MTLD.shrink_to_fit();
	this->inMTLfile_counter = -1;
	TL.RESET();
}

int _NL::Tools::MTLfileReader::LOAD(const char * OBJpath) {
	FILE * file = std::fopen(OBJpath, "r");
	if (!file) {
		std::cout << "ERROR: FAILED TO LOAD: " << OBJpath << std::endl;
		return -1;
	}
	for (;;) {
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);

		if (res == EOF)
			break;

		if (std::strcmp(lineHeader, "newmtl") == 0) {
			_NL::Core::MaterialInstanceData md;
			inMTLfile_counter++;
			md.MTL_ID = inMTLfile_counter;
			MTLD.push_back(md);
		}
		else if (std::strcmp(lineHeader, "Ns") == 0) {
			//fscanf(file, "%f\n",
			//	&MTLD[inMTLfile_counter].Ns
			//);
		}
		else if (std::strcmp(lineHeader, "Ka") == 0) {
			//fscanf(file, "%f %f %f\n",
			//	&MTLD[inMTLfile_counter].Ka.x,
			//	&MTLD[inMTLfile_counter].Ka.y,
			//	&MTLD[inMTLfile_counter].Ka.z
			//);
		}
		else if (std::strcmp(lineHeader, "Kd") == 0) {
			//fscanf(file, "%f %f %f\n",
			//	&MTLD[inMTLfile_counter].Kd.x,
			//	&MTLD[inMTLfile_counter].Kd.y,
			//	&MTLD[inMTLfile_counter].Kd.z
			//);
		}
		else if (std::strcmp(lineHeader, "Ks") == 0) {
			//fscanf(file, "%f %f %f\n",
			//	&MTLD[inMTLfile_counter].Ks.x,
			//	&MTLD[inMTLfile_counter].Ks.y,
			//	&MTLD[inMTLfile_counter].Ks.z
			//);
		}
		else if (std::strcmp(lineHeader, "Ke") == 0) {
			//fscanf(file, "%f %f %f\n",
			//	&MTLD[inMTLfile_counter].Ke.x,
			//	&MTLD[inMTLfile_counter].Ke.y,
			//	&MTLD[inMTLfile_counter].Ke.z
			//);
		}
		else if (std::strcmp(lineHeader, "map_Kd") == 0) {
			std::string texName;
			fscanf(file, "%s\n",
				texName.c_str()
			);
			TL.GenerateTexure(texName.c_str(),true);
		}
	}
	std::fclose(file);
}


_NL::Tools::MTLfileReader::~MTLfileReader()
{
}
