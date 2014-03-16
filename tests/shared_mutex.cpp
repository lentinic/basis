#include <basis/shared_mutex.h>
#include <basis/unit_test.h>
#include <vector>
#include <thread>

void test_1w5r();
void test_5w10r();
void test_10w20r();

BASIS_TEST_LIST_BEGIN()
BASIS_DECLARE_TEST(test_1w5r)
BASIS_DECLARE_TEST(test_5w10r)
BASIS_DECLARE_TEST(test_10w20r)
BASIS_TEST_LIST_END()

class start_trigger
{
public:
	start_trigger()
		:	m_triggered(false)
	{}

	void wait()
	{
		if (!m_triggered)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			while (!m_triggered)
			{
				m_condition.wait(lock);
			}
		}
	}

	void signal()
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_triggered = true;
		}
		m_condition.notify_all();
	}

private:
	std::atomic<bool> m_triggered;
	std::mutex m_mutex;
	std::condition_variable m_condition;
};

template<int WRITERS, int READERS, int ITERATIONS>
void test_template()
{
	start_trigger start;
	basis::shared_mutex mutex;
	std::vector<int> array;

	std::thread writer[WRITERS];
	for (int i=0; i<WRITERS; i++)
	{
		writer[i] = std::thread([&]() -> void {
			start.wait();

			for (int j=0; j<ITERATIONS; j++)
			{
				mutex.lock();
				array.push_back((int)array.size());
				mutex.unlock();
			}
		});
	}

	std::thread reader[READERS];
	for (int i=0; i<READERS; i++)
	{
		reader[i] = std::thread([&]() -> void {
			start.wait();

			int last = 0;
			std::vector<int> local;
			while (last < ITERATIONS)
			{
				mutex.lock_shared();
				for (; last<array.size(); last++)
				{
					local.push_back(array[last]);
				}
				mutex.unlock_shared();
			}

			for (int j=0; j<ITERATIONS; j++)
			{
				BASIS_TEST_VERIFY(local[j] == j);
			}
		});
	}

	start.signal();
	
	for (int i=0; i<WRITERS; i++)
	{
		writer[i].join();
	}

	for (int i=0; i<READERS; i++)
	{
		reader[i].join();
	}
}

void test_1w5r()
{
	test_template<1,5,10000>();
}

void test_5w10r()
{
	test_template<5,10,10000>();
}

void test_10w20r()
{
	test_template<10,20,10000>();
}

int main()
{
	BASIS_RUN_TESTS()
	return 0;
}
