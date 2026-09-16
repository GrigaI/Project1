#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  define NOMINMAX
#  include <Windows.h>
#endif

#include <iostream>
#include <limits>
#include "TaskManager.h"


namespace {
	void clearInput() {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	int askInt(const std::string& prompt) {
		while (true) {
			std::cout << prompt;
			int value;
			if (std::cin >> value) {
				clearInput();
				return value;
			}
			std::cout << "Это не число, попробуй ещё раз.\n";
			clearInput();
		}
	}
	void printMenu() {
		std::cout << "\n"
			<< "1. Добавить задачу.\n"
			<< "2. Показать список задач.\n"
			<< "3. Удалить задачу по id.\n"
			<< "4. Переключить статус задачи.\n"
			<< "5. Выход.\n";
	}

	void addTask(TaskManager &manager) {
		std::cout << "Напишите задачу: ";
		std::string curTask;
		std::getline(std::cin, curTask);
		if (!manager.add(curTask)) {
			std::cout << "Пустая задача, отмена.\n";
			return;
		}
		
		std::cout << "Добавлено. Всего задач: " << manager.size() << "\n";
	}

	void removeTask(TaskManager &manager) {
		if (manager.empty()) {
			std::cout << "Список пуст, нечего удалять\n";
			return;
		}
		manager.printTasks();

		const int id = askInt("Введите id, который хотите удалить: ");
		if (!manager.remove(id)) {
			std::cout << "Задача с таким id не найдена.\n";
			return;
		}
		std::cout <<"Задача id=" << id << " удалена.\n";	
	}

	void toggleTask(TaskManager& manager) {
		if (manager.empty()) {
			std::cout << "Список пуст, нечего изменять\n";
			return;
		}
		manager.printTasks();

		const int id = askInt("Введите id задачи: ");
		if (!manager.toggleDone(id)) {
			std::cout << "Нет задачи с таким id " << id << "\n";
			return;
		}
		std::cout << "Задача id=" << id << " статус изменен.\n";
	}
} //namespace

int main() {
#ifdef _WIN32
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
#endif

	bool running = true;
	TaskManager manager;
	while (running) {
		printMenu();
		switch (askInt("> ")) {
		case 1: addTask(manager); break;
		case 2: manager.printTasks(); break;
		case 3: removeTask(manager); break;
		case 4: toggleTask(manager); break;
		case 5: running = false; break;
		default: std::cout << "Нет такого задания!\n"; break;
		}
	}
	
	return 0;
}