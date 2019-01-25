#include <gtest/gtest.h>
#include "TerrainData.h"


int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}


//--------------------------------------------------------------------------------------------------------------------
///VERTEX STRUCT TESTS
//--------------------------------------------------------------------------------------------------------------------
TEST(Vertex, userCtor)
{
  TerrainData::Vertex vertex(2,3,1.5,5,0.5);
  EXPECT_EQ(vertex.originalX,2);
  EXPECT_EQ(vertex.originalY,3);
  EXPECT_EQ(vertex.originalZ,1.5);
  EXPECT_EQ(vertex.sceneX,0);
  EXPECT_EQ(vertex.sceneY,0.5);
  EXPECT_EQ(vertex.sceneZ,0.75);
}

TEST(Vertex, distanceTo)
{
  TerrainData::Vertex vertex1(2,3,1.5,5,0.5);
  TerrainData::Vertex vertex2(1,1,1,5,1);
  EXPECT_EQ(vertex1.distanceTo(vertex2),vertex2.distanceTo(vertex1));
  EXPECT_FLOAT_EQ(vertex1.distanceTo(vertex2), 1.820027);
}

//--------------------------------------------------------------------------------------------------------------------
///TERRAINDATA TESTS
//--------------------------------------------------------------------------------------------------------------------

TEST(TerrainData, getX)
{
  int dimension = 3;
  std::vector<float> heightMap(9);
  TerrainData terrain(dimension, heightMap);
  EXPECT_EQ(terrain.getX(2),2);
  EXPECT_EQ(terrain.getX(6),0);
}

TEST(TerrainData, getY)
{
  int dimension = 3;
  std::vector<float> heightMap(9);
  TerrainData terrain(dimension, heightMap);
  EXPECT_EQ(terrain.getY(2),0);
  EXPECT_EQ(terrain.getY(6),2);
}

TEST(TerrainData, getHeightMapIndex)
{
  int dimension = 3;
  std::vector<float> heightMap(9);
  TerrainData terrain(dimension, heightMap);
  EXPECT_EQ(terrain.getHeightMapIndex(0,2),6);
  EXPECT_EQ(terrain.getHeightMapIndex(2,1),5);
}

TEST(TerrainData, getVertex)
{
  int dimension = 3;
  std::vector<float> heightMap = {0,1,2,1,1,1,5,3,4};
  TerrainData terrain(dimension, heightMap);
  terrain.m_scale = 2;
  EXPECT_EQ(terrain.getVertex(1).originalX,1);
  EXPECT_EQ(terrain.getVertex(5).originalY,1);
  EXPECT_EQ(terrain.getVertex(7).originalZ,3);
  EXPECT_EQ(terrain.getVertex(4).sceneX,0);
  EXPECT_EQ(terrain.getVertex(3).sceneY,0);
  EXPECT_EQ(terrain.getVertex(0).sceneZ,0);
}

//----------------------------------------DATA-FOR-THE-NEXT-FEW-TESTS--------------------------------------------------//
// A 5 by 5 grid of heightmap values with corresponding delta values, augmented delta values and bounding sphere       //
// radii. The corresponding m_vertices and m_verticesArrangedByGraphLevel lists should store vertices in the orders    //
// given below (where the numbers correspond to the heightMap indexes of the vertices)                                 //
//---------------------------------------------------------------------------------------------------------------------//
// M_VERTICES ORDER: {20,24,4,0,12,10,22,14,2,16,18,8,6,5,11,15,-,17,23,-,21,9,-,19,13,-,3,7,1}                        //
//---------------------------------------------------------------------------------------------------------------------//
// HEIGHT MAP INDICES /// HEIGHT MAP VALUES ///   DELTA VALUES  /// AUGMENTED DELTAS ///    BOUNDING SPHERE RADII      //
//--------------------///-------------------///-----------------///------------------///-------------------------------//
// 0   1   2   3   4  ///   1  2  1  4  3   ///  0  2  2  4  0  ///   0  2  4  4  0  ///  0     0    4.57   0     0    //
// 5   6   7   8   9  ///   1  2  3  1  2   ///  2  2  4  2  1  ///   2  4  4  4  1  ///  0    2.24   0    3.16   0    //
// 10  11  12  13  14 ///   3  4  1  3  2   ///  4  4  0  3  1  ///   6  4  6  3  4  /// 5.86   0    10.1   0    4.89  //
// 15  16  17  18  19 ///   2  4  3  1  4   ///  0  6  1  1  4  ///   0  6  1  4  4  ///  0    4.12   0    3.16   0    //
// 20  21  22  23  24 ///   1  0  4  3  2   ///  0  5  5  0  0  ///   0  5  6  0  0  ///  0     0    6.48   0     0    //
//---------------------------------------------------------------------------------------------------------------------//

