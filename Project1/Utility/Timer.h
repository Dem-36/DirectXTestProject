#ifndef _TIMER_H_
#define _TIMER_H_

//時間ライブラリ
#include<chrono>

//タイマークラス
class Timer {
public:
	Timer()noexcept;
	float Mark()noexcept;
	float Peek()const noexcept;
private:
	std::chrono::steady_clock::time_point last;
};

#endif