#pragma once
#include "NamelessCore.h"
#include "ShaderObj.h"
#include "MeshObj.h"


namespace _NL {
	namespace Component {
		class MeshRenderer : public _NL::Core::Component
		{
		public:
			MeshRenderer();
			~MeshRenderer();

			_NL::Element::MeshObj* Mesh;
			_NL::Element::ShaderObj* Shader;
			///INFO
			std::string ClassName() const override;
		private:
			void RenderObj();
		};
	}
}

