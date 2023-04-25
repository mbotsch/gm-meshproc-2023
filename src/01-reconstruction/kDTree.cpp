//=============================================================================
//
//   Exercise code for the lecture "Geometric Modeling"
//   by Prof. Dr. Mario Botsch, TU Dortmund
//
//   Copyright (C) 2023 Computer Graphics Group, TU Dortmund.
//
//=============================================================================

#include "kDTree.h"
#include <algorithm>
#include <float.h>

using namespace pmp;

//=============================================================================


unsigned int
kDTree::build(unsigned int _max_handles, unsigned int _max_depth)
{
    // copy points to element array
    elements_.clear();
    elements_.reserve(points_.size());
    ConstPointIter p_it;
    int i;

    for (p_it=points_.begin(), i=0; p_it!=points_.end(); ++p_it, ++i)
        elements_.push_back( Element(*p_it, i) );


    // init
    delete root_;
    root_ = new Node(elements_.begin(), elements_.end());
    n_nodes_ = 0;


    // call recursive helper
    _build(root_, _max_handles, _max_depth);


    return n_nodes_;
}


//-----------------------------------------------------------------------------


void
kDTree::
_build(Node*         _node,
       unsigned int  _max_handles,
       unsigned int  _depth)
{
    const unsigned int n = _node->end_-_node->begin_;


    // should we stop at this level ?
    if ((_depth == 0) || (n < _max_handles))
        return;


    // compute bounding box
    ElementIter it(_node->begin_);
    Point bb_min = it->point;
    Point bb_max = it->point;
    for (; it!=_node->end_; ++it)
    {
        bb_min = min(bb_min, it->point);
        bb_max = max(bb_max, it->point);
    }


    // split longest side of bounding box
    Point bb = bb_max - bb_min;
    Scalar length = bb[0];
    int axis = 0;
    if (bb[1] > length) length = bb[axis=1];
    if (bb[2] > length) length = bb[axis=2];
    Scalar cv = 0.5*(bb_min[axis]+bb_max[axis]);


    // store cut dimension and value
    _node->cut_dim_ = axis;
    _node->cut_val_ = cv;


    // partition for left and right child
    it = std::partition(_node->begin_, _node->end_, PartPlane(axis, cv));


    // create children
    n_nodes_ += 2;
    _node->left_child_  = new Node(_node->begin_, it);
    _node->right_child_ = new Node(it, _node->end_);


    // recurse to childen
    _build(_node->left_child_,  _max_handles, _depth-1);
    _build(_node->right_child_, _max_handles, _depth-1);
}


//-----------------------------------------------------------------------------


kDTree::NearestNeighborData
kDTree::
nearest(const Point& _p) const
{
    // init data
    NearestNeighborData  data;
    data.ref        = _p;
    data.dist       = FLT_MAX;
    data.leaf_tests = 0;

    // recursive search
    _nearest(root_, data);

    // dist was computed as sqr-dist
    data.dist = sqrt(data.dist);

    return data;
}


//-----------------------------------------------------------------------------


void
kDTree::
_nearest(Node* _node, NearestNeighborData& _data) const
{
    if (_node->left_child_)
    {
        int cd = _node->cut_dim_;
        Scalar off = _data.ref[cd] - _node->cut_val_;

        if (off > 0.0)
        {
            _nearest(_node->left_child_, _data);
            if (off*off < _data.dist)
            {
                _nearest(_node->right_child_, _data);
            }
        }
        else
        {
            _nearest(_node->right_child_, _data);
            if (off*off < _data.dist)
            {
                _nearest(_node->left_child_, _data);
            }
        }
    }

    // terminal node
    else
    {
        ++_data.leaf_tests;
        Scalar dist;

        for (ElementIter it=_node->begin_; it!=_node->end_; ++it)
        {
            dist = sqrnorm(it->point - _data.ref);
            if (dist < _data.dist)
            {
                _data.dist    = dist;
                _data.nearest = it->idx;
            }
        }
    }
}


//=============================================================================