#ifndef NMATH_HPP_INCLUDED
#define NMATH_HPP_INCLUDED

namespace NMath
{

int random(int min, int max);
unsigned int random(unsigned int min, unsigned int max);
float random(float min, float max);
float randomDev(float middle, float deviation);
void setRandomSeed(unsigned long seed);

float sin(float deg);
float cos(float deg);
float tan(float deg);
float asin(float value);
float acos(float value);
float atan2(float y, float x);
float pi();
float radToDeg(float rad);
float degToRad(float deg);

} // namespace NMath

#endif // NMATH_HPP_INCLUDED
