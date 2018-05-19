#include "stdafx.h"
#include "baku_set.h"
#include "Globals.h"
#include <cassert>

void baku_set::hensu_syokika()
{
	mb.x = 0;
	mb.y = 0;
	mb.f = false;
	mb.k = 0;

	eb.x = 0;
	eb.y = 0;
	eb.f = false;
	eb.k = 0;
}
int baku_set::mb_return(int k) const
{
	switch (k) {
	case 0:
		return mb.x;
	case 1:
		return mb.y;
	case 2:
		return mb.f;
	case 3:
		return mb.k;
	default:
		assert(false && "out of range");
		return 0;
	}
}
int baku_set::eb_return(int k) const
{
	switch (k) {
	case 0:
		return eb.x;
	case 1:
		return eb.y;
	case 2:
		return eb.f;
	case 3:
		return eb.k;
	default:
		assert(false && "out of range");
		return 0;
	}
}
void baku_set::mb_set(int x, int y)
{
	mb.f = true;
	mb.x = x;
	mb.y = y;
	mb.k = 0;
}
void baku_set::eb_set(int x, int y)
{
	eb.f = true;
	eb.x = x;
	eb.y = y;
	eb.k = 0;
}
void baku_set::b_ido()
{
	if (mb.f && sys.men_kw%8==0) {
		mb.k++;
		if (mb.k > 3) {
			mb.f = false;
		}
	}
	if (eb.f && sys.men_kw%3==0) {
		eb.k++;
		if (eb.k > 3) {
			eb.f = false;
		}
	}
}
