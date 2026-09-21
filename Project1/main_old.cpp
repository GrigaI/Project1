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
			<< "5. Сохранить\n"
			<< "6. Сортировать по id\n"
			<< "7. Сортировать по статусу\n"
			<< "8. Сортировать по названию\n"
			<< "9. Выход.\n";
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

	void saveTasks(TaskManager& manager) {
		if (!manager.save("tasks.txt")) {
			std::cout << "Не удалось открыть файл.\n";
			return;
		}
		std::cout << "Сохранено.\n";
	}

	void sortTasks(TaskManager& manager, TaskManager::SortMode mode) {
		if (manager.empty()) {
			std::cout << "Список пуст, нечего сортировать\n";
			return;
		}
		manager.sort(mode);
		manager.printTasks();
	}
	
} //namespace

int main() {
#ifdef _WIN32
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
#endif

	bool running = true;
	TaskManager manager;
	const LoadResult res = manager.load("tasks.txt");
	if (!res.success) {
		std::cout << "Не удалось открыть tasks.txt.\n";
	}
	else if (res.skipped > 0) {
		std::cout << "Внимание: " << res.skipped << " строк(и) не удалось прочитать.\n";
	}
	while (running) {
		printMenu();
		switch (askInt("> ")) {
		case 1: addTask(manager); break;
		case 2: manager.printTasks(); break;
		case 3: removeTask(manager); break;
		case 4: toggleTask(manager); break;
		case 5: saveTasks(manager); break;
		case 6: sortTasks(manager, TaskManager::SortMode::ById); break;
		case 7: sortTasks(manager, TaskManager::SortMode::ByStatus); break;
		case 8: sortTasks(manager, TaskManager::SortMode::ByTitle); break;
		case 9: running = false; break;
		default: std::cout << "Нет такого задания!\n"; break;
		}
	}
	if (!manager.save("tasks.txt")) {
		std::cout << "ВНИМАНИЕ: не удалось сохранить задачи!\n";
	}
	return 0;
}