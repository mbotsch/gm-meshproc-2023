//=============================================================================
//
//   Exercise code for the lecture "Geometric Modeling"
//   by Prof. Dr. Mario Botsch, TU Dortmund
//
//   Copyright (C) 2023 Computer Graphics Group, TU Dortmund.
//
//=============================================================================

// our includes
#include "PointSet.h"
#include <pmp/algorithms/SurfaceNormals.h>

// system includes
#include <fstream>
#include <string>
#include <clocale>

using namespace pmp;

//=============================================================================


// helper function from PMP
template <typename T>
void tfread(FILE* in, const T& t)
{
    size_t n_items = fread((char*)&t, 1, sizeof(t), in);
    assert(n_items > 0);
}

// helper function from PMP
template <typename T>
void tfwrite(FILE* out, const T& t)
{
    size_t n_items = fwrite((char*)&t, 1, sizeof(t), out);
    assert(n_items > 0);
}


//-----------------------------------------------------------------------------


PointSet::PointSet()
    :SurfaceMeshGL()
{
    has_colors_ = false;
}


//-----------------------------------------------------------------------------


bool PointSet::read_data(const char *_filename)
{
    std::setlocale(LC_NUMERIC, "C");

    // extract file extension
    std::string filename(_filename);
    std::string::size_type dot(filename.rfind("."));
    std::string ext = filename.substr(dot+1, filename.length()-dot-1);
    std::transform(ext.begin(), ext.end(), ext.begin(), tolower);

    bool ok = false;
    if (ext == "xyz")
    {
        ok = read_xyz(_filename);
        has_colors_ = false;
    }
    else if (ext == "cnoff")
    {
        ok = read_cnoff(_filename);
        has_colors_ = true;
    }
    else if (ext == "txt")
    {
        ok = read_txt(_filename);
        has_colors_ = true;
    }
    else if (ext == "pts")
    {
        ok = read_pts(_filename);
    }
    else
    {
        has_colors_ = false;
        try
        {
            read(_filename);

            // make sure normals are in v:normal
            SurfaceNormals::compute_vertex_normals(*this);

            // set pointset to mesh vertices and normals
            auto vnormal = get_vertex_property<Normal>("v:normal");
            auto vpoint = get_vertex_property<Point>("v:point");

            points_.resize(n_vertices());
            normals_.resize(n_vertices());
            for (auto v : vertices())
            {
                points_[v.idx()] = vpoint[v];
                normals_[v.idx()] = vnormal[v];
            }

            ok = true;
        }
        catch (const IOException& e)
        {
            ok = false;
        }
    }

    if (!ok)
    {
        std::cerr << "Cannot read " << filename << std::endl;
        return false;
    }

    clear();
    for(size_t i = 0; i < points_.size(); i++)
    {
        add_vertex(points_[i]);
    }

    auto vnormal = vertex_property<Normal>("v:normal");
    for(auto v : vertices())
    {
        vnormal[v] = normals_[v.idx()];
    }

    set_specular(0.15);
    if(!has_colors_)
    {
        set_front_color(Color(1,0,0));
        auto vcolor = get_vertex_property<Normal>("v:color");
        if (vcolor) remove_vertex_property(vcolor);
    }
    else
    {
        auto vcolor = vertex_property<Color>("v:color");
        for(auto v : vertices())
        {
            vcolor[v] = colors_[v.idx()];
        }
    }

    set_point_size(3);

    update_opengl_buffers();

    return true;
}


//-----------------------------------------------------------------------------


void PointSet::update_opengl()
{
    auto vpoint = get_vertex_property<Point>("v:point");
    auto vnormal = get_vertex_property<Normal>("v:normal");
    for(auto v : vertices())
    {
        vpoint[v] = points_[v.idx()];
        vnormal[v] = normals_[v.idx()];
    }

    update_opengl_buffers();
}


//-----------------------------------------------------------------------------