TEST(TerrainData, fillQuadTrees)
{
  TerrainData terrain;
  terrain.m_dimension = 5;
  terrain.m_scale = 1;
  terrain.m_maxRefinementLevel = 4;
  terrain.m_heightMap = {1,2,1,4,3,
                         1,2,3,1,2,
                         3,4,1,3,2,
                         2,4,3,1,4,
                         1,0,4,3,2};

  //Resize m_vertices
  terrain.m_vertices.resize(9,TerrainData::Vertex(0,0,0,5, terrain.m_scale));

  //Assign the first 9 vertices of m_vertices manually
  terrain.m_vertices[0] = terrain.getVertex(20);  //SW corner
  terrain.m_vertices[1] = terrain.getVertex(24);  //SE corner
  terrain.m_vertices[2] = terrain.getVertex(4);   //NE corner
  terrain.m_vertices[3] = terrain.getVertex(0);   //NW corner
  terrain.m_vertices[4] = terrain.getVertex(12);  //centre
  terrain.m_vertices[5] = terrain.getVertex(10);  //left middle
  terrain.m_vertices[6] = terrain.getVertex(22);  //bottom middle
  terrain.m_vertices[7] = terrain.getVertex(14);  //right middle
  terrain.m_vertices[8] = terrain.getVertex(2);   //top middle

  //now use quadTree methods to fill the rest
  terrain.createVerticesWQT(12,4,3,1); //fill from root of white quadtree
  terrain.createVerticesBQT(10,5,4,1); //fill from root of first black quadtree
  terrain.createVerticesBQT(22,6,4,1); //fill from root of second black quadtree
  terrain.createVerticesBQT(14,7,4,1); //fill from root of third black quadtree
  terrain.createVerticesBQT(2,8,4,1); //fill from root of fourth black quadtree

  std::vector<size_t> vertexOrder = {20,24,4,0,12,10,22,14,2,16,18,8,6,5,11,15,0,17,23,0,21,9,0,19,13,0,3,7,1};

  for(size_t i =0; i<terrain.m_vertices.size(); ++i)
  {
    EXPECT_EQ(terrain.getHeightMapIndex( terrain.m_vertices[i].originalX, terrain.m_vertices[i].originalY),
                                                                                                 vertexOrder[i]);
  }
}

TEST(TerrainData, getChildren)
{
  TerrainData terrain;
  terrain.m_dimension = 5;
  terrain.m_scale = 1;
  terrain.m_maxRefinementLevel = 4;
  terrain.m_heightMap = {1,2,1,4,3,
                         1,2,3,1,2,
                         3,4,1,3,2,
                         2,4,3,1,4,
                         1,0,4,3,2};

  //vertexOrder = {20,24,4,0,12,10,22,14,2,16,18,8,6,5,11,15,0,17,23,0,21,9,0,19,13,0,3,7,1};
  //Child1 of the vertex at index 2 in the heightMap, using grandparent vertex at index 12 in the heightMap is the vertex at index 8 in the heightMap
  //The vertices at indexes 2,12 and 8 in the heightMap are at indexes 8,4 and 11 in m_vertices (see vertexOrder)
  //Child2 of the same vertices is at index 6 in the heightMap, or index 12 in m_vertices
  EXPECT_EQ(terrain.getChild1(4,8),11);
  EXPECT_EQ(terrain.getChild2(4,8),12);
}

