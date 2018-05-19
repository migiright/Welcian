#pragma once

class baku_set
{
private:
	struct b_set
	{
		int x;	//自機爆発x
		int y;	//自機爆発y
		bool f;	//自機爆発フラグ
		int k;	//自機爆発カウント(兼アニメ)
	} mb, eb;

public:
	void hensu_syokika();
	int mb_return(int k) const;
	int eb_return(int k) const;
	void mb_set(int x, int y);
	void eb_set(int x, int y);
	void b_ido();
};
