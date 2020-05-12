#ifndef _VERTEXSHADER_H_
#define _VERTEXSHADER_H_

#include"Bindable.h"
#include"WinExceptionMacro.h"

class VertexShader:public Bindable
{
public:
	VertexShader(Graphics& gfx, const std::wstring& path);
	void Bind(Graphics& gfx)noexcept override;
	ID3DBlob* GetByteCode()const noexcept;
protected:
	std::string path;
	Microsoft::WRL::ComPtr<ID3DBlob> pByteCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
};
#endif
