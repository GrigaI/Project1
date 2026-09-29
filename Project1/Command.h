#pragma once
#include <memory>
#include <string>
#include "Task.h"

class TaskManager;

class Command {
public:
    virtual ~Command() = default;

    virtual void undo() = 0;
    virtual std::string name() const = 0;
};

class AddCommand : public Command {
public:
    AddCommand(TaskManager& manager, int id);

    void undo() override;
    std::string name() const override;

private:
    TaskManager& m_manager;
    int m_id;
};

class ToggleCommand : public Command {
public:
    ToggleCommand(TaskManager& manager, int id);

    void undo() override;
    std::string name() const override;

private:
    TaskManager& m_manager;
    int m_id;
};

class RemoveCommand : public Command {
public:
    RemoveCommand(TaskManager& manager, std::unique_ptr<Task> task, std::size_t position);

    void undo() override;
    std::string name() const override;

private:
    TaskManager& m_manager;
    std::unique_ptr<Task> m_task;
    std::size_t m_position;
    int m_id;
};