#ifndef _VERTEXBUFFER_H_
#define _VERTEXBUFFER_H_

#include"Bindable.h"
#include"WinExceptionMacro.h"

class VertexBuffer :public Bindable {
public:
	template<typename T>
	VertexBuffer(Graphics& gfx, const std::vector<T>& vertices)
		:stride(sizeof(T)) {
		INFOMAN(gfx);
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(T) * vertices.size());
		bd.StructureByteStride = sizeof(T);

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = vertices.data();
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &initData, &pVertexBuffer));
	}
	void Bind(Graphics& gfx)noexcept override;

protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer = nullptr;
};

#endif