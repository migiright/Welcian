#include "StdAfx.h"

#define MAIN_CPP
#include "Globals.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
#ifdef NDEBUG
	SetOutApplicationLogValidFlag(FALSE);
#endif
	ChangeWindowMode(1);	//ウインドウモード
	if (DxLib_Init() == -1)return -1;
	SetDrawScreen(DX_SCREEN_BACK);	//裏画面に描画
	SetDrawArea(96, 0, 544, 480);

	draw.picload();	//画像ロード
	scoreRecorder.load(RankingFileName);

	//デモ画面用に敵を配置してゲームオーバー
	jiki.hensu_syokika();	//jiki変数初期化
	sys.hensu_syokika();	//sys変数初期化
	enemy.hensu_syokika();	//enemy変数初期化
	enemy.tekigun_haiti();	//敵群配置
	sys.game_over_set();
	sys.shows_rank = false;

	while (ProcessMessage() >= 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		ClearDrawScreen();
		clsDx();
		swap(ky, prev_ky);
		GetHitKeyStateAll(&ky[0]);
		for (auto i = 0; i < 256; ++i) pushed_ky[i] = ky[i] && !prev_ky[i];
		pad = GetJoypadInputState(DX_INPUT_PAD1);

		baku.b_ido();

		if (sys.state != State::Ready) {
			jiki.m_ido();
			jiki.mt_ido();
			jiki.mt_set();
		}

		enemy.enemy_ido();
		if (sys.state == State::Playing) {
			enemy.enemy_athantei();
		}
		
		draw.draw();
		sys.system();

#ifdef _DEBUG
		printfDx("%d\n", GetFps());
		printfDx("state_kw:%d\n", sys.state_kw);
		printfDx("shows_rank:%d\n", sys.shows_rank);
		if (pushed_ky[KEY_INPUT_K]) enemy.kill_all();
		if (pushed_ky[KEY_INPUT_R]) {
			sys.shows_rank = !sys.shows_rank;
			sys.state_kw = 10 * 60;
		}
		if(pushed_ky[KEY_INPUT_M]) {
			sys.state = State::Missing;
		}
#endif

		kw++;
		Flip();
		//WaitTimer(100);
	}

	scoreRecorder.save(RankingFileName);
	DxLib_End();			//ＤＸライブラリ使用の終了処理
	return 0;				//ソフトの終了
}