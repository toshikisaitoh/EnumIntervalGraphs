#include "./interval.h"
#include "./ultimate.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <functional>
#include <iterator>
#include <limits>
#include <numeric>
#include <utility>
#include <type_traits>
#include <cmath>
#include <cassert>
#include <cstdio>

using namespace std;
using namespace placeholders;

using LL = long long;
using ULL = unsigned long long;
using VI = vector< int >;
using VVI = vector< vector< int > >;
using VS = vector< string >;
using ISS = istringstream;
using OSS = ostringstream;
using PII = pair< int, int >;
using VPII = vector< pair< int, int > >;
template < typename T = int > using VT = vector< T >;
template < typename T = int > using VVT = vector< vector< T > >;
template < typename T = int > using LIM = numeric_limits< T >;

template < typename T > inline istream& operator>>( istream &s, vector< T > &v ){ for ( T &t : v ) { s >> t; } return s; }
template < typename T > inline ostream& operator<<( ostream &s, const vector< T > &v ){ for ( int i = 0; i < int( v.size() ); ++i ){ s << ( " " + !i ) << v[i]; } return s; }

void in_impl(){};
template < typename T, typename... TS > void in_impl( T &head, TS &... tail ){ cin >> head; in_impl( tail ... ); }
#define IN( T, ... ) T __VA_ARGS__; in_impl( __VA_ARGS__ );

template < typename T > struct getv_fmt;
template <> struct getv_fmt<       int >{ static constexpr const char *fmt = "%d"; };
template <> struct getv_fmt< long long >{ static constexpr const char *fmt = "%lld"; };
template < typename T > void getv( std::vector< T > &v ){ for_each( begin( v ), end( v ), []( T &a ){ scanf( getv_fmt< T >::fmt, &a ); } ); };

template < typename T > inline T fromString( const string &s ) { T res; istringstream iss( s ); iss >> res; return res; }
template < typename T > inline string toString( const T &a ) { ostringstream oss; oss << a; return oss.str(); }

#define NUMBERED( name, number ) NUMBERED2( name, number )
#define NUMBERED2( name, number ) name ## _ ## number
#define REP1( n ) REP2( NUMBERED( REP_COUNTER, __LINE__ ), n )
#define REP2( i, n ) REP3( i, 0, n )
#define REP3( i, m, n ) for ( int i = ( int )( m ); i < ( int )( n ); ++i )
#define GET_REP( a, b, c, F, ... ) F
#define REP( ... ) GET_REP( __VA_ARGS__, REP3, REP2, REP1 )( __VA_ARGS__ )
#define FOR( e, c ) for ( auto &&e : c )
#define ALL( c ) begin( c ), end( c )
#define AALL( a ) ( remove_all_extents< decltype( a ) >::type * )a, ( remove_all_extents< decltype( a ) >::type * )a + sizeof( a ) / sizeof( remove_all_extents< decltype( a ) >::type )
#define DRANGE( c, p ) begin( c ), begin( c ) + ( p ), end( c )

#define SZ( v ) ( (int)( v ).size() )
#define EXIST( c, e ) ( ( c ).find( e ) != ( c ).end() )

template < typename T > inline bool chmin( T &a, const T &b ){ if ( b < a ) { a = b; return true; } return false; }
template < typename T > inline bool chmax( T &a, const T &b ){ if ( a < b ) { a = b; return true; } return false; }

#define PB push_back
#define EM emplace
#define EB emplace_back
#define BI back_inserter

#define MP make_pair
#define fst first
#define snd second

#define DUMP( x ) cerr << #x << " = " << ( x ) << endl

IntervalEnumerator::IntervalEnumerator( const int n, const bool c ) : AbstructEnumerator( n ), connected( c )
{
	return;
}

VI IntervalEnumerator::root( const int N ) const
{
 	VI result( 2 * N );
	std::iota( std::begin( result ), std::begin( result ) + N, 0 );
	std::iota( result.rbegin(), result.rbegin() + N, 0 );
	return std::move( result );
}

// bool IntervalEnumerator::exist_edge( const VI &intervals, const int u, const int v ) const
// {
// 	VI opened( N );
// 	for ( const int i : intervals )
// 	{
// 		opened[i] ^= 1;
// 		if ( opened[u] && opened[v] )
// 		{
// 			return true;
// 		}
// 	}
// 	return false;
// }

