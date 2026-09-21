#include <iostream>
#include <memory>
#include <vector>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  define NOMINMAX
#  include <Windows.h>
#endif

#include "Task.h"

int main() {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif

   std::vector<std::unique_ptr<Task>> tasks;
    tasks.push_back(std::make_unique<RegularTask>(1, "Купить хлеб", false));
    tasks.push_back(std::make_unique<DeadlineTask>(2, "Позвонить маме", false, "2026-12-31"));
    tasks.push_back(std::make_unique<RecurringTask>(3, "Зарядка", true, "daily"));

    for (const auto& task : tasks) {
        std::cout << task->describe() << '\n';
        std::cout << task->serialize() << '\n';
    }
    
    for (auto i = 0; i < 10; i++) {

    }

    return 0;
}