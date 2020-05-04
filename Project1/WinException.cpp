#include"WinException.h"
#include<sstream>

//コンストラクタ
WinException::WinException(int line, const char* file)noexcept
	:line(line), file(file) {

}

const char* WinException::what()const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* WinException::GetType()const noexcept {
	return "Win Exception";
}

//例外の発生した行番号を返す
int WinException::GetLine()const noexcept {
	return line;
}

const std::string& WinException::GetFile()const noexcept {
	return file;
}

//どこのファイルのどの行でエラーが出たかを取得する
std::string WinException::GetOriginString()const noexcept {
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}