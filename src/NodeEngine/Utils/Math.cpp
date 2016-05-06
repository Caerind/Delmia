#include "Math.hpp"
#include "Assume.hpp"

#include <chrono>
#include <random>

namespace NMath
{

std::mt19937 randomGenerator(std::chrono::system_clock::now().time_since_epoch().count());

int random(int min, int max)
{
	Assume(min <= max);
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(randomGenerator);
}

unsigned int random(unsigned int min, unsigned int max)
{
	Assume(min <= max);
	std::uniform_int_distribution<unsigned int> distribution(min, max);
	return distribution(randomGenerator);
}

float random(float min, float max)
{
	Assume(min <= max);
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(randomGenerator);
}

float randomDev(float middle, float deviation)
{
	Assume(deviation >= 0.f);
	return random(middle-deviation, middle+deviation);
}

void setRandomSeed(unsigned long seed)
{
	randomGenerator.seed(seed);
}

float sin(float deg)
{
    return std::sin(degToRad(deg));
}

float cos(float deg)
{
    return std::cos(degToRad(deg));
}

float tan(float deg)
{
    return std::tan(degToRad(deg));
}

float asin(float value)
{
    return radToDeg(std::asin(value));
}

float acos(float value)
{
    return radToDeg(std::acos(value));
}

float atan2(float y, float x)
{
    return radToDeg(std::atan2(y, x));
}

float pi()
{
    return 3.141592f;
}

float radToDeg(float rad)
{
    return 180.f / pi() * rad;
}

float degToRad(float deg)
{
    return pi() / 180.f * deg;
}

} // namespace NMath
