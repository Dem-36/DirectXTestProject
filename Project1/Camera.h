#ifndef _CAMERA_H_
#define _CAMERA_H_

#include"Graphics.h"

class Camera
{
public:
	DirectX::XMMATRIX GetMatrix()const noexcept;
	void SpawnControlWindow()noexcept;
	void Reset()noexcept;
private:
	float r = 20.0f;
	float theta = 0.0f;
	float phi = 0.0f;
	//�J�����p�x
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
};
#endif

