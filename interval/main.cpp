#include <bits/stdc++.h>

#include "./cmdline.h"
#include "./interval.h"

#ifndef USE_MPI
#error "USE_MPI macro must be defined as 0 or 1"
#endif

#if USE_MPI
#include "./../common/parallel.h"
int NUM_PROC;
int MY_RANK;
MPI_Status mpi_status;
#endif

void print_vi( const VI &v )
{
	for ( const int a : v )
	{
		std::cout << a + 1 << ' ';
	}
	std::cout << std::endl;
	return;
}

int main( int argc, char *argv[] )
{
#if USE_MPI
	MPI_Init(&argc, &argv);
	MPI_Comm_size( MPI_COMM_WORLD, &NUM_PROC );
	MPI_Comm_rank( MPI_COMM_WORLD, &MY_RANK );

	assert( 2 <= NUM_PROC );
#endif

	cmdline::parser optparser;
	optparser.add< int >( "size", 'n', "Number of vertices of graph to enumerate", true );
	optparser.add( "connected", 'c', "Enumerate connected graphs only" );
	optparser.add( "all", 'a', "Enumerate all graphs such that number of its vertices is less than or equal to n" );

	if ( !optparser.parse( argc, argv ) )
	{
		std::cerr << optparser.error_full();
		std::cerr << optparser.usage();
		std::cerr << std::flush;
		return 1;
	}

	const int N = optparser.get< int >( "size" );
	const bool connected = optparser.exist( "connected" );
	const bool enum_all = optparser.exist( "all" );

	for ( int i = enum_all ? 1 : N; i <= N; ++i )
	{
		IntervalEnumerator enumerator( i, connected );
#if USE_MPI
		if ( MY_RANK == 0 )
#endif
		{
			std::ostringstream oss;
			oss << ( connected ? "interval_connected_" : "interval_disconnected_" ) << i << ".out";
			std::ofstream out( oss.str() );

			std::cout << "size : " << i << std::endl;

			std::ofstream exec_time( "exec_times.csv", std::ofstream::app );
			const clock_t clock_start = clock();

			enumerator.enumerate( out );

			exec_time << ( USE_MPI ? "MPI" : "NON-MPI" ) << '\t';
			exec_time << ( connected ? "connected" : "disconnected" ) << '\t';
			exec_time << i << '\t';
			exec_time << std::setprecision( 2 ) << std::fixed << ( ( 1. * clock() - clock_start ) / CLOCKS_PER_SEC ) << std::endl;
		}
#if USE_MPI
		else
		{
			enumerator.slave();
		}
#endif
	}

#if USE_MPI
	MPI_Finalize();  
#endif

	return 0;
}
