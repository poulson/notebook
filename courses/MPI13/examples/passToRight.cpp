#include <iostream>
#include <sstream>
#include "mpi.h"

int main( int argc, char* argv[] )
{
    MPI_Init( &argc, &argv );

    int commRank; MPI_Comm_rank( MPI_COMM_WORLD, &commRank );
    int commSize; MPI_Comm_size( MPI_COMM_WORLD, &commSize );

    const int rightRank = (commRank+1) % commSize;
    const int leftRank = (commRank+commSize-1) % commSize;

    int recvRank;
    MPI_Request recvReq;
    MPI_Irecv( &recvRank, 1, MPI_INT, leftRank, 0, MPI_COMM_WORLD, &recvReq );

    MPI_Request sendReq;
    MPI_Isend( &commRank, 1, MPI_INT, rightRank, 0, MPI_COMM_WORLD, &sendReq );

    MPI_Status recvStat;
    MPI_Wait( &recvReq, &recvStat );

    std::ostringstream os;    
    os << "Process " << commRank << " received from rank " << recvRank 
       << std::endl;
    std::cout << os.str();

    MPI_Finalize();
    return 0;
}
