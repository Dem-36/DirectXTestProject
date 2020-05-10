#ifndef _GRAPHICSEXCEPTION_H_
#define _GRAPHICSEXCEPTION_H_

#include"WinException.h"
#include"WinInc.h"

#include<vector>

//GraphicsÇÃó·äOèàóùÉNÉâÉX
class GraphicsException:public WinException
{
public:
	GraphicsException(int line, const char* file, HRESULT hr, std::vector <std::string> infoMsgs = {})noexcept;
	const char* what()const noexcept override;
	virtual const char* GetType()const noexcept override;
	HRESULT GetErrorCode()const noexcept;
	std::string GetErrorString()const noexcept;
	std::string GetErrorDescrpiton()const noexcept;
	std::string GetErrorInfo()const noexcept;
private:
	HRESULT hr;
	std::string info;
};

class DeviceRemovedException :public GraphicsException {
	using GraphicsException::GraphicsException;
public:
	//DeviceRemovedException(int line, const char* file, HRESULT hr,)noexcept;
	const char* GetType()const noexcept override;
private:
	std::string reason;
};

class GraphicsInfoException :public WinException {
public:
	GraphicsInfoException(int line, const char* file, std::vector<std::string> infoMsgs)noexcept;
	const char* what()const noexcept override;
	const char* GetType() const noexcept override;
	std::string GetErrorInfo()const noexcept;
private:
	std::string info;
};
#endif
