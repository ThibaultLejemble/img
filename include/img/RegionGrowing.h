#pragma once

#include <stack>

namespace img {

//!
//! \brief Unseeded region growing using the 8-neighborhood
//!
//! \param f: (int i, int j, int k, int l) return true if a region can grow from
//! pixel (i,j) to pixel (k,l)
//! \return the number of region created
//!
template<class ImageT, class LabelImage, typename CompFuncT>
auto region_growing(const ImageT& img, LabelImage& labels, CompFuncT&& f)
{
    using LabelType = typename LabelImage::Type;
    using LabelPair = std::pair<LabelType,LabelType>;

    constexpr auto INVALID = LabelType(-1);

    const auto h = img.height();
    const auto w = img.width();

    labels.resize(h, w);
    labels.fill(INVALID);

    auto label_count = LabelType(0);

    for(auto i=0; i<h; ++i)
    {
        for(auto j=0; j<w; ++j)
        {
            if(labels(i,j)[0] == INVALID)
            {
                const auto label_current = label_count;
                ++label_count;

                labels(i,j)[0] = label_current;

                std::stack<LabelPair> stack;
                stack.push(std::make_pair(i,j));

                while(!stack.empty())
                {
                    const auto i2 = stack.top().first;
                    const auto j2 = stack.top().second;
                    stack.pop();

                    for(auto di = -1; di <= +1; ++di)
                    {
                        for(auto dj = -1; dj <= +1; ++dj)
                        {
                            const auto i3 = i2 + di;
                            const auto j3 = j2 + dj;

                            if(0 <= i3 && i3 < h &&
                               0 <= j3 && j3 < w &&
                               labels(i3, j3)[0] == INVALID &&
                               f(i2,j2, i3,j3))
                            {
                                labels(i3, j3)[0] = label_current;
                                stack.push(std::make_pair(i3,j3));
                            }
                        }
                    }
                }
            }
        }
    }
    return label_count;
}

} // namespace img
