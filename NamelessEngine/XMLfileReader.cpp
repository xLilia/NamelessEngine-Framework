#include "XMLfileReader.h"
#include "XMLfileReader.h"

XMLfileReader::XMLfileReader()
{
}

XMLfileReader::XMLfileReader(const const char* File_Path)
{
	Load(File_Path);
}

int XMLfileReader::Load(const char* File_Path, bool Full_headers, bool debugMode) {
	

	std::ifstream f(File_Path);
	std::string strf;

	f.seekg(0, std::ios::out);
	strf.reserve(f.tellg());
	f.seekg(0, std::ios::beg);

	strf.assign(
		std::istreambuf_iterator<char>(f),
		std::istreambuf_iterator<char>()
	);

	f.close();

	size_t TypeSnipBEG = -1;
	size_t TypeSnipOUT = -1;
	size_t DataSnipBEG = -1;
	size_t DataSnipOUT = -1;
	std::string LastHeader;

	for (unsigned int cPos = 0; cPos < strf.length(); cPos++)
	{

		if (DataSnipBEG != -1 && strf[cPos] == '<' && strf[cPos+1] == '/') { //END "DATA capture"
		
			DataSnipOUT = cPos;

			std::string D = strf.substr(DataSnipBEG+1, DataSnipOUT - DataSnipBEG - 1);

			if (D[0] == '\n') {
				D = "NULL_DATA";
			}

			if(debugMode) std::cout << D << std::endl << std::endl;
		
			XMLData.push_back(D);
			XMLHeaders.push_back(LastHeader);
			LastHeader.clear();

			DataSnipBEG = -1;
			DataSnipOUT = -1;
			continue;
		}
		
		if (strf[cPos] == '<' && strf[cPos+1] != '/') { //START "header capture"
			TypeSnipBEG = cPos;
		}

		if (TypeSnipBEG != -1 && strf[cPos] == '>') { //END "header capture"
			TypeSnipOUT = cPos;

			std::string T = strf.substr(TypeSnipBEG, TypeSnipOUT - TypeSnipBEG + 1);

			if (debugMode) std::cout << T << std::endl;
			
			DataSnipBEG = TypeSnipOUT; //START "DATA capture"

			if (!Full_headers) {
				LastHeader = T;
			}
			else {
				LastHeader += T;
			}
	
			TypeSnipBEG = -1;
			TypeSnipOUT = -1;
			continue;
		}
	}
	return 0;
}
