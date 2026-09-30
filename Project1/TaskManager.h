#pragma once
#include <vector>
#include <memory>
#include <string>
#include <optional>
#include <utility>
#include <cstddef>
#include <fstream>
#include <format>
#include "Task.h"
#include "Command.h"

struct LoadResult {
	bool success = false;
	int loaded = 0;
	int skipped = 0;
};

class TextWriter {
public:
	std::string write(const Task& task) const {
		return task.serialize();
	}
};

class CsvWriter {
public:
	std::string write(const Task& task) const {
		return std::format("{};{};{}", task.id(), task.title(), task.done() ? 1 : 0);
	}
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
	LoadResult load(const std::string& fileName);
	void sort(SortMode mode);
	int nextId() const { return m_nextId; }
	void insert(std::unique_ptr<Task> task, std::size_t position);
	std::optional<std::pair<std::unique_ptr<Task>, std::size_t>> extractWithPosition(int id);
	void pushCommand(std::unique_ptr<Command> cmd);
	bool undo();

	template <typename TWriter>
	bool saveAs(const std::string& fileName) const;

	friend class TaskManagerTest;

private:
	std::vector<std::unique_ptr<Task>> m_tasks;
	std::vector<std::unique_ptr<Command>> m_history;
	int m_nextId = 1;
};

template<typename TWriter>
bool TaskManager::saveAs(const std::string& fileName) const {
	std::ofstream out(fileName);
	if (!out) return false;
	out.write("\xEF\xBB\xBF", 3);   // UTF-8 BOM
	TWriter writer;
	for (const auto& task : m_tasks) {
		out << writer.write(*task) << '\n';
	}
	return true;
}