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
            std::cout << "Scatter <n>\n" << std::endl;
        MPI_Finalize();
        return 0;
    }

    // Read in the length of the buffer to scatter
    const int n = atoi( argv[1] );

    // For now, we will require that the length of the buffer is an integer 
    // multiple of the number of processes
    if( n % commSize != 0 )
    {
        if( commRank == 0 )
            std::cout << "Vector length was not multiple of number of procs" 
                      << std::endl;
        MPI_Finalize();
        return 0;
    }

    // Each process will receive n/commSize entries
    const int recvSize = n/commSize;
    std::vector<double> recvBuf( recvSize ); 

    if( commRank == 0 )
    {
        // Initialize a vector of length n on the root
        std::vector<double> sendBuf( n );
        for( int i=0; i<n; ++i )
            sendBuf[i] = i;

        MPI_Scatter
        ( &sendBuf[0], recvSize, MPI_DOUBLE, 
          &recvBuf[0], recvSize, MPI_DOUBLE, 0, comm );
    }
    else
    {
        MPI_Scatter
        ( 0,           recvSize, MPI_DOUBLE,
          &recvBuf[0], recvSize, MPI_DOUBLE, 0, comm );
    }

    // Now have each process in turn print its received data
    for( int q=0; q<commSize; ++q )
    {
        if( commRank == q )
        {
            std::cout << "Process " << q << "'s received data:\n";
            for( int i=0; i<recvSize; ++i )
                std::cout << recvBuf[i] << "\n";
            std::cout << std::endl;
        }
        MPI_Barrier( comm );
    }

    MPI_Finalize();
    return 0;
}
