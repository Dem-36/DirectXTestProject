#include "Camera.h"
#include"imgui/imgui.h"

namespace dx = DirectX;

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	//3次元ベクトルと行列の乗算
	//XMMatrixRotationRollPitchYawは焦点を中心に回転する
	const auto position = dx::XMVector3Transform(
		dx::XMVectorSet(0.0f, 0.0f, -r, 0.0f),
		dx::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f)
		);

	//カメラ位置、上方向、焦点を指定して左手座標系のView行列
	//XMMatrixRotationRollPitchYawはカメラ自体の回転
	return dx::XMMatrixLookAtLH(
		position, dx::XMVectorZero(),
		dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) *
		dx::XMMatrixRotationRollPitchYaw(pitch, -yaw, roll);
}

void Camera::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Camera")) {
		ImGui::Text("Positioon");
		ImGui::SliderFloat("R", &r, 0.0f, 80.0f, "%.1f");
		ImGui::SliderAngle("Theta", &theta, -180.0f, 180.0f);
		ImGui::SliderAngle("Phi", &phi, -89.0f, 89.0f);
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
		if (ImGui::Button("Reset")) {
			Reset();
		}
	}
	ImGui::End();
}

void Camera::Reset() noexcept
{
	r = 20.0f;
	theta = 0.0f;
	phi = 0.0f;
	//カメラ角度
	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;

}
