#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_

#include"Graphics.h"
#include<DirectXMath.h>

class Bindable;

//各オブジェクトが持つべき情報を保存するクラス
class Drawable
{
	//DrawableBaseクラスはDrawableクラスのprivate関数を使える
	template<class T>
	friend class DrawableBase;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	//モデル行列を返す
	virtual DirectX::XMMATRIX GetTransformXM()const noexcept = 0;
	//描画処理
	void Draw(Graphics& gfx)const noexcept;
	//定数バッファ更新
	virtual void Update(float dt)noexcept = 0;
	virtual ~Drawable() = default;
protected:
	template<class T>
	T* QueryBindable()noexcept {
		for (auto& pb : binds) {
			if (auto pt = dynamic_cast<T*>(pb.get())) {
				return pt;
			}
		}
		return nullptr;
	}
	//各オブジェクトのバインド
	void AddBind(std::unique_ptr<Bindable> bind)noexcept;
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf)noexcept;
private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds()const noexcept = 0;
private:
	const class IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};

#endif