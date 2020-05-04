#ifndef _WINXCEPTION_H_
#define _WINEXCEPTION_H_

#include<exception>
#include<string>

//例外処理クラス
//noexcept = その関数が例外処理を投げないことを保証する
//exception = すべての標準例外処理クラスの基底
class WinException:public std::exception
{
public:
	WinException(int line, const char* file)noexcept;
	//実装定義のエラー内容の取得
	const char* what()const noexcept override;
	virtual const char* GetType()const noexcept;
	int GetLine()const noexcept;
	const std::string& GetFile()const noexcept;
	std::string GetOriginString()const noexcept;
private:
	//例外がスローされた行番号
	int line;
	//エラーの発生したファイル名
	std::string file;
protected:
	//mutable = const内で値をいじるときに使用
	mutable std::string whatBuffer;
};

#endif