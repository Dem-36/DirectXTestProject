#include "GraphicsException.h"
#include<sstream>
#include"dxerr.h"

GraphicsException::GraphicsException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	:WinException(line, file), hr(hr)
{
	for (const auto& m : infoMsgs) {
		info += m;
		info.push_back('\n');
	}
	//�Ō�̉��s���폜����
	if (!info.empty()) {
		info.pop_back();
	}
}

const char* GraphicsException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescrpiton() << std::endl;
	if (!info.empty()) {
		oss << "\n[Error Inof]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOriginString();
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
	return DXGetErrorString(hr);
}

//HRESULT�̐��������擾
std::string GraphicsException::GetErrorDescrpiton() const noexcept
{
	char buf[512];
	DXGetErrorDescription(hr, buf, sizeof(buf));
	return buf;
}

std::string GraphicsException::GetErrorInfo()const noexcept {
	return info;
}

//DeviceRemovedException::DeviceRemovedException(int line, const char* file, HRESULT hr) noexcept
//	:GraphicsException(line, file, hr)
//{
//}

const char* DeviceRemovedException::GetType() const noexcept
{
	return "DirectX11 Graphics Exception [Device Removed] (DXGI_ERROR_DRIVE_REMOVED)";
}

GraphicsInfoException::GraphicsInfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
	:WinException(line, file)
{
	//�͈̓x�[�X����(begin,end�ɑΉ������R���e�i�N���X���ׂĂŎg�p�\)
	//�����I�ɃC�e���[�^�̎d�g�݂𗘗p���Ă���H
	for (const auto& m : infoMsgs) {
		info += m;
		info.push_back('\n');
	}
	//�Ō�̉��s���폜����
	if (!info.empty()) {
		info.pop_back();
	}
}

const char* GraphicsInfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* GraphicsInfoException::GetType() const noexcept
{
	return"DirectX11 Graphics Info Exception";
}

std::string GraphicsInfoException::GetErrorInfo() const noexcept
{
	return info;
}
