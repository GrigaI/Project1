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

RegularTask::RegularTask(int id, const std::string& title, bool done) 
	: Task(id, title, done) {
}

std::string RegularTask::describe() const {
	return "Обычная: " + title();
}

std::string RegularTask::serialize() const {
	return "R|" + std::to_string(id()) + "|" + title() + "|" + std::to_string(done());
}

DeadlineTask::DeadlineTask(int id, const std::string& title, bool done, const std::string& deadline)
	: Task(id, title, done), m_deadline(deadline) {
}

std::string DeadlineTask::describe() const {
	return "Дедлайн " + m_deadline + ": " + title();
}

std::string DeadlineTask::serialize() const {
	return "D|" + std::to_string(id()) + "|" + title() + "|" + std::to_string(done()) + "|" + m_deadline;
}

RecurringTask::RecurringTask(int id, const std::string& title, bool done, const std::string& period)
	: Task(id, title, done), m_period(period) {
}

std::string RecurringTask::describe() const {
	return "Повтор (" + m_period + "): " + title();
}

std::string RecurringTask::serialize() const {
	return "C|" + std::to_string(id()) + "|" + title() + "|" + std::to_string(done()) + "|" + m_period;
}