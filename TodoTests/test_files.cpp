#include <gtest/gtest.h>
#include <cstdio>
#include "TaskManager.h"

namespace {
    std::unique_ptr<Task> makeRegular(TaskManager& m, const std::string& title) {
        return std::make_unique<RegularTask>(m.nextId(), title, false);
    }

    // RAII: удаляет файл при выходе из области видимости
    struct TempFile {
        std::string path;
        TempFile(std::string p) : path(std::move(p)) {}
        ~TempFile() { std::remove(path.c_str()); }
    };
}

TEST(FileTest, LoadNonExistentReturnsSuccessEmpty) {
    TaskManager m;
    const auto result = m.load("no_such_file_xyz.txt");
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.loaded, 0);
    EXPECT_EQ(result.skipped, 0);
}

TEST(FileTest, RoundTripPreservesTasks) {
    TempFile tmp("test_roundtrip.txt");

    TaskManager m1;
    m1.add(makeRegular(m1, "Купить хлеб"));
    m1.add(std::make_unique<DeadlineTask>(m1.nextId(), "Позвонить", false, "26.10.2026"));
    m1.add(std::make_unique<RecurringTask>(m1.nextId(), "Зарядка", false, "ежедневно"));

    EXPECT_TRUE(m1.saveAs<TextWriter>(tmp.path));

    TaskManager m2;
    const auto result = m2.load(tmp.path);
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.loaded, 3);
    EXPECT_EQ(result.skipped, 0);
    EXPECT_EQ(m2.size(), 3);
    EXPECT_EQ(m2.nextId(), 4);   // следующий id = max + 1

    // Проверяем, что типы сохранились — через extractWithPosition
    auto t1 = m2.extractWithPosition(1);
    ASSERT_TRUE(t1.has_value());
    EXPECT_EQ(t1->first->title(), "Купить хлеб");

    auto t2 = m2.extractWithPosition(2);
    ASSERT_TRUE(t2.has_value());
    EXPECT_EQ(t2->first->serialize(), "D|2|Позвонить|0|26.10.2026");

    auto t3 = m2.extractWithPosition(3);
    ASSERT_TRUE(t3.has_value());
    EXPECT_EQ(t3->first->serialize(), "C|3|Зарядка|0|ежедневно");
}