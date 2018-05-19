#include "stdafx.h"
#include "enemy_set.h"
#include "Globals.h"
#include <utility>
#include <cassert>

//enemyクラス関数
void enemy_set::hensu_syokika()
{
	for (int i = 0; i < 2; i++) {
		hako[i].f = 0;
		hako[i].x = 0;
		hako[i].y = 0;
		hako[i].ix = 0;
		hako[i].ktkw = 0;
		hako[i].ktkwb = 0;
		hako[i].j = 0;
		hako[i].kw = 0;
		hako[i].ang = 0;
		hako[i].gk_a = 0;
		hako[i].gk_b = 0;
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			em[i][j] = NOT;
		}
	}

	g.x = 32;
	g.y = 48;	//箱の16下
	g.d = 1;
	g.k = 0;
	g.rt = 9;
	g.lt = 0;
	g.ak = 0;

	//敵攻撃
	for (int i = 0; i < EKMAX; i++) {
		ek[i].f = false;
		ek[i].x = 0.0;
		ek[i].y = 0;
		ek[i].ix = 0;
		ek[i].ktkw = 0;
		ek[i].ktkwb = 0;
		ek[i].s = K_KAZE;
		ek[i].j = 0;
		ek[i].kw = 0;
		ek[i].ang = 0;
		ek[i].gx = 0;
		ek[i].gy = 0;
		ek[i].muki = true;
		ek[i].boss = 0;
		ek[i].biti = 0;
	}
	ek_set_lr = false;	//右
	hk_set_kw = 0;

	//敵弾
	for (int i = 0; i < ETMAX; i++) {
		et[i].f = false;
		et[i].x = 0;
		et[i].y = 0;
		et[i].ix = 0;
	}

	ekaz = 0;
}

