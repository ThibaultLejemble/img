#include <img/ImageT.h>

#include <iostream>

using namespace img;

int main()
{
    constexpr auto N = 256;
    ImageRGBAd image(N,N);
    for (int i = 0; i < image.height(); ++i) {
        for (int j = 0; j < image.width(); ++j) {
            const double u = double(i)/(image.height()-1);
            const double v = double(j)/(image.width()-1);

            const double r = 0.5 * (1.0 + std::cos(M_PI * (1+1*v) * u));
            const double g = 0.5 * (1.0 + std::cos(M_PI * (1+2*u) * v));
            const double b = 0.5 * (1.0 + std::cos(M_PI * (1+4*v) * u));
            const double a = 0.5 * (1.0 + std::cos(M_PI * (1-u)*v));

            image(i,j)[0] = r;
            image(i,j)[1] = g;
            image(i,j)[2] = b;
            image(i,j)[3] = a;
        }
    }

    save("exmaple0_Gi.png",    ImageGi   (image));
    save("exmaple0_Gf.png",    ImageGf   (image));
    save("exmaple0_Gd.png",    ImageGd   (image));
    save("exmaple0_GAi.png",   ImageGAi  (image));
    save("exmaple0_GAf.png",   ImageGAf  (image));
    save("exmaple0_GAd.png",   ImageGAd  (image));
    save("exmaple0_RGBi.png",  ImageRGBi (image));
    save("exmaple0_RGBf.png",  ImageRGBf (image));
    save("exmaple0_RGBd.png",  ImageRGBd (image));
    save("exmaple0_RGBAi.png", ImageRGBAi(image));
    save("exmaple0_RGBAf.png", ImageRGBAf(image));
    save("exmaple0_RGBAd.png", ImageRGBAd(image));

    {
        {ImageGi    image2; load("exmaple0_Gi.png",    image2); save("exmaple0_Gi2.png",    image2);}
        {ImageGf    image2; load("exmaple0_Gf.png",    image2); save("exmaple0_Gf2.png",    image2);}
        {ImageGd    image2; load("exmaple0_Gd.png",    image2); save("exmaple0_Gd2.png",    image2);}
        {ImageGAi   image2; load("exmaple0_GAi.png",   image2); save("exmaple0_GAi2.png",   image2);}
        {ImageGAf   image2; load("exmaple0_GAf.png",   image2); save("exmaple0_GAf2.png",   image2);}
        {ImageGAd   image2; load("exmaple0_GAd.png",   image2); save("exmaple0_GAd2.png",   image2);}
        {ImageRGBi  image2; load("exmaple0_RGBi.png",  image2); save("exmaple0_RGBi2.png",  image2);}
        {ImageRGBf  image2; load("exmaple0_RGBf.png",  image2); save("exmaple0_RGBf2.png",  image2);}
        {ImageRGBd  image2; load("exmaple0_RGBd.png",  image2); save("exmaple0_RGBd2.png",  image2);}
        {ImageRGBAi image2; load("exmaple0_RGBAi.png", image2); save("exmaple0_RGBAi2.png", image2);}
        {ImageRGBAf image2; load("exmaple0_RGBAf.png", image2); save("exmaple0_RGBAf2.png", image2);}
        {ImageRGBAd image2; load("exmaple0_RGBAd.png", image2); save("exmaple0_RGBAd2.png", image2);}
    }


    return 1;
}
