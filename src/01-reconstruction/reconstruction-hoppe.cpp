//=============================================================================
//
//   Exercise code for the lecture "Geometric Modeling"
//   by Prof. Dr. Mario Botsch, TU Dortmund
//
//   Copyright (C) 2023 Computer Graphics Group, TU Dortmund.
//
//=============================================================================

#include "reconstruction.h"
#include "Grid.h"
#include "MarchingCubes.h"
#include "kDTree.h"
#include <pmp/Timer.h>
#include <float.h>

using namespace pmp;

//=============================================================================

void reconstruct_hoppe(const PointSet &pointset,
                       pmp::SurfaceMesh &mesh,
                       unsigned int resolution,
                       unsigned int nneighbors)
{
    // we need some points...
    if (pointset.points_.empty())
    {
        return;
    }

    // measure time for reconstruction
    Timer t; t.start();


    // compute boundingbox and slightly enlage it
    auto bb = pointset.bounds();
    Scalar bb_size = norm(bb.max() - bb.min());
    Point bb_min = bb.min() - Point(0.04 * bb_size);
    Point bb_max = bb.max() + Point(0.04 * bb_size);
    Point bb_diag = bb.max() - bb.min();


    // determine grid resolution
    float max_diag = std::max(bb_diag[0], std::max(bb_diag[1], bb_diag[2]));
    float grid_spacing = max_diag / resolution;
    int res_x = std::max(2, (int)(bb_diag[0] / grid_spacing));
    int res_y = std::max(2, (int)(bb_diag[1] / grid_spacing));
    int res_z = std::max(2, (int)(bb_diag[2] / grid_spacing));


    // setup grid for storing distance values
    Grid grid(bb_min,
              Point(bb_max[0] - bb_min[0], 0, 0),
              Point(0, bb_max[1] - bb_min[1], 0),
              Point(0, 0, bb_max[2] - bb_min[2]), 
              res_x, res_y, res_z);

    /**
     * @todo Compite SDF for each grid node, extract mesh with marching cubes
     * - Grid has resolution `(res_x, res_y, res_)`
     * - Get the position of grid point with index (i,j,k) by `grid.point(i,j,k).
     * - Get positions and normals of pointset with `pointset.points_[i]` and `pointset.normals_[i]`.
     * - Store signed distance in `grid(i,j,k)`.
     * - Extract mesh with `marching_cubes(grid, mesh)`
     */


    // print timing
    t.stop();
    std::cout << "Reconstruction took " << t << std::endl;
}

//=============================================================================