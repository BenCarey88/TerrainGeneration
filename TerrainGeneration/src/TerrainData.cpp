#include <iostream>
#include <cmath>
#include <algorithm>
#include "TerrainData.h"

TerrainData::TerrainData(size_t _dimension, std::vector<float> _heightMap) :
    m_dimension(_dimension), m_heightMap(_heightMap)
{
    std::cout<<"Initialising...\n";

    /*for (size_t i=0; i<_heightMap.size(); ++i)
    {
        m_heightMap.push_back(Vertex(getX(i),getY(i),_heightMap[i]));
    }*/

    m_vertices.resize(9,Vertex(0,0,0,m_dimension, m_scale));

    m_vertices[0] = getVertex(m_dimension*m_dimension - m_dimension);          //SW corner
    m_vertices[1] = getVertex(m_dimension*m_dimension - 1);                    //SE corner
    m_vertices[2] = getVertex(m_dimension - 1);                                //NE corner
    m_vertices[3] = getVertex(0);                                              //NW corner
    m_vertices[4] = getVertex((m_dimension*m_dimension - 1)/2);                //centre
    m_vertices[5] = getVertex((m_dimension*m_dimension - m_dimension)/2);      //left middle
    m_vertices[6] = getVertex(m_dimension*m_dimension - (m_dimension+1)/2);    //bottom middle
    m_vertices[7] = getVertex((m_dimension*m_dimension + m_dimension - 2)/2);  //right middle
    m_vertices[8] = getVertex((m_dimension - 1)/2);                            //top middle

    std::cout<<"Filling Vertexlist...\n";

    createVerticesWQT((m_dimension*m_dimension - 1)/2,               4,3,(m_dimension-1)/4);

    //std::cout<<"checkpoint2"<<'\n';

    createVerticesBQT((m_dimension*m_dimension - m_dimension)/2,     5,4,(m_dimension-1)/4);
    createVerticesBQT(m_dimension*m_dimension - (m_dimension+1)/2,   6,4,(m_dimension-1)/4);
    createVerticesBQT((m_dimension*m_dimension + m_dimension - 2)/2, 7,4,(m_dimension-1)/4);
    createVerticesBQT((m_dimension - 1)/2,                           8,4,(m_dimension-1)/4);

    std::cout<<"Assigning Children...\n";

    m_verticesArrangedByGraphLevel.resize(m_maxRefinementLevel,{});

    assignChildren(4,5,m_maxRefinementLevel-2);
    assignChildren(4,6,m_maxRefinementLevel-2);
    assignChildren(4,7,m_maxRefinementLevel-2);
    assignChildren(4,8,m_maxRefinementLevel-2);
    m_vertices[4].childList = {5,6,7,8};
    m_verticesArrangedByGraphLevel[m_maxRefinementLevel-2]={5,6,7,8};
    m_verticesArrangedByGraphLevel[m_maxRefinementLevel-1]={4};

    std::cout<<"Assigning Bounding Sphere Radii...\n";

    assignRadius();

    std::cout<<"Assigning Delta Values...\n";

    assignDelta(0,4,5,2);
    assignDelta(1,4,6,2);
    assignDelta(2,4,7,2);
    assignDelta(3,4,8,2);

    std::cout<<"Assigning Augmented Delta Values...\n";

    //LONG BIT:
    assignAugmentedDelta();

    std::cout<<"Done \n";

/*  m_heightMap[m_vertices[4]].radius = std::max
            ({
                distance(m_heightMap[m_vertices[4]],m_heightMap[m_vertices[5]])
                                                            + m_heightMap[m_vertices[5]].radius,
                distance(m_heightMap[m_vertices[4]],m_heightMap[m_vertices[6]])
                                                            + m_heightMap[m_vertices[6]].radius,
                distance(m_heightMap[m_vertices[4]],m_heightMap[m_vertices[5]])
                                                            + m_heightMap[m_vertices[7]].radius,
                distance(m_heightMap[m_vertices[4]],m_heightMap[m_vertices[5]])
                                                            + m_heightMap[m_vertices[8]].radius
             });*/
}

