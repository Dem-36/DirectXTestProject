#include "Box.h"
#include"../WinExceptionMacro.h"
#include"../Bindable/BindableBase.h"
#include"../Geometry/Cube.h"
#include"../imgui/imgui.h"

using namespace Bind;

//�R���X�g���N�^
Box::Box(Graphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& bdist,
	DirectX::XMFLOAT3 materialColor)
	:TestObject(gfx, rng, adist, ddist, odist, rdist)
{
	namespace dx = DirectX;

	//����Box�̏�񂪃o�C���h����Ă��Ȃ��Ȃ�
	if (!IsStaticInitialized()) {
		struct Vertex {
			dx::XMFLOAT3 position;
			dx::XMFLOAT3 normal;
		};

		auto model = Cube::MakeIndependent<Vertex>();
		//box�̖@�������v�Z����
		model.SetNormalsIndependentFlat();

		//���_�o�b�t�@�̍쐬
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		//���_�V�F�[�_�[���쐬
		auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
		//IndexBuffer�Ŏg�p���邽�߁ABlob�����擾
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));

		//�s�N�Z���V�F�[�_�[���쐬
		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));

		//IndexBuffer�̓o�^(�`�掞�ɒ��_������͂��邽�ߕʂ̕��@�ŕۑ�)
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		//���_�V�F�[�_�[�ɓn���Z�}���e�B�N�X�����쐬
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
		//�v���~�e�B�u�w��
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else {
		//�C���f�b�N�X�o�b�t�@�̏����擾����
		SetIndexFromStatic();
	}

	//���ׂĂ�Box�Ɍʂ̏�񂪂��邽��
	//(�萔�o�b�t�@�͋��ʂ����AGetTransformXM()�̒��g�����ꂼ��Ⴄ����)
	AddBind(std::make_unique<TransformCbuf>(gfx, *this));

	materialConstants.color = materialColor;
	//�X�ŐF�������̂�static�ł̓o�C���h���Ȃ�
	AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, materialConstants, 1u));

	//XMMatrix��3�~3�s��ɕϊ����� = �g��k���s��
	//bdist(rng)��Z�̑傫���������_���Ŏw��
	dx::XMStoreFloat3x3(
		&mt,
		dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng)));
}


//���f���s���Ԃ�
DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMLoadFloat3x3(&mt) * TestObject::GetTransformXM();
}

//Box����ύX����
bool Box::SpawnControlWindow(int id, Graphics& gfx) noexcept
{
	using namespace std::string_literals;

	bool dirty = false;
	bool open = true;
	if (ImGui::Begin(("Box "s + std::to_string(id)).c_str(),&open))
	{ 
		ImGui::Text("Material Properties");
		const bool cd = ImGui::ColorEdit3("Material Color", &materialConstants.color.x);
		const bool sid =  ImGui::SliderFloat("Specular Intensity", &materialConstants.specularIntensity, 0.05f, 4.0f, "%.2f", 2);
		const bool spd =  ImGui::SliderFloat("Specular Power", &materialConstants.specularPower, 1.0f, 200.0f, "%.2f", 2);
		dirty = cd | sid | spd;

		ImGui::Text("Position");
		ImGui::SliderFloat("R", &r, 0.0f, 80.0f, "%.1f");
		//�œ_�𒆐S�Ƃ�����]�p�x
		ImGui::SliderAngle("Theta", &theta, -180.0f, 180.0f);
		ImGui::SliderAngle("Phi", &phi, -180.0f,180.0f);
		//��]�p�x
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
	}
	ImGui::End();

	if (dirty)
	{
		SyncMaterial(gfx);
	}
	return open;
}

void Box::SyncMaterial(Graphics& gfx) noexcept
{
	//�ꎞ�I�ɒ萔�o�b�t�@��������
	auto pConstPS = QueryBindable<MaterialCBuf>();
	assert(pConstPS != nullptr);
	pConstPS->Update(gfx, materialConstants);
}
