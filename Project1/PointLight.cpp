#include "PointLight.h"
#include"imgui/imgui.h"

PointLight::PointLight(Graphics& gfx, float radius)
	:mesh(gfx,radius),cbuf(gfx)
{
}

void PointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Light")) {
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &position.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &position.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &position.z, -60.0f, 60.0f, "%.1f");
		if (ImGui::Button("Reset")) {
			Reset();
		}
	}

	ImGui::End();
}

void PointLight::Reset() noexcept
{
	position = { 0.0f,0.0f,0.0f};
}

void PointLight::Draw(Graphics& gfx) const noexcept
{
	mesh.SetPosition(position);
	mesh.Draw(gfx);
}

void PointLight::Bind(Graphics& gfx) const noexcept
{
	cbuf.Update(gfx, PointLightCBuf{ position });
	cbuf.Bind(gfx);
}
