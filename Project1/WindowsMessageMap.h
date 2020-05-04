#ifndef _WINDOWSMESSAGEMAP_H_
#define _WINDOWSMESSAGEMAP_H_
#include<unordered_map>
#include<Windows.h>

//�v���V�[�W���ɑ����郁�b�Z�[�W��LPARAM WPARAM�Ƃ��ɏo�͂ɕ\������
class WindowsMessageMap
{
public:
	WindowsMessageMap();
	std::string operator()(DWORD msg, LPARAM lp, WPARAM wm)const;
private:
	//Dictionary�̂悤�Ȃ���
	std::unordered_map<DWORD, std::string> map;
};
#endif
