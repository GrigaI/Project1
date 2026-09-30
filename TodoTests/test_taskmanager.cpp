#include <gtest/gtest.h>
#include "TaskManager.h"

namespace {
    std::unique_ptr<Task> makeRegular(TaskManager& m, const std::string& title) {
        return std::make_unique<RegularTask>(m.nextId(), title, false);
    }
}

class TaskManagerTest {
public:
    static const std::vector<std::unique_ptr<Task>>& tasks(const TaskManager& m) {
        return m.m_tasks;
    }
};

TEST(TaskManagerAddTest, ValidTaskReturnsTrue) {
    TaskManager m;
    EXPECT_TRUE(m.add(makeRegular(m, "Купить хлеб")));
}

TEST(TaskManagerAddTest, ValidTaskIncreasesSize) {
    TaskManager m;
    m.add(makeRegular(m, "a"));
    EXPECT_EQ(m.size(), 1);
    m.add(makeRegular(m, "b"));
    EXPECT_EQ(m.size(), 2);
}

TEST(TaskManagerAddTest, NullptrReturnsFalse) {
    TaskManager m;
    EXPECT_FALSE(m.add(nullptr));
    EXPECT_EQ(m.size(), 0);
}

TEST(TaskManagerAddTest, EmptyTitleReturnsFalse) {
    TaskManager m;
    EXPECT_FALSE(m.add(makeRegular(m, "")));
    EXPECT_EQ(m.size(), 0);
}

TEST(TaskManagerAddTest, WrongIdReturnsFalse) {
    TaskManager m;
    auto task = std::make_unique<RegularTask>(99, "a", false);
    EXPECT_FALSE(m.add(std::move(task)));
    EXPECT_EQ(m.size(), 0);
}

TEST(TaskManagerAddTest, NextIdIncreasesAfterSuccess) {
    TaskManager m;
    EXPECT_EQ(m.nextId(), 1);
    EXPECT_TRUE(m.add(makeRegular(m, "a")));
    EXPECT_EQ(m.nextId(), 2);
}

TEST(TaskManagerAddTest, NextIdUnchangedAfterFailure) {
    TaskManager m;
    EXPECT_FALSE(m.add(nullptr));
    EXPECT_EQ(m.nextId(), 1);
}

TEST(TaskManagerRemoveTest, RemoveExistingReturnsTrue) {
    TaskManager m;
    EXPECT_TRUE(m.add(makeRegular(m, "a")));
    EXPECT_TRUE(m.remove(1));
    EXPECT_EQ(m.size(), 0);
    EXPECT_TRUE(m.empty());
}

TEST(TaskManagerRemoveTest, RemoveDecreasesSize) {
    TaskManager m;
    EXPECT_TRUE(m.add(makeRegular(m, "a")));
    EXPECT_EQ(m.size(), 1);
    EXPECT_TRUE(m.add(makeRegular(m, "b")));
    EXPECT_EQ(m.size(), 2);
    EXPECT_TRUE(m.remove(1));
    EXPECT_EQ(m.size(), 1); 
}

TEST(TaskManagerRemoveTest, RemoveNonExistentReturnsFalse) {
    TaskManager m;
    EXPECT_TRUE(m.add(makeRegular(m, "a")));
    EXPECT_FALSE(m.remove(3));
    EXPECT_EQ(m.size(), 1);
}

TEST(TaskManagerRemoveTest, RemoveOnEmptyReturnsFalse) {
    TaskManager m;
    EXPECT_FALSE(m.remove(1));
}

TEST(TaskManagerRemoveTest, RemoveDoesNotChangeNextId) {
    TaskManager m;
    m.add(makeRegular(m, "a")); 
    m.add(makeRegular(m, "b"));   
    EXPECT_TRUE(m.remove(1));
    EXPECT_EQ(m.nextId(), 3);
}

TEST(TaskManagerRemoveTest, RemoveTwiceReturnsFalse) {
    TaskManager m;
    m.add(makeRegular(m, "a"));   
    EXPECT_TRUE(m.remove(1));
    EXPECT_FALSE(m.remove(1));    
    EXPECT_EQ(m.size(), 0);
}

TEST(TaskManagerToggleTest, ToggleExistingFlipsDone) {
    TaskManager m;
    m.add(makeRegular(m, "a"));           
    EXPECT_TRUE(m.toggleDone(1));

    auto extracted = m.extractWithPosition(1);
    ASSERT_TRUE(extracted.has_value());
    EXPECT_TRUE(extracted->first->done());   
}

TEST(TaskManagerToggleTest, ToggleTwiceReturnsToOriginal) {
    TaskManager m;
    m.add(makeRegular(m, "a"));
    m.toggleDone(1);
    m.toggleDone(1);

    auto extracted = m.extractWithPosition(1);
    ASSERT_TRUE(extracted.has_value());
    EXPECT_FALSE(extracted->first->done());  
}

TEST(TaskManagerToggleTest, ToggleNonExistentReturnsFalse) {
    TaskManager m;
    m.add(makeRegular(m, "a"));
    EXPECT_FALSE(m.toggleDone(99));
}

TEST(TaskManagerToggleTest, ToggleOnEmptyReturnsFalse) {
    TaskManager m;
    EXPECT_FALSE(m.toggleDone(1));
}

TEST(TaskManagerStateTest, NewManagerIsEmpty) {
    TaskManager m;
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0);
    EXPECT_EQ(m.nextId(), 1);
}

TEST(TaskManagerSortTest, SortByIdAscending) {
    TaskManager m;
    m.add(makeRegular(m, "a"));   
    m.add(makeRegular(m, "b"));   
    m.add(makeRegular(m, "c"));   

    m.sort(TaskManager::SortMode::ById);

    const auto& tasks = TaskManagerTest::tasks(m);
    ASSERT_EQ(tasks.size(), 3);
    EXPECT_EQ(tasks[0]->id(), 1);
    EXPECT_EQ(tasks[1]->id(), 2);
    EXPECT_EQ(tasks[2]->id(), 3);
}

TEST(TaskManagerSortTest, SortByStatusPutsUnfinishedFirst) {
    TaskManager m;
    m.add(makeRegular(m, "a"));   
    m.add(makeRegular(m, "b"));   
    m.add(makeRegular(m, "c"));   

    m.toggleDone(2);              
    m.sort(TaskManager::SortMode::ByStatus);

    const auto& tasks = TaskManagerTest::tasks(m);
    ASSERT_EQ(tasks.size(), 3);
    EXPECT_FALSE(tasks[0]->done());   
    EXPECT_FALSE(tasks[1]->done());   
    EXPECT_TRUE(tasks[2]->done());    
}

TEST(TaskManagerSortTest, SortByTitleAlphabetical) {
    TaskManager m;
    m.add(makeRegular(m, "banana"));   
    m.add(makeRegular(m, "apple"));    
    m.add(makeRegular(m, "cherry"));   

    m.sort(TaskManager::SortMode::ByTitle);

    const auto& tasks = TaskManagerTest::tasks(m);
    ASSERT_EQ(tasks.size(), 3);
    EXPECT_EQ(tasks[0]->title(), "apple");
    EXPECT_EQ(tasks[1]->title(), "banana");
    EXPECT_EQ(tasks[2]->title(), "cherry");
}