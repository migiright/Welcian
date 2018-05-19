#pragma once

#define EKMAX 10
#define ETMAX 20

class enemy_set
{
public:
	enum elist
	{
		KOUGEKI = -1,	//攻撃中
		NOT,	//ナッシング
		KEGA,	//けが
		NETU,	//熱
		KAZE,	//風邪
	};
	enum ek_list
	{
		K_GOEI,	//箱の護衛
		K_KEGA,	//けが
		K_NETU,
		K_KAZE,
	};
	void hensu_syokika();
	void enemy_ido();
	int enemy_athantei();
	int hako_f_return(int bango) const;
	double hako_xy_return(int bango, int k) const; //k 0:x 1:y
	int hako_ang_return(int bango) const;
	int em_return(int x, int y) const;
	int g_return(int k)const;
	int ek_return(int bango, int k) const;	//k 0:f, 1:x, 2:y, 3:s 4:ang
	int et_return(int bango, int k) const;	//k 0:f, 1:x, 2:y
	int ekaz_return() const;
	void tekigun_haiti();		//敵群配置(systemで使う)
	void kill_all();

private:
	//箱関係
	struct hako_set
	{
		char f;		//箱状態 0:無し 1:普通 2:攻撃
		double x;	//箱位置x
		int y;		//箱位置y
		double ix;	//攻撃箱移動量x
		int ktkw;	//攻撃箱強制退去カウント
		char ktkwb;	//攻撃箱強制退去カウントB
		int j;		//攻撃箱状態
		int kw;		//攻撃箱カウント
		int ang;	//攻撃箱角度(上から時計回りに16段階)
		char gk_a;	//初期の護衛の数
		char gk_b;	//倒された護衛の数
	} hako[2];

	//敵群関係
	enum elist em[10][5];	//敵群配置
	struct g_set
	{
		int x;	//敵群位置x
		int y;	//敵群位置y
		int d;	//敵群移動方向
		int k;	//敵群STOPカウント
		int rt;	//敵群終端右
		int lt;	//敵群終端左
		int ak;	//敵群アニメーションカウント
	} g;
	//攻撃敵関係
	constexpr static int ekkx[32] = {	//敵が回転するときの位置x
		16, 15, 14, 13, 11, 8, 6, 3, 0, -3, -6, -8, -11, -13, -14, -15, -16,
		-15, -14, -13, -11, -8, -6, -3, 0, 3, 6, 8, 11, 13, 14, 15
	};
	constexpr static int ekky[32] = {	//敵が回転するときの位置y
		0, 3, 6, 8, 11, 13, 14, 15, 16, 15, 14, 13, 11, 8, 6, 3,
		0, -3, -6, -8, -11, -13, -14, -15, -16, -15, -14, -13, -11, -8, -6, -3
	};
	struct ek_set
	{
		bool f;		//攻撃敵フラグ
		double x;	//攻撃敵x
		int y;		//攻撃敵y
		double ix;	//攻撃敵移動量x
		int ktkw;	//攻撃敵強制退去カウント
		char ktkwb;	//攻撃敵強制退去カウントB
		ek_list s;	//攻撃敵種類
		int j;		//攻撃敵状態
		int kw;		//攻撃敵カウント
		int ang;	//攻撃敵角度(上から時計回りに16段階)
		int gx;		//群のx
		int gy;		//群のy
		bool muki;	//回転する向き(true:左 false:右)
		char boss;	//護衛のボス
		int biti;	//ボスからの位置
	} ek[EKMAX];		//攻撃中の敵
	bool ek_set_lr;	//攻撃敵セット右左フラグ(true:左、false:右)
	int hk_set_kw;		//箱攻撃カウント(0で攻撃)
						//敵弾関係
	struct et_set
	{
		bool f;		//敵弾フラグ
		double x;	//敵弾x
		int y;		//敵弾y
		double ix;	//敵弾移動量x
	} et[ETMAX];	//敵弾
					//その他
	char ekaz;	//敵の数

	int ryouhasi(int x);
	void enemy_kougeki_set(int x, int y, bool muki);	//敵攻撃配置
	void enemy_kougeki_ido();						//敵攻撃移動、enemy_idoで使用
	void enemy_kougeki_joutai2_ido(int i);			//状態2の移動、enemy_kougeki_idoで使用
	void enemy_goei_joutai2_ido(int i);				//護衛の状態2の移動、enemy_kougeki_idoで使用
	int enemy_k_hako_set(int bango);				//箱攻撃配置
	int enemy_k_goei_set(int x, int boss);			//護衛配置、enemy_k_hako_setで使用
	void enemy_k_hako_ido();						//箱攻撃移動
	void enemy_k_hako_joutai2_ido(int i);			//箱攻撃状態2の移動、enemy_k_hako_idoで使用
	int enemy_tama_set(int x, int y, int s);		//敵弾配置
	void enemy_tama_ido();							//敵弾移動、enemy_idoで使用
};
