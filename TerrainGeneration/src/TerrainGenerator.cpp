#include "TerrainGenerator.h"

TerrainGenerator::TerrainGenerator(size_t _dimension) : m_dimension(_dimension) {}

void TerrainGenerator::generate()
{
    noise::module::Perlin perlinModule;

    perlinModule.SetOctaveCount(m_octaves);
    perlinModule.SetFrequency(m_frequency);
    perlinModule.SetPersistence(m_persistence);
    perlinModule.SetLacunarity(m_lacunarity);

    m_heightMap = {};
    for (size_t i =0; i<m_dimension*m_dimension; ++i)
    {
        float height = perlinModule.GetValue(getX(i)/10,getY(i)/10,m_seed);
        m_heightMap.push_back(m_amplitude*height);
    }
}

float TerrainGenerator::getX(const int _index) const
{
    return _index%m_dimension;
}

float TerrainGenerator::getY(const int _index) const
{
    return _index/m_dimension;
}
