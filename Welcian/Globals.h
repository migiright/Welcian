#pragma once

#include <vector>

#include "draw_set.h"
#include "sys_set.h"
#include "baku_set.h"
#include "jiki_set.h"
#include "enemy_set.h"
#include "ScoreRecorder.h"

#ifdef MAIN_CPP
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN draw_set draw;
EXTERN sys_set sys;
EXTERN jiki_set jiki;
EXTERN enemy_set enemy;
EXTERN baku_set baku;
#ifdef MAIN_CPP
ScoreRecorder scoreRecorder(10);
#else
extern ScoreRecorder scoreRecorder;
#endif

EXTERN int kw;
#ifdef MAIN_CPP
std::vector<char> ky(256), prev_ky(256), pushed_ky(256);
#else
extern std::vector<char> ky, prev_ky, pushed_ky;
#endif
EXTERN int pad;
EXTERN int padz;

constexpr const char *RankingFileName = "ranking.dat";