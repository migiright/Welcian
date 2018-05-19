#include "stdafx.h"
#include "jiki_set.h"
#include "Globals.h"

//jikiクラス関数
int jiki_set::hensu_syokika()
{
	m.x = 104;
	m.h = 3;
	mt.x = 0;
	mt.y = 220;
	mt.f = false;	//false:発射準備完了 true:発射中
	tsetf = true;	//true:準備完了 false:ボタンを離していない
	return 0;
}
int jiki_set::m_ido()
{
	if ((ky[KEY_INPUT_RIGHT] == 1) || (pad & PAD_INPUT_RIGHT)) {
		m.x += (sys.men_kw%2)+1;
		if (m.x > 208)
			m.x = 208;
	}
	if ((ky[KEY_INPUT_LEFT] == 1) || (pad & PAD_INPUT_LEFT)) {
		m.x -= (sys.men_kw%2)+1;
		if (m.x < 0)
			m.x = 0;
	}
	return 0;
}
int jiki_set::mt_set()
{
	if ((ky[KEY_INPUT_SPACE] == 1 || pad & padz) && tsetf && sys.state != State::Ready && sys.state != State::Missing) {
		if (!mt.f) {	//発射していない
			mt.f = !mt.f;
			mt.x = m.x+4;
		}
		tsetf = false;
	}
	if (ky[KEY_INPUT_SPACE] == 0 && !(pad & padz)) {
		tsetf = true;
	}
	return 0;
}
int jiki_set::mt_ido()
{
	if (mt.f) {	//発射中
		mt.y -= 5;
		if (mt.y <= -8) {
			mt.f = false;
			mt.y = 220;
			mt.x = m.x + 4;
		}
		//当たり判定(予定)
	} else {
		mt.x = m.x + 4;
	}
	return 0;

}
