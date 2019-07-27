# Terrain Generation using LOD reduction algorithm

This programme allows the user to quickly generate a variety of simple terrains using Perlin noise by changing number of octaves, frequency, persistence and lacunarity of the noise. It then runs a reduction algorithm on them to speed up rendering time. The user can also change the tolerance of the LOD algorithm and view the algorithm in action by setting the draw mode to wireframe and moving the camera.

To be built, this project requires the NCCA NGL graphics library 2018 and the libnoise library - a link to a github repository with an easy to set up linux build for libnoise can be found here: https://github.com/qknight/libnoise. The github repo for NGL is here: https://github.com/NCCA/NGL.

Additionaly googletest library is needed to run the tests.
