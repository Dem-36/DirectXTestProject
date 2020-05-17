#ifndef _INDEXBUFFER_H_
#define _INDEXBUFFER_H_

#include"Bindable.h"

//�C���f�b�N�X�o�b�t�@
class IndexBuffer :public Bindable {
public:
	//�R���X�g���N�^
	IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices);
	//�o�b�t�@�̏����o�C���h����
	void Bind(Graphics& gfx)noexcept override;
	//���_����Ԃ�
	UINT GetCount()const noexcept;
protected:
	//���_��
	UINT count;
	//�C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>pIndexBuffer = nullptr;
};

#endif