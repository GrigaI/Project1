#include "Command.h"
#include "TaskManager.h"

AddCommand::AddCommand(TaskManager& manager, int id) : m_manager(manager), m_id(id) {

}

void AddCommand::undo() {
	m_manager.remove(m_id);
}

std::string AddCommand::name() const {
	return "Добавлена задача: " + std::to_string(m_id);
}

ToggleCommand::ToggleCommand(TaskManager& manager, int id) : m_manager(manager), m_id(id) {

}

void ToggleCommand::undo() {
	m_manager.toggleDone(m_id);
}

std::string ToggleCommand::name() const {
	return "Изменен статус задачи: " + std::to_string(m_id);
}

RemoveCommand::RemoveCommand(TaskManager& manager, std::unique_ptr<Task> task, std::size_t position) 
	: m_manager(manager), m_task(std::move(task)), m_position(position), m_id(m_task->id()) {

}

void RemoveCommand::undo() {
	if (!m_task) return;
	m_manager.insert(std::move(m_task), m_position);
}

std::string RemoveCommand::name() const {
	return "Удалена задача " + std::to_string(m_id);
}