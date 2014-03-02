#include <basis/bitset.h>
#include <basis/unit_test.h>

void test_set_or();
void test_set_xor();
void test_set_and();

BASIS_TEST_LIST_BEGIN()
	BASIS_DECLARE_TEST(test_set_or)
	BASIS_DECLARE_TEST(test_set_xor)
	BASIS_DECLARE_TEST(test_set_and)
BASIS_TEST_LIST_END()

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

		if (a[i] != ((i & 7) == 0 ? 1 : 0))
		{
			incorrect++;
		}
	}

	BASIS_TEST_VERIFY_MSG(incorrect == 0, "%d incorrectly set bits", incorrect);
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
		if (a[i] != (((i & 3) == 0 && (i & 7) != 0) ? 1 : 0))
		{
			incorrect++;
		}
	}

	BASIS_TEST_VERIFY_MSG(incorrect == 0, "%d incorrectly set bits", incorrect);
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
		if (a[i] != (((i & 3) == 0 && (i % 3) == 0) ? 1 : 0))
		{
			incorrect++;
		}
	}

	BASIS_TEST_VERIFY_MSG(incorrect == 0, "%d incorrectly set bits", incorrect);
}

int main()
{
	BASIS_RUN_TESTS()

	return 0;
}
