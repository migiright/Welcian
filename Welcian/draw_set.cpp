#include "stdafx.h"
#include "draw_set.h"
#include "Globals.h"

namespace
{
	void drawGraphic(double x, double y, int w, int h, int graphic)
	{
		const auto tx = static_cast<int>(x*2+96),ty = static_cast<int>(y*2);
		DrawExtendGraph(tx, ty, tx + w*2, ty + h*2, graphic, TRUE);
	}
}

void draw_set::baku_draw()
{
	if (baku.eb_return(2)) {
		drawGraphic(baku.eb_return(0), baku.eb_return(1), 16, 16, hand.eb[baku.eb_return(3)]);
	}
	if (baku.mb_return(2)) {
		drawGraphic(baku.mb_return(0), baku.mb_return(1), 32, 32, hand.mb[baku.mb_return(3)]);
	}
}
void draw_set::tekigun_draw()
{
	const int azx[4] = {-1, 0, 1, 0};
	const int azy[4] = {-1, 0, -1, 0};

	//敵群
	for (int j = limit(10-sys.men_kw, 0, 10); j < 10; j++) {
		//limit(10-sys.men_kw, 0, 10)は、右から表示を増やしていくため
		int az = kai(enemy.g_return(2)+j, 4);
		for (int i = 0; i < 5; i++) {
			if (enemy.em_return(j, i) == enemy.NOT || enemy.em_return(j, i) == enemy.KOUGEKI)
				continue;
			drawGraphic(azx[az]+enemy.g_return(0) + j*16, azy[az]+enemy.g_return(1) + i*12, 16, 16,
				hand.e[enemy.em_return(j, i)][0]);
		}
	}
}
void draw_set::tekikougeki_draw()
{
	for (int i = 0; i < EKMAX; i++) {
		if (enemy.ek_return(i, 0) == 1) {	//敵がいた
			if (enemy.ek_return(i, 3) == 0) {	//護衛
				drawGraphic(enemy.ek_return(i, 1), enemy.ek_return(i, 2), 16, 16,
					hand.e[1][enemy.ek_return(i, 4)]);
			} else {
				drawGraphic(enemy.ek_return(i, 1), enemy.ek_return(i, 2), 16, 16,
					hand.e[enemy.ek_return(i, 3)][enemy.ek_return(i, 4)]);
			}
		}
	}
}
void draw_set::tekidan_draw()
{
	for (int i = 0; i < ETMAX; i++) {
		if (enemy.et_return(i, 0)) {		//弾がある
			drawGraphic(enemy.et_return(i, 1)-2, enemy.et_return(i, 2), 4, 4, hand.et);
		}
	}
}

void draw_set::playing_draw()
{
	system_draw();
	tekigun_draw();
	hako_draw();
	tekikougeki_draw();
	baku_draw();
	tekidan_draw();
}

void draw_set::hako_draw()
{
	for (int i = limit(10-sys.men_kw-1, 0, 10)/3; i < 2; i++) {
		if (enemy.hako_f_return(i) == 1) {		//状態が普通
			drawGraphic(enemy.hako_xy_return(i, 0), enemy.hako_xy_return(i, 1), 16, 16, hand.e[0][0]);
		} else if (enemy.hako_f_return(i) == 2) {	//状態が攻撃
			drawGraphic(enemy.hako_xy_return(i, 0), enemy.hako_xy_return(i, 1), 16, 16, hand.e[0][enemy.hako_ang_return(i)]);
		}
	}
}

int draw_set::picload()
{
	int teki[64];

	hand.sonota = LoadGraph("sonota.png");
	if (hand.sonota == -1)return -1;
	hand.m = DerivationGraph(64, 0, 16, 16, hand.sonota);
	if (hand.m == -1)return -1;
	hand.mt = DerivationGraph(80, 0, 8, 8, hand.sonota);
	if (hand.mt == -1)return -1;
	hand.et = DerivationGraph(88, 0, 4, 4, hand.sonota);
	if (hand.et == -1)return -1;
	for (int i = 0; i < 4; i++) {	//ebとmb
		hand.eb[i] = DerivationGraph(i*16, 0, 16, 16, hand.sonota);
		if (hand.eb[i] == -1)return -1;
		hand.mb[i] = DerivationGraph(i*32, 16, 32, 32, hand.sonota);
		if (hand.mb[i] == -1)return -1;
	}

	if (LoadDivGraph("teki.png", 64, 16, 4, 16, 16, teki) == -1)return -1;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 16; j++) {
			hand.e[i][j] = teki[i*16 + j];
		}
	}

	if (LoadDivGraph("system.png", 60, 20, 3, 8, 8, hand.sys) == -1)return -1;

	hand.haikei = LoadGraph("haikei.jpg");
	if (hand.haikei == -1)return -1;
	int h;
	GetGraphSize(hand.haikei, &haikei_w, &h);

	return 0;
}

void draw_set::suuji_draw(int x, int y, int suuji, int keta, int zero, int iro)
{
	iro = iro==0 ? 0 : 10;	//色を決める
	suuji = suuji > 0 ? suuji : -suuji;	//絶対値を求める

	if (suuji == 0 && zero == 0) {		//0のときは特別処理
		drawGraphic(x+(keta-1)*8, y, 8, 8, hand.sys[iro]);
	} else {
		int sketa = (int)log10((double)suuji) + 1;	//桁数を求める
		keta = sketa > keta ? sketa : keta;	//桁数調整
		for (int i = zero == 0 ? keta - sketa : 0; i < keta; i++) {
			drawGraphic(x+i*8, y, 8, 8, hand.sys[iro+ suuji / (int)pow(10.0, keta-i-1) %10]);
		}
	}
}

