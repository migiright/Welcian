#pragma once

class jiki_set
{
public:
	struct m_set
	{
		int x;
		static const int y = 224;
		int h;	//残機
	}m;
	struct mt_set
	{
		int x;
		int y;
		bool f;
	}mt;
	bool tsetf;	//自機弾セットフラグ

	int hensu_syokika();
	int m_ido();
	int mt_set();
	int mt_ido();
};