void enemy_set::enemy_ido()
{
	//敵群横移動
	if (sys.men_kw % 4 == 0) {
		if (g.k <= 0) {		//STOP中でない
			g.x += g.d;		//移動	
			if (g.x >= (224-(g.rt+1)*16) && g.d == 1) {
				g.d = -1;			//移動方向右
			} else if (g.x <= -(g.lt*16) && g.d == -1) {
				g.d = 1;			//移動方向左
			}

			if (GetRand(120-g.k) == 0) {	//たまにストップする
				g.k = GetRand(6) + GetRand(6);
			}

		}
		g.k--;		//カウントを減らす
	}

	//箱移動
	for (int i = 0; i < 2; i++) {
		if (hako[i].f == 1) {		//通常状態
			hako[i].x = g.x + 48 + i*48;	//x位置調整
		}
	}
	//箱攻撃セット
	if (sys.enemies_are_running() && ekaz <= 15 && hk_set_kw == 0) {
		if (hako[0].f != 2 && hako[1].f != 2) {	//箱が攻撃中で無い
			if (hako[ek_set_lr].f == 1) {
				enemy_k_hako_set(ek_set_lr);
				ek_set_lr = !ek_set_lr;	//左右切替
				hk_set_kw = GetRand(600) + 600;
			} else if (hako[!ek_set_lr].f == 1) {
				enemy_k_hako_set(!ek_set_lr);
				hk_set_kw = GetRand(600) + 600;
			}
		}
	}
	if (hk_set_kw > 0) {
		hk_set_kw--;
	}

	//箱攻撃移動
	enemy_k_hako_ido();
	//敵攻撃移動
	enemy_kougeki_ido();

	//敵群アニメーションカウント
	if (sys.men_kw % 30 == 0) {
		g.ak++;
		if (g.ak == 4) {
			g.ak = 0;
		}
	}

	//敵攻撃セット
	if (sys.enemies_are_running()) {
		int *lr = ek_set_lr ? &g.lt : &g.rt;
		if (sys.men_kw % 200 == 0) {
			for (int i = *lr; ek_set_lr ? i <= g.rt : i >= g.lt; ek_set_lr ? i++ : i--) {
				for (int j = 1; j < 5; j++) {
					if (em[i][j] != NOT && em[i][j] != KOUGEKI) {	//敵がいた
						enemy_kougeki_set(i, j, ek_set_lr);	//敵攻撃セット
						ek_set_lr = !ek_set_lr;		//右左切替
						goto tekikougeki_endloop;
					}
				}
			}
		}
	}
tekikougeki_endloop:

	//敵弾移動
	enemy_tama_ido();
}
int enemy_set::enemy_athantei()
{					
	const int azx[4] = {-1, 0, 1, 0};
	const int azy[4] = {-1, 0, -1, 0};

	if (sys.enemies_are_running()) {

		//他群
		for (int i = 0; i < 10; i++) {
			int az = kai(g.ak+i, 4);	//他群で使う
			if (g.x + i*16 + 13 + azx[az] >= jiki.mt.x+3 &&
				jiki.mt.x+3 >= g.x + i*16 + 2 + azx[az]) {
				for (int j = 0; j < 5; j++) {
					if (em[i][j] != NOT && em[i][j] != KOUGEKI &&
						g.y + j*12 + 12 + azy[az] >= jiki.mt.y &&
						jiki.mt.y + 8 >= g.y + j*12 + 3 + azy[az]) {
						if (em[i][j] == KEGA) {			//スコアプラス
							sys.score_plus(50);
						} else if (em[i][j] == NETU) {
							sys.score_plus(40);
						} else if (em[i][j] == KAZE) {
							sys.score_plus(30);
						}

						em[i][j] = NOT;		//消す
						ryouhasi(i);		//両端を設定
						jiki.mt.f = false;	//弾を消す
						jiki.mt.y = 220;
						jiki.mt.x = jiki.m.x + 4;
						baku.eb_set(g.x + i*16 + azx[az], g.y + j*12 + azy[az]);	//爆発
						ekaz--;				//敵の数を1つ減らす
						return 1;			//弾が無くなったので終了
					}
				}
			}
		}

		//箱
		for (int i = 0; i < 2; i++) {
			if (hako[i].f != 0) {		//通常状態、攻撃状態
				if (hako[i].x + 13 >= jiki.mt.x+3 && jiki.mt.x + 3 >= hako[i].x + 2 &&
					hako[i].y + 13 >= jiki.mt.y && jiki.mt.y + 8 >= hako[i].y + 2) {
					//スコア
					if (hako[i].f == 1) {	//普通状態
						sys.score_plus(60);	//スコアプラス
					} else {			//攻撃状態
						sys.bns.f = true;
						sys.bns.x = (int)hako[i].x;
						sys.bns.y = hako[i].y;
						if (hako[i].gk_b == 2) {		//護衛2機倒した
							sys.score_plus(800);
							sys.bns.s = 3;
						} else if (hako[i].gk_a == 2) {	//護衛2機
							sys.score_plus(300);
							sys.bns.s = 2;
						} else if (hako[i].gk_a == 1) {	//護衛1機
							sys.score_plus(200);
							sys.bns.s = 1;
						} else if (hako[i].gk_a == 0) {	//護衛0機
							sys.score_plus(150);
							sys.bns.s = 0;
						}
					}

					hako[i].f = 0;		//消す
					ryouhasi(3 + i*3);	//両端を設定
					jiki.mt.f = false;	//弾を消す
					jiki.mt.y = 220;
					jiki.mt.x = jiki.m.x + 4;
					baku.eb_set((int)hako[i].x, hako[i].y);	//爆発
					ekaz--;				//敵の数を1つ減らす
					return 1;			//弾が亡くなったので終了
				}
			}
		}

		//攻撃敵
		for (int i = 0; i < EKMAX; i++) {
			if (ek[i].f) {
				if (jiki.mt.f &&
					ek[i].x + 13 >= jiki.mt.x + 3 && jiki.mt.x + 3 >= ek[i].x + 2 &&
					ek[i].y + 13 >= jiki.mt.y && jiki.mt.y + 8 >= ek[i].y + 2) {
					if (ek[i].s == K_GOEI || ek[i].s == K_KEGA) {			//スコアプラス
						sys.score_plus(100);
					} else if (ek[i].s == K_NETU) {
						sys.score_plus(80);
					} else if (ek[i].s == K_KAZE) {
						sys.score_plus(60);
					}

					if (ek[i].s == K_GOEI) {	//護衛の場合倒された数を+
						hako[ek[i].boss].gk_b++;
					}

					ek[i].f = NOT;		//消す
					em[ek[i].gx][ek[i].gy] = NOT;	//敵群からも消す
					ryouhasi(ek[i].gx);		//両端を設定
					jiki.mt.f = false;	//弾を消す
					jiki.mt.y = 220;
					jiki.mt.x = jiki.m.x + 4;
					baku.eb_set((int)ek[i].x, ek[i].y);	//爆発
					ekaz--;				//敵の数を1つ減らす
					return 1;			//弾が無くなったので終了
				}
			}
		}

	}

	return 0;
}

