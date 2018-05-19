#pragma once

#include <string>
#include <vector>

struct Record
{
	unsigned score;
	unsigned stage;
	std::string name;
};

class ScoreRecorder
{
public:
	ScoreRecorder(int maxRecords);
	~ScoreRecorder() = default;
	ScoreRecorder(const ScoreRecorder &) = delete;
	ScoreRecorder(ScoreRecorder &&) = delete;
	void operator=(const ScoreRecorder &) = delete;
	void operator=(ScoreRecorder &&) = delete;

	void add(const Record &record);
	int getRank(unsigned score) ;
	const Record &record(int rank) const;
	int latestAdded() const;
	void save(const std::string &file);
	void load(const std::string &file);

	const int NumRecords;
private:
	std::vector<Record> records_;
	int latestAdded_;
};
