#include "Geometry.h"
#include "MarchingCubes.h"
#include "Octree.h"
#include "SparseMatrix.h"
#include "PPolynomial.h"
#include "Ply.h"
#include "MultiGridOctreeData.h"

#ifdef _OPENMP
#include "omp.h"
#endif


template< int Degree , class Vertex , bool OutputDensity >
int Execute(std::vector< Point3D<float> >& pts, std::vector< Point3D<float> >& normals,
            CoredPoissonVectorMeshData< PlyVertex<float> >& mesh,
            int octree_depth = 8, int solver_divide = 8, float point_weight = 4.0f,
            float samples_per_node = 1.0f, float offset = 1.0f)
{
    float isoValue = 0;
    int MaxSolveDepth = octree_depth;
    int MinDepth = 5;
    int IsoDivide = 8;
    int BoundaryType = 1;
    int AdaptiveExponent = 1;
    int MinIters = 24;
    int FixedIters = -1;
    float Scale = 1.1f;
    float SolverAccuracy = 1e-3;
    bool const clip_tree = true;
    bool ConfidenceSet = false;
    bool ShowResidual = false;
    bool NonManifold = false;
    bool PolygonMesh = false;

    XForm4x4< float > xForm = XForm4x4< float >::Identity();

    Octree< Degree , OutputDensity > tree;

#if _OPENMP
    tree.threads = omp_get_num_procs();
#else
    tree.threads = 1;
#endif

    if( solver_divide < MinDepth )
    {
        solver_divide = MinDepth;
    }

    if( IsoDivide < MinDepth )
    {
        IsoDivide = MinDepth;
    }

    OctNode< TreeNodeData< OutputDensity > , float >::SetAllocator( MEMORY_ALLOCATOR_BLOCK_SIZE );

    int kernelDepth = octree_depth - 2;

    tree.setBSplineData( octree_depth , BoundaryType );
    if( kernelDepth > octree_depth )
    {
        fprintf( stderr,"[ERROR] kernelDepth can't be greater than octree_depth\n" );
        return EXIT_FAILURE;
    }

    tree.setTree( pts, normals, octree_depth , MinDepth , kernelDepth , Real(samples_per_node) , Scale , ConfidenceSet , point_weight , AdaptiveExponent , xForm );

    if(clip_tree)
    {
        tree.ClipTree();
    }
    tree.finalize( IsoDivide );

    tree.SetLaplacianConstraints();

    tree.LaplacianMatrixIteration( solver_divide, ShowResidual , MinIters , SolverAccuracy , MaxSolveDepth , FixedIters );

    isoValue = tree.GetIsoValue();
    isoValue *= offset; //?? im ursprungscode nicht drin

    tree.GetMCIsoTriangles( isoValue , IsoDivide , &mesh , 0 , 1 , !NonManifold , PolygonMesh );

    return 1;
}


int Execute2(std::vector< Point3D<float> >& pts, std::vector< Point3D<float> >& normals, CoredPoissonVectorMeshData< PlyVertex<float> >& mesh,
             int octree = 8, int solver = 8, float point_weight = 4.0f, float samples = 1.0f, float offset = 1.0f)
{
    return Execute< 2, PlyVertex<Real> , false >(pts, normals, mesh, octree, solver, point_weight, samples, offset);
}

