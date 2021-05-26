#include <img/Image/ImageT.h>

using namespace img;

struct Foo {};
void bar(Foo){}

int main()
{
    constexpr auto N = 32;

    img::ImageRGBAf rgba(N, N);
    img::ImageGd gray;
    img::cast(rgba, gray);



    return 1;
}
