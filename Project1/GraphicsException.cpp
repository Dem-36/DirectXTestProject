#include "GraphicsException.h"
#include<sstream>

GraphicsException::GraphicsException(int line, const char* file, HRESULT hr) noexcept
	:WinException(line, file), hr(hr)
{
}

const char* GraphicsException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescrpiton() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* GraphicsException::GetType() const noexcept
{
	return "DirectX11 Graphics Exception";
}

HRESULT GraphicsException::GetErrorCode() const noexcept
{
	return hr;
}

//�擾����HRESULT�̖��O��\��
std::string GraphicsException::GetErrorString() const noexcept
{
	//����
	return std::string();
}

//HRESULT�̐��������擾
std::string GraphicsException::GetErrorDescrpiton() const noexcept
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

DeviceRemovedException::DeviceRemovedException(int line, const char* file, HRESULT hr) noexcept
	:GraphicsException(line, file,hr)
{
}

const char* DeviceRemovedException::GetType() const noexcept
{
	return "DirectX11 Graphics Exception [Device Removed] (DXGI_ERROR_DRIVE_REMOVED)";
}
