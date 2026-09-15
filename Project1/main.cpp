#include <iostream>
#include <string>
#include <vector>
#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  define NOMINMAX
#  include <Windows.h>
#endif

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
			<< "3. Удалить задачу по индексу.\n"
			<< "4. Выход.\n";
	}

	void addTask(std::vector<std::string>& curVector) {
		std::cout << "Напишите задачу: ";
		std::string curTask;
		std::getline(std::cin, curTask);
		if (curTask.empty()) {
			std::cout << "Пустая задача, отмена.\n";
			return;
		}
		curVector.push_back(std::move(curTask));
		std::cout << "Добавлено. Всего задач: " << curVector.size() << "\n";
	}

	void showTasks(std::vector<std::string>& curVector) {
		if (curVector.empty()) {
			std::cout << "Список пуст.\n";
			return;
		}
		for (auto it = curVector.begin(); it != curVector.end(); ++it) {
			std::cout << *it << "\n";
		}
	}

	void removeTask(std::vector<std::string>& curVector) {
		if (curVector.empty()) {
			std::cout << "Список пуст, нечего удалять.\n";
			return;
		}

		std::cout << "Введите индекс, который хотите удалить: ";
		auto index = askInt("Введите индекс: ");
		if (index < 0 || static_cast<size_t>(index) >= curVector.size()) {
			std::cout << "Индекс вне диапазона.\n";
			return;
		}		
		curVector.erase(curVector.begin() + index);
		std::cout << "Удалено.\n";
	}
} //namespace

int main() {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif

	std::vector<std::string> myVector;
	bool running = true;
	while (running) {
		printMenu();
		switch (askInt("> ")) {
		case 1: addTask(myVector); break;
		case 2: showTasks(myVector); break;
		case 3: removeTask(myVector); break;
		case 4: running = false; break;
		default: std::cout << "Нет такого задания!\n"; break;
		}
	}
	
	return 0;
}