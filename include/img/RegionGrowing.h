#pragma once

#include <img/Image/LabelImage.h>

#include <stack>
#include <iostream>

namespace img {

//!
//! \brief Unseeded region growing using the 8-neighborhood
//!
//! \param f: (int i, int j, int k, int l) return true if a region can grow from
//! pixel (i,j) to pixel (k,l)
//! \return the number of region created
//!
template<class ImageT, typename CompFuncT>
int region_growing(const ImageT& img, LabelImage& labels, CompFuncT&& f, bool verbose = false)
{
    using LabelPair = std::pair<int,int>;
    constexpr int INVALID = -1;

    const int h = img.height();
    const int w = img.width();

    labels.resize(h, w);
    labels.fill(INVALID);

    int label_count = 0;

    for(int i=0; i<h; ++i)
    {
        for(int j=0; j<w; ++j)
        {
            if(labels(i,j) == INVALID)
            {
                const int label_current = label_count;
                ++label_count;

                labels(i,j) = label_current;

                std::stack<LabelPair> stack;
                stack.push(std::make_pair(i,j));

                while(!stack.empty())
                {
                    const int i2 = stack.top().first;
                    const int j2 = stack.top().second;
                    stack.pop();

                    for(int di = -1; di <= +1; ++di)
                    {
                        for(int dj = -1; dj <= +1; ++dj)
                        {
                            const int i3 = i2 + di;
                            const int j3 = j2 + dj;

                            if(0 <= i3 && i3 < h &&
                               0 <= j3 && j3 < w &&
                               labels(i3, j3) == INVALID &&
                               f(i2,j2, i3,j3))
                            {
                                labels(i3, j3) = label_current;
                                stack.push(std::make_pair(i3,j3));
                            }
                        }
                    }
                }
            }
        }
        if(verbose)
        {
            std::cout << "row " << i+1 << "/" << h
                      << " (" << int(float(i)/(h-1)*100) << "%)"
                      << std::endl;
        }
    }
    return label_count;
}

} // namespace img
