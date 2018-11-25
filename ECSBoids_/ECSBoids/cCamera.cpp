// Includes
//=========
#include "cCamera.h"

#include "cCameraComponent.h"

#include <Engine/EntityComponentSystem/ComponentManager.h>

// Initialization / Clean Up
//--------------------------

eae6320::cCamera::cCamera()
{

}

void eae6320::cCamera::Initialize()
{
	AddComponent<cCameraComponent>(Math::sVector(0.0f, 2.5f, 5.0f), Math::cQuaternion());
}
