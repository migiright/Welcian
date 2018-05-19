#pragma once
#include <string>

enum class State
{
	GameOver,
	Ready,
	Playing,
	Missing,
	NameEntly,
};

class sys_set
{
public:
	int men_kw;	//面ごとにカウントが初期化される
	State state; //現在の状態
	int state_kw; //汎用カウント
	bool shows_rank; //trueでランキング、falseでデモを表示
	std::string name; //NameEntlyで入力中の文字
	int rank; //NameEntlyで入力中に使う順位

	struct ns_set
	{
		bool f;		//ボーナス表示フラグ
		int kw;		//ボーナス表示カウント
		char s;		//ボーナス表示種類
		int x;		//ボーナス表示位置x
		int y;		//ボーナス表示位置y
	} bns;

	void hensu_syokika();
	void system();
	void ready_set();		//準備
	void game_over_set();
	unsigned int score_return() const;
	unsigned int high_score_return() const;
	unsigned int score_plus(int plus);
	int men_return() const;
	bool enemies_are_running() const;

private:
	unsigned int hiscore;	//ハイスコア
	unsigned int score;		//スコア
	unsigned int men;		//面
};