bool maximal_clique( const VVI &G, const int s )
{
	const int N = SZ( G );

	VI C;
	REP( i, N )
	{
		if ( s & 1 << i )
		{
			C.PB( i );
		}
	}

	FOR( i, C )
	{
		FOR( j, C )
		{
			if ( i != j && !G[i][j] )
			{
				return false;
			}
		}
	}

	REP( i, N )
	{
		if ( s & 1 << i )
		{
			continue;
		}

		bool f = true;
		FOR( j, C )
		{
			f &= G[i][j];
		}
		if ( f )
		{
			return false;
		}
	}
	return true;
}

VI IntervalEnumerator::remove_edge( const VI &intervals, const int u, const int v ) const
{
// 	MPQ_Tree mpqtree( intervals );
// 	mpqtree.normalize();
// 	mpqtree.relabel_nodes();
// 	mpqtree.unlink( u, v );
// // 	std::cout << "removed : ";
// // 	output(  mpqtree.normalize().relabel_nodes().canonical_form() );
// 	return MPQ_Tree( mpqtree.normalize().relabel_nodes().canonical_form() ).normalize().relabel_nodes().canonical_form();

// 	cout << "remove edge : " << u << ' ' << v << endl;
	const int N = SZ( intervals ) / 2;
	VVI G( N, VI( N ) );
	{
		set< int > s;
		FOR( i, intervals )
		{
			FOR( j, s )
			{
				G[i][j] = G[j][i] = 1;
			}

			if ( EXIST( s, i ) )
			{
				s.erase( i );
			}
			else
			{
				s.insert( i );
			}
		}
	}

	if ( !G[u][v] )
	{
		return {};
	}

	G[u][v] = G[v][u] = 0;

	VI used( N );;
	vector< VVI > Gs;
	for ( int u; ( u = find( ALL( used ), 0 ) - begin( used ) ) < N; )
	{
		VI use( N );
		{
			const function< void( const int ) > dfs = [&]( const  int u )
			{
				used[u] = use[u] = 1;
				for ( int v = 0; v < N; ++v )
				{
					if ( G[u][v] && !use[v] )
					{
						dfs( v );
					}
				}
			};
			dfs( u );
		}

		VVI G2;
		for ( int i = 0; i < N; ++i )
		{
			if ( !use[i] )
			{
				continue;
			}

			G2.emplace_back();
			for ( int j = 0; j < N; ++j )
			{
				if ( use[j] )
				{
					G2.back().push_back( G[i][j] );
				}
			}
		}
		

		Gs.push_back( G2 );
	}

	if ( connected && 2 <= Gs.size() )
	{
		return {};
	}

	VI res;
	for ( const auto G2 : Gs )
	{
		const VI S = graph_to_string_form( G2 );
		if ( S.empty() )
		{
			return {};
		}

		const int base = res.size() / 2;
		transform( ALL( S ), BI( res ), bind( plus< int >(), _1, base ) );
	}
	return MPQ_Tree( res ).normalize().relabel_nodes().canonical_form();
	
// 	VI cliques;
// 	REP( s, 1 << N )
// 	{
// 		if ( maximal_clique( G, s ) )
// 		{
// 			cliques.PB( s );
// 		}
// 	}
// 	const int C = SZ( cliques );
//
// 	VI order;
// 	{
// 		VVI dp( 1 << C, VI( C, -1 ) );
// 		// dp[ visited clique ][ last visited clique ] := prev clique
//
// 		REP( i, C )
// 		{
// 			dp[ 1 << i ][i] = 0;
// 		}
//
// 		REP( s, 1 << C )
// 		{
// 			REP( i, C )
// 			{
// 				if ( dp[s][i] == -1 )
// 				{
// 					continue;
// 				}
//
// 				REP( j, C )
// 				{
// 					if ( connected && ( s & 1 << j || !( cliques[i] & cliques[j] ) ) ||
// 							!connected && ( s & 1 << j ) )
// 					{
// 						continue;
// 					}
//
// 					bool ok = true;
// 					REP( k, C )
// 					{
// 						if ( j == k || s & 1 << k )
// 						{
// 							continue;
// 						}
// 						ok &= !( cliques[i] & ~cliques[j] & cliques[k] );
// 					}
//
// 					if ( ok )
// 					{
// 						dp[ s | 1 << j ][j] = i;
// 					}
// 				}
// 			}
// 		}
//
// 		const auto it = find_if( ALL( dp.back() ), []( const int a ){ return a != -1; } );
// 		if ( it == std::end( dp.back() ) )
// 		{
// 			return {};
// 		}
//
// 		for ( int s = ( 1 << C ) - 1, i = it - std::begin( dp.back() ); s; )
// 		{
// 			DUMP( i);
// 			if ( i == -1 )
// 			{
// 				return {};
// 			}
//
// 			order.PB( i );
//
// 			const int ns = s ^ 1 << i;
// 			const int ni = dp[s][i];
// 			DUMP( dp[s][i] );
//
// 			s = ns;
// 			i = ni;
// 		}
// 	}
//
//
// 	VI res;
// 	REP( i, N )
// 	{
// 		if ( cliques[ order[0] ] & 1 << i )
// 		{
// 			res.PB( i );
// 		}
// 	}
// 	REP( i, SZ( order ) - 1 )
// 	{
// 		const int idx1 = order[i];
// 		const int idx2 = order[ i + 1 ];
//
// 		REP( t, 2 )
// 		{
// 			REP( j, N )
// 			{
// 				const bool f1 = cliques[ idx1 ] & 1 << j;
// 				const bool f2 = cliques[ idx2 ] & 1 << j;
//
// 				if ( f1 ^ f2 )
// 				{
// 					if ( t == 0 && f1 || t == 1 && f2 )
// 					{
// 						res.PB( j );
// 					}
// 				}
// 			}
// 		}
// 	}
// 	REP( i, N )
// 	{
// 		if ( cliques[ order.back() ] & 1 << i )
// 		{
// 			res.PB( i );
// 		}
// 	}
// // 	std::cout << "remove edge : " << u + 1 << ' ' << v + 1 << endl;
// // 	output( res );
// // 	output( MPQ_Tree( res ).normalize().relabel_nodes().canonical_form() );
// // 	output( MPQ_Tree( res ).normalize().normalize().relabel_nodes().canonical_form() );
// // 	output( MPQ_Tree( res ).normalize().normalize().normalize().relabel_nodes().canonical_form() );
// 	return MPQ_Tree( res ).normalize().relabel_nodes().canonical_form();
}

