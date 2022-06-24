#include <basis/assert.h>
#include <basis/signal.h>
#include <iostream>

using namespace std;


int number = 0;

void Handler0()
{
    number |= 1;
}

void Handler1()
{
    number |= 2;
}

void Handler2()
{
    number |= 4;
}

void Handler3()
{
    number ^= 8;
}

int main(int argc, char * argv[])
{
    BASIS_UNUSED(argc);
    BASIS_UNUSED(argv);
    
    basis::signal<void()> sig;

    basis::handle h0 = sig.connect(&Handler0);
    basis::handle h1 = sig.connect(&Handler1);
    basis::handle h2 = sig.connect(&Handler2);
    basis::handle h3 = sig.connect(&Handler3);
    basis::handle h4 = sig.connect(&Handler3);

    BASIS_ASSERT(h0.id != h1.id);
    BASIS_ASSERT(h0.id != h2.id);
    BASIS_ASSERT(h0.id != h3.id);
    BASIS_ASSERT(h0.id != h4.id);
    BASIS_ASSERT(h1.id != h2.id);
    BASIS_ASSERT(h1.id != h3.id);
    BASIS_ASSERT(h1.id != h4.id);
    BASIS_ASSERT(h2.id != h3.id);
    BASIS_ASSERT(h2.id != h4.id);
    BASIS_ASSERT(h3.id != h4.id);

    sig();

    BASIS_ASSERT(number == 7);

    sig.disconnect(h3);

    sig();

    BASIS_ASSERT(number == 15);

    sig.reset();

    sig();

    BASIS_ASSERT(number == 15);

    return 0;
}
