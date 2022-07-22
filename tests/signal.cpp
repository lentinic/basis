#include <basis/assert.h>
#include <basis/signal.h>

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

    auto h0 = sig.connect(&Handler0);
    auto h1 = sig.connect(&Handler1);
    auto h2 = sig.connect(&Handler2);
    auto h3 = sig.connect(&Handler3);
    auto h4 = sig.connect(&Handler3);

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
