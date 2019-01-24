#pragma once

#include "terrainData.h"
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <cmath>

using namespace testing;

//--------------------------------------------------------------------------------------------------------------------
///VERTEX STRUCT TESTS
//--------------------------------------------------------------------------------------------------------------------
TEST(terrainData::Vertex, userCtor)
{
  TerrainData::Vertex vertex(2,3,1.5,5,0.5);
  EXPECT_EQ(vertex.originalX,2);
  EXPECT_EQ(vertex.originalY,3);
  EXPECT_EQ(vertex.originalZ,1.5);
  EXPECT_EQ(vertex.screenX,0);
  EXPECT_EQ(vertex.screenY,0.5);
  EXPECT_EQ(vertex.screenZ,2.5);
}

TEST(terrainData::Vertex, distanceTo)
{
  TerrainData::Vertex vertex1(2,3,1.5,5,0.5);
  TerrainData::Vertex vertex2(1,1,1,5,1);
  EXPECT_EQ(vertex1.distanceTo(vertex2),vertex2.distanceTo(vertex1));
  EXPECT_FLOAT_EQ(vertex1.distanceTo(vertex2), std::sqrt(22.0f)*0.5f);
}

//--------------------------------------------------------------------------------------------------------------------
///GENERIC PRIVATE MEMBER FUNCTION TESTS
//--------------------------------------------------------------------------------------------------------------------

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

TEST(terrainData, getVertex)
{
  size_t dimension = 3;
  std::vector<float> heightMap = {0,1,2,1,1,1,2,3,4};
  TerrainData terrain(dimension, heightMap);
  EXPECT_EQ(terrain.getVertex(1).originalX,1);
  EXPECT_EQ(terrain.getVertex(5).originalY,2);
  EXPECT_EQ(terrain.getVertex(7).originalZ,2);
  EXPECT_EQ(terrain.getVertex(4).sceneX,(50.0/3.0)*);
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