// 	std::cerr << "remove edge" << std::endl;

// 	VI relation;
// 	for ( const int i : intervals )
// 	{
// 		if ( i == u || i == v )
// 		{
// 			relation.push_back( i );
// 		}
// 	}
//
// 	if ( relation[0] == relation[1] )
// 	{
// 		return {};
// 	}
//
// 	if ( relation[0] == relation[3] ) // one contains another ( "abba" -> "bbaa" )
// 	{
// 		MPQ_Tree mpqtree( intervals );
// // 		std::cout << "original canonical ";
// // 		output(	mpqtree.canonical_form() );
// 		mpqtree.rotate( relation[0], relation[1] );
// 		VI ints = mpqtree.canonical_form();
// // 		std::cout << "unlink " << u + 1 << ' ' << v + 1 << std::endl;
// // 		std::cout << "ints : ";
// // 		output( ints );
//
// 		{ // "axyzbb---a" -> "xyzbba---a"
// 			// "a(((b(((b----a" -> "(((b(((ba----a
// 			// "a(((b)))b----a" -> "(((ab)))ba----a" -> "(((abb)))----a" -> "(((bba)))----a
//
// 			VI opened( N );
// 			int i = 0;
// 			for ( ; i + 2 < int( intervals.size() ) && ints[i] != relation[0]; ++i )
// 			{
// 				opened[ ints[i] ] = 1;
// 			}
// // 			std::cout << i << std::endl;
// 			for ( int j = i + 1; j < int( intervals.size() ); ++j )
// 			{
// 				if ( opened[ ints[j] ] && ints[j] == relation[1] )
// 				{
// 					std::rotate( std::begin( ints ) + i, std::begin( ints ) + i + 1, std::begin( ints ) + j + 1 );
// // 					std::cout << "unlink " << u + 1 << ' ' << v + 1 << std::endl;
// // 					std::cout << "######## result : ";
// // 					output( ints );
// // 					output( MPQ_Tree( ints ).normalize().relabel_nodes().canonical_form() );
// 					return std::move( MPQ_Tree( ints ).normalize().relabel_nodes().canonical_form() );
// 				}
//
// // 				if ( !( i == 0 && j == 1 ) && ints[j] == relation[1] && ints[ j + 1 ] == relation[1] )
// // 				{
// // 					std::rotate( std::begin( ints ) + i, std::begin( ints ) + i + 1, std::begin( ints ) + j + 2 );
// // // 					std::cout << "rotated : ";
// // // 					output( ints );
// // 					return std::move( MPQ_Tree( ints ).normalize().relabel_nodes().canonical_form() );
// // 				}
//
// 				opened[ ints[j] ] = true;
// 			}
// 		}
// // 		{ // "abb---a"
// // 			VI opened( N );
// // 			for ( int i = 1; i + 2 < 2 * N; ++i )
// // 			{
// // 				if ( !opened[ ints[i] ] && ints[i] == relation[0] &&
// // 						ints[ i + 1 ] == relation[1] && ints[ i + 2 ] == relation[1] )
// // 				{
// // 					std::swap( ints[i], ints[ i + 2 ] );
// // 					return std::move( MPQ_Tree( ints ).normalize().relabel_nodes().canonical_form() );
// // 				}
// // 				opened[ ints[i] ] = 1;
// // 			}
// // 		}
// // 		{ // "a---bba"
// // 			VI opened( N );
// // 			for ( int i = 0; i + 2 < 2 * N - 1; ++i )
// // 			{
// // 				if ( opened[ ints[i] ] && ints[i] == relation[1] && ints[i] == ints[ i + 1 ] &&
// // 						ints[ i + 2 ] == relation[0] )
// // 				{
// // 					std::swap( ints[i], ints[ i + 2 ] );
// // 					return std::move( MPQ_Tree( ints ).normalize().relabel_nodes().canonical_form() );
// // 				}
// // 				opened[ ints[i] ] = 1;
// // 			}
// // 		}
// 	}
// 	else // partially intersects
// 	{
// 		// "axyzb(((azyxb" -> "axyz(((abzyxb"
// 		// "axyzb)))azyxb" -> "axyzab)))xzyb"
// 		// "a(((ba)))a
//
// 		for ( int rev = 0; rev < 2; ++rev )
// 		{
// 			VI opened( N ), ints( intervals );
// 			int i = 0;
// 			for ( ; i < 2 * N && ints[i] != relation[1]; ++i )
// 			{
// 				opened[ ints[i] ] = 1;
// 			}
// 			for ( int j = i + 1; j < 2 * N; ++j )
// 			{
// 				if ( opened[ ints[j] ] )
// 				{
// 					break;
// 				}
//
// 				if ( ints[j] == relation[0] )
// 				{
// 					std::rotate( std::begin( ints ) + i, std::begin( ints ) + i + 1, std::begin( ints ) + j + 1 );
// 					return std::move( MPQ_Tree( ints ).normalize().relabel_nodes().canonical_form() );
// 				}
//
// 				opened[ ints[j] ] = 1;
// 			}
//
// 			std::reverse( std::begin( ints ), std::end( ints ) );
// 			std::swap( relation[0], relation[1] );
// 		}
//
//
//
// // 		for ( int i = 0; i + 1 < 2 * N; ++i )
// // 		{
// // 			if ( ints[i] == relation[1] && !opened[ relation[1] ] &&
// // 					ints[ i + 1 ] == relation[0] && opened[ relation[0] ] )
// // 			{
// // 				std::swap( ints[i], ints[ i + 1 ] );
// // 				return std::move( MPQ_Tree( ints ).canonical_form() );
// // 			}
// //
// // 			opened[ ints[i] ] = 1;
// // 		}
// 	}
//
// 	return {};
// }

