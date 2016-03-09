#ifndef NMATH_HPP_INCLUDED
#define NMATH_HPP_INCLUDED

namespace NMath
{

int random(int min, int max);
unsigned int random(unsigned int min, unsigned int max);
float random(float min, float max);
float randomDev(float middle, float deviation);
void setRandomSeed(unsigned long seed);

} // namespace NMath

#endif // NMATH_HPP_INCLUDED
