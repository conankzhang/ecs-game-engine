/*
	Effect Component interface.
*/

#ifndef CEFFECTCOMPONENT_H
#define CEFFECTCOMPONENT_H

// Includes
//=========
#include <Engine/EntityComponentSystem/ECS.h>

// Forward Declarations
//=====================

namespace eae6320
{
	namespace Graphics
	{
		class cEffect;
	}
}

// Class Declaration
//==================
namespace eae6320
{
	class cEffectComponent : public eae6320::ECS::Component<cEffectComponent>
	{
	public:
		cEffectComponent(const char* const i_effectPath);
		~cEffectComponent();

		eae6320::Graphics::cEffect* GetEffect();
		void CleanUp();

	protected:
		eae6320::Graphics::cEffect* m_effect;
	};
}

#endif // EFFECTCOMPONENT_H