size_t TerrainData::getX(const size_t _index) const
{
    return _index%m_dimension;
}
size_t TerrainData::getY(const size_t _index) const
{
    return _index/m_dimension; //note that since these are both integers, we should get the floor of the fraction as required
}
size_t TerrainData::getHeightMapIndex(const size_t _x, const size_t _y) const
{
    return _y*m_dimension+_x;
}
TerrainData::Vertex TerrainData::getVertex(const size_t _index) const
{
    return Vertex(getX(_index), getY(_index), m_heightMap[_index], m_dimension, m_scale);
}

void TerrainData::createVerticesWQT(size_t _QTParentHeightMapIndex, size_t _QTParentVerticesIndex,
                                                 size_t _refinementLevel, size_t _distance)
{
    if(_refinementLevel<m_maxRefinementLevel)
    {
        size_t x = getX(_QTParentHeightMapIndex);
        size_t y = getY(_QTParentHeightMapIndex);

        size_t indexVertexStorageChild1 = 4 * _QTParentVerticesIndex - 7;
        size_t indexVertexStorageChild2 = indexVertexStorageChild1 + 1;
        size_t indexVertexStorageChild3 = indexVertexStorageChild2 + 1;
        size_t indexVertexStorageChild4 = indexVertexStorageChild3 + 1;
        if(m_vertices.size()<indexVertexStorageChild4+1)
            m_vertices.resize(indexVertexStorageChild4+1,Vertex(0,0,0,m_dimension,m_scale));

        size_t indexHeightMapChild1 = getHeightMapIndex(x - _distance, y + _distance);
        size_t indexHeightMapChild2 = getHeightMapIndex(x + _distance, y + _distance);
        size_t indexHeightMapChild3 = getHeightMapIndex(x + _distance, y - _distance);
        size_t indexHeightMapChild4 = getHeightMapIndex(x - _distance, y - _distance);

        m_vertices[indexVertexStorageChild1] = getVertex(indexHeightMapChild1);
        m_vertices[indexVertexStorageChild2] = getVertex(indexHeightMapChild2);
        m_vertices[indexVertexStorageChild3] = getVertex(indexHeightMapChild3);
        m_vertices[indexVertexStorageChild4] = getVertex(indexHeightMapChild4);

        createVerticesWQT(indexHeightMapChild1, indexVertexStorageChild1,
                                       _refinementLevel + 2, _distance/2);
        createVerticesWQT(indexHeightMapChild2, indexVertexStorageChild2,
                                       _refinementLevel + 2, _distance/2);
        createVerticesWQT(indexHeightMapChild3, indexVertexStorageChild3,
                                       _refinementLevel + 2, _distance/2);
        createVerticesWQT(indexHeightMapChild4, indexVertexStorageChild4,
                                       _refinementLevel + 2, _distance/2);
    }
}

void TerrainData::createVerticesBQT(size_t _QTParentHeightMapIndex, size_t _QTParentVertexStorageIndex,
                                                 size_t _refinementLevel, size_t _distance)
{
    if(_refinementLevel<=m_maxRefinementLevel)
    {
        size_t x = getX(_QTParentHeightMapIndex);
        size_t y = getY(_QTParentHeightMapIndex);

        size_t indexVertexStorageChild1 = 4 * _QTParentVertexStorageIndex - 7;
        size_t indexVertexStorageChild2 = indexVertexStorageChild1 + 1;
        size_t indexVertexStorageChild3 = indexVertexStorageChild2 + 1;
        size_t indexVertexStorageChild4 = indexVertexStorageChild3 + 1;
        if(m_vertices.size()<indexVertexStorageChild4+1)
            m_vertices.resize(indexVertexStorageChild4+1,Vertex(0,0,0,m_dimension, m_scale));

        if(y>0)
        {
            size_t indexHeightMapChild1 = getHeightMapIndex(x, y - _distance);
            m_vertices[indexVertexStorageChild1] = getVertex(indexHeightMapChild1);
            createVerticesBQT(indexHeightMapChild1, indexVertexStorageChild1,
                                           _refinementLevel + 2, _distance/2);
        }
        if(x<m_dimension-1)
        {
            size_t indexHeightMapChild2 = getHeightMapIndex(x + _distance, y);
            m_vertices[indexVertexStorageChild2] = getVertex(indexHeightMapChild2);
            createVerticesBQT(indexHeightMapChild2, indexVertexStorageChild2,
                                           _refinementLevel + 2, _distance/2);
        }
        if(y<m_dimension-1)
        {
            size_t indexHeightMapChild3 = getHeightMapIndex(x, y + _distance);
            m_vertices[indexVertexStorageChild3] = getVertex(indexHeightMapChild3);
            createVerticesBQT(indexHeightMapChild3, indexVertexStorageChild3,
                                           _refinementLevel + 2, _distance/2);
        }
        if(x>0)
        {
            size_t indexHeightMapChild4 = getHeightMapIndex(x - _distance, y);
            m_vertices[indexVertexStorageChild4] = getVertex(indexHeightMapChild4);
            createVerticesBQT(indexHeightMapChild4, indexVertexStorageChild4,
                                           _refinementLevel + 2, _distance/2);
        }
    }
}

