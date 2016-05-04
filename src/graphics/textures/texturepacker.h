#ifndef TEXTUREPACKER_H
#define TEXTUREPACKER_H
#include <inttypes.h>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>

/**
 *  T - image type
 *  SIZE_TYPE - type used to mesure width/height/offset in pixels
 *  GET - function object of type (std::pair<SIZE_TYPE,SIZE_TYPE> f(const T& t))
 *  SET - function object of type (void f(T& t, SIZE_TYPE xoffset, SIZE_TYPE yoffset, SIZE_TYPE borderWidth)) */
template<typename T, typename SIZE_TYPE, typename GET, typename SET>
class packNode
{
    public:
        packNode(SIZE_TYPE width, SIZE_TYPE height,
                 SIZE_TYPE xoff, SIZE_TYPE yoff, SIZE_TYPE border) :
            width(width), height(height), xoff(xoff), yoff(yoff),
            placed(nullptr), right(nullptr), bottom(nullptr), border(border)
            {}

        bool placeImage( T* image, const GET& getSizePred, const SET& setOffsetPred)
        {
            if(placed) {
                if(tryPlaceToRight(image, getSizePred, setOffsetPred)) return true;
                if(tryPlaceToBottom(image, getSizePred, setOffsetPred)) return true; }
            else if(isFit(*image, getSizePred)){
                placed = image;
                setOffsetPred(*placed, xoff, yoff, border);
                return true;}
            return false;
        }

        bool isFit(const T& a, const GET& p)
        {
            const auto border2 = border*2;
            const auto& tsize = p(a);
            return (tsize.first <= width-border2)
                    && (tsize.second <= height-border2);
        }

        SIZE_TYPE width;
        SIZE_TYPE height;
        SIZE_TYPE xoff;
        SIZE_TYPE yoff;
        T*    placed;
        std::unique_ptr<packNode> right;
        std::unique_ptr<packNode> bottom;
        SIZE_TYPE border;

    private:
        bool tryPlaceToRight(T*image, const GET& getSizePred, const SET& setOffsetPred)
        {
            const std::pair<float,float> placedSize = getSizePred(*placed);
            if(right==nullptr)
            {
                const auto placedWidth = placedSize.first + 2*border;
                const auto placedHight = placedSize.second + 2*border;
                const auto newWidth  = width  - placedWidth;
                const auto newHeight = placedHight;
                const auto newXOff = xoff + placedWidth;
                const auto newYOff = yoff;
                right.reset( new packNode(newWidth, newHeight, newXOff, newYOff, border));
            }
            return right->placeImage(image, getSizePred, setOffsetPred);
        }

        bool tryPlaceToBottom(T* image, const GET& getSizePred, const SET& setOffsetPred)
        {
            const std::pair<float,float> placedSize = getSizePred(*placed);
            if(bottom==nullptr)
            {
                const auto placedHight = placedSize.second + 2*border;
                const auto newWidth  = width;
                const auto newHeight = height - placedHight;
                const auto newXOff = xoff;
                const auto newYOff = yoff + placedHight;
                bottom.reset( new packNode(newWidth, newHeight, newXOff, newYOff, border));
            }
            return bottom->placeImage(image, getSizePred, setOffsetPred);
        }
};




template<typename T, typename SIZE_TYPE, typename GET, typename SORT, typename SETOFFSET>
/**
 * Return positions of objects from input packed at rectangle.
 * @param input Input sequence of objects.
 * @param sortPred Predicate to sort input sequence in ascending order.
 * @param getSizePred Predicate that return std::pair of width and height of single object from input sequence.
 * @param width Widht of rectangle.
 * @param height Height of rectangle.
 * @return Vector of positions in rectangle related to input sequence.  */
std::vector<std::shared_ptr<T> >
pack2d(const std::vector<std::shared_ptr<T>>& input, SIZE_TYPE width, SIZE_TYPE height,
                const SORT& sortPred, const GET &getSizePred, const SETOFFSET& setOffset,
                std::vector<std::shared_ptr<T>>& notPlaced, int border=0)
{
    using namespace std;

    vector<shared_ptr<T>> ret;
    auto inputCopy = input;
    sort(inputCopy.begin(), inputCopy.end(), sortPred);
    auto rootNode = packNode<T,SIZE_TYPE,GET,SETOFFSET>(width, height, 0, 0, border);
    for_each( inputCopy.rbegin(), inputCopy.rend(),
                    [&](shared_ptr<T>& current)
                    {
                        if(rootNode.placeImage(current.get(), getSizePred, setOffset))
                            ret.push_back(current);
                        else
                            notPlaced.push_back(current);
                    });
    return ret;
}



#endif // TEXTUREPACKER_H

