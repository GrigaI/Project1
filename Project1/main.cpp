#include <iostream>
#include <vector>
#include <Windows.h>

static void printTitle() {
	std::cout << std::endl;
	std::cout << "1. Добавить задачу." << std::endl;
	std::cout << "2. Показать список задач." << std::endl;
	std::cout << "3. Удалить задачу по индексу." << std::endl;
	std::cout << "4. Выход." << std::endl;
}

static int task() {
	printTitle();

	int answer = 0;
	std::cin >> answer;
	std::cout << std::endl;

	return answer;
}

static void addTask(std::vector<std::string> &curVector) {
	std::cout << "Напишите задачу: ";
	std::string curTask;
	std::cin >> curTask;
	curVector.push_back(curTask);
}

static void showTasks(std::vector<std::string>& curVector) {
	for (auto it = curVector.begin(); it != curVector.end(); ++it) {
		std::cout << *it << std::endl;
	}
}

static void removeTask(std::vector<std::string>& curVector, size_t &index) {
	if (index < curVector.size()) curVector.erase(curVector.begin() + index);
}
int main() {
	SetConsoleOutputCP(CP_UTF8);
	std::vector<std::string> myVector;
	bool isStoped = false;
	while (!isStoped) {
		switch (task()) {
		case 1:
			addTask(myVector);
			break;
		case 2:
			showTasks(myVector);
			break;
		case 3:
			std::cout << "Введите индекс, который хотите удалить: ";
			size_t index;
			std::cin >> index;
			std::cout << std::endl;
			removeTask(myVector, index);
			break;
		case 4:
			isStoped = true;
			break;
		default:
			std::cout << "Нет такого задания!" << std::endl;
			break;
		}
		continue;
	}
	
	
}