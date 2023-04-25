//=============================================================================
//
//   Exercise code for the lecture "Geometric Modeling"
//   by Prof. Dr. Mario Botsch, TU Dortmund
//
//   Copyright (C) 2023 Computer Graphics Group, TU Dortmund.
//
//=============================================================================

#include <Viewer.h>
#include <01-reconstruction/reconstruction.h>
#include <pmp/algorithms/SurfaceNormals.h>
#include <imgui.h>
#include <fstream>

//=============================================================================

const std::string point_dir = POINTSET_DIRECTORY;
const std::string mesh_dir = MESH_DIRECTORY;

std::vector<std::string> pointsets = {
    "bunny.pts", "mario_hq.pts", "mario_lq.pts", "mario_laser.pts",
    "open_sphere.pts", "sphere.pts", "plane.pts", "myscan.txt" };

std::vector<std::string> meshes = {
    "bunny.off", "boar.off", "blubb.off", "dragon.off", "spot.off",
    "max.off", "open_bunny.off", "hemisphere.off", "sphere.off" };

//=============================================================================

Viewer::Viewer(const char *title, int width, int height)
    : pmp::MeshViewer(title, width, height)
{
    draw_pointset_ = false;
    draw_mesh_ = true;

    // setup draw modes for viewer
    clear_draw_modes();
    add_draw_mode("Smooth Shading");
    add_draw_mode("Hidden Line");
    add_draw_mode("Texture");
    set_draw_mode("Smooth Shading");

    // check which pointsets exist
    {
        std::ifstream ifs;
        auto filenames = pointsets;
        pointsets.clear();
        for (auto filename : filenames)
        {
            ifs.open(std::string(POINTSET_DIRECTORY) + filename);
            if (ifs)
                pointsets.push_back(filename);
            ifs.close();
        }
    }

    // check which meshes exist
    {
        std::ifstream ifs;
        auto filenames = meshes;
        meshes.clear();
        for (auto filename : filenames)
        {
            ifs.open(std::string(MESH_DIRECTORY) + filename);
            if (ifs)
                meshes.push_back(filename);
            ifs.close();
        }
    }
}

//-----------------------------------------------------------------------------

bool Viewer::load_data(const char *_filename)
{
    std::string filename(_filename);
    std::string::size_type dot(filename.rfind("."));
    std::string ext = filename.substr(dot + 1, filename.length() - dot - 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), tolower);

    bool ok;
    mesh_.clear();

    // load as pointset
    ok = pointset_.read_data(_filename);
    if (!ok)
    {
        std::cerr << "cannot read file " << _filename << std::endl;
        return false;
    }

    // load mesh
    if (ext == "obj" || ext == "off")
    {
        try
        {
            MeshViewer::load_mesh(_filename);
        }
        catch (const std::exception &e)
        {
            std::cerr << "cannot read file " << _filename << std::endl;
            return false;
        }
    }

    // update scene center and bounds
    BoundingBox bb = pointset_.bounds();
    set_scene((vec3)bb.center(), 0.5 * bb.size());

    if (mesh_.n_vertices() != pointset_.n_vertices())
        draw_pointset_ = true;

    filename_ = filename;

    return true;
}

//-----------------------------------------------------------------------------

void Viewer::draw(const std::string &draw_mode)
{
    if (draw_mesh_)
        MeshViewer::draw(draw_mode);

    if (draw_pointset_)
        pointset_.draw(projection_matrix_, modelview_matrix_, "Points");
}

//-----------------------------------------------------------------------------

void Viewer::keyboard(int key, int scancode, int action,
                      int mods)
{
    if (action != GLFW_PRESS && action != GLFW_REPEAT)
        return;

    switch (key)
    {
        case GLFW_KEY_BACKSPACE: // reload model
        {
            load_data(filename_.c_str());
            break;
        }
        default:
        {
            MeshViewer::keyboard(key, scancode, action, mods);
            break;
        }
    }
}

//-----------------------------------------------------------------------------

