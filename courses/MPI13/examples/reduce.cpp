#include <cstdlib>  // for atoi
#include <iostream> // for std::cout
#include <vector>   // for std::vector
#include "mpi.h"    // for MPI...

int
main( int argc, char* argv[] )
{
    // Initialize MPI by passing in the command-line argument count (argc)
    // and list (argv)
    MPI_Init( &argc, &argv );

    // Query our process rank and the number of processes
    MPI_Comm comm = MPI_COMM_WORLD;
    int commSize; MPI_Comm_size( comm, &commSize );
    int commRank; MPI_Comm_rank( comm, &commRank );

    // Ensure that there is at least one command-line argument 
    // (in addition to the routine name, which is argv[0])
    if( argc < 2 )
    {
        if( commRank == 0 )
            std::cout << "Reduce <n>\n" << std::endl;
        MPI_Finalize();
        return 0;
    }

    // Read in the length of the vector to reduce
    const int n = atoi( argv[1] );

    // Initialize a vector of length n to all ones
    std::vector<double> sendBuf( n, 1. );
    
    if( commRank == 0 )
    {
        // If we are the root process, we will receive the sum of commSize 
        // vectors of all ones
        std::vector<double> recvBuf( n );
        MPI_Reduce( &sendBuf[0], &recvBuf[0], n, MPI_DOUBLE, MPI_SUM, 0, comm );

        // The root can now print the result
        std::cout << "recvBuf:\n";
        for( int i=0; i<n; ++i )
            std::cout << recvBuf[i] << "\n";
        std::cout << std::endl;
    }
    else
    {
        // If we are not the root process, we do not need to allocate a recv buf
        MPI_Reduce( &sendBuf[0], 0, n, MPI_DOUBLE, MPI_SUM, 0, comm );
    }

    MPI_Finalize();
    return 0;
}
