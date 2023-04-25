//=============================================================================
//
//   Exercise code for the lecture "Geometric Modeling"
//   by Prof. Dr. Mario Botsch, TU Dortmund
//
//   Copyright (C) 2023 Computer Graphics Group, TU Dortmund.
//
//=============================================================================

#pragma once

#include "Grid.h"
#include <pmp/SurfaceMesh.h>
#include <map>

using namespace pmp;

//=============================================================================

/** use the Marching Cubes algorithm to extract the iso-surface to a certain
    iso-value (\c _isoval) from a grid of scalar values (\c _grid) and store
    the resulting triangle mesh in \c _mesh. 
*/
void marching_cubes(const Grid& _grid, SurfaceMesh& _mesh, Scalar _isoval=0);


//=============================================================================
