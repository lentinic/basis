#include <basis/assert.h>
#include <basis/bitset.h>
#include <basis/timer.h>
#include <iostream>

unsigned ErrorCount = 0;

void test_set_or()
{
	int count = (rand() & 127) + 128;
	basis::bitset a(count);
	for (int i=0; i<count; i++)
	{
		a.set_or(i, (i & 7) == 0 ? 1 : 0);
	}

	int incorrect = 0;
	for (int i=0; i<count; i++)
	{
		BASIS_ASSERT_SIGNAL(a[i] == ((i & 7) == 0 ? 1 : 0), [&]() -> bool {
			incorrect++;
			return false;
		});
	}

	if (incorrect > 0)
	{
		ErrorCount++;
		std::cout<<"Error: There were "<<incorrect<<" incorrectly set bits in test_set_or."<<std::endl;
	}
}

void test_set_xor()
{
	int count = (rand() & 127) + 128;
	basis::bitset a(count);
	for (int i=0; i<count; i++)
	{
		a.set_xor(i, (i & 3) == 0 ? 1 : 0);
		a.set_xor(i, (i & 7) == 0 ? 1 : 0);
	}

	int incorrect = 0;
	for (int i=0; i<count; i++)
	{
		BASIS_ASSERT_SIGNAL(a[i] == (((i & 3) == 0 && (i & 7) != 0) ? 1 : 0), [&]() -> bool {
			incorrect++;
			return false;
		});
	}

	if (incorrect > 0)
	{
		ErrorCount++;
		std::cout<<"Error: There were "<<incorrect<<" incorrectly set bits in test_set_xor."<<std::endl;
	}
}

void test_set_and()
{
	int count = (rand() & 127) + 128;
	basis::bitset a(count);
	a.set_all_one();

	for (int i=0; i<count; i++)
	{
		a.set_and(i, ((i & 3) == 0 && (i % 3) == 0) ? 1 : 0);
	}

	int incorrect = 0;
	for (int i=0; i<count; i++)
	{
		BASIS_ASSERT_SIGNAL(a[i] == (((i & 3) == 0 && (i % 3) == 0) ? 1 : 0), [&]() -> bool {
			incorrect++;
			return false;
		});
	}

	if (incorrect > 0)
	{
		ErrorCount++;
		std::cout<<"Error: There were "<<incorrect<<" incorrectly set bits in test_set_and."<<std::endl;
	}
}

int main()
{
	auto start = basis::GetTimestamp();
	test_set_or();
	test_set_xor();
	test_set_and();
	float elapsed = basis::GetTimeElapsedMS(start) / 1000.f;

	if (ErrorCount)
	{
		std::cout<<"bitset tests completed with "<<ErrorCount<<" error(s) in "<<elapsed<<" seconds."<<std::endl;
	}
	else
	{
		std::cout<<"bitset tests completed successfully in "<<elapsed<<" seconds."<<std::endl;
	}

	return 0;
}
