#pragma once
#include <string>

class draw_set
{
private:
	struct hand_set
	{
		int sonota;		//sonota.png 読み込み用
		int m;			//sonotaから抜きだし
		int mt;			//〃
		int et; 		//〃
		int eb[4];		//〃
		int mb[4];		//〃
		int e[4][16];	//teki[64]からハンドルをコピー
		int sys[60];	//system.png
		int haikei;		//haikei.jpg
	} hand;
	int haikei_w;

	void baku_draw();	//drawで使う
	void hako_draw();	//drawで使う
	void tekigun_draw();	//〃
	void tekikougeki_draw();	//〃
	void tekidan_draw();	//〃
	void playing_draw();

public:
	int picload();
	void suuji_draw(int x, int y, int suuji, int keta = 0, int zero = 0, int iro = 0);
	void moji_draw(int x, int y, const std::string &moji);
	void haikei_draw();
	void draw();
	void system_draw();
	void rank_draw();
	void name_entry_draw();
};
