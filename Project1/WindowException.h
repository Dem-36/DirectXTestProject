#ifndef _WINDOWEXCEPTION_H_
#define _WINDOWEXCEPTION_H_

#include"WinException.h"
#include"WinInc.h"

//WindowÉNÉâÉXì‡ó·äOèàóù
class WindowException:public WinException
{
public:
	WindowException(int line, const char* file, HRESULT hr)noexcept;
	const char* what()const noexcept override;
	virtual const char* GetType()const noexcept;
	static std::string TranslateErrorCode(HRESULT hr)noexcept;
	HRESULT GetErrorCode()const noexcept;
	std::string GetErrorString()const noexcept;
private:
	HRESULT hr;
};
#endif
