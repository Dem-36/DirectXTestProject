#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keyCode) const noexcept
{
	return keyStates[keyCode];
}

//キーの読み込み
Keyboard::Event Keyboard::ReadKey() noexcept
{
	if (keyBuffer.size() > 0u) {
		//先頭の要素を取得
		Event e = keyBuffer.front();
		//先頭の要素を削除する(eに先頭要素がコピーされている)
		keyBuffer.pop();
		return e;
	}
	else {
		return Event();
	}
}

//中身が空 = 要素数が０かどうか
bool Keyboard::KeyIsEmpty() const noexcept
{
	return keyBuffer.empty();
}

//新しいqueueを作成
void Keyboard::FlushKey() noexcept
{
	keyBuffer = std::queue<Event>();
}

//char型のキーの読み込み
char Keyboard::ReadChar() noexcept
{
	if (charBuffer.size() > 0u) {
		//先頭の要素を取得
		unsigned char charCode = charBuffer.front();
		//先頭の要素を削除する(eに先頭要素がコピーされている)
		charBuffer.pop();
		return charCode;
	}
	else {
		return 0;
	}
}

//中身が空 = 要素数が０かどうか
bool Keyboard::CharIsEmpty() const noexcept
{
	return charBuffer.empty();
}

//新しいqueueを作成
void Keyboard::FlushChar() noexcept
{
	charBuffer = std::queue<char>();
}

//二つのqueueを新しく生成
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

//キーが押されたときに呼ばれる
//queue = 後入れ先出
void Keyboard::OnKeyPressed(unsigned char keyCode) noexcept
{
	keyStates[keyCode] = true;
	//最後尾に要素を追加
	keyBuffer.push(Keyboard::Event(Keyboard::Event::KeyType::Press, keyCode));
	//バッファが大きすぎるときにそな得て呼ぶ
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

//バッファのサイズが最大許容を超えているかどうか
template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer)noexcept {
	//基底のサイズになるまで先頭要素を削除する
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}
