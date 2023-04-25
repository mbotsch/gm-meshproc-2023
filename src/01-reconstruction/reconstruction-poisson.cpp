//=============================================================================
//
//   Exercise code for the lecture "Geometric Modeling"
//   by Prof. Dr. Mario Botsch, TU Dortmund
//
//   Copyright (C) 2023 Computer Graphics Group, TU Dortmund.
//
//=============================================================================

#include "reconstruction.h"
#include <poisson/poisson.h>

using namespace pmp;

//=============================================================================

void reconstruct_poisson(const PointSet &pointset,
                         SurfaceMesh &mesh,
                         int depth,
                         int solver_divide,
                         float point_weight)
{
    // store points and normals in two arrays
    const unsigned int N = pointset.points_.size();
    std::vector<Point3D<float>> points(N), normals(N);
    for (unsigned int i = 0; i < N; i++)
    {
        points[i].coords[0] = pointset.points_[i][0];
        points[i].coords[1] = pointset.points_[i][1];
        points[i].coords[2] = pointset.points_[i][2];
        normals[i].coords[0] = pointset.normals_[i][0];
        normals[i].coords[1] = pointset.normals_[i][1];
        normals[i].coords[2] = pointset.normals_[i][2];
    }

    // perform Poisson reconstruction
    CoredPoissonVectorMeshData<PlyVertex<float>> reconstructed_mesh;
    Execute2(points, normals, reconstructed_mesh, depth, solver_divide,
             point_weight);

    // initialize
    mesh.clear();
    reconstructed_mesh.resetIterator();

    // add vertices to mesh
    PlyVertex<float> p;
    for (int i(0); i < (int)reconstructed_mesh.inCorePoints.size(); i++)
    {
        p = reconstructed_mesh.inCorePoints[i];
        mesh.add_vertex(
            vec3(p.point.coords[0], p.point.coords[1], p.point.coords[2]));
    }
    for (int i(0); i < reconstructed_mesh.outOfCorePointCount(); i++)
    {
        reconstructed_mesh.nextOutOfCorePoint(p);
        mesh.add_vertex(
            vec3(p.point.coords[0], p.point.coords[1], p.point.coords[2]));
    }

    // add faces to mesh
    std::vector<CoredVertexIndex> polygon;
    std::vector<Vertex> vertices;
    for (int i(0); i < reconstructed_mesh.polygonCount(); i++)
    {
        reconstructed_mesh.nextPolygon(polygon);
        vertices.resize(polygon.size());

        for (unsigned int j = 0; j < polygon.size(); ++j)
        {
            if (polygon[j].inCore)
                vertices[j] = Vertex(polygon[j].idx);
            else
                vertices[j] =
                    Vertex(polygon[j].idx +
                           int(reconstructed_mesh.inCorePoints.size()));
        }
        mesh.add_face(vertices);
    }
}

//=============================================================================
