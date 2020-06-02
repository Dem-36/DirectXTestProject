#ifndef _MOUSE_H_
#define _MOUSE_H_

#include<queue>

//マウス処理クラス
class Mouse
{
	friend class Window;
public:
	class Event {
	public:
		//マウス処理の列挙型
		enum class MouseType {
			LPress,LRelease,   //左クリック処理
			RPress,RRlease,    //右クリック処理
			WheelUp,WheelDown, //マウスホイール処理
			Move,              //マウスの動き
			Enter,
			Leave,
			Invalid,           //無効
		};
	private:
		MouseType type;        //マウスのタイプ
		bool leftIsPressed;    //左クリックがされたかどうか
		bool rightIsPressed;   //右クリックされたかどうか
		int x, y;              //マウスの座標
	public:
		Event()noexcept
			:type(MouseType::Invalid),
			leftIsPressed(false),
			rightIsPressed(false),
			x(0),
			y(0){}

		Event(MouseType type,const Mouse& parent)noexcept
			:type(type),
			leftIsPressed(parent.leftIsPressed),
			rightIsPressed(parent.rightIsPressed),
			x(parent.x),
			y(parent.y){}

		//タイプが無効でないならtrue
		bool IsValid()const noexcept {
			return type != MouseType::Invalid;
		}

		//現在のタイプを取得
		MouseType GetType()const noexcept {
			return type;
		}

		//現在のマウスの位置を取得
		std::pair<int, int> GetPosition()const noexcept {
			return{ x,y };
		}

		//マウスのX座標を取得
		int GetX()const noexcept {
			return x;
		}

		//マウスのY座標を取得
		int GetY()const noexcept {
			return y;
		}

		//マウスの左クリックが押されているかどうか
		bool LeftIsPressed()const noexcept {
			return leftIsPressed;
		}

		//マウスの右クリックが押されているかどうか
		bool RightIsPressed()const noexcept {
			return rightIsPressed;
		}
	};

public:
	Mouse():x(0),y(0){}
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	std::pair<int, int> GetMousePosition()const noexcept;
	int GetX()const noexcept;
	int GetY()const noexcept;
	bool IsInWindow()const noexcept;
	bool LeftIsPressed()const noexcept;
	bool RightIsPredded()const noexcept;
	Mouse::Event Read();
	bool IsEmpty()const noexcept;
	void Flush()noexcept;

private:
	void OnMouseMove(int _x, int _y)noexcept;
	void OnMouseLeave()noexcept;
	void OnMouseEnter()noexcept;
	void OnLeftPressed(int _x, int y)noexcept;
	void OnLeftReleased(int _x, int _y)noexcept;
	void OnRightPressed(int _x, int _y)noexcept;
	void OnRightReleased(int _x, int _y)noexcept;
	void OnWheelUp(int _x, int _y)noexcept;
	void OnWheelDown(int _x, int _y)noexcept;
	void TrimBuffer()noexcept;
	void OnWheelDelta(int _x, int _y,int delta)noexcept;
private:
	static constexpr unsigned int bufferSize = 16u;
	int x, y;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	bool isInWindow = false;
	int wheelDeltaCarry = 0;
	std::queue<Event>buffer;
};

#endif