int enemy_set::hako_f_return(int bango) const
{
	return hako[bango].f;
}

double enemy_set::hako_xy_return(int bango, int k) const
{
	switch (k) {
	case 0:
		return hako[bango].x;
	case 1:
		return hako[bango].y;
	default:
		assert(false && "out of range");
		return 0;
	}
}

int enemy_set::hako_ang_return(int bango) const
{
	return hako[bango].ang;
}

int enemy_set::em_return(int x, int y) const
{
	return em[x][y];
}
int enemy_set::g_return(int k) const
{
	switch (k) {
	case 0:
		return g.x;
	case 1:
		return g.y;
	case 2:
		return g.ak;
	default:
		assert(false && "out of range");
		return 0;
	}
}
int enemy_set::ek_return(int bango, int k) const
{
	switch (k) {
	case 0:
		return ek[bango].f;
	case 1:
		return (int)ek[bango].x;
	case 2:
		return ek[bango].y;
	case 3:
		return ek[bango].s;
	case 4:
		return ek[bango].ang;
	default:
		assert(false && "out of range");
		return 0;
	}
}
int enemy_set::et_return(int bango, int k) const
{
	switch (k) {
	case 0:
		return et[bango].f;
	case 1:
		return (int)et[bango].x;
	case 2:
		return et[bango].y;
	default:
		assert(false && "out of range");
		return 0;
	}
}
void enemy_set::tekigun_haiti()
{
	int i, j;

	//箱配置
	for (i = 0; i < 2; i++) {
		hako[i].f = 1;
		hako[i].x = 80 + i*48;
		hako[i].y = 32;
	}

	//敵群配置
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 5; j++) {
			em[i][j] = NOT;
		}
	}
	for (i = 2; i < 8; i++) {
		em[i][0] = KEGA;
	}
	for (i = 1; i < 9; i++) {
		em[i][1] = NETU;
	}
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 3; j++)
			em[i][2+j] = KAZE;
	}

	g.x = 32;
	g.y = 48;	//箱の16下
	g.d = 1;
	g.k = 0;
	g.rt = 9;
	g.lt = 0;
	g.ak = 0;

	ekaz = 46;
}
int enemy_set::ekaz_return() const
{
	return ekaz;
}
void enemy_set::kill_all()
{
	for(int i = 0; i < 10; ++i) {
		for(int j = 0; j < 5; ++j) {
			em[i][j] = NOT;
		}
	}
	for(int i = 0; i < 2; ++i) {
		hako[i].f = false;
	}
	for (int i = 0; i < EKMAX; i++) {
		ek[i].f = false;
	}
	ekaz = 0;
}
int enemy_set::ryouhasi(int x)
{
	if (x == g.lt) {	//左端だった
		for (int j = x; j < 10; j++) {
			int i;
			for (i = 0; i < 5; i++) {
				if (em[j][i] != NOT)	//敵がいた
					return 0;
			}
			if (j == 3 && hako[0].f != 0) {	//箱がいた
				return 0;
			}
			if (j == 6 && hako[1].f != 0) {	//箱がいた
				return 1;
			}
			if (i == 5) {	//敵がいなかった
				g.lt++;		//左端をひとつずらす
			}
		}
	}

	if (x == g.rt) {	//右端だった
		for (int j = x; j >= 0; j--) {
			int i;
			for (i = 0; i < 5; i++) {
				if (em[j][i] != NOT)	//敵がいた
					return 0;
			}
			if (j == 3 && hako[0].f != 0) {	//箱がいた
				return 0;
			}
			if (j == 6 && hako[1].f != 0) {	//箱がいた
				return 1;
			}
			if (i == 5) {	//敵がいなかった
				g.rt--;		//右端をひとつずらす
			}
		}
	}
	return 0;
}


