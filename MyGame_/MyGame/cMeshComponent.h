/*
	Mesh Component interface.
*/

#ifndef CMESHCOMPONENT_H
#define CMESHCOMPONENT_H

// Includes
//=========
#include <Engine/EntityComponentSystem/ECS.h>

// Forward Declarations
//=====================

namespace eae6320
{
	namespace Graphics
	{
		class cMesh;
	}
}

// Class Declaration
//==================
namespace eae6320
{
	class cMeshComponent : public eae6320::ECS::Component<cMeshComponent>
	{
	public:
		cMeshComponent(const char* const i_meshPath);
		~cMeshComponent();

		eae6320::Graphics::cMesh* GetMesh();
		void CleanUp();

	protected:
		eae6320::Graphics::cMesh* m_mesh;
	};
}

#endif // CMESHCOMPONENT_H
