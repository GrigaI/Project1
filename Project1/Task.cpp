#include "Task.h"

Task::Task(int id, const std::string& title, bool done) 
	: m_id(id), m_title(title), m_done(done) {
}

void Task::toggle() {
	m_done = !m_done;
}

int Task::id() const {
	return m_id;
}

const std::string& Task::title() const {
	return m_title;
}

bool Task::done() const {
	return m_done;
}