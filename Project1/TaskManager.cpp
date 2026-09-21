#include "TaskManager.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>


TaskManager::TaskIter TaskManager::findById(int id) {
	return std::find_if(m_tasks.begin(), m_tasks.end(), [id](const std::unique_ptr<Task>& task) {return task->id() == id;});
}

bool TaskManager::add(std::unique_ptr<Task> task) {
	if (!task) return false;
	if (task->title().empty()) return false;
	if (task->id() != m_nextId) return false;
	m_tasks.push_back(std::move(task));
	++m_nextId;
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
	(*iter)->toggle();
	return true;
}

void TaskManager::printTasks() const {
	if (m_tasks.empty()) {
		std::cout << "Список пуст.\n";
		return;
	}
	
	for (const auto& task : m_tasks) {
		const char mark = task->done() ? 'x' : ' ';
		std::cout << "["
			<< mark
			<< "] "
			<< task->id()
			<< ". "
			<< task->describe()
			<< "\n";		
	}
}

size_t TaskManager::size() const {
	return m_tasks.size();
}

bool TaskManager::empty() const {
	return m_tasks.empty();
}

bool TaskManager::save(const std::string& fileName) const {
	std::ofstream outFile(fileName);
	if (!outFile) return false;
	for (const auto& task : m_tasks) {
		outFile << task->serialize() << '\n';
	}

	return true;
}

std::unique_ptr<Task> TaskManager::parseLine(const std::string& line) {
	try {
		std::istringstream ss(line);
		std::string str;
		if (!std::getline(ss, str, '|')) return nullptr;
		if (str != "R" && str != "D" && str != "C") return nullptr;
		const std::string  type = str;
		if (!std::getline(ss, str, '|')) return nullptr;
		int id = std::stoi(str);
		if (!std::getline(ss, str, '|')) return nullptr;
		std::string title = str;
		if (!std::getline(ss, str, '|')) return nullptr;
		const bool done = std::stoi(str) != 0;
		if (type == "R") {
			return std::make_unique<RegularTask>(id, title, done);
		} else if (type == "D") {
			if (!std::getline(ss, str)) return nullptr;
			return std::make_unique<DeadlineTask>(id, title, done, str);
		}
		else if (type == "C") {
			if (!std::getline(ss, str)) return nullptr;
			return std::make_unique<RecurringTask>(id, title, done, str);
		}
		return nullptr;
	} catch (const std::exception&) {
		return nullptr;
	}
}

LoadResult TaskManager::load(const std::string& fileName) {
	LoadResult result;
	std::ifstream inFile(fileName);
	m_tasks.clear();
	if (!inFile) {		
		m_nextId = 1;
		result.success = true;
		return result;
	}

	std::string line;
	
	while (std::getline(inFile, line)) {
		auto task = parseLine(line);
		if (task) {
			m_tasks.push_back(std::move(task));
			++result.loaded;
		}
		else {
			++result.skipped;
		}
	}
	if (m_tasks.empty()) {
		m_nextId = 1;
	}
	else {
		auto maxIt = std::max_element(m_tasks.begin(), m_tasks.end(), 
			[](const std::unique_ptr<Task>& a, const std::unique_ptr<Task>& b) {return a->id() < b->id();});
		m_nextId = (*maxIt)->id() + 1;
	}
	result.success = true;
	return result;
}

void TaskManager::sort(SortMode mode) {
	switch (mode) {
	case SortMode::ById: 
		std::sort(m_tasks.begin(), m_tasks.end(), 
		[](const std::unique_ptr<Task>& a, const std::unique_ptr<Task>& b) { return a->id() < b->id(); });
		break;
	case SortMode::ByStatus: 
		std::sort(m_tasks.begin(), m_tasks.end(),
		[](const std::unique_ptr<Task>& a, const std::unique_ptr<Task>& b) {
			if (a->done() != b->done()) return a->done() < b->done();
			return a->id() < b->id();
		});
		break;
	case SortMode::ByTitle: 
		std::sort(m_tasks.begin(), m_tasks.end(), 
		[](const std::unique_ptr<Task>& a, const std::unique_ptr<Task>& b) { return a->title() < b->title(); });
		break;
	}
	
}
