#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keyCode) const noexcept
{
	return keyStates[keyCode];
}

//�L�[�̓ǂݍ���
Keyboard::Event Keyboard::ReadKey() noexcept
{
	if (keyBuffer.size() > 0u) {
		//�擪�̗v�f���擾
		Event e = keyBuffer.front();
		//�擪�̗v�f���폜����(e�ɐ擪�v�f���R�s�[����Ă���)
		keyBuffer.pop();
		return e;
	}
	else {
		return Event();
	}
}

//���g���� = �v�f�����O���ǂ���
bool Keyboard::KeyIsEmpty() const noexcept
{
	return keyBuffer.empty();
}

//�V����queue���쐬
void Keyboard::FlushKey() noexcept
{
	keyBuffer = std::queue<Event>();
}

//char�^�̃L�[�̓ǂݍ���
char Keyboard::ReadChar() noexcept
{
	if (charBuffer.size() > 0u) {
		//�擪�̗v�f���擾
		unsigned char charCode = charBuffer.front();
		//�擪�̗v�f���폜����(e�ɐ擪�v�f���R�s�[����Ă���)
		charBuffer.pop();
		return charCode;
	}
	else {
		return 0;
	}
}

//���g���� = �v�f�����O���ǂ���
bool Keyboard::CharIsEmpty() const noexcept
{
	return charBuffer.empty();
}

//�V����queue���쐬
void Keyboard::FlushChar() noexcept
{
	charBuffer = std::queue<char>();
}

//���queue��V��������
void Keyboard::Flush()
{
	FlushKey();
	FlushChar();
}

void Keyboard::EnableAutoRepeat() noexcept
{
	autoRepeaEnabled = true;
}

void Keyboard::DisableAutoRepeat() noexcept
{
	autoRepeaEnabled = false;
}

bool Keyboard::AutoRepeatIsEnabled() const noexcept
{
	return autoRepeaEnabled;
}

//private side

//�L�[�������ꂽ�Ƃ��ɌĂ΂��
//queue = ������o
void Keyboard::OnKeyPressed(unsigned char keyCode) noexcept
{
	keyStates[keyCode] = true;
	//�Ō���ɗv�f��ǉ�
	keyBuffer.push(Keyboard::Event(Keyboard::Event::KeyType::Press, keyCode));
	//�o�b�t�@���傫������Ƃ��ɂ��ȓ��ČĂ�
	TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keyCode) noexcept
{
	keyStates[keyCode] = false;
	keyBuffer.push(Keyboard::Event(Keyboard::Event::KeyType::Release, keyCode));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(char character) noexcept
{
	charBuffer.push(character);
	TrimBuffer(charBuffer);
}

void Keyboard::ClearState() noexcept
{
	keyStates.reset();
}

//�o�b�t�@�̃T�C�Y���ő勖�e�𒴂��Ă��邩�ǂ���
template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer)noexcept {
	//���̃T�C�Y�ɂȂ�܂Ő擪�v�f���폜����
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}
