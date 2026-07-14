#pragma once
class CWeapon {

public:
	float  fireRate = 0.2f;
	float  cooldown = 0.2f;;
	float  bulletSpeed = 0.0f;

	CWeapon(float fireRateIn = 0.2f, float cooldownIn = 0.2f, float bulletSpeedIn = 0.0f) : fireRate(fireRateIn),cooldown(cooldownIn),bulletSpeed(bulletSpeedIn)
	{

	}

};