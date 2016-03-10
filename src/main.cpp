#include <memory>
#include <iomanip>
#include <string>

using namespace std;

template<typename T>
T randUniform(T low, T high)
{
    return low + static_cast <T> (rand()) /( static_cast <T> (RAND_MAX/(high-low)));
}

float randUniform(){return randUniform(0.0f,1.0f);}


int main(int argc, char** argv)
{
    return 0;
}


