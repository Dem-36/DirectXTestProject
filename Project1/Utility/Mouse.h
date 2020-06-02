#ifndef _MOUSE_H_
#define _MOUSE_H_

#include<queue>

//�}�E�X�����N���X
class Mouse
{
	friend class Window;
public:
	class Event {
	public:
		//�}�E�X�����̗񋓌^
		enum class MouseType {
			LPress,LRelease,   //���N���b�N����
			RPress,RRlease,    //�E�N���b�N����
			WheelUp,WheelDown, //�}�E�X�z�C�[������
			Move,              //�}�E�X�̓���
			Enter,
			Leave,
			Invalid,           //����
		};
	private:
		MouseType type;        //�}�E�X�̃^�C�v
		bool leftIsPressed;    //���N���b�N�����ꂽ���ǂ���
		bool rightIsPressed;   //�E�N���b�N���ꂽ���ǂ���
		int x, y;              //�}�E�X�̍��W
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

		//�^�C�v�������łȂ��Ȃ�true
		bool IsValid()const noexcept {
			return type != MouseType::Invalid;
		}

		//���݂̃^�C�v���擾
		MouseType GetType()const noexcept {
			return type;
		}

		//���݂̃}�E�X�̈ʒu���擾
		std::pair<int, int> GetPosition()const noexcept {
			return{ x,y };
		}

		//�}�E�X��X���W���擾
		int GetX()const noexcept {
			return x;
		}

		//�}�E�X��Y���W���擾
		int GetY()const noexcept {
			return y;
		}

		//�}�E�X�̍��N���b�N��������Ă��邩�ǂ���
		bool LeftIsPressed()const noexcept {
			return leftIsPressed;
		}

		//�}�E�X�̉E�N���b�N��������Ă��邩�ǂ���
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