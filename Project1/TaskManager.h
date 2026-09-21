#pragma once
#include <vector>
#include "Task.h"
#include <sstream>
#include <optional>

struct LoadResult {
	bool success = false;
	int loaded = 0;
	int skipped = 0;
};
class TaskManager 
{
	using TaskIter = std::vector<std::unique_ptr<Task>>::iterator;
	TaskIter findById(int id);
	std::optional<Task> parseLine(std::string& line);
public:
	enum class SortMode{
		ById, ByStatus, ByTitle
	};

	bool add(std::unique_ptr<Task> task);
	bool remove(int id);
	bool toggleDone(int id);
	void printTasks() const;
	size_t size() const;
	bool empty();
	bool save(const std::string fileName) const;
	LoadResult load(const std::string& fileName);
	void sort(SortMode mode);

private:
	std::vector<std::unique_ptr<Task>> m_tasks;
	int m_nextId = 1;
};

