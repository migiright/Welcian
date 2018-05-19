#include "StdAfx.h"
#include "sys_Set.h"
#include "Globals.h"

namespace
{
	//ネームエントリーで使える文字をローテーション
char nextChar(char c)
{
	if (isupper(c+1)) return c+1;
	if (c == 'Z') return '-';
	if (c == '-') return ' ';
	if (c == ' ') return 'A';
	return c;
}
char prevChar(char c)
{
	if (isupper(c-1)) return c-1;
	if (c == 'A') return ' ';
	if (c == ' ') return '-';
	if (c == '-') return 'Z';
	return c;
}

}

//systemクラス関数
void sys_set::hensu_syokika()
{
	score = 0;
	hiscore = 0;
	men = 0;
	men_kw = 0;
	bns.f = false;
	bns.kw = 0;
	bns.s = 0;
	bns.x = 0;
	bns.y = 0;
	hiscore = scoreRecorder.record(0).score;
}
void sys_set::system()
{
	men_kw++;

	switch (state) {
	case State::GameOver:
		--state_kw;
		if (state_kw <= 0) {
			shows_rank = !shows_rank;
			state_kw = 10 * 60;
		}
		if (pushed_ky[KEY_INPUT_SPACE]) {
			jiki.hensu_syokika();	//jiki変数初期化
			sys.hensu_syokika();	//sys変数初期化
			enemy.hensu_syokika();	//enemy変数初期化
			enemy.tekigun_haiti();	//敵群配置
			sys.shows_rank = false;
			sys.ready_set();
		}
		break;
	case State::Ready:
		--state_kw;
		if (state_kw == 0) {
			jiki.m.h--;
			state = State::Playing;
		}
		break;
	case State::Playing:
		if (enemy.ekaz_return() == 0) {	//敵がいなくなった
			men++;	//面アップ
			men_kw = 0;
			enemy.tekigun_haiti();
		}
		if (bns.f && bns.kw == 0) {	//ボーナス表示フラグがたった
			bns.kw = 60;
		} else if (bns.f) {		//ボーナス表示中
			bns.kw--;
			if (bns.kw == 0) {
				bns.f = false;
			}
		}
		break;
	case State::Missing:
		if (state_kw == 0) {	//ミスフラグがたった
			state_kw = 100;							//ミスカウントセット
			baku.mb_set(jiki.m.x-8, jiki.m.y-8);	//爆発
		} else {	//ミス中
			--state_kw;
			if (state_kw == 0) {
				if (jiki.m.h == 0) {	//残機がない
					int r = scoreRecorder.getRank(score);
					if (r < scoreRecorder.NumRecords) {
						name = "A";
						rank = r;
						state_kw = 60;
						state = State::NameEntly;
					} else {
						game_over_set();
						shows_rank = false;
					}
				} else {
					ready_set();
				}
			}
		}
		break;
	case State::NameEntly:
		--state_kw;
		if (state_kw <= 0) state_kw = 60;
		if(pushed_ky[KEY_INPUT_UP]) {
			name.back() = nextChar(name.back());
			state_kw = 0;
		}
		if(pushed_ky[KEY_INPUT_DOWN]) {
			name.back() = prevChar(name.back());
			state_kw = 0;
		}
		if(pushed_ky[KEY_INPUT_SPACE]) {
			if(name.length() == 3) {
				scoreRecorder.add({score, men, name});
				game_over_set();
				shows_rank = true;
			} else {
				name.push_back(name.back());
				state_kw = 0;
			}
		}
		break;
	}
}
void sys_set::ready_set()
{
	state = State::Ready;
	state_kw = 60;		//readyカウントセット
	jiki.m.x = 104;		//位置初期化
}

void sys_set::game_over_set()
{
	state = State::GameOver;
	state_kw = 10 * 60;
}

unsigned int sys_set::score_return() const
{
	return score;
}

unsigned int sys_set::high_score_return() const
{
	return hiscore;
}

unsigned int sys_set::score_plus(int plus)
{
	score += plus;
	if (score > hiscore)
		hiscore = score;
	return score;
}
int sys_set::men_return() const
{
	return men;
}
bool sys_set::enemies_are_running() const
{
	switch (sys.state) {
	case State::GameOver:
	case State::Playing:
		return true;
	default:
		return false;
	}
}