size_t TerrainData::getChild1(size_t _QTParent, size_t _DAGParent) const
{
    size_t child1 = 4 * _QTParent + (2 * _QTParent + _DAGParent - 6) % 4 - 7;
    return child1;
}
size_t TerrainData::getChild2(size_t _QTParent, size_t _DAGParent) const
{
    size_t child2 = 4 * _QTParent + (2 * _QTParent + _DAGParent - 5) % 4 - 7;
    return child2;
}

void TerrainData::assignChildren(size_t _DAGParent, size_t _currentVertex, size_t _refinementLevel)
{
    if(_refinementLevel != 0)
    {
        size_t child1 = getChild1(_DAGParent,_currentVertex);
        size_t child2 = getChild2(_DAGParent,_currentVertex);
        m_vertices[_currentVertex].childList.push_back(child1);
        m_vertices[_currentVertex].childList.push_back(child2);
        assignChildren(_currentVertex, child1, _refinementLevel-1);
        assignChildren(_currentVertex, child2, _refinementLevel-1);

        m_verticesArrangedByGraphLevel[_refinementLevel-1].push_back(child1);
        m_verticesArrangedByGraphLevel[_refinementLevel-1].push_back(child2);
    }
}

void TerrainData::assignRadius()
{
    /*if(_refinementLevel == 1)
        m_vertices[_vertex].radius = 0;
    else
    {
        Vertex v = m_vertices[_vertex];
        for(auto i : v.childList)
        {
            assignRadius(i, _refinementLevel-1);
            Vertex w = m_vertices[i];
            if(m_vertices[_vertex].radius < v.distanceTo(w) + w.radius)
                m_vertices[_vertex].radius = v.distanceTo(w) + w.radius;
        }
    }*/
    for (auto list : m_verticesArrangedByGraphLevel)
    {
        for (auto i : list)
        {
            Vertex v = m_vertices[i];
            for (auto j : v.childList)
            {
                Vertex w = m_vertices[j];

                if(m_vertices[i].radius < v.distanceTo(w) + w.radius)
                    m_vertices[i].radius = v.distanceTo(w) + w.radius;
            }
        }
    }
}

void TerrainData::assignDelta(size_t _DAGGrandParent, size_t _DAGParent, size_t _currentVertex, size_t _refinementLevel)
{
    if(_refinementLevel <= m_maxRefinementLevel)
    {
        Vertex B = m_vertices[_currentVertex];
        Vertex A = m_vertices[_DAGGrandParent];
        Vertex C = getVertex(getHeightMapIndex(2*B.originalX-A.originalX, 2*B.originalY - A.originalY));
        m_vertices[_currentVertex].delta = std::abs(2*B.sceneZ - C.sceneZ - A.sceneZ);
        assignDelta(_DAGParent,_currentVertex,getChild1(_DAGParent,_currentVertex),_refinementLevel+1);
        assignDelta(_DAGParent,_currentVertex,getChild2(_DAGParent,_currentVertex),_refinementLevel+1);
    }
}

void TerrainData::assignAugmentedDelta()
{
    /*m_vertices[_vertex].augmentedDelta = m_vertices[_vertex].delta;
    if(_refinementLevel < m_maxRefinementLevel)
    {
        Vertex v = m_vertices[_vertex];
        for(auto i : v.childList)
        {
            assignAugmentedDelta(i, _refinementLevel+1);
            Vertex w = m_vertices[i];
            if(m_vertices[_vertex].augmentedDelta < w.augmentedDelta)
                m_vertices[_vertex].augmentedDelta = w.augmentedDelta;
        }
    }*/

    for (auto list : m_verticesArrangedByGraphLevel)
    {
        for (auto i : list)
        {
            m_vertices[i].augmentedDelta = m_vertices[i].delta;
            for (auto j : m_vertices[i].childList)
            {
                Vertex w = m_vertices[j];
                if(m_vertices[i].augmentedDelta < w.augmentedDelta)
                        m_vertices[i].augmentedDelta = w.augmentedDelta;
            }
        }
    }
}

