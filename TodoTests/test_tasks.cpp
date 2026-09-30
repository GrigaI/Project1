#include <gtest/gtest.h>
#include "Task.h"

TEST(RegularTaskTest, DescribeIncludesTitle) {
	RegularTask task(1, "Купить хлеб", true);
	EXPECT_EQ(task.describe(), "Обычная: Купить хлеб");
}

TEST(RegularTaskTest, SerializeHasCorrectFormat) {
	RegularTask task(1, "Купить хлеб", true);
	EXPECT_EQ(task.serialize(), "R|1|Купить хлеб|1");
}

TEST(RegularTaskTest, ToggleFlipsDone) {
	RegularTask task(1, "Купить хлеб", true);
	EXPECT_TRUE(task.done());
	task.toggle();
	EXPECT_FALSE(task.done());
	task.toggle();
	EXPECT_TRUE(task.done());
}

TEST(DeadlineTaskTest, DescribeIncludesTitle) {
	DeadlineTask task(1, "Купить хлеб", true, "26.10.2026");
	EXPECT_EQ(task.describe(), "Дедлайн 26.10.2026: Купить хлеб");
}

TEST(DeadlineTaskTest, SerializeHasCorrectFormat) {
	DeadlineTask task(1, "Купить хлеб", true, "26.10.2026");
	EXPECT_EQ(task.serialize(), "D|1|Купить хлеб|1|26.10.2026");
}

TEST(DeadlineTaskTest, ToggleFlipsDone) {
	DeadlineTask task(1, "Купить хлеб", true, "26.10.2026");
	EXPECT_TRUE(task.done());
	task.toggle();
	EXPECT_FALSE(task.done());
	task.toggle();
	EXPECT_TRUE(task.done());
}

TEST(RecurringTaskTest, DescribeIncludesTitle) {
	RecurringTask task(1, "Купить хлеб", true, "ежедневно");
	EXPECT_EQ(task.describe(), "Повтор (ежедневно): Купить хлеб");
}

TEST(RecurringTaskTest, SerializeHasCorrectFormat) {
	RecurringTask task(1, "Купить хлеб", true, "ежедневно");
	EXPECT_EQ(task.serialize(), "C|1|Купить хлеб|1|ежедневно");
}

TEST(RecurringTaskTest, ToggleFlipsDone) {
	RecurringTask task(1, "Купить хлеб", true, "ежедневно");
	EXPECT_TRUE(task.done());
	task.toggle();
	EXPECT_FALSE(task.done());
	task.toggle();
	EXPECT_TRUE(task.done());
}