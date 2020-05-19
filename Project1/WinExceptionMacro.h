#ifndef _WINEXCEPTIONMACRO_H_
#define _WINEXCEPTIONMACRO_H_

#include"WindowException.h"
#include"GraphicsException.h"

//NDEBUG = Debugモードでないとき(Releaseだと宣言される)

//プリプロセッサが__LINE__を行番号の整数に変換する
//プリプロセッサが__FILE__をPGMのファイル名
#define WIN_EXCEPT(hr) WindowException(__LINE__,__FILE__,hr)
//GetLastError() = 呼び出し側のスレッドが持つ最新のエラーコードを取得
#define WIN_LAST_EXCEPT() WindowException(__LINE__,__FILE__,GetLastError())


#define GFX_EXCEPT_NOINFO(hr) GraphicsException(__LINE__,__FILE__,(hr))
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw GraphicsException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) GraphicsException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVE_EXCEPTION(hr) DeviceRemovedException(__LINE__,__FILE__,(hr),infoManager.GetMessages())
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw GraphicsInfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) GraphicsException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVE_EXCEPTION(hr) DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

//Debugではないとき
#ifdef NDEBUG
#define INFOMAN(gfx) HRESULT hr
#else
//HRESULTとDxgiInfoManagerを作成する
#define INFOMAN(gfx) HRESULT hr; DxgiInfoManager& infoManager = GetInfoManager((gfx))
#endif

#ifdef NDEBUG
#define INFOMAN_NOHR(gfx)
#else
#define INFOMAN_NOHR(gfx) DxgiInfoManager& infoManager = GetInfoManager((gfx))
#endif

#endif