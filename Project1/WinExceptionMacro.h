#ifndef _WINEXCEPTIONMACRO_H_
#define _WINEXCEPTIONMACRO_H_

#include"WindowException.h"]
#include"GraphicsException.h"

//�v���v���Z�b�T��__LINE__���s�ԍ��̐����ɕϊ�����
//�v���v���Z�b�T��__FILE__��PGM�̃t�@�C����
#define WIN_EXCEPT(hr) WindowException(__LINE__,__FILE__,hr)
//GetLastError() = �Ăяo�����̃X���b�h�����ŐV�̃G���[�R�[�h���擾
#define WIN_LAST_EXCEPT() WindowException(__LINE__,__FILE__,GetLastError())

#define GFX_THROW_FAILED(hrcall) if(FAILED(hr = (hrcall))) throw GraphicsException(__LINE__,__FILE__,hrcall)
#define GFX_DEVICE_REMOVED_EXPRCT(hr) DeviceRemovedException(__LINE__,__FILE__,(hr))

#endif