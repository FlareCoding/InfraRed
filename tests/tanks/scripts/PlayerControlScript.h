#pragma once
#include <InfraRed.h>
using namespace ifr;

class PlayerControlScript : public UserScript
{
public:
	PlayerControlScript(Entity entityID, Ref<Scene> scene, Ref<MaterialInstance> bullet_material);

	// Inherited via UserScript
	virtual void OnUpdate() override;

	uint32_t BulletDamage = 40;

private:
	float m_MovemenetSpeed = 100.0f;
	float m_RotationSpeed = 12.0f;

private:
	Ref<Scene> m_Scene;
	Entity m_EID;
	TransformComponent* Transform;
	ColliderComponent* Collider;

	bool m_bCanShoot = true;
	double m_ReloadTime = 1.0f;

	Entity SpawnBullet();
	void Shoot();
	void OnEnemyHit(Entity enemy);

	StaticModel m_BulletModel;
	Ref<MaterialInstance> m_BulletMaterial;
};