#ifndef _WINMATH_H_
#define _WINMATH_H_

#include<math.h>

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;

template<typename T>
constexpr auto sq(const T& x)noexcept {
	return x * x;
}

//角度の範囲固定(ラジアンで求めるため、数値がオーバーフローしないように制限する)
template<typename T>
T wrap_angle(T theta) {
	//2π = 360度
	constexpr T twoPi = (T)2 * (T)PI_D;
	float a = theta * 180.0f / PI;
	//余りを浮動小数点の値で返す(%は整数値)
	const T mod = (T)fmod(theta, twoPi);
	//角度が180度を超えているなら-180にする
	return (mod > (T)PI_D) ?
		(mod - twoPi) : mod;
}

template<typename T>
constexpr T interpolate(const T& src, const T& dst, float alpha)noexcept {
	return src + (dst - src) * alpha;
}

//角度をラジアンに変換する
template<typename T>
constexpr T to_rad(T deg)noexcept {
	return deg * PI / (T)180.0;
}

template<typename T>
constexpr T gauss(T x, T sigma)noexcept {
	const auto ss = sq(sigma);
	//exp = 指数のx乗
	return ((T)1.0 / sqrt((T)2.0 * (T)PI_D * ss)) * exp(-sq(x) / ((T)2.0 * ss));
}

#endif