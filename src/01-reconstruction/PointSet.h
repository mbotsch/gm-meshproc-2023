//=============================================================================
//
//   Exercise code for the lecture "Geometric Modeling"
//   by Prof. Dr. Mario Botsch, TU Dortmund
//
//   Copyright (C) 2023 Computer Graphics Group, TU Dortmund.
//
//=============================================================================

#pragma once

// our includes
#include <pmp/Types.h>
#include <pmp/visualization/SurfaceMeshGL.h>

// system includes
#include <vector>


/// A class representing a point set with normals and colors.
class PointSet : public pmp::SurfaceMeshGL
{
public:

    /// constructor
    PointSet();

    /// encapsulates read functions
    bool read_data(const char* _filename);

    /// resets points and normals to original
    void reset();

    /// copies point cloud data to Surfacemesh for openGL rendering
    void update_opengl();

private:

    /// Read a point set with normals from a .xyz file.
    bool read_xyz(const char* filename);

    /// Read a point set with normals and colors from a .cnoff file.
    bool read_cnoff(const char* filename);

    /// Read a point set with normals and colors from a .cxyz file.
    bool read_cxyz(const char* filename);

    /// Read a point set with normals and colors from a .txt file.
    bool read_txt(const char* filename);

    /// Read a point set with normals and colors from a binary .pts file.
    bool read_pts(const char* filename);

public:

    std::vector<pmp::Point>  points_;
    std::vector<pmp::Normal> normals_;
    std::vector<pmp::Color>  colors_;

    bool has_colors_;
};


//==============================================================================
