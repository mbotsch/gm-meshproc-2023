//=============================================================================
//
//   Exercise code for the lecture "Geometric Modeling"
//   by Prof. Dr. Mario Botsch, TU Dortmund
//
//   Copyright (C) 2023 Computer Graphics Group, TU Dortmund.
//
//=============================================================================

#include "Viewer.h"

int main(int argc, char **argv)
{
    Viewer window("Geometric Modeling", 800, 600);

    if (argc == 2)
        window.load_data(argv[1]);

    return window.run();
}

//=============================================================================
