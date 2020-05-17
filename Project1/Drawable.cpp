#include "Drawable.h"
#include"WinExceptionMacro.h"
#include"IndexBuffer.h"
#include<cassert>
#include<typeinfo>

void Drawable::Draw(Graphics& gfx) const noexcept
{
	//各オブジェクトでバインドするもの
	for (auto& b : binds) {
		b->Bind(gfx);
	}
	//オブジェクトごとに共通でバインドするもの
	for (auto& b : GetStaticBinds()) {
		b->Bind(gfx);
	}
	//描画
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept
{
	//bindはIndexBuffer出ないものを指定する
	assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept
{
	assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
	//ポインタが指す先のデータを変更不可なだけで、ポインタを移動することは可能。
	pIndexBuffer = ibuf.get();
	binds.push_back(std::move(ibuf));
}
