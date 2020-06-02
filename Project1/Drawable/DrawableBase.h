#ifndef _DRAWABLEBASE_H_
#define _DRAWABLEBASE_H_

#include"Drawable.h"
#include"../Bindable/IndexBuffer.h"

//各オブジェクトが共通して持つ情報を保存するクラス
template<class T>
class DrawableBase :public Drawable
{
protected:
	//中身が空かどうかを確認する(初期化確認。空でないならtrue)
	static bool IsStaticInitialized() noexcept {
		return !staticBinds.empty();
	}

	//Bindするものを登録する
	static void AddStaticBind(std::unique_ptr <Bind::Bindable> bind)noexcept {
		assert("*Must* use AddStaticIndexBuffer to bind index buffer" && typeid(*bind) != typeid(Bind::IndexBuffer));
		staticBinds.push_back(std::move(bind));
	}

	//IndexBufferを登録する(pIndexBufferは各継承先が持っている staticではない)
	void AddStaticIndexBuffer(std::unique_ptr<Bind::IndexBuffer> ibuf)noexcept {
		assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
		pIndexBuffer = ibuf.get();
		staticBinds.push_back(std::move(ibuf));
	}

	void SetIndexFromStatic()noexcept {
		assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
		for (const auto& b : staticBinds) {
			if (const auto p = dynamic_cast<Bind::IndexBuffer*>(b.get())) {
				pIndexBuffer = p;
				return;
			}
		}
		assert("Failed to find index buffer in static binds" && pIndexBuffer != nullptr);
	}

private:
	//Bindされているインターフェースのvectorを返す = Drawableの純粋仮想関数
	const std::vector<std::unique_ptr<Bind::Bindable>>& GetStaticBinds()const noexcept override {
		return staticBinds;
	}

private:
	static std::vector<std::unique_ptr<Bind::Bindable>> staticBinds;
};

//static変数の初期化をここでしている
//templateでstaticを扱う場合、ヘッダの外で初期化 
//テンプレートによってstatic情報が分かれる
template<class T>
std::vector<std::unique_ptr<Bind::Bindable>> DrawableBase<T>::staticBinds;

#endif