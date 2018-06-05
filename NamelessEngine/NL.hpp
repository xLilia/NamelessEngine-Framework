#pragma once

//INCLUDE THIS HPP FILE BEFORE USING THE ENGINE TO HAVE ACCESS
//TO EVERY IMPLEMENTED THE HEADERS

/*
MIT License

Copyright (c) 2018 [mattateusb7]Eventyon_MateusBranco

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

///CORE
//{
#include "NamelessCore.hpp"
//}

///STABLE
//{
	#include "GameObject.h"
	#include "WorldSpace.h"
	#include "CameraObj.h"
	#include "LightObject.h"
	#include "AudioSource.h"
	#include "Transform.h"
	#include "MeshRenderer.h"
	#include "ShaderInstance.h"
	#include "MeshInstance.h"
	#include "MaterialInstance.h"
	#include "TextureInstance.h"
	#include "TextureLoader.h"

	///ENGINE
	//{
	#include "GameManager.h"
	#include "CppScript.h"
	#include "NL_Time.h"
	//}

//}

///UNSTABLE
//{
	#include "ParticleSystem.h"
	#include "ParticleObj.h"
	#include "UICanvas.h"
//}

///TOOLS
//{
	#include "GLError.h"
	#include "OBJfileReader.h"
	#include "XMLfileReader.h"
	#include "ShaderWizard.h"
//}

///Deprecated
//{
	//#include "MTLfileReader.h"
//}


///FUTURE VERSIONS
//{
	///PHYSICS
	//{
		//#include "NLphysxRigidBody.h"
		//#include "NLphysxCollider.h"
	//}
//}




