#include <basis/bitset.h>
#include <basis/unit_test.h>

void test_push_get_pop();
void test_set_toggle();
void test_bitwise_operators();

BASIS_TEST_LIST_BEGIN()
	BASIS_DECLARE_TEST(test_push_get_pop)
	BASIS_DECLARE_TEST(test_set_toggle)
	BASIS_DECLARE_TEST(test_bitwise_operators)
BASIS_TEST_LIST_END()

void test_push_get_pop()
{
	int disagree = 0;
	int incorrect = 0;
	int count = (rand() & 127) + 128;
	basis::bitset a;

	for (int i=0; i<count; i++)
	{
		a.push_back((i & 7) == 0 ? 1 : 0);
	}

	for (int i=(count-1); i>=0; i--)
	{
		bool a0 = a[i];
		bool a1 = a.pop_back();

		if (a0 != a1)
		{
			disagree++;
		}

		if (a0 != ((i & 7) == 0 ? 1 : 0))
		{
			incorrect++;
		}
	}

	BASIS_TEST_VERIFY_MSG(a.count() == 0, "bitset has an unexpected size - %d; expected 0", a.count());
	BASIS_TEST_VERIFY_MSG(disagree == 0, "get_bit disagreed with pop_back %d times", disagree);
	BASIS_TEST_VERIFY_MSG(incorrect == 0, "%d incorrectly set bits", incorrect);
}

void test_set_toggle()
{
	int incorrect = 0;
	int count = (rand() & 127) + 128;
	basis::bitset a(count);

	for (int i=0; i<count; i++)
	{
		a.set_bit(i, (i & 3) == 0 ? 1 : 0);
		if ((i & 7) == 0)
		{
			a.toggle_bit(i);
		}
	}

	for (int i=0; i<count; i++)
	{
		if (a[i] != (((i & 3) == 0 && (i & 7) != 0) ? 1 : 0))
		{
			incorrect++;
		}
	}

	BASIS_TEST_VERIFY_MSG(incorrect == 0, "%d incorrectly set bits", incorrect);
}

void test_bitwise_operators()
{
	int count = (rand() & 127) + 128;
	int xor_incorrect = 0;
	int or_incorrect = 0;
	int and_incorrect = 0;

	basis::bitset a(count);
	basis::bitset b(count);
	basis::bitset c(count * 2);
	basis::bitset d(count * 2);

	for (int i=0; i<count; i++)
	{
		a.set_bit(i, (i % 2) == 0);
		b.set_bit(i, (i % 5) == 0);
	}

	for (int i=0; i<(count*2); i++)
	{
		c.set_bit(i, (i % 3) == 0);
		d.set_bit(i, (i % 7) == 0);
	}

	a = a ^ b; // multiple of 2 or 5 but not both

	BASIS_TEST_VERIFY_MSG(a.count() == count, "bitset 'a' had an unexpected size - %d; expected - %d", a.count(), count);

	for (int i=0; i<count; i++)
	{
		if(a[i] != (((i % 2) == 0 && (i % 5) != 0) || ((i % 2) != 0 && (i % 5)  == 0)))
		{
			xor_incorrect++;
		}
	}

	BASIS_TEST_VERIFY_MSG(xor_incorrect == 0, "%d incorrect bits set during bitwise xor", xor_incorrect);

	c = c | d; // all multiples of 3 and 7

	BASIS_TEST_VERIFY_MSG(c.count() == (count*2), "bitset 'c' had an unexpected size - %d; expected - %d", c.count(), (count*2));

	for (int i=0; i<(count*2); i++)
	{
		if (c[i] != ((i % 3) == 0 || (i % 7) == 0))
		{
			or_incorrect++;
		}
	}

	BASIS_TEST_VERIFY_MSG(or_incorrect == 0, "%d incorrect bits set during bitwise or", or_incorrect);

	d = c & a; // all multiples of 3 and 7 that are either multiples of 2 or 5 (but not both) and that are less than count

	BASIS_TEST_VERIFY_MSG(d.count() == (count*2), "bitset 'd' had an unexpected size - %d; expected - %d", d.count(), (count*2));

	for (int i=0; i<(count*2); i++)
	{
		if (d[i] != (i < count) &&
					((i % 3) == 0 || (i % 7) == 0) && 
					(((i % 2) == 0 && (i % 5) != 0) || ((i % 2) != 0 && (i % 5) == 0)))
		{
			and_incorrect++; 
		}
	}

	BASIS_TEST_VERIFY_MSG(and_incorrect == 0, "%d incorrect bits set during bitwise and", and_incorrect);
}

int main()
{
	BASIS_RUN_TESTS()

	return 0;
}
