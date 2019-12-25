#pragma once

/**
 * Random number generator.
 * Makes use of the random classes in the C++11 standard template library.
 */

#include <random>

class ZN_API Random
{
public:
    // Initialize the random generator with
    // an initial seed. Default is 0. 
    // If you require non-deterministic random-number generator,
    // Then seed should be something like the current time.
    // Consider using std::time() to seed the random number generator.
    Random(int seed = 1);

    // Generate a random number.
    int32_t NextInt() const;

    // Generate a non-negative random number.
    uint32_t NextUInt() const;

    // Generates a random float in the range [0, 1]
    float NextFloat() const;

    // Generates a random number in the specified range
    float Range(const float min, const float max) const;

    // Generates a random vec2 (non-normalized)
    // Each component is in the range [0, 1].
    vec2 NextVec2f() const;

    // Generates a random vec3 (non-normalized)
    // Each component is in the range [0, 1].
    vec3 NextVec3f() const;

    // Generate a 2D random unit vector.
    vec2 UnitVector2f() const;

    // Generate a 3D random unit vector.
    vec3 UnitVector3f() const;

    // Generate a random unit vector in the hemisphere 
    // of the passed-in normal vector.
    vec3 Hemisphere(cvec3 normal);

private:
    // Mersene Twister 19937 generator (64 bit) 
    mutable std::mt19937_64 m_RandomGenerator;

    std::uniform_int_distribution<int> m_IntDistribution;
    std::uniform_int_distribution<uint32_t> m_UIntDistribution;
    std::uniform_real_distribution<float> m_FloatDistribution;
};