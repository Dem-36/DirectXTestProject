#include "Box.h"
#include"WinExceptionMacro.h"
#include"BindableBase.h"
#include"Sphere.h"
#include"Cube.h"

//�R���X�g���N�^
Box::Box(Graphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& bdist)
	:r(rdist(rng)), droll(ddist(rng)), dpitch(ddist(rng)), dyaw(ddist(rng)),
	dphi(odist(rng)), dtheta(odist(rng)), dchi(odist(rng)), chi(adist(rng)),
	theta(adist(rng)), phi(adist(rng))
{
	namespace dx = DirectX;

	//����Box�̏�񂪃o�C���h����Ă��Ȃ��Ȃ�
	if (!IsStaticInitialized()) {
		struct Vertex {
			dx::XMFLOAT3 position;
		};

		auto model = Cube::Make<Vertex>();

		//���_�o�b�t�@�̍쐬
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

	    //���_�V�F�[�_�[���쐬
		auto pvs = std::make_unique<VertexShader>(gfx, L"ColorIndexVS.cso");
		//IndexBuffer�Ŏg�p���邽�߁ABlob�����擾
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));

		//�s�N�Z���V�F�[�_�[���쐬
		AddStaticBind(std::make_unique<PixelShader>(gfx, L"ColorIndexPS.cso"));

		//IndexBuffer�̓o�^(�`�掞�ɒ��_������͂��邽�ߕʂ̕��@�ŕۑ�)
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		struct PixelShaderConstants {
			struct {
				float r, g, b, a;
			}face_colors[8];
		};

		//�s�N�Z���V�F�[�_�[�ɓn���萔�o�b�t�@�쐬
		const PixelShaderConstants cb2 = {
			{
				{1.0f,1.0f,1.0f},
				{1.0f,0.0f,0.0f},
				{0.0f,1.0f,0.0f},
				{1.0f,1.0f,0.0f},
				{0.0f,0.0f,1.0f},
				{1.0f,0.0f,1.0f},
				{0.0f,1.0f,1.0f},
				{0.0f,0.0f,0.0f},
 			}
		};

		//�s�N�Z���V�F�[�_�[�̒萔�o�b�t�@�o�^
		AddStaticBind(std::make_unique<PixelConstantBuffer<PixelShaderConstants>>(gfx, cb2));

		//���_�V�F�[�_�[�ɓn���Z�}���e�B�N�X�����쐬
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
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

	//XMMatrix��3�~3�s��ɕϊ����� = �g��k���s��
	//bdist(rng)��Z�̑傫���������_���Ŏw��
	dx::XMStoreFloat3x3(
		&mt,
		dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng)));
}

void Box::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

//���f���s���Ԃ�
DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	//XMLoadFloat3�~3 = 3�~3�s���XMMATRIX�ɕϊ����� mt�̓��f���s��
	//XMMatrixRotationRollPitchYaw = �w�肳�ꂽ�I�C���[�p�Ɋ�Â��ĉ�]�s����쐬
	return dx::XMLoadFloat3x3(&mt) *
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
		dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}
