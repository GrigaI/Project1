#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Task.h"

struct LoadResult {
	bool success = false;
	int loaded = 0;
	int skipped = 0;
};
class TaskManager 
{
	using TaskIter = std::vector<std::unique_ptr<Task>>::iterator;
	TaskIter findById(int id);
	std::unique_ptr<Task> parseLine(const std::string& line);
public:
	enum class SortMode{
		ById, ByStatus, ByTitle
	};

	bool add(std::unique_ptr<Task> task);
	bool remove(int id);
	bool toggleDone(int id);
	void printTasks() const;
	std::size_t size() const;
	bool empty() const;
	bool save(const std::string& fileName) const;
	LoadResult load(const std::string& fileName);
	void sort(SortMode mode);
	int nextId() const { return m_nextId; }

private:
	std::vector<std::unique_ptr<Task>> m_tasks;
	int m_nextId = 1;
};