void draw_set::moji_draw(int x, int y, const std::string &moji)
{
	for (int i = 0; moji[i] != '\0'; i++) {
		char c;
		if (moji[i] >= 'A' && moji[i] <= 'Z') {			//大文字
			c = 20 + moji[i]-0x41;
		} else if (moji[i] >= 'a' && moji[i] <= 'z') {	//小文字
			c = 20 + moji[i]-0x61;
		} else if (moji[i] >= '0' && moji[i] <= '9') {	//数字
			c = 10 + moji[i]-0x30;
		} else if (moji[i] == '-') {					//ハイフン
			c = 46;
		} else {										//それ以外
			c = -1;
		}
		if (c != -1) {	//表示
			drawGraphic(x+i*8, y, 8, 8, hand.sys[c]);
		}
	}
}

void draw_set::haikei_draw()
{
	static int x = 0, f = 1, k = 0;
	const int r = haikei_w - (640 - 96*2);
	DrawRectGraph(96, 0, x, 0, 448, 480, hand.haikei, false, false);
	if (k <= 0) {
		if (x >= r-10 || x <= 10) {
			if (x >= r-3 || x <= 3) {
				if (kw % 8 == 0) {
					x += f;
				}
			} else {
				if (kw % 3 == 0) {
					x += f;
				}
			}
		} else {
			if (kw % 2 == 0) {
				x += f;
			}
		}
	}

	if (x >= r && f == 1) {
		k = 20;
		f = -1;
	} else if (x <= 0 && f == -1) {
		k = 20;
		f = 1;
	}
	k--;
}
void draw_set::draw()
{
	haikei_draw();
	switch (sys.state) {
	case State::Playing:
		drawGraphic(jiki.mt.x, jiki.mt.y, 8, 8, hand.mt);
		drawGraphic(jiki.m.x, jiki.m.y, 16, 16, hand.m);
		playing_draw();
		break;
	case State::GameOver:
		if (sys.shows_rank) {
			rank_draw();
		} else {
			playing_draw();
		}
		break;
	case State::Ready:
	case State::Missing:
		playing_draw();
		break;
	case State::NameEntly:
		name_entry_draw();
		break;
	}
}

void draw_set::system_draw()
{
	moji_draw(8, 0, "1UP");		//record
	suuji_draw(8, 8, sys.score_return(), 5);

	moji_draw(72, 0, "HI-SCORE");	//hi-record
	suuji_draw(88, 8, sys.high_score_return(), 5);

	for (int i = 0; i < jiki.m.h; i++) {	//自機
		drawGraphic(144+i*8, 8, 8, 8, hand.sys[55]);
	}

	if (sys.men_return()+1 > 5) {		//面が5より多い
		drawGraphic(176, 0, 8, 8, hand.sys[56]);
		drawGraphic(176, 8, 8, 8, hand.sys[57]);
		suuji_draw(184, 8, sys.men_return()+1, 2, 1);
	} else {						//少ない
		for (int i = 0; i < sys.men_return()+1; i++) {
			drawGraphic(176+i*8, 0, 8, 8, hand.sys[56]);
			drawGraphic(176+i*8, 8, 8, 8, hand.sys[57]);
		}
	}

	switch(sys.state) {
	case State::Ready:
		moji_draw(92, 144, "READY");	//ready
		break;
	case State::GameOver:
		moji_draw(76, 128, "GAME OVER");
		moji_draw(56, 144, "PUSH SPACE KEY");
		break;
	default:
		break;
	}

	if (sys.bns.f) {	//ボーナスフラグ
		for (int i = 0; i < 2; i++) {
			drawGraphic(sys.bns.x + i*8, sys.bns.y, 8, 8, hand.sys[47 + sys.bns.s*2 + i]);
		}
	}
}

void draw_set::rank_draw()
{
	moji_draw(84, 8, "RANKING");
	moji_draw(16, 24, "   NAME    SCORE   STAGE");
	for (int i = 0; i < 10; ++i) {
		if (i != scoreRecorder.latestAdded() || kw/30 % 2 == 0) {
			auto y = 40 + 16*i;
			suuji_draw(16, y, i+1, 2, 0, 1);
			const auto &r = scoreRecorder.record(i);
			moji_draw(40, y, r.name);
			suuji_draw(80, y, r.score, 10, 0, 1);
			suuji_draw(176, y, r.stage + 1, 3, 0, 1);
		}
	}
}

void draw_set::name_entry_draw()
{
	moji_draw(84, 8, "RANK IN");
	moji_draw(16, 24, "   NAME    SCORE   STAGE");
	auto y = 40 + 16*sys.rank;
	moji_draw(40, y, sys.state_kw > 30 ? sys.name : sys.name.substr(0, sys.name.length() - 1));
	drawGraphic(40 + (sys.name.length()-1) * 8, y - 8, 8, 8, hand.sys[58]);
	drawGraphic(40 + (sys.name.length()-1) * 8, y + 8, 8, 8, hand.sys[59]);
	suuji_draw(80, y, sys.score_return(), 10, 0, 1);
	suuji_draw(176, y, sys.men_return() + 1, 3, 0, 1);
}