TEST(TerrainData, assignChildren)
{
  TerrainData terrain;
  terrain.m_dimension = 5;
  terrain.m_scale = 1;
  terrain.m_maxRefinementLevel = 4;
  terrain.m_heightMap = {1,2,1,4,3,
                         1,2,3,1,2,
                         3,4,1,3,2,
                         2,4,3,1,4,
                         1,0,4,3,2};
  std::vector<size_t> vertexOrder = {20,24,4,0,12,10,22,14,2,16,18,8,6,5,11,15,0,17,23,0,21,9,0,19,13,0,3,7,1};
  //fill m_vertices using above order
  for(auto i : vertexOrder)
  {
    terrain.m_vertices.push_back(terrain.getVertex(i));
  }

  //resize list
  terrain.m_verticesArrangedByGraphLevel.resize(size_t(terrain.m_maxRefinementLevel),{});
  //add second highest and higest levels of graph to list
  terrain.m_verticesArrangedByGraphLevel[size_t(terrain.m_maxRefinementLevel)-2]={5,6,7,8};
  terrain.m_verticesArrangedByGraphLevel[size_t(terrain.m_maxRefinementLevel)-1]={4};
  //assign children to the West section of the grid
  terrain.assignChildren(4,5,terrain.m_maxRefinementLevel-2);
  //assign children to the South section of the grid
  terrain.assignChildren(4,6,terrain.m_maxRefinementLevel-2);
  //assign children to the East section of the grid
  terrain.assignChildren(4,7,terrain.m_maxRefinementLevel-2);
  //assign children to the North section of the grid
  terrain.assignChildren(4,8,terrain.m_maxRefinementLevel-2);
  //finally update the childList of the central vertex
  terrain.m_vertices[4].childList = {5,6,7,8};

  //Using vertexOrder, we see m_vertices[8] is the vertex at index 2 in the heightMap, so its
  //children are at index 6 and 8 in the heightMap, or 11 and 12 in m_vertices
  ASSERT_EQ(terrain.m_vertices[8].childList[0],11);
  ASSERT_EQ(terrain.m_vertices[8].childList[1],12);
}

TEST(TerrainData, assignDelta)
{
  TerrainData terrain;
  terrain.m_dimension = 5;
  terrain.m_scale = 1;
  terrain.m_maxRefinementLevel = 4;
  terrain.m_heightMap = {1,2,1,4,3,
                         1,2,3,1,2,
                         3,4,1,3,2,
                         2,4,3,1,4,
                         1,0,4,3,2};
  std::vector<size_t> vertexOrder = {20,24,4,0,12,10,22,14,2,16,18,8,6,5,11,15,0,17,23,0,21,9,0,19,13,0,3,7,1};
  std::vector<float> deltaValues  = {0 ,0 ,0,0,0 ,4 ,5 ,1 ,2,6 ,1 ,2,2,2,4 ,0 ,0,1 ,0 ,0,5 ,1,0,4 ,3 ,0,4,4,2};

  //fill m_vertices using above order
  for(auto i : vertexOrder)
  {
    terrain.m_vertices.push_back(terrain.getVertex(i));
  }

  //assign delta values to the West section of the grid
  terrain.assignDelta(0,4,5,2);
  //assign delta values to the South section of the grid
  terrain.assignDelta(1,4,6,2);
  //assign delta values to the East section of the grid
  terrain.assignDelta(2,4,7,2);
  //assign delta values to the North section of the grid
  terrain.assignDelta(3,4,8,2);

  for(size_t i=0; i<terrain.m_vertices.size(); ++i)
  {
    EXPECT_FLOAT_EQ(terrain.m_vertices[i].delta, deltaValues[i]);
  }
}

TEST(TerrainData, assignAugmentedDelta)
{
  TerrainData terrain;
  terrain.m_dimension = 5;
  terrain.m_scale = 1;
  terrain.m_maxRefinementLevel = 4;
  terrain.m_heightMap = {1,2,1,4,3,
                         1,2,3,1,2,
                         3,4,1,3,2,
                         2,4,3,1,4,
                         1,0,4,3,2};
  std::vector<size_t> vertexOrder    = {20,24,4,0,12,10,22,14,2,16,18,8,6,5,11,15,0,17,23,0,21,9,0,19,13,0,3,7,1};
  std::vector<float> augmentedDeltas = {0 ,0 ,0,0,6 ,6 ,6 ,4 ,4,6 ,4 ,4,4,2,4 ,0 ,0,1 ,0 ,0,5 ,1,0,4 ,3 ,0,4,4,2};

  //fill m_vertices using above order
  for(auto i : vertexOrder)
  {
    terrain.m_vertices.push_back(terrain.getVertex(i));
  }

  //assignAugmentedDeltas relies on m_verticesArrangedByGraphLevel and the vertex childList attributes so we need to use assignChildren to fill that:
  //resize list
  terrain.m_verticesArrangedByGraphLevel.resize(size_t(terrain.m_maxRefinementLevel),{});
  //add second highest and higest levels of graph to list
  terrain.m_verticesArrangedByGraphLevel[size_t(terrain.m_maxRefinementLevel)-2]={5,6,7,8};
  terrain.m_verticesArrangedByGraphLevel[size_t(terrain.m_maxRefinementLevel)-1]={4};
  //assign children to the West section of the grid
  terrain.assignChildren(4,5,terrain.m_maxRefinementLevel-2);
  //assign children to the South section of the grid
  terrain.assignChildren(4,6,terrain.m_maxRefinementLevel-2);
  //assign children to the East section of the grid
  terrain.assignChildren(4,7,terrain.m_maxRefinementLevel-2);
  //assign children to the North section of the grid
  terrain.assignChildren(4,8,terrain.m_maxRefinementLevel-2);

  //assign delta values to the West section of the grid
  terrain.assignDelta(0,4,5,2);
  //assign delta values to the South section of the grid
  terrain.assignDelta(1,4,6,2);
  //assign delta values to the East section of the grid
  terrain.assignDelta(2,4,7,2);
  //assign delta values to the North section of the grid
  terrain.assignDelta(3,4,8,2);
  //finally update the childList of the central vertex
  terrain.m_vertices[4].childList = {5,6,7,8};

  terrain.assignAugmentedDelta();
  for(size_t i=0; i<terrain.m_vertices.size(); ++i)
  {
    EXPECT_FLOAT_EQ(terrain.m_vertices[i].augmentedDelta, augmentedDeltas[i]);
  }
}

TEST(TerrainData, assignRadius)
{
  TerrainData terrain;
  terrain.m_dimension = 5;
  terrain.m_scale = 1;
  terrain.m_maxRefinementLevel = 4;
  terrain.m_heightMap = {1,2,1,4,3,
                         1,2,3,1,2,
                         3,4,1,3,2,
                         2,4,3,1,4,
                         1,0,4,3,2};
  std::vector<size_t> vertexOrder         =
      {20,24,4,0,12        ,10       ,22        ,14        ,2         ,16        ,18        ,8         ,6        ,5,11,15,0,17,23,0,21,9,0,19,13,0,3,7,1};
  std::vector<float>  boundingSphereRadii =
      {0 ,0 ,0,0,10.084454f,5.855156f,6.4789028f,4.8943286f,4.5764914f,4.1231055f,3.1622777f,3.1622777f,2.236068f,0,0 ,0 ,0,0 ,0 ,0,0 ,0,0,0 ,0 ,0,0,0,0};

  //fill m_vertices using above order
  for(auto i : vertexOrder)
  {
    terrain.m_vertices.push_back(terrain.getVertex(i));
  }

  //assignRadius relies on m_verticesArrangedByGraphLevel and the vertex childList attributes so we need to use assignChildren to fill that:
  //resize list
  terrain.m_verticesArrangedByGraphLevel.resize(size_t(terrain.m_maxRefinementLevel),{});
  //add second highest and higest levels of graph to list
  terrain.m_verticesArrangedByGraphLevel[size_t(terrain.m_maxRefinementLevel)-2]={5,6,7,8};
  terrain.m_verticesArrangedByGraphLevel[size_t(terrain.m_maxRefinementLevel)-1]={4};
  //assign children to the West section of the grid
  terrain.assignChildren(4,5,terrain.m_maxRefinementLevel-2);
  //assign children to the South section of the grid
  terrain.assignChildren(4,6,terrain.m_maxRefinementLevel-2);
  //assign children to the East section of the grid
  terrain.assignChildren(4,7,terrain.m_maxRefinementLevel-2);
  //assign children to the North section of the grid
  terrain.assignChildren(4,8,terrain.m_maxRefinementLevel-2);
  //finally update the childList of the central vertex
  terrain.m_vertices[4].childList = {5,6,7,8};

  terrain.assignRadius();

  for(size_t i=0; i<terrain.m_vertices.size(); ++i)
  {
    EXPECT_FLOAT_EQ(terrain.m_vertices[i].radius, boundingSphereRadii[i]);
  }
}

