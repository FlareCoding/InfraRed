#pragma once
#include "../IComponent.h"

namespace ifr
{
	class InformationComponent : public IComponent
	{
	public:
		InformationComponent() = default;

		// Inherited via IComponent
		virtual ComponentType GetType() override;
		virtual void OnAttach() override {}
		virtual void OnUpdate() override {}

		int Param1 = 0;
		int Param2 = 0;
		int Param3 = 0;
		
		uint32_t Param4 = 0;
		uint32_t Param5 = 0;
		uint64_t Param6 = 0;

		float Param7 = 0;
		float Param8 = 0;

		int Flag1 = 0;
		int Flag2 = 0;
	};
}
