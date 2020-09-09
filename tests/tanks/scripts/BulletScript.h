#pragma once
#include <InfraRed.h>
using namespace ifr;

class BulletScript : public UserScript
{
public:
	BulletScript(Entity ID, Ref<Scene> scene);

	double BulletLifetime = 2;

	// Inherited via UserScript
	virtual void OnUpdate() override;

private:
	Entity m_EID;
	Ref<Scene> m_Scene;

	double SpawnTimestamp = 0;
};
