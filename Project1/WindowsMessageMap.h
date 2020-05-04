#ifndef _WINDOWSMESSAGEMAP_H_
#define _WINDOWSMESSAGEMAP_H_
#include<unordered_map>
#include<Windows.h>

//プロシージャに送られるメッセージをLPARAM WPARAMともに出力に表示する
class WindowsMessageMap
{
public:
	WindowsMessageMap();
	std::string operator()(DWORD msg, LPARAM lp, WPARAM wm)const;
private:
	//Dictionaryのようなもの
	std::unordered_map<DWORD, std::string> map;
};
#endif
