#ifndef _WINXCEPTION_H_
#define _WINEXCEPTION_H_

#include<exception>
#include<string>

//��O�����N���X
//noexcept = ���̊֐�����O�����𓊂��Ȃ����Ƃ�ۏ؂���
//exception = ���ׂĂ̕W����O�����N���X�̊��
class WinException:public std::exception
{
public:
	WinException(int line, const char* file)noexcept;
	//������`�̃G���[���e�̎擾
	const char* what()const noexcept override;
	virtual const char* GetType()const noexcept;
	int GetLine()const noexcept;
	const std::string& GetFile()const noexcept;
	std::string GetOriginString()const noexcept;
private:
	//��O���X���[���ꂽ�s�ԍ�
	int line;
	//�G���[�̔��������t�@�C����
	std::string file;
protected:
	//mutable = const���Œl��������Ƃ��Ɏg�p
	mutable std::string whatBuffer;
};

#endif