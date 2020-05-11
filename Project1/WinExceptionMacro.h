#ifndef _WINEXCEPTIONMACRO_H_
#define _WINEXCEPTIONMACRO_H_

#include"WindowException.h"
#include"GraphicsException.h"

//�v���v���Z�b�T��__LINE__���s�ԍ��̐����ɕϊ�����
//�v���v���Z�b�T��__FILE__��PGM�̃t�@�C����
#define WIN_EXCEPT(hr) WindowException(__LINE__,__FILE__,hr)
//GetLastError() = �Ăяo�����̃X���b�h�����ŐV�̃G���[�R�[�h���擾
#define WIN_LAST_EXCEPT() WindowException(__LINE__,__FILE__,GetLastError())


#define GFX_EXCEPT_NOINFO(hr) GraphicsException(__LINE__,__FILE__,(hr))
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw GraphicsException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) GraphicsException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVE_EXCEPTION(hr) DeviceRemovedException(__LINE__,__FILE__,(hr),infoManager.GetMessages())
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw GraphicsInfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

#endif