#include <stdio.h>
#include "mpi.h"

int main( int argc, char* argv[] )
{
    MPI_Init( &argc, &argv );

    int commRank; MPI_Comm_rank( MPI_COMM_WORLD, &commRank );
    int commSize; MPI_Comm_size( MPI_COMM_WORLD, &commSize );

    int rightRank = (commRank+1) % commSize;
    int leftRank = (commRank+commSize-1) % commSize;

    int recvRank;
    MPI_Request recvReq;
    MPI_Irecv( &recvRank, 1, MPI_INT, leftRank, 0, MPI_COMM_WORLD, &recvReq );

    MPI_Request sendReq;
    MPI_Isend( &commRank, 1, MPI_INT, rightRank, 0, MPI_COMM_WORLD, &sendReq );

    MPI_Status recvStat;
    MPI_Wait( &recvReq, &recvStat );

    printf("Process %d received from rank %d\n",commRank,recvRank);

    MPI_Finalize();
    return 0;
}
