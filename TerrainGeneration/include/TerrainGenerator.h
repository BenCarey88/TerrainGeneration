#ifndef TERRAINGENERATOR_H_
#define TERRAINGENERATOR_H_

#include <iostream>
#include "noiseutils.h"
#include <vector>

class TerrainGenerator
{
public:
    TerrainGenerator()=default;
    TerrainGenerator(size_t _dimension);
    ~TerrainGenerator()=default;
    TerrainGenerator(const TerrainGenerator &)=default;
    TerrainGenerator(TerrainGenerator &&)=default;
    TerrainGenerator & operator =(const TerrainGenerator &)=default;
    TerrainGenerator & operator =(TerrainGenerator &&)=default;

    size_t m_dimension;
    std::vector<float> m_heightMap;

    float m_seed = 0;
    int m_octaves = 3;
    float m_frequency = 1;
    float m_persistence = 0.8f;
    float m_lacunarity = 0.5f;
    float m_amplitude = 10;

    float getX(const int _index) const;
    float getY(const int _index) const;
    void generate();

protected:

};

#endif //TERRAINGENERATOR_H_
