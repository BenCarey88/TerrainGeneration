#pragma once

#include "terrainData.h"
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using namespace testing;

TEST(terrainData, getX)
{
    size_t dimension = 3;
    std::vector<float> heightMap = {0,0,0,1,1,1,2,2,2};
    TerrainData terrain(dimension, heightMap);
    EXPECT_EQ(terrain.getX(2),0);
    EXPECT_EQ(terrain.getX(3),1);
}

TEST(terrainData, getY)
{
    size_t dimension = 3;
    std::vector<float> heightMap = {0,0,0,1,1,1,2,2,2};
    TerrainData terrain(dimension, heightMap);
    EXPECT_EQ(terrain.getY(2),2);
    EXPECT_EQ(terrain.getY(3),0);
}

TEST(terrainData, getHeightMapIndex)
{
    size_t dimension = 3;
    std::vector<float> heightMap = {0,0,0,1,1,1,2,2,2};
    TerrainData terrain(dimension, heightMap);
    EXPECT_EQ(terrain.getHeightMapIndex(0,2),2);
    EXPECT_EQ(terrain.getHeightMapIndex(2,1),7);
}

/*TEST(terrainData,getChildren)
{
    size_t dimension = 5;
    std::vector<float> heightMap(25);
    TerrainData terrain(dimension, heightMap);

    terrain.assignChildren(terrain.m_heightMap[2],terrain.m_heightMap[12],1,2,1);
    terrain.assignChildren(terrain.m_heightMap[14],terrain.m_heightMap[12],1,2,1);
    terrain.assignChildren(terrain.m_heightMap[22],terrain.m_heightMap[12],1,2,1);
    terrain.assignChildren(terrain.m_heightMap[10],terrain.m_heightMap[12],1,2,1);

    EXPECT_TRUE(terrain.m_heightMap[6].children_of_parent1[0]==terrain.m_heightMap[1]);
    EXPECT_TRUE(terrain.m_heightMap[18].children_of_parent2[1]==terrain.m_heightMap[19]);
    EXPECT_EQ(terrain.m_heightMap[1].children_of_parent2.size(),0);
}

TEST(terrainData,userCtor)
{
    size_t dimension = 5;
    std::vector<float> heightMap(25);
    TerrainData terrain(dimension, heightMap);

    EXPECT_TRUE(terrain.m_heightMap[6].children_of_parent1[0]==terrain.m_heightMap[1]);
    EXPECT_TRUE(terrain.m_heightMap[18].children_of_parent2[1]==terrain.m_heightMap[19]);
    EXPECT_EQ(terrain.m_heightMap[1].children_of_parent2.size(),0);
}

TEST(terrainData, vertexChildren)
{
    size_t dimension = 5;
    std::vector<float> heightMap(25);
    TerrainData terrain(dimension, heightMap);

    EXPECT_TRUE(terrain.m_heightMap[6].child1(terrain.m_heightMap[2])==terrain.m_heightMap[1]);
    EXPECT_TRUE(terrain.m_heightMap[18].child2(terrain.m_heightMap[14])==terrain.m_heightMap[19]);
}*/