bool
PointSet::
read_xyz(const char* filename)
{
    FILE* in = fopen(filename, "r");
    if (!in) return false;

    char line[200];
    float x, y, z;
    float nx, ny, nz;
    int n;

    points_.clear();
    normals_.clear();
    colors_.clear();

    while (in && !feof(in) && fgets(line, 200, in))
    {
        n = sscanf(line, "%f %f %f %f %f %f", &x, &y, &z, &nx, &ny, &nz);
        if (n >= 6)
        {
            points_.push_back(pmp::Point(x,y,z));
            normals_.push_back(pmp::Normal(nx,ny,nz));
            colors_.push_back(pmp::Color(0.0,0.0,0.0));
        }
    }

    fclose(in);
    return true;
}


//-----------------------------------------------------------------------------


bool
PointSet::
read_cnoff(const char* filename)
{
    FILE* in = fopen(filename, "r");
    if (!in) return false;

    char line[200];
    float x, y, z;
    float nx, ny, nz;
    float cx, cy, cz;
    int n;

    points_.clear();
    normals_.clear();
    colors_.clear();

    while (in && !feof(in) && fgets(line, 200, in))
    {
        n = sscanf(line, "%f %f %f %f %f %f %f %f %f", &x, &y, &z, &nx, &ny, &nz, &cx, &cy, &cz);
        if (n >= 9)
        {
            points_.push_back(pmp::Point(x,y,z));
            normals_.push_back(pmp::Normal(nx,ny,nz));
            colors_.push_back(pmp::Color(cx/255.0,cy/255.0,cz/255.0));
        }
    }

    fclose(in);
    return true;
}


//-----------------------------------------------------------------------------


bool
PointSet::
read_cxyz(const char* filename)
{
    std::ifstream ifs(filename);
    if (!ifs) return false;

    float x, y, z;
    float nx, ny, nz;
    float cx, cy, cz;

    points_.clear();
    normals_.clear();
    colors_.clear();

    std::string dummy;
    std::getline(ifs, dummy);
    std::getline(ifs, dummy);
    while (ifs && !ifs.eof())
    {
        ifs >> x >> y >> z;
        ifs >> nx >> ny >> nz;
        ifs >> cx >> cy >> cz;
        points_.push_back(pmp::Point(x,y,z));
        normals_.push_back(pmp::Normal(nx,ny,nz));
        colors_.push_back(pmp::Color(cx,cy,cz));
    }

    ifs.close();

    return true;
}


//-----------------------------------------------------------------------------


bool
PointSet::
read_txt(const char* filename)
{
    FILE* in = fopen(filename, "r");
    if (!in) return false;

    char line[200];
    float x, y, z;
    float nx, ny, nz;
    float cx, cy, cz;
    int n;

    points_.clear();
    normals_.clear();
    colors_.clear();

    while (in && !feof(in) && fgets(line, 200, in))
    {
        n = sscanf(line, "%f %f %f %f %f %f %f %f %f", &x, &y, &z, &cx, &cy, &cz, &nx, &ny, &nz);
        if (n >= 9)
        {
            points_.push_back(pmp::Point(x,y,z));
            normals_.push_back(pmp::Normal(nx,ny,nz));
            colors_.push_back(pmp::Color(cx/255.0,cy/255.0,cz/255.0));
        }
    }

    fclose(in);
    return true;
}


//-----------------------------------------------------------------------------


bool
PointSet::
read_pts(const char* filename)
{
    FILE* in = fopen(filename, "rb");
    if (!in) return false;

    unsigned int n;
    tfread(in, n);
    tfread(in, has_colors_);

    std::cout << n << " points " << (has_colors_ ? "with" : "without") << " colors\n";

    points_.resize(n);
    fread((char*)points_.data(), sizeof(pmp::Point), n, in);

    normals_.resize(n);
    fread((char*)normals_.data(), sizeof(pmp::Normal), n, in);

    colors_.resize(n, pmp::Color(0,0,0));
    if (has_colors_)
        fread((char*)colors_.data(), sizeof(pmp::Color), n, in);

    fclose(in);
    return true;
}


//=============================================================================
