#pragma once
#include <string>
#include <optional>

class Task {
public:
	Task(int id, const std::string& title, bool done);
	virtual ~Task() = default;

	virtual std::string describe() const = 0;
	virtual std::string serialize() const = 0;
	virtual std::optional<std::string> deadline() const { return std::nullopt; }

	void toggle();
	int id() const;
	const std::string& title() const;
	bool done() const;

private:
	int m_id;
	std::string m_title;
	bool m_done;
};

class RegularTask : public Task {
public:
	RegularTask(int id, const std::string& title, bool done);

	std::string describe() const override;
	std::string serialize() const override;
};

class DeadlineTask : public Task {
public:
	DeadlineTask(int id, const std::string& title, bool done, const std::string& deadline);

	std::string describe() const override;
	std::string serialize() const override;
	std::optional<std::string> deadline() const override;
private:
	std::string m_deadline;
};

class RecurringTask : public Task {
public:
	RecurringTask(int id, const std::string& title, bool done, const std::string& period);

	std::string describe() const override;
	std::string serialize() const override;
private:
	std::string m_period;
};