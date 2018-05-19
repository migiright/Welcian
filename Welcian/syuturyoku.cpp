#include "StdAfx.h"
#include "syuturyoku.h"
#include <DxLib.h>
#include <math.h>

//fps取得 "DxLib.h"
int GetFps(){
	static int kw = 0, fps = 0;
	static int jikan = GetNowCount();
	if(jikan < GetNowCount()){
		jikan += 1000;
		fps = kw;
		kw = 0;
	}
	kw++;
	return fps;
}

//画面表示(スキップあり)
int Flip()
{
	static int time = GetNowCount();
	static int kw = 0;
	if (GetNowCount() > time + 17 && kw < 5) {
		time = GetNowCount();
		kw++;
		return 0;
	}
	ScreenFlip();
	time = GetNowCount();
	kw = 0;
	return 1;
}

//回転値を取得 <math.h>
double kai(double moto, double saidai){
	return fmod(saidai + fmod(moto, saidai), saidai);
}

//回転値を取得int
int kai(int moto, int  saidai){
	return (saidai + moto%saidai) % saidai;
}
