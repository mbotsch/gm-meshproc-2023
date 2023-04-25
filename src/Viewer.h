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

#include <pmp/visualization/MeshViewer.h>
#include <01-reconstruction/PointSet.h>

using namespace pmp;


//== CLASS DEFINITION =========================================================

/// 3D viewer for point cloud and reconstructed mesh
class Viewer : public MeshViewer
{
public:
    /// constructor
    Viewer(const char* title, int width, int height);

    /// load points or mesh from file \p filename
    bool load_data(const char* _filename);

    /// draw the scene in different draw modes
    virtual void draw(const std::string& draw_mode) override;

protected:
    /// this function handles keyboard events
    void keyboard(int key, int code, int action, int mod) override;

    /// draw the scene in different draw modes
    virtual void process_imgui() override;

private:
    
    /// input point set for surface reconstruction
    PointSet pointset_;

    /// draw the pointset?
    bool draw_pointset_;

    /// draw the mesh?
    bool draw_mesh_;
};

//=============================================================================
