#ifndef _TIMER_H_
#define _TIMER_H_

//���ԃ��C�u����
#include<chrono>

//�^�C�}�[�N���X
class Timer {
public:
	Timer()noexcept;
	float Mark()noexcept;
	float Peek()const noexcept;
private:
	std::chrono::steady_clock::time_point last;
};

#endif