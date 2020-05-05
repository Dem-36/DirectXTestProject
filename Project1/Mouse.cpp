#include "Mouse.h"
#include<Windows.h>

//���݂̃}�E�X�̈ʒu��Ԃ�
std::pair<int, int> Mouse::GetMousePosition() const noexcept
{
	return { x,y };
}

//���݂̃}�E�X�̈ʒu��X���W��Ԃ�
int Mouse::GetX() const noexcept
{
	return x;
}

//���݂̃}�E�X�̈ʒu��Y���W��Ԃ�
int Mouse::GetY() const noexcept
{
	return 0;
}

//�}�E�X���E�B���h�E���ɂ��邩�ǂ���
bool Mouse::IsInWindow() const noexcept
{
	return isInWindow;
}

//�}�E�X���N���b�N�̔����Ԃ�
bool Mouse::LeftIsPressed() const noexcept
{
	return leftIsPressed;
}

//�}�E�X�E�N���b�N�̔����Ԃ�
bool Mouse::RightIsPredded() const noexcept
{
	return rightIsPressed;
}

Mouse::Event Mouse::Read()
{
	if (buffer.size() > 0u) {
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	else {
		return Mouse::Event();
	}
}

//�o�b�t�@�̒��g���󂩂ǂ���
bool Mouse::IsEmpty() const noexcept
{
	return buffer.empty();
}

//�o�b�t�@��V�����쐬����
void Mouse::Flush() noexcept
{
	buffer = std::queue<Mouse::Event>();
}

//private side
//�v���V�[�W���ŏ������s��

//�}�E�X���������Ƃ��Ƀo�b�t�@�ɋL�^����
void Mouse::OnMouseMove(int _x, int _y) noexcept
{
	x = _x;
	y = _y;

	//*this�Ō��݂̎����̏���n��(Event���ō~������Ȃǂ��Z�b�g���邽��)
	buffer.push(Mouse::Event(Mouse::Event::MouseType::Move, *this));
	//�]���ȃo�b�t�@����菜��
	TrimBuffer();
}

//�}�E�X���E�B���h�E���ɂ��Ȃ��ꍇ�A�o�b�t�@�ɋL�^����
void Mouse::OnMouseLeave() noexcept
{
	isInWindow = false;
	buffer.push(Mouse::Event(Mouse::Event::MouseType::Leave, *this));
	TrimBuffer();
}

//�}�E�X���E�B���h�E���ɂ���ꍇ�A�o�b�t�@�ɋL�^����
void Mouse::OnMouseEnter() noexcept
{
	isInWindow = true;
	buffer.push(Mouse::Event(Mouse::Event::MouseType::Enter, *this));
	TrimBuffer();
}

//�}�E�X�̍��N���b�N�������ꂽ�Ƃ��Ƀo�b�t�@�ɋL�^����
void Mouse::OnLeftPressed(int _x, int _y) noexcept
{
	x = _x;
	y = _y;

	leftIsPressed = true;

	//*this�Ō��݂̎����̏���n��(Event���ō~������Ȃǂ��Z�b�g���邽��)
	buffer.push(Mouse::Event(Mouse::Event::MouseType::LPress, *this));
	//�]���ȃo�b�t�@����菜��
	TrimBuffer();
}

//�}�E�X�̍��N���b�N�����ꂽ�Ƃ��Ƀo�b�t�@�ɋL�^����
void Mouse::OnLeftReleased(int _x, int _y) noexcept
{
	x = _x;
	y = _y;

	leftIsPressed = false;

	//*this�Ō��݂̎����̏���n��(Event���ō~������Ȃǂ��Z�b�g���邽��)
	buffer.push(Mouse::Event(Mouse::Event::MouseType::LRelease, *this));
	//�]���ȃo�b�t�@����菜��
	TrimBuffer();
}

//�}�E�X�̉E�N���b�N�������ꂽ�Ƃ��Ƀo�b�t�@�ɋL�^����
void Mouse::OnRightPressed(int _x, int _y) noexcept
{
	x = _x;
	y = _y;

	rightIsPressed = true;

	//*this�Ō��݂̎����̏���n��(Event���ō~������Ȃǂ��Z�b�g���邽��)
	buffer.push(Mouse::Event(Mouse::Event::MouseType::RPress, *this));
	//�]���ȃo�b�t�@����菜��
	TrimBuffer();
}

//�}�E�X�̉E�N���b�N�����ꂽ�Ƃ��Ƀo�b�t�@�ɋL�^����
void Mouse::OnRightReleased(int _x, int _y) noexcept
{
	x = _x;
	y = _y;

	rightIsPressed = false;

	//*this�Ō��݂̎����̏���n��(Event���ō~������Ȃǂ��Z�b�g���邽��)
	buffer.push(Mouse::Event(Mouse::Event::MouseType::RRlease, *this));
	//�]���ȃo�b�t�@����菜��
	TrimBuffer();
}

//�}�E�X���O�Ƀz�C�[�����ꂽ�Ƃ��Ƀo�b�t�@�ɋL�^����
void Mouse::OnWheelUp(int _x, int _y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::MouseType::WheelUp, *this));
	TrimBuffer();
}

//�}�E�X�����Ƀz�C�[�����ꂽ�Ƃ��Ƀo�b�t�@�ɋL�^����
void Mouse::OnWheelDown(int _x, int _y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::MouseType::WheelDown, *this));
	TrimBuffer();
}

//�o�b�t�@���K��l���������Ȃ�����Â��L�^����폜����
void Mouse::TrimBuffer() noexcept
{
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}

void Mouse::OnWheelDelta(int _x, int _y,int delta) noexcept
{
	wheelDeltaCarry += delta;
	//generate eventsw for every 120
	while (wheelDeltaCarry >= WHEEL_DELTA) {
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA) {
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}
