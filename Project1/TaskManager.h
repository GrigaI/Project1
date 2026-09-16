#pragma once
#include <vector>
#include "Task.h"

class TaskManager 
{
	using TaskIter = std::vector<Task>::iterator;
	TaskIter findById(int id);
public:
	bool add(const std::string &task);
	bool remove(int id);
	bool toggleDone(int id);
	void printTasks() const;
	size_t size() const;
	bool empty();

private:
	std::vector<Task> m_tasks;
	int m_nextId = 1;
};

