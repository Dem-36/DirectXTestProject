#ifndef _WINEXCEPTIONMACRO_H_
#define _WINEXCEPTIONMACRO_H_

#include"WindowException.h"]
#include"GraphicsException.h"

//プリプロセッサが__LINE__を行番号の整数に変換する
//プリプロセッサが__FILE__をPGMのファイル名
#define WIN_EXCEPT(hr) WindowException(__LINE__,__FILE__,hr)
//GetLastError() = 呼び出し側のスレッドが持つ最新のエラーコードを取得
#define WIN_LAST_EXCEPT() WindowException(__LINE__,__FILE__,GetLastError())

#define GFX_THROW_FAILED(hrcall) if(FAILED(hr = (hrcall))) throw GraphicsException(__LINE__,__FILE__,hrcall)
#define GFX_DEVICE_REMOVED_EXPRCT(hr) DeviceRemovedException(__LINE__,__FILE__,(hr))

#endif