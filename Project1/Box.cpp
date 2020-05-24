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
	std::uniform_real_distribution<float>& bdist,
	DirectX::XMFLOAT3 materialColor)
	:TestObject(gfx,rng,adist,ddist,odist,rdist)
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

	struct PSMaterialConstant {
		alignas(16) dx::XMFLOAT3 color;
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
	}colorConst;

	colorConst.color = materialColor;
	//�X�ŐF�������̂�static�ł̓o�C���h���Ȃ�
	AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, colorConst, 1u));

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
