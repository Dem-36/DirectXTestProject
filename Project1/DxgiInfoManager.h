#ifndef _DXGIINFOMANAGER_H_
#define _DXGIINFOMANAGER_H_

#include"WinInc.h"
#pragma warning(disable:4265)
#include <wrl.h>
#pragma warning(default:4265)
#include<vector>
#include<dxgidebug.h>
#include<string>

class DxgiInfoManager
{
public:
	DxgiInfoManager();
	~DxgiInfoManager() = default;
	DxgiInfoManager(const DxgiInfoManager&) = delete;
	DxgiInfoManager& operator = (const DxgiInfoManager&) = delete;
	void Set()noexcept;
	std::vector<std::string> GetMessages()const;
private:
	unsigned long long next = 0u;
	//デバッグ情報をQueueで制御する
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue;
};

#endif