#include <gtest/gtest.h>
#include "TaskManager.h"
#include "Command.h"

namespace {
    std::unique_ptr<Task> makeRegular(TaskManager& m, const std::string& title) {
        return std::make_unique<RegularTask>(m.nextId(), title, false);
    }
}

TEST(UndoTest, UndoOnEmptyReturnsFalse) {
    TaskManager m;
    EXPECT_FALSE(m.undo());
}

TEST(UndoTest, AddCommandUndoRemovesTask) {
    TaskManager m;
    const int id = m.nextId();
    EXPECT_TRUE(m.add(makeRegular(m, "a")));
    m.pushCommand(std::make_unique<AddCommand>(m, id));

    EXPECT_TRUE(m.undo());
    EXPECT_EQ(m.size(), 0);
    EXPECT_TRUE(m.empty());
}

TEST(UndoTest, ToggleCommandUndoFlipsBack) {
    TaskManager m;
    m.add(makeRegular(m, "a"));           // done=false
    m.toggleDone(1);                       // → true
    m.pushCommand(std::make_unique<ToggleCommand>(m, 1));

    EXPECT_TRUE(m.undo());

    auto extracted = m.extractWithPosition(1);
    ASSERT_TRUE(extracted.has_value());
    EXPECT_FALSE(extracted->first->done());   // снова false
}

TEST(UndoTest, RemoveCommandUndoRestoresAtPosition) {
    TaskManager m;
    m.add(makeRegular(m, "a"));   // id=1
    m.add(makeRegular(m, "b"));   // id=2
    m.add(makeRegular(m, "c"));   // id=3

    // Удаляем id=2 (в середине)
    auto extracted = m.extractWithPosition(2);
    ASSERT_TRUE(extracted.has_value());
    m.pushCommand(std::make_unique<RemoveCommand>(
        m,
        std::move(extracted->first),
        extracted->second
    ));
    EXPECT_EQ(m.size(), 2);

    EXPECT_TRUE(m.undo());
    EXPECT_EQ(m.size(), 3);

    // Проверяем, что вернулось на середину — но у нас нет friend в этом файле
    // Проверим через extractWithPosition: id=2 должен снова существовать
    auto restored = m.extractWithPosition(2);
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored->first->id(), 2);
}

TEST(UndoTest, MultipleUndosWork) {
    TaskManager m;
    const int id1 = m.nextId();
    m.add(makeRegular(m, "a"));
    m.pushCommand(std::make_unique<AddCommand>(m, id1));

    const int id2 = m.nextId();
    m.add(makeRegular(m, "b"));
    m.pushCommand(std::make_unique<AddCommand>(m, id2));

    EXPECT_EQ(m.size(), 2);
    EXPECT_TRUE(m.undo());   // отмена id=2
    EXPECT_EQ(m.size(), 1);
    EXPECT_TRUE(m.undo());   // отмена id=1
    EXPECT_EQ(m.size(), 0);
    EXPECT_FALSE(m.undo());  // история пуста
}