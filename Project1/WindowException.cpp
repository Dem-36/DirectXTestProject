#include "WindowException.h"
#include<sstream>


//��O�����N���X
//�R���X�g���N�^
//�p�����WinException�̃R���X�g���N�^��������(C#��base�I��)
WindowException::WindowException(int line, const char* file, HRESULT hr) noexcept
	:WinException(line, file), hr(hr)
{
}

const char* WindowException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* WindowException::GetType() const noexcept
{
	return "DirectX11 Window Exception";
}

//HRESULT�̃G���[����string�ɒ���
std::string WindowException::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	//FormatMessage = �G���[�R�[�h�̐������擾
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);
	if (nMsgLen == 0) {
		return "Unidentified error code";
	}
	std::string errorString = pMsgBuf;
	//�w�肳�ꂽ���[�J��������������A�n���h���𖳌��ɂ���
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT WindowException::GetErrorCode() const noexcept
{
	return hr;
}

std::string WindowException::GetErrorDescription() const noexcept
{
	return TranslateErrorCode(hr);
}

NoGfxException::NoGfxException(int line, const char* file, HRESULT hr) noexcept
	:WindowException(line,file,hr)
{
}

const char* NoGfxException::GetType() const noexcept
{
	return "DirectX11 Window Exception [No Graphics]";
}
