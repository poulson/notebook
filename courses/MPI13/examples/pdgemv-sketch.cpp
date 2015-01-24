#include <cstdlib> 
#include <iostream>
#include <vector>
#include "mpi.h"

int
main( int argc, char* argv[] )
{
    MPI_Init( &argc, &argv );
    MPI_Comm comm = MPI_COMM_WORLD;

    int commSize; MPI_Comm_size( comm, &commSize );
    int commRank; MPI_Comm_rank( comm, &commRank );

    // Read in the command-line argument
    if( argc < 2 )
    {
        if( commRank == 0 )
            std::cout << "pdgemv <n>\n" << std::endl;
        MPI_Finalize();
        return 0;
    }
    const int n = atoi( argv[1] );

    // Ensure that n is an integer multiple of the number of processes
    if( n % commSize != 0 )
    {
        if( commRank == 0 )
            std::cout << "n \% commSize != 0\n" << std::endl;
        MPI_Finalize();
        return 0;
    }

    // Ensure that commSize is a perfect-square
    int sqrtCommSize = 1;
    while( sqrtCommSize*sqrtCommSize < commSize )
        ++sqrtCommSize;
    if( sqrtCommSize*sqrtCommSize != commSize )
    {
        if( commRank == 0 )
            std::cout << "commSize was not a perfect-square\n" << std::endl;
        MPI_Finalize();
        return 0;
    }

    // Create space for the local portions of x and A
    const int ALocalHeight = n / sqrtCommSize;
    const int ALocalWidth = n / sqrtCommSize;
    const int xLocalHeight = n / commSize;
    std::vector<double> ALocal( ALocalHeight*ALocalWidth ),
                        xLocal( xLocalHeight );

    if( commRank == 0 )
    {
        std::cout << "Distributing " << n << " x " << n << " matrix over "
                  << sqrtCommSize << " x " << sqrtCommSize << " grid with "
                  << "blocks of size " << ALocalHeight << " x " << ALocalWidth
                  << ", and a local vector size of " << xLocalHeight << "\n"
                  << std::endl;
    }

    // Set the local portions of A and x to something arbitrary
    // NOTE: drand48() is not available in Windows
    for( int i=0; i<ALocal.size(); ++i )
        ALocal[i] = drand48();
    for( int i=0; i<xLocal.size(); ++i )
        xLocal[i] = drand48();

    // Create the row and column communicators
    MPI_Comm rowComm, colComm;
    const int row = commRank % sqrtCommSize;
    const int col = commRank / sqrtCommSize;
    MPI_Comm_split( comm, row, col, &rowComm );
    MPI_Comm_split( comm, col, row, &colComm );

    // TODO: Run MPI_Allgather within colComm on xLocal
    std::vector<double> xAllgatherLocal( ALocalWidth );

    // TODO: Run local matrix-vector mult with ALocal and xAllgatherLocal
    std::vector<double> zLocal( ALocalHeight, 0 );

    // TODO: Run MPI_Reduce_scatter or MPI_Reduce_scatter_block over rowComm on
    //       zLocal, which is of length n/sqrt(p), and overwrite xLocal, which 
    //       is of length n/p.

    // TODO: Use MPI_Sendrecv (or MPI_Isend and MPI_Irecv) to move the data from
    //       a row-major 1D distribution over the process grid back into the 
    //       original column-major distribution over the process grid, e.g., 
    //       if the process grid is:
    //
    //       0 4 8  12
    //       1 5 9  13
    //       2 6 10 14
    //       3 7 11 15
    //
    //       x would originally have been distributed as
    //
    //       0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
    //
    //       but after the MPI_Reduce_scatter_block or MPI_Reduce+MPI_Scatter
    //       call, it would be distributed as
    //
    //       0 4 8 12 1 5 9 13 2 6 10 14 3 7 11 15
    //
    //       and this can be fixed by having the process in the (s,t) position 
    //       of the grid exchange its data with the process in the (t,s) 
    //       position in the grid.

    MPI_Finalize();
    return 0;
}
