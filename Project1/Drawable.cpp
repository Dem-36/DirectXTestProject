#include "Drawable.h"
#include"WinExceptionMacro.h"
#include"IndexBuffer.h"
#include<cassert>
#include<typeinfo>

void Drawable::Draw(Graphics& gfx) const noexcept
{
	//�e�I�u�W�F�N�g�Ńo�C���h�������
	for (auto& b : binds) {
		b->Bind(gfx);
	}
	//�I�u�W�F�N�g���Ƃɋ��ʂŃo�C���h�������
	for (auto& b : GetStaticBinds()) {
		b->Bind(gfx);
	}
	//�`��
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept
{
	//bind��IndexBuffer�o�Ȃ����̂��w�肷��
	assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept
{
	assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
	//�|�C���^���w����̃f�[�^��ύX�s�Ȃ����ŁA�|�C���^���ړ����邱�Ƃ͉\�B
	pIndexBuffer = ibuf.get();
	binds.push_back(std::move(ibuf));
}