bool TerrainData::isActive(size_t _vertex, ngl::Vec3 _cameraPos, float _tolerance, float _lambda)
{
    float mu = _lambda/_tolerance;
    Vertex v = m_vertices[_vertex];

    //ngl::Vec4 vertex = screenPosition(v);

    /*if((mu * v.augmentedDelta + v.radius)*(mu * v.augmentedDelta + v.radius)
            < (v.x-_cameraX)*(v.x-_cameraX) + (v.y-_cameraY)*(v.y-_cameraY) + (v.z-_cameraZ)*(v.z-_cameraZ))
    {
        std::cout<<"("<<v.x<<", "<<v.y<<"):  "<<(mu * v.augmentedDelta + v.radius)*(mu * v.augmentedDelta + v.radius)
                <<"  "<<(v.x-_cameraX)*(v.x-_cameraX) + (v.y-_cameraY)*(v.y-_cameraY) + (v.z-_cameraZ)*(v.z-_cameraZ)<<'\n';
    }*/

    return (mu * v.augmentedDelta + v.radius)*(mu * v.augmentedDelta + v.radius) >
                                        (v.sceneX-_cameraPos.m_x)*(v.sceneX-_cameraPos.m_x)
                                      + (v.sceneY-_cameraPos.m_y)*(v.sceneY-_cameraPos.m_y)
                                      + (v.sceneZ-_cameraPos.m_z)*(v.sceneZ-_cameraPos.m_z);
    //return true;
}

void TerrainData::meshRefine(ngl::Vec3 _cameraPos, float _tolerance, float _lambda)
{
    m_indices = {};
    m_parity = 0;
    m_indices.push_back(0);
    m_indices.push_back(0);
    submeshRefine(4,6,1, _cameraPos, _tolerance, _lambda);
    tstripAppend(1,0);
    submeshRefine(4,7,1, _cameraPos, _tolerance, _lambda);
    tstripAppend(2,0);
    submeshRefine(4,8,1, _cameraPos, _tolerance, _lambda);
    tstripAppend(3,0);
    submeshRefine(4,5,1, _cameraPos, _tolerance, _lambda);
    m_indices.push_back(0);
}

void TerrainData::submeshRefine(size_t _currentVertex, size_t _DAGChildVertex, size_t _refinementLevel,
                                ngl::Vec3 _cameraPos, float _tolerance, float _lambda)
{
    if(_refinementLevel <= m_maxRefinementLevel &&
            isActive(_currentVertex, _cameraPos, _tolerance, _lambda))
    {
        submeshRefine(_DAGChildVertex,
                      4 * _currentVertex + (2 * _currentVertex + _DAGChildVertex - 6) % 4 - 7,
                      _refinementLevel + 1, _cameraPos, _tolerance, _lambda);
        tstripAppend(_currentVertex, _refinementLevel % 2);
        submeshRefine(_DAGChildVertex,
                      4 * _currentVertex + (2 * _currentVertex + _DAGChildVertex - 5) % 4 - 7,
                      _refinementLevel + 1, _cameraPos, _tolerance, _lambda);
    }
}

void TerrainData::tstripAppend(size_t _vertex, size_t _parity)
{
    size_t l = m_indices.size();
    if (m_indices[l-1] != _vertex && m_indices[l-2] != _vertex)
    {
        if(_parity != m_parity)
        {
            //std::cout<<"parity not equal: "<<_vertexIndex<<'\n';
            m_parity = _parity;
        }
        else
        {
            //std::cout<<"parity equal: "<<m_indices[l-2]<<' '<<_vertexIndex<<'\n';
            m_indices.push_back(m_indices[l-2]);
        }
        m_indices.push_back(_vertex);
    }
}

TerrainData::Vertex::Vertex(int _x, int _y, float _z, int _dimension, float _scale)
{
  originalX = _x;
  originalY = _y;
  originalZ = _z;
  sceneX = (float(_x-_dimension*0.5f))*_scale;
  sceneY = (float(_y-_dimension*0.5f))*_scale;
  sceneZ = _z*_scale;
}