VI IntervalEnumerator::parent( const VI &intervals ) const
{
	VI ints( intervals ), opened( N );
	for ( int i = 0; i + 1 < 2 * N; ++i )
	{
		if ( opened[ ints[i] ] && !opened[ ints[ i + 1 ] ] )
		{
			std::swap( ints[i], ints[ i + 1 ] );
// 			output( ints );
			return std::move( MPQ_Tree( ints ).normalize().relabel_nodes().canonical_form() );
		}

		opened[ ints[i] ] = 1;
	}

	assert( false );
	return {};
}

bool IntervalEnumerator::recognition( const VI &intervals ) const
{
	if ( int( intervals.size() ) != 2 * N )
	{
		return false;
	}
	return true;

// 	int depth = 0;
// 	VI opened( N );
//
// 	for ( int i = 0; i < int( intervals.size() ); ++i )
// 	{
// 		opened[ intervals[i] ] ^= 1;
// 		depth += opened[ intervals[i] ] ? 1 : -1;
// 		if ( depth == 0 && i + 1 != int( intervals.size() ) )
// 		{
// 			return false;
// 		}
// 	}
// 	return depth == 0;
}

bool IntervalEnumerator::isomorphic( const VI &v1, const VI &v2 ) const
{
// 	return v1 == v2;
	return MPQ_Tree( v1 ).normalize().relabel_nodes().canonical_form() == MPQ_Tree( v2 ).normalize().relabel_nodes().canonical_form();
}

void IntervalEnumerator::output( std::ostream &out, const VI &intervals ) const
{
	for ( int i = 0; i < int( intervals.size() ); ++i )
	{
		out << intervals[i] + 1 << ( i + 1 < int( intervals.size() ) ? ' ' : '\n' );
	}
	out << std::flush;

	return;
}
