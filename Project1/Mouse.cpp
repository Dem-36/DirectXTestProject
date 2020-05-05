#include "Mouse.h"
#include<Windows.h>

//現在のマウスの位置を返す
std::pair<int, int> Mouse::GetMousePosition() const noexcept
{
	return { x,y };
}

//現在のマウスの位置のX座標を返す
int Mouse::GetX() const noexcept
{
	return x;
}

//現在のマウスの位置のY座標を返す
int Mouse::GetY() const noexcept
{
	return 0;
}

//マウスがウィンドウ内にあるかどうか
bool Mouse::IsInWindow() const noexcept
{
	return isInWindow;
}

//マウス左クリックの判定を返す
bool Mouse::LeftIsPressed() const noexcept
{
	return leftIsPressed;
}

//マウス右クリックの判定を返す
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

//バッファの中身が空かどうか
bool Mouse::IsEmpty() const noexcept
{
	return buffer.empty();
}

//バッファを新しく作成する
void Mouse::Flush() noexcept
{
	buffer = std::queue<Mouse::Event>();
}

//private side
//プロシージャで処理を行う

//マウスが動いたときにバッファに記録する
void Mouse::OnMouseMove(int _x, int _y) noexcept
{
	x = _x;
	y = _y;

	//*thisで現在の自分の情報を渡す(Event内で降下判定などをセットするため)
	buffer.push(Mouse::Event(Mouse::Event::MouseType::Move, *this));
	//余分なバッファを取り除く
	TrimBuffer();
}

//マウスがウィンドウ内にいない場合、バッファに記録する
void Mouse::OnMouseLeave() noexcept
{
	isInWindow = false;
	buffer.push(Mouse::Event(Mouse::Event::MouseType::Leave, *this));
	TrimBuffer();
}

//マウスがウィンドウ内にある場合、バッファに記録する
void Mouse::OnMouseEnter() noexcept
{
	isInWindow = true;
	buffer.push(Mouse::Event(Mouse::Event::MouseType::Enter, *this));
	TrimBuffer();
}

//マウスの左クリックが押されたときにバッファに記録する
void Mouse::OnLeftPressed(int _x, int _y) noexcept
{
	x = _x;
	y = _y;

	leftIsPressed = true;

	//*thisで現在の自分の情報を渡す(Event内で降下判定などをセットするため)
	buffer.push(Mouse::Event(Mouse::Event::MouseType::LPress, *this));
	//余分なバッファを取り除く
	TrimBuffer();
}

//マウスの左クリックが離れたときにバッファに記録する
void Mouse::OnLeftReleased(int _x, int _y) noexcept
{
	x = _x;
	y = _y;

	leftIsPressed = false;

	//*thisで現在の自分の情報を渡す(Event内で降下判定などをセットするため)
	buffer.push(Mouse::Event(Mouse::Event::MouseType::LRelease, *this));
	//余分なバッファを取り除く
	TrimBuffer();
}

//マウスの右クリックが押されたときにバッファに記録する
void Mouse::OnRightPressed(int _x, int _y) noexcept
{
	x = _x;
	y = _y;

	rightIsPressed = true;

	//*thisで現在の自分の情報を渡す(Event内で降下判定などをセットするため)
	buffer.push(Mouse::Event(Mouse::Event::MouseType::RPress, *this));
	//余分なバッファを取り除く
	TrimBuffer();
}

//マウスの右クリックが離れたときにバッファに記録する
void Mouse::OnRightReleased(int _x, int _y) noexcept
{
	x = _x;
	y = _y;

	rightIsPressed = false;

	//*thisで現在の自分の情報を渡す(Event内で降下判定などをセットするため)
	buffer.push(Mouse::Event(Mouse::Event::MouseType::RRlease, *this));
	//余分なバッファを取り除く
	TrimBuffer();
}

//マウスが前にホイールされたときにバッファに記録する
void Mouse::OnWheelUp(int _x, int _y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::MouseType::WheelUp, *this));
	TrimBuffer();
}

//マウスが後ろにホイールされたときにバッファに記録する
void Mouse::OnWheelDown(int _x, int _y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::MouseType::WheelDown, *this));
	TrimBuffer();
}

//バッファが規定値よりも多くなったら古い記録から削除する
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