void enemy_set::enemy_kougeki_set(int x, int y, bool muki)
{
	for (int i = 0; i < EKMAX; i++) {
		if (!ek[i].f) {		//敵が無い
			ek[i].f = true;
			ek[i].x = g.x + x*16;
			ek[i].y = g.y + y*12;
			ek[i].ix = 0;
			ek[i].ktkw = 0;
			ek[i].gx = x;
			ek[i].gy = y;
			if (em[x][y] == KAZE) {
				ek[i].s = K_KAZE;
			} else if (em[x][y] == NETU) {
				ek[i].s = K_NETU;
			} else if (em[x][y] == KEGA) {
				ek[i].s = K_KEGA;
			}
			ek[i].j = 0;		//状態は0に
			ek[i].kw = 0;		//カウントも0
			ek[i].ang = 0;		//角度も0
			ek[i].muki = muki;	//向き
			em[x][y] = KOUGEKI;	//攻撃状態に
			break;
		}
	}
}
void enemy_set::enemy_kougeki_ido()
{
	for (int i = 0; i < EKMAX; i++) {
		if (ek[i].f) {
			//動く
			if (ek[i].j == 0) {			//---状態0(回転)---
				if (ek[i].muki) {		//左向き
					if (ek[i].kw % 2 == 0) {
						ek[i].ang = kai(0-ek[i].kw/2/2, 16);	//左に曲がる
						ek[i].x = g.x + ek[i].gx*16 + ekkx[kai(0-ek[i].kw/2, 32)] - 8 -8;
						ek[i].y = g.y + ek[i].gy*16 + ekky[kai(0-ek[i].kw/2, 32)] -8;
					}
				} else {			//右向き
					if (ek[i].kw % 2 == 0) {
						ek[i].ang = kai(ek[i].kw/2/2, 16);		//右に曲がる
						ek[i].x = g.x + ek[i].gx*16 + ekkx[16+kai(ek[i].kw/2, 32)] + 8 +8;
						ek[i].y = g.y + ek[i].gy*16 + ekky[16+kai(ek[i].kw/2, 32)] -8;
						if (ek[i].kw == 32) {
							ek[i].x = g.x + ek[i].gx*16 + 32;
							ek[i].y = g.y + ek[i].gy*16;
						}
					}
				}
				if (ek[i].kw == 32) {	//180度回転した
					ek[i].j = 1;
				}

			} else if (ek[i].j == 1) {	//---状態1(降下)---

				if (ek[i].s == K_GOEI) {
					enemy_goei_joutai2_ido(i);	//移動(護衛)
				} else {
					enemy_kougeki_joutai2_ido(i);		//移動
				}

				//敵弾発射
				if (ek[i].kw == 60 || ek[i].kw == 68) {
					enemy_tama_set((int)ek[i].x + 8, ek[i].y + 6, (ek[i].s == K_NETU) ? 1 : 0);
				}

				//当たり判定
				if (sys.state == State::Playing &&
					abs(jiki.m.x - ek[i].x) < 12 && abs(jiki.m.y - ek[i].y) < 12) {
					sys.state = State::Missing;
				}


			} else if (ek[i].j == 2) {	//---状態2(帰還)---
				ek[i].x = g.x + ek[i].gx*16;
				ek[i].y += 1;
				ek[i].ang = limit(((g.y + ek[i].gy * 12) - ek[i].y) / 2, 0, 8);

				if (ek[i].y == g.y + ek[i].gy * 12) {	//元の位置に戻った
					ek[i].f = false;
					if (ek[i].s == K_GOEI) {
						em[ek[i].gx][ek[i].gy] = KEGA;
					} else if (ek[i].s == K_KAZE) {
						em[ek[i].gx][ek[i].gy] = KAZE;
					} else if (ek[i].s == K_NETU) {
						em[ek[i].gx][ek[i].gy] = NETU;
					} else if (ek[i].s == K_KEGA) {
						em[ek[i].gx][ek[i].gy] = KEGA;
					}
				}
			}

			ek[i].kw++;
		}
	}
}
void enemy_set::enemy_kougeki_joutai2_ido(int i)
{
	ek[i].y += 1;	//縦移動
	
	//敵と自機のx位置が近い、移動量が少ない、移動量カウントが0
	if (jiki.m.x - ek[i].x <= 48 && ek[i].x - jiki.m.x <= 48 &&
		ek[i].ix < 0.06 && ek[i].ix > -0.06 && ek[i].ktkw == 0) {
		ek[i].ktkwb++;	//強制退去カウントBを+
		if (ek[i].ktkwb == 10) {		//強制退去カウントBが10以上
			if (ek[i].ix > 0) {
				ek[i].ktkw = 50;
			} else if (ek[i].ix < 0) {
				ek[i].ktkw = -50;
			} else {
				ek[i].ktkw = GetRand(1) ? 50 : -50;
			}
			//ねつは弾発射
			if (ek[i].s == K_NETU) {
				enemy_tama_set((int)ek[i].x + 8, ek[i].y + 6, ek[i].s == K_NETU ? 1 : 0);
			}
		}
	} else {
		ek[i].ktkwb = 0;
	}
	//移動量
	const double isx = ek[i].s == K_NETU ? 0.04 : 0.02;	//移動量設定
	const double isxmax = ek[i].s == K_NETU ? 5 : 3;	//移動量上限設定

	if (ek[i].ktkw != 0) {	//移動量カウントが0でない
		if (ek[i].ktkw > 0) {	//移動量カウントが+
			ek[i].ix = limit(ek[i].ix + isx, -isxmax, isxmax);
			ek[i].ktkw--;
		} else {			//移動量カウントが-
			ek[i].ix = limit(ek[i].ix - isx, -isxmax, isxmax);
			ek[i].ktkw++;
		}
	} else {
		//移動量カウントが0
		int isxplmi = ek[i].ix > 0 ? 1 : -1;		//移動量±設定

		if (jiki.m.x > ek[i].x) {		//敵が自機より左にある
			if (jiki.m.x - ek[i].x > 32) {	//敵と自機の位置が離れている
				ek[i].ix = limit(ek[i].ix + isx, -isxmax, isxmax);
			} else {						//敵と自機の位置が近い
				ek[i].ix = ek[i].ix - isx * isxplmi;
			}
		} else {					//敵が自機より右にある
			if (ek[i].x - jiki.m.x > 32) {	//敵と自機に位置が離れている
				ek[i].ix = limit(ek[i].ix - isx, -isxmax, isxmax);
			} else {
				ek[i].ix = ek[i].ix - isx * isxplmi;
			}
		}

	}

	ek[i].x += ek[i].ix;

	//向きを変える
	if (ek[i].y < 208) {
		double ang = atan2(double(jiki.m.y - ek[i].y), double(jiki.m.x - ek[i].x));
		if (ang < 0) {			//-だったら+に戻す
			ang = 6.283 + ang;
		}
		ek[i].ang = limit(kai(int(2.546 * kai(ang + 0.196)) + 4, 16), 4, 12);
	}

	//出現からしばらく経ち、はみ出したら戻る
	if (ek[i].kw > 100 && (ek[i].x <= -33 || ek[i].x >= 241 || ek[i].y >= 240)) {
		if (ekaz <= 5 && sys.enemies_are_running()) {	//敵の数が4体以下、ready中でない
														//状態1(上に戻る)
			ek[i].y = 0;
			ek[i].x = limit(ek[i].x, 0., 208.);
			ek[i].ix = 0;
		} else {
			//動作2(帰還状態へ)
			ek[i].j = 2;
			ek[i].y = 0;					//y位置を画面の上に
			ek[i].x = g.x + ek[i].gx*16;	//x位置を敵群の位置に
		}
	}
}
void enemy_set::enemy_goei_joutai2_ido(int i)
{
	ek[i].y += 1;	//縦移動
	ek[i].x = hako[ek[i].boss].x + ek[i].biti;

	ek[i].ang = hako[ek[i].boss].ang;	//箱と同じ角度

										//出現からしばらく経ち、はみ出したら戻る
	if (ek[i].kw > 100 && ek[i].y >= 240) {
		if (ekaz <= 5 && sys.enemies_are_running()) {	//敵の数が4体以下、ready中でない
														//状態1(上に戻る)
			ek[i].y = 0;
			ek[i].x = limit(ek[i].x, 0., 208.);
			ek[i].ix = 0;
		} else {
			//動作2(帰還状態へ)
			ek[i].j = 2;
			ek[i].y = 0;					//y位置を画面の上に
			ek[i].x = g.x + ek[i].gx*16;	//x位置を敵群の位置に
		}
	}

	//箱が攻撃状態いなかったら、怪我に戻る
	if (hako[ek[i].boss].f != 2) {
		ek[i].s = K_KEGA;
		ek[i].ix = hako[ek[i].boss].ix;
		ek[i].ktkw = hako[ek[i].boss].ktkw;
		ek[i].ktkwb = hako[ek[i].boss].ktkwb;
	}

	//箱が状態2だったら、状態2になる
	if (hako[ek[i].boss].j == 2) {
		ek[i].j = 2;
		ek[i].y = 0;					//y位置を画面の上に
		ek[i].x = g.x + ek[i].gx*16;	//x位置を敵群の位置に
	}
}
int enemy_set::enemy_k_hako_set(int bango)
{
	if (hako[bango].f == 1) {		//箱がいる
		hako[bango].f = 2;
		hako[bango].x = g.x + 48 + bango*48;
		hako[bango].y = 32;
		hako[bango].ix = 0;
		hako[bango].ktkw = 0;
		hako[bango].j = 0;		//状態は0に
		hako[bango].kw = 0;		//カウントも0
		hako[bango].ang = 0;		//角度も0
		hako[bango].gk_a = 0;
		hako[bango].gk_b = 0;

		//護衛配置
		if (bango == 0) {	//左
			for (int i = 0; i < 2; i++) {
				for (int j = 2; j <= 4; j++) {
					if (em[j][0] == KEGA) {	//怪我がいる
						enemy_k_goei_set(j, bango);	//護衛配置
						hako[bango].gk_a++;			//護衛の数
						break;
					}
				}
			}
		} else {		//右
			for (int i = 0; i < 2; i++) {
				for (int j = 7; j >= 5; j--) {
					if (em[j][0] == KEGA) {	//怪我がいる
						enemy_k_goei_set(j, bango);	//護衛配置
						hako[bango].gk_a++;			//護衛の数
						break;
					}
				}
			}
		}
		return 1;
	}

	return 0;
}
int enemy_set::enemy_k_goei_set(int x, int boss)
{
	for (int i = 0; i < EKMAX; i++) {
		if (!ek[i].f) {
			ek[i].f = true;
			ek[i].x = g.x + x*16;
			ek[i].y = g.y;
			ek[i].ix = 0;
			ek[i].ktkw = 0;
			ek[i].gx = x;
			ek[i].gy = 0;
			ek[i].s = K_GOEI;
			ek[i].j = 0;		//状態は0に
			ek[i].kw = 0;		//カウントも0
			ek[i].ang = 0;		//角度も0
			if (boss == 0) {
				ek[i].muki = true;	//向き
				ek[i].boss = 0;
			} else {
				ek[i].muki = false;
				ek[i].boss = 1;
			}
			ek[i].biti = (int)(ek[i].x - hako[boss].x);
			em[x][0] = KOUGEKI;	//攻撃状態に
			return 1;
		}
	}

	return 0;
}
void enemy_set::enemy_k_hako_ido()
{
	for (int i = 0; i < 2; i++) {
		if (hako[i].f == 2) {	//攻撃状態
								//動く
			if (hako[i].j == 0) {			//---状態0(回転)---
				if (i == 0) {		//左向き
					if (hako[i].kw % 2 == 0) {
						hako[i].ang = kai(0-hako[i].kw/2/2, 16);	//左に曲がる
						hako[i].x = g.x + 48 + i*48 + ekkx[kai(0-hako[i].kw/2, 32)] - 8 -8;
						hako[i].y = 32 + ekky[kai(0-hako[i].kw/2, 32)] -8;
					}
				} else {			//右向き
					if (hako[i].kw % 2 == 0) {
						hako[i].ang = kai(hako[i].kw/2/2, 16);		//右に曲がる
						hako[i].x = g.x + 48 + i*48 + ekkx[16+kai(hako[i].kw/2, 32)] + 8 +8;
						hako[i].y = 32 + ekky[16+kai(hako[i].kw/2, 32)] -8;
						if (hako[i].kw == 32) {
							hako[i].x = g.x + 48 + i*48 + 32;
							hako[i].y = 32;
						}
					}
				}
				if (hako[i].kw == 32) {	//180度回転した
					hako[i].j = 1;
				}

			} else if (hako[i].j == 1) {	//---状態1(降下)---

				enemy_k_hako_joutai2_ido(i);	//移動

												//敵弾発射
				if (hako[i].kw == 60 || hako[i].kw == 68) {
					enemy_tama_set((int)hako[i].x + 8, hako[i].y + 6, 0);
				}

				//当たり判定
				if (sys.state == State::Playing &&
					abs(jiki.m.x - hako[i].x) < 12 && abs(jiki.m.y - hako[i].y) < 12) {
					sys.state = State::Missing;
				}

			} else if (hako[i].j == 2) {	//---状態2(帰還)---
				hako[i].x = g.x + 48 + i*48;
				hako[i].y += 1;
				hako[i].ang = limit((32 - hako[i].y) / 2, 0, 8);

				if (hako[i].y == 32) {	//元の位置に戻った
					hako[i].f = 1;
				}
			}

			hako[i].kw++;
		}
	}
}
void enemy_set::enemy_k_hako_joutai2_ido(int i)
{
	hako[i].y += 1;	//縦移動

	//敵と自機のx位置が近い、移動量が少ない、移動量カウントが0
	if (jiki.m.x - hako[i].x <= 48 && hako[i].x - jiki.m.x <= 48 &&
		hako[i].ix < 0.06 && hako[i].ix > -0.06 && hako[i].ktkw == 0) {
		hako[i].ktkwb++;	//強制退去カウントBを+
		if (hako[i].ktkwb == 10) {		//強制退去カウントBが10以上
			if (hako[i].ix > 0) {
				hako[i].ktkw = 50;
			} else if (hako[i].ix < 0) {
				hako[i].ktkw = -50;
			} else {
				hako[i].ktkw = (GetRand(1)) ? 50 : -50;
			}
			//弾発射
			enemy_tama_set((int)hako[i].x + 8, hako[i].y + 6, 0);
		}
	} else {
		hako[i].ktkwb = 0;
	}
	//移動量
	double isx = 0.02;	//移動量設定
	double isxmax = 3;	//移動量上限設定

	if (hako[i].ktkw != 0) {	//移動量カウントが0でない
		if (hako[i].ktkw > 0) {	//移動量カウントが+
			hako[i].ix = limit(hako[i].ix +isx, -isxmax, isxmax);
			hako[i].ktkw--;
		} else {			//移動量カウントが-
			hako[i].ix = limit(hako[i].ix -isx, -isxmax, isxmax);
			hako[i].ktkw++;
		}
	} else {
		//移動量カウントが0
		const int isxplmi = hako[i].ix > 0 ? 1 : -1;		//移動量±設定

		if (jiki.m.x > hako[i].x) {		//敵が自機より左にある
			if (jiki.m.x - hako[i].x > 32) {	//敵と自機の位置が離れている
				hako[i].ix = limit(hako[i].ix + isx, -isxmax, isxmax);
			} else {						//敵と自機の位置が近い
				hako[i].ix = hako[i].ix - isx * isxplmi;
			}
		} else {					//敵が自機より右にある
			if (hako[i].x - jiki.m.x > 32) {	//敵と自機に位置が離れている
				hako[i].ix = limit(hako[i].ix - isx, -isxmax, isxmax);
			} else {
				hako[i].ix = hako[i].ix - isx * isxplmi;
			}
		}

	}

	hako[i].x += hako[i].ix;

	//向きを変える
	if (hako[i].y < 208) {
		double ang = atan2(double(jiki.m.y - hako[i].y), double(jiki.m.x - hako[i].x));
		if (ang < 0) {			//-だったら+に戻す
			ang = 6.283 + ang;
		}
		hako[i].ang = limit(kai(int(2.546 * kai(ang + 0.196)) + 4, 16), 4, 12);
	}

	//出現からしばらく経ち、はみ出したら戻る
	if (hako[i].kw > 100 && (hako[i].x <= -33 || hako[i].x >= 241 || hako[i].y >= 240)) {
		if (ekaz <= 5 && sys.enemies_are_running()) {	//敵の数が4体以下、ready中でない
														//状態1(上に戻る)
			hako[i].y = 0;
			hako[i].x = limit(hako[i].x, 0., 208.);
			hako[i].ix = 0;
		} else {
			//動作2(帰還状態へ)
			hako[i].j = 2;
			hako[i].y = 0;					//y位置を画面の上に
			hako[i].x = g.x + 48 + i*48;	//x位置を敵群の位置に
		}
	}
}
int enemy_set::enemy_tama_set(int x, int y, int s)
{
	for (int i = 0; i < ETMAX; i++) {
		if (!et[i].f) {	//弾がない
			et[i].f = true;
			et[i].x = x;
			et[i].y = y;

			//移動量設定
			double kaisu = (jiki.m.y+8 - et[i].y)/2.0	//移動回数
				, xsa = jiki.m.x+8 - et[i].x	//x位置の差
				, idoryo = xsa / kaisu; //移動量
			if (s == 0) {	//ねつ以外
				et[i].ix = limit(idoryo, -1., 1.);
			} else {
				et[i].ix = limit(idoryo, -1.5, 1.5);
			}
			return 1;
		}
	}

	return 0;
}
void enemy_set::enemy_tama_ido()
{
	for (int i = 0; i < ETMAX; i++) {
		if (et[i].f) {	//弾がある
			et[i].y += 2;
			et[i].x += et[i].ix;
			if (et[i].y > 240) {	//はみ出たらけす
				et[i].f = false;
			}
			//当たり判定
			if (sys.state == State::Playing &&
				et[i].x >= jiki.m.x+2 && jiki.m.x+13 >= et[i].x &&
				et[i].y+4 >= jiki.m.y+2 && jiki.m.y+13 >= et[i].y) {
				sys.state = State::Missing;
			}
		}
	}
}
