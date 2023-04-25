//=============================================================================
//
//   Exercise code for the lecture "Geometric Modeling"
//   by Prof. Dr. Mario Botsch, TU Dortmund
//
//   Copyright (C) 2023 Computer Graphics Group, TU Dortmund.
//
//=============================================================================

#pragma once

#include <pmp/Types.h>
#include <vector>

using namespace pmp;

//==============================================================================

/// A class representing a kD-tree for points.
class kDTree
{
public:

    //-------------------------------------------------------------- public types

    typedef std::vector<Point>      Points;
    typedef Points::const_iterator  ConstPointIter;


    /// the element stored in the tree
    struct Element
    {
        Element(const Point& _p, int _idx) : point(_p), idx(_idx) {}
        Point point;
        int   idx;
    };

    typedef std::vector<Element>      Elements;
    typedef Elements::iterator        ElementIter;
    typedef Elements::const_iterator  ConstElementIter;


    /// Functor for partitioning wrt splitting plane
    struct PartPlane
    {
        PartPlane(unsigned char _cut_dim, Scalar _cut_val)
            : cut_dim_(_cut_dim), cut_val_(_cut_val) {}

        bool operator()(const Element& _e) const { return _e.point[cut_dim_] > cut_val_; }

        unsigned char   cut_dim_;
        Scalar          cut_val_;
    };


    /// Store nearest neighbor information
    struct NearestNeighborData
    {
        // Point for which we compute the nearest neighbor
        Point          ref;

        // distance to the nearest neighbor
        Scalar         dist;

        // index of the nearest neighbor
        int            nearest;
        unsigned int   leaf_tests;
    };


    /// Node of the tree: contains parent, children and splitting plane
    struct Node
    {
        Node(ElementIter _begin, ElementIter _end)
            : left_child_(0), right_child_(0), begin_(_begin), end_(_end) {}

        ~Node()
        {
            delete left_child_;
            delete right_child_;
        }

        Node *left_child_, *right_child_;
        ElementIter begin_, end_;
        unsigned char  cut_dim_;
        Scalar cut_val_;
    };



public:

    //----------------------------------------------------------- public methods

    /** Constructor: need traits that define the types and
        give us the points by traits_.point(PointHandle) */
    kDTree(const Points& _points) : points_(_points), root_(0) {}

    /// Destructor
    ~kDTree() { delete root_; }

    /// Build the tree. Returns number of nodes.
    unsigned int build(unsigned int _max_handles=100, unsigned int _max_depth=50);

    /// Return handle of the nearest neighbor
    NearestNeighborData nearest(const Point& _p) const;

private:

    //----------------------------------------------------------- private methods

    /// Recursive part of build()
    void _build(Node*        _node,
                unsigned int _max_handles,
                unsigned int _depth);

    /// Recursive part of nearest()
    void _nearest(Node* _node, NearestNeighborData& _data) const;


    //-------------------------------------------------------------- private data

    const Points&  points_;
    Elements       elements_;
    Node           *root_;
    unsigned int   n_nodes_;
};

//=============================================================================
