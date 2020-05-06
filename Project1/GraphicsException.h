#ifndef _GRAPHICSEXCEPTION_H_
#define _GRAPHICSEXCEPTION_H_

#include"WinException.h"
#include"WinInc.h"

//GraphicsÇÃó·äOèàóùÉNÉâÉX
class GraphicsException:public WinException
{
public:
	GraphicsException(int line, const char* file, HRESULT hr)noexcept;
	const char* what()const noexcept override;
	virtual const char* GetType()const noexcept override;
	HRESULT GetErrorCode()const noexcept;
	std::string GetErrorString()const noexcept;
	std::string GetErrorDescrpiton()const noexcept;
private:
	HRESULT hr;
};

class DeviceRemovedException :public GraphicsException {
public:
	DeviceRemovedException(int line, const char* file, HRESULT hr)noexcept;
	const char* GetType()const noexcept override;
};
#endif
