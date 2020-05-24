#include "PointLight.h"
#include"imgui/imgui.h"

PointLight::PointLight(Graphics& gfx, float radius)
	:mesh(gfx,radius),cbuf(gfx)
{
	Reset();
}

void PointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Light")) {
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &cbData.position.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &cbData.position.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &cbData.position.z, -60.0f, 60.0f, "%.1f");

		ImGui::Text("Intensity/Color");
		ImGui::SliderFloat("Intensity", &cbData.diffuseIntensity, 0.05f, 10.0f, "%.2f", 4);
		ImGui::ColorEdit3("Diffuse Color", &cbData.diffuseColor.x);
		ImGui::ColorEdit3("Ambient", &cbData.ambient.x);
		//ImGui::ColorEdit3("Material", &cbData.materialColor.x);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Constant", &cbData.attConst, 0.05f, 10.0f, "%.2f", 4);
		ImGui::SliderFloat("Linear", &cbData.attLin, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &cbData.attQuad, 0.0000001f, 10.0f, "%.7f", 10);

		if (ImGui::Button("Reset")) {
			Reset();
		}
	}

	ImGui::End();
}

void PointLight::Reset() noexcept
{
	cbData = {
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{1.0f,1.0f,1.0f},
		//{1.0f,1.0f,1.0f},
		1.0f,
		1.0f,
		0.045f,
		0.0075f
	};
}

void PointLight::Draw(Graphics& gfx) const noexcept
{
	mesh.SetPosition(cbData.position);
	mesh.Draw(gfx);
}

void PointLight::Bind(Graphics& gfx) const noexcept
{
	cbuf.Update(gfx, cbData);
	cbuf.Bind(gfx);
}
