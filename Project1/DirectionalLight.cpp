#include "DirectionalLight.h"
#include"imgui/imgui.h"

DirectionalLight::DirectionalLight(Graphics& gfx, float radius)
	:lightMesh(gfx, radius),buffer(gfx)
{
	Reset();
}

void DirectionalLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Directional Light")) {
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &data.position.x, -60.0f, 60.0f, "%.2f");
		ImGui::SliderFloat("Y", &data.position.y, -60.0f, 60.0f, "%.2f");
		ImGui::SliderFloat("Z", &data.position.z, -60.0f, 60.0f, "%.2f");

		ImGui::Text("Color / Intensity");
		ImGui::ColorEdit3("Color", &data.lightColor.x);
		ImGui::SliderFloat("Intensity", &data.intensity, 0.0f, 10.0f, "%.2f", 4);

		if (ImGui::Button("Reset")) {
			Reset();
		}
	}
	ImGui::End();
}

//数値をリセットする
void DirectionalLight::Reset() noexcept
{
	data = {
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,0.9f},
		1.0f
	};
}

void DirectionalLight::Draw(Graphics& gfx) const noexcept
{
	lightMesh.SetPosition(data.position);
	lightMesh.Draw(gfx);
}

void DirectionalLight::Bind(Graphics& gfx) const noexcept
{
	buffer.Update(gfx, data);
	buffer.Bind(gfx);
}