void Viewer::process_imgui()
{
    if (ImGui::CollapsingHeader("Load pointset or mesh",
                                ImGuiTreeNodeFlags_DefaultOpen))
    {
        static std::string current_pointset = "- load pointset -";
        static std::string current_mesh = "- load mesh -";

        ImGui::Text("Point Cloud");
        ImGui::Indent(10);

        ImGui::PushItemWidth(150);
        if (ImGui::BeginCombo("##PC to load", current_pointset.c_str()))
        {
            ImGui::Selectable("- load pointset -", false);
            for (auto item : pointsets)
            {
                bool is_selected = (current_pointset == item);
                if (ImGui::Selectable(item.c_str(), is_selected))
                {
                    current_mesh = "- load mesh -";
                    current_pointset = item;
                    std::string fn = point_dir + current_pointset;
                    load_data(fn.c_str());
                }
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();

        // output point statistics
        ImGui::BulletText("%d points", (int)pointset_.points_.size());
        ImGui::Unindent(10);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Mesh");
        ImGui::Indent(10);

        ImGui::PushItemWidth(150);
        if (ImGui::BeginCombo(
                "##Mesh to load",
                current_mesh
                    .c_str())) // The second parameter is the label previewed before opening the combo.
        {
            ImGui::Selectable("- load mesh -", false);
            for (auto item : meshes)
            {
                bool is_selected = (current_mesh == item);
                if (ImGui::Selectable(item.c_str(), is_selected))
                {
                    current_pointset = "- load pointset -";
                    current_mesh = item;
                    std::string fn = mesh_dir + current_mesh;
                    load_data(fn.c_str());
                    draw_pointset_ = false;
                }
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();

        // output mesh statistics
        ImGui::BulletText("%d vertices", (int)mesh_.n_vertices());
        ImGui::BulletText("%d edges", (int)mesh_.n_edges());
        ImGui::BulletText("%d faces", (int)mesh_.n_faces());
        ImGui::Unindent(10);
    }

    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Draw Options"))
    {
        ImGui::Text("Point Cloud");
        ImGui::Spacing();

        if (!pointset_.points_.empty())
        {
            ImGui::Indent(10);
            ImGui::Checkbox("Draw point cloud", &draw_pointset_);

            if (draw_pointset_)
            {
                // control point size
                int point_size = pointset_.point_size();
                ImGui::PushItemWidth(100);
                ImGui::Text("Point Size:");
                if (ImGui::SliderInt("##PointSize", &point_size, 1, 10))
                {
                    pointset_.set_point_size(point_size);
                }
                ImGui::PopItemWidth();
            }
            ImGui::Unindent(10);
            ImGui::Spacing();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Mesh");
        ImGui::Spacing();
        ImGui::Indent(10);

        if (mesh_.n_vertices() > 0)
        {
            ImGui::Checkbox("Draw mesh", &draw_mesh_);

            if (draw_mesh_)
            {
                ImGui::Text("Draw modes:");
                ImGui::PushItemWidth(150);
                int mode = static_cast<int>(draw_mode_);

                for (size_t i = 0; i < draw_mode_names_.size(); ++i)
                {
                    ImGui::RadioButton(draw_mode_names_[i].c_str(), &mode, i);
                }
                ImGui::PopItemWidth();
                if (mode != static_cast<int>(draw_mode_))
                {
                    draw_mode_ = static_cast<unsigned int>(mode);
                }
            }
        }
        else
        {
            ImGui::Text("Reconstruct first!");
        }
        ImGui::Unindent(10);
    }

    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Surface Reconstruction"))
    {
        if (!pointset_.points_.empty())
        {
            // Hoppe parameters
            static int hoppe_resolution = 50;
            static int hoppe_nneighbors = 1;
            ImGui::PushItemWidth(100);
            ImGui::Text("Grid resolution");
            ImGui::SliderInt("##MC Resolution", &hoppe_resolution, 10, 200);
            ImGui::PopItemWidth();

            if (ImGui::Button("Hoppe reconstruction"))
            {
                reconstruct_hoppe(pointset_, mesh_, hoppe_resolution, hoppe_nneighbors);
                update_mesh();
                draw_pointset_ = false;
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            // Poisson parameters
            static int octree_depth = 7;
            ImGui::PushItemWidth(100);
            ImGui::Text("Octree depth");
            ImGui::SliderInt("##Poisson OD", &octree_depth, 5, 10);
            ImGui::PopItemWidth();

            if (ImGui::Button("Poisson reconstruction"))
            {
                reconstruct_poisson(pointset_, mesh_, octree_depth, 8, 2.0);
                update_mesh();
                draw_pointset_ = false;
            }
        }
        else
        {
            ImGui::Text("Load point cloud first");
        }
    }

#ifndef __EMSCRIPTEN__
    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Write Mesh"))
    {
        if (mesh_.n_vertices() > 0)
        {
            if (ImGui::Button("Write .off"))
            {
                mesh_.write("mesh.off");
            }

            if (ImGui::Button("Write .obj"))
            {
                mesh_.write("mesh.obj");
            }
        }
        else
        {
            ImGui::Text("Reconstruct mesh first!");
        }
    }
#endif
}

//=============================================================================