//Here I will test that all of the above tests still hold just after constructing the class, since the ctor should call all the above methods
TEST(TerrainData, ctor)
{
  int dimension = 5;
  std::vector<float> heightMap = {1,2,1,4,3,
                                  1,2,3,1,2,
                                  3,4,1,3,2,
                                  2,4,3,1,4,
                                  1,0,4,3,2};
  TerrainData terrain(dimension, heightMap);

  std::vector<size_t> vertexOrder         =
      {20,24,4,0,12        ,10       ,22        ,14        ,2         ,16        ,18        ,8         ,6        ,5,11,15,0,17,23,0,21,9,0,19,13,0,3,7,1};
  //Note that here m_scale is set to 10 whereas in prev examples it was at 1, so the deltas and radii are multiplied by 10 here
  std::vector<float>  boundingSphereRadii =
      {0 ,0 ,0,0,100.84454f,58.55156f,64.789028f,48.943286f,45.764914f,41.231055f,31.622777f,31.622777f,22.36068f,0,0 ,0 ,0,0 ,0 ,0,0 ,0,0,0 ,0 ,0,0,0,0};
  std::vector<float> augmentedDeltas = {0,0,0,0,60,60,60,40,40,60,40,40,40,20,40,0,0,10,0,0,50,10,0,40,30,0,40,40,20};
  std::vector<float> deltaValues  = {0,0,0,0,0,40,50,10,20,60,10,20,20,20,40,0,0,10,0,0,50,10,0,40,30,0,40,40,20};

  for(size_t i =0; i<terrain.m_vertices.size(); ++i)
  {
    EXPECT_EQ(terrain.getHeightMapIndex( terrain.m_vertices[i].originalX, terrain.m_vertices[i].originalY),
                                                                                                 vertexOrder[i]);
  }
  for(size_t i=0; i<terrain.m_vertices.size(); ++i)
  {
    EXPECT_FLOAT_EQ(terrain.m_vertices[i].delta, deltaValues[i]);
  }
  for(size_t i=0; i<terrain.m_vertices.size(); ++i)
  {
    EXPECT_FLOAT_EQ(terrain.m_vertices[i].augmentedDelta, augmentedDeltas[i]);
  }
  for(size_t i=0; i<terrain.m_vertices.size(); ++i)
  {
    EXPECT_FLOAT_EQ(terrain.m_vertices[i].radius, boundingSphereRadii[i]);
  }
}

//---------------------------------------------------------------------------------------------------------------------------------
//MESH-REFINE TESTS
//---------------------------------------------------------------------------------------------------------------------------------

TEST(TerrainData, isActive)
{
  int dimension = 5;
  std::vector<float> heightMap = {1,2,1,4,3,
                                  1,2,3,1,2,
                                  3,4,1,3,2,
                                  2,4,3,1,4,
                                  1,0,4,3,2};
  TerrainData terrain(dimension, heightMap);
  ngl::Vec3 cameraPos(0,0,0);
  EXPECT_TRUE(terrain.isActive(28,cameraPos,0.1f,1));
  EXPECT_TRUE(terrain.isActive(13,cameraPos,0.01f,1));
  EXPECT_FALSE(terrain.isActive(18,cameraPos,10,0.1f));
  EXPECT_FALSE(terrain.isActive(22,cameraPos,0.5,0.1f));
}

// Data: if mesh refine is called with a camera at (0,0,0), a tolerance of 10 and a lamda value of 0.1 on the following heightMap,
// we expect m_indices to list vertices in the following order (where these numbers correspond to the heightMap index of the vertex,
// rather than the m_indices index):
// {20,20,20,22,12,22,18,24,18,14,12,14,8,4,8,2,12,0,12,10,20}

TEST(TerrainData, meshRefine)
{
  int dimension = 5;
  std::vector<float> heightMap = {1,2,1,4,3,
                                  1,2,3,1,2,
                                  3,4,1,3,2,
                                  2,4,3,1,4,
                                  1,0,4,3,2};
  TerrainData terrain(dimension, heightMap);
  ngl::Vec3 cameraPos(0,0,0);
  terrain.meshRefine(cameraPos,10,0.1f);

  std::vector<int> heightMapIndices = {20,20,20,22,12,22,18,24,18,14,12,14,8,4,8,2,12,0,12,10,20};
  for (size_t i = 0; i<terrain.m_indices.size(); ++i)
  {
    EXPECT_EQ(terrain.getHeightMapIndex(terrain.m_vertices[terrain.m_indices[i]].originalX,
                                        terrain.m_vertices[terrain.m_indices[i]].originalY),
              heightMapIndices[i]);
  }
}
