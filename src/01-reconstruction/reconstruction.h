//=============================================================================
//
//   Exercise code for the lecture "Geometric Modeling"
//   by Prof. Dr. Mario Botsch, TU Dortmund
//
//   Copyright (C) 2023 Computer Graphics Group, TU Dortmund.
//
//=============================================================================
#pragma once
//=============================================================================

#include <pmp/SurfaceMesh.h>
#include "PointSet.h"

//=============================================================================

//! reconstruct mesh using Poisson surface reconstruction
void reconstruct_poisson(const PointSet &pointset,
                         pmp::SurfaceMesh &mesh,
                         int depth,
                         int solver_divide,
                         float point_weight);

//! reconstruct mesh using Hoppe's approach
void reconstruct_hoppe(const PointSet &pointset,
                       pmp::SurfaceMesh &mesh,
                       unsigned int resolution,
                       unsigned int nneighbors = 1);

//=============================================================================
