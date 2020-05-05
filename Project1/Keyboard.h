#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include<queue>
#include<bitset>

//�L�[�{�[�h�N���X
class Keyboard
{
	//�t�����h�N���X�̐ݒ�(Window�N���X����Keyboard�N���X��private��������)
	friend class Window;
public:
	class Event {
	public:
		enum class KeyType {
			Press,
			Release,
			Invalid,
		};
	private:
		KeyType type;
		unsigned char code;
	public:
		Event()
			:type(KeyType::Invalid),code(0u){}
		Event(KeyType type,unsigned char code)noexcept
			:type(type),code(code){}

		//�L�[�������ꂽ���ǂ���
		bool IsPress()const noexcept {
			return type == KeyType::Press;
		}

		//�L�[�������ꂽ���ǂ���
		bool IsRelease()const noexcept {
			return type == KeyType::Release;
		}

		bool IsValid()const noexcept {
			return type == KeyType::Invalid;
		}

		unsigned char GetCode()const noexcept {
			return code;
		}
	};
public:
	Keyboard() = default;
	//�R�s�[�͕K�v�Ȃ��̂Ńf�t�H���g�̋@�\���֎~����
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	//�L�[�̃C�x���g
	bool KeyIsPressed(unsigned char keyCode)const noexcept;
	Event ReadKey()noexcept;
	bool KeyIsEmpty()const noexcept;
	void FlushKey()noexcept;

	//char�^�̃C�x���g
	char ReadChar()noexcept;
	bool CharIsEmpty()const noexcept;
	void FlushChar()noexcept;
	void Flush();

	//����������
	void EnableAutoRepeat()noexcept;
	void DisableAutoRepeat()noexcept;
	bool AutoRepeatIsEnabled()const noexcept;

private:
	//�����Windows���b�Z�[�W���Ă΂ꂽ�Ƃ��Ɏg��
	void OnKeyPressed(unsigned char keyCode)noexcept;
	void OnKeyReleased(unsigned char keyCode)noexcept;
	void OnChar(char character)noexcept;
	void ClearState()noexcept;
	
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer)noexcept;
private:
	//constexpr�������֐��̓R���p�C�����Ɍv�Z���邱�Ƃ��ł���ƕ\��
	//����u = �����Ȃ�����
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool autoRepeaEnabled = false;
	//bitset�̓R���p�C�����_�ŗv�f���͊m�肵�ĂȂ���΂Ȃ�Ȃ�
	std::bitset<nKeys> keyStates;
	std::queue<Event> keyBuffer;
	std::queue<char>charBuffer;
};
#endif