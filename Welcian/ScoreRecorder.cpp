#include "stdafx.h"
#include "ScoreRecorder.h"
#include <algorithm>
#include <fstream>
#include <regex>

using namespace std;

namespace
{
class RecordComparer
{
public:
	using is_transparent = void;
	bool operator()(const Record &lhs, const Record &rhs) const;
	bool operator()(unsigned lhs, const Record &rhs) const;
	bool operator()(const Record &lhs, unsigned rhs) const;
};
}

bool RecordComparer::operator()(const Record &lhs, const Record &rhs) const {
	return lhs.score > rhs.score;
}

bool RecordComparer::operator()(unsigned lhs, const Record &rhs) const {
	return lhs > rhs.score;
}

bool RecordComparer::operator()(const Record &lhs, unsigned rhs) const {
	return lhs.score > rhs;
}

ScoreRecorder::ScoreRecorder(int maxRecords)
	: NumRecords(maxRecords)
	, records_()
	, latestAdded_(maxRecords)
{
	records_.reserve(NumRecords);
	for(int i = NumRecords; i > 0; --i) {
		unsigned score = i * 10;
		records_.push_back({score, 0, "   "});
	}
}

void ScoreRecorder::add(const Record &record)
{
	latestAdded_ = getRank(record.score);
	if (latestAdded() < NumRecords) {
		records_.pop_back();
		records_.insert(upper_bound(begin(records_), end(records_), record, RecordComparer()), record);
	}
}

int ScoreRecorder::getRank(unsigned score) 
{
	return upper_bound(begin(records_), end(records_), score, RecordComparer()) - begin(records_);
}

const Record &ScoreRecorder::record(int rank) const
{
	return records_[rank];
}

int ScoreRecorder::latestAdded() const
{
	return latestAdded_;
}

void ScoreRecorder::save(const string &file)
{
	ofstream ofs(file);
	for(const auto &r : records_) {
		ofs << r.score << "," << r.stage << ",\"" << r.name << "\"" << endl;
	}
}

void ScoreRecorder::load(const string &file)
{
	ifstream ifs(file);
	vector<Record> nrs(NumRecords);
	if (!ifs.is_open()) return;
	regex r(R"xxx(^\s*(\d+)\s*,\s*(\d+)\s*,\s*"([\da-zA-Z \-]{3})"\s*$)xxx");
	for(int i = 0; i < NumRecords; ++i) {
		string b;
		getline(ifs, b);
		smatch m;
		if (!regex_match(b, m, r)) return;
		nrs[i].score = stoi(m[1]);
		nrs[i].stage = stoi(m[2]);
		nrs[i].name = m[3];
	}
	stable_sort(nrs.begin(), nrs.end(), RecordComparer());
	swap(nrs, records_);
}
