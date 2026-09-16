#include "TaskManager.h"
#include <algorithm>
#include <iostream>

TaskManager::TaskIter TaskManager::findById(int id) {
	return std::find_if(m_tasks.begin(), m_tasks.end(), [id](const Task& task) {return task.id == id;});
}

bool TaskManager::add(const std::string& task) {
	if (task.empty()) return false;
	m_tasks.push_back(Task{ m_nextId, task, false });
	m_nextId++;
	return true;
}

bool TaskManager::remove(int id) {
	auto iter = findById(id);
	if (iter == m_tasks.end()) return false;
	m_tasks.erase(iter);
	return true;
}

bool TaskManager::toggleDone(int id) {
	auto iter = findById(id);
	if (iter == m_tasks.end()) return false;
	iter->done = !iter->done;
	return true;
}

void TaskManager::printTasks() const {
	if (m_tasks.empty()) {
		std::cout << "Список пуст.\n";
		return;
	}
	
	for (const auto& task : m_tasks) {
		const char mark = task.done ? 'x' : ' ';
		std::cout << "["
			<< mark
			<< "] "
			<< task.id
			<< " "
			<< task.title
			<< "\n";		
	}
}

size_t TaskManager::size() const {
	return m_tasks.size();
}

bool TaskManager::empty() {
	return m_tasks.empty();
}