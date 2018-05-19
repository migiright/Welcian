#pragma once

//fps取得 "DxLib.h"
int GetFps();

//画面表示(スキップあり)
int Flip();

//回転値を取得
double kai(double moto, double saidai = 6.283);

//回転値を取得int
int kai(int moto, int  saidai);

//決められた値以上/以下の値を取得
template <typename T>	//limitで使用
T limit(T moto, T saisyou, T saidai)
{
	if (moto < saisyou)
		return saisyou;
	if (moto > saidai)
		return saidai;
	return moto;
}