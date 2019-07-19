#include <bits/stdc++.h>

#include "./mpq_tree.h"
#include "./mpq_tree_node.h"

VI MPQ_Tree::label;
int MPQ_Tree::n;

int MPQ_Tree::relabel( const int v )
{
	if ( v == -1 )
	{
		return -1;
	}

	if ( label[v] == -1 )
	{
		label[v] = n++;
	}
	return label[v];
}

MPQ_Tree::MPQ_Tree( const VI &intervals ) : N( intervals.size() / 2 ), root( [&]{
			const VVI llci = ll_reorder( N, compactization( intervals ) );
			const auto laminar = classify( N, llci );
			return link_nodes( N, llci, laminar );
			}() )
{

	return;
}

MPQ_Tree::MPQ_Tree( const MPQ_Tree &t ) : N( t.N ), root( t.root->clone() )
{
	return;
}

MPQ_Tree& MPQ_Tree::normalize()
{
	root->calculate_size();
	root->normalize();

	return *this;
}

MPQ_Tree& MPQ_Tree::relabel_nodes()
{
	MPQ_Tree::label.clear();
	MPQ_Tree::label.resize( N, -1 );
	MPQ_Tree::n = 0;
	root->relabel();
	return *this;
}

VI MPQ_Tree::canonical_form() const
{
	root->canonical_form();
	return root->canostr;
}

VVI MPQ_Tree::compactization( const VI &intervals )
{
	const int N = intervals.size() / 2;

	bool last_r = false;
	std::vector< bool > opened( N );
	VVI result( 1 );

	for ( const int e : intervals )
	{
		if ( !opened[e] && last_r )
		{
			result.emplace_back();
		}

		result.back().push_back( e );
		last_r = opened[e];
		opened[e] = true;
	}

	return std::move( result );
}

VVI MPQ_Tree::ll_reorder( const int N, const VVI &ci )
{
	int num_left = 0;
	VI opened( N ), exist_l( N, -1 ), exist_r( N, -1 ), begins( ci.size(), -1 );
	VVI result( ci.size() );

	for ( int i = 0; i < int( ci.size() ); ++i )
	{
		for ( const int e : ci[i] )
		{
			if ( !opened[e] )
			{
				result[i].push_back( num_left );
				exist_l[e] = i;
				begins[i] = num_left;
				++num_left;

				opened[e] = true;
			}
			else
			{
				exist_r[ begins[ exist_l[e] ] ] = i;
				--begins[ exist_l[e] ];
			}
		}
	}

	for ( int i = N - 1; 0 <= i; --i )
	{
		result[ exist_r[i] ].push_back( i );
	}
	
	return std::move( result );
}

// std::vector< std::pair< MPQ_Tree::NODE_TYPE, int > > MPQ_Tree::classify( const int N, const VVI &llci )
// {
// 	std::vector< int > stack;
// 	std::vector< std::pair< NODE_TYPE, int > > laminar( N + 1, std::make_pair( NODE_TYPE::NIL, -1 ) );
// 	VI flags( N + 1, -1 ), pi( N, -1 );
// 	std::vector< bool > opened( N );
// 	int flag = 0, p = -1, q = -1;
//
// 	for ( int i = 0; i < int( llci.size() ); ++i )
// 	{
// 		for ( const int e : llci[i] )
// 		{
// 			std::cout << "flags : ";
// 			for ( const int i : flags )
// 			{
// 				std::cout << i << ' ';
// 			}
// 			std::cout << std::endl;
//
// 			if ( !opened[e] )
// 			{
// 				stack.push_back( e );
// 				opened[e] = true;
// 				continue;
// 			}
//
// 			const int x = stack.back();
// // 			stack.pop();
//
// 			std::cout << "( x, e ) : " << x + 1 << ' ' << e + 1 << std::endl;
// 			if ( x != e || laminar[ stack.back() ].first == NODE_TYPE::Q )
// 			{
// 				if ( laminar[ stack.back() ].first == NODE_TYPE::NIL )
// 				{
// 					++q;
// 					laminar[ stack.back() ] = NODE_TAG( NODE_TAG::Q, q );
// 				}
//
// 				for ( int i = stack.size() - 1; 0 <= i; --i )
// 				{
// 					laminar[ stack[i] ] = laminar[ stack.back() ];
// 					if ( stack.back() == e )
// 					{
// 						for ( const auto tag = laminar[ stack.back() ]; laminar[ stack.back() ] == tag; stack.pop_back() );
// 						flag = 1;
// 					}
//
// 					if ( x == e 
//
//
// 			if ( x == e )
// 			{
// 				if ( flags[ stack.size() + 1 ] == -1 )
// 				{
// 					const auto q_flag = std::make_pair( NODE_TYPE::Q, flag );
// 					if ( laminar[ e + 1 ] == q_flag )
// 					{
// 						laminar[e] = q_flag;
// 					}
// 					else
// 					{
// 						// algorithm 4
// 						if ( laminar[ e + 1 ] != std::make_pair( NODE_TYPE::P, p ) )
// 						{
// 							++p;
// 						}
// 						laminar[e] = std::make_pair( NODE_TYPE::P, p );
// 						// --
//
// 						flag = -1;
// // 						flag = 0;
// 					}
// 				}
// 				else
// 				{
// 					laminar[e] = std::make_pair( NODE_TYPE::Q, flags[ stack.size() + 1 ] );
// 					flags[ stack.size() ] = flags[ stack.size() + 1 ];
// 				}
// 			}
// 			else
// 			{
// 				pi[e] = x;
// 				if ( pi[x] == -1 )
// 				{
// 					//  algorithm 5
// 					if ( laminar[e].first == NODE_TYPE::NIL )
// 					{
// 						++q;
// 						laminar[e] = std::make_pair( NODE_TYPE::Q, q );
// 						laminar[x] = laminar[e];
// 					}
// 					else
// 					{
// 						laminar[x] = laminar[e];
// 					}
// 					// --
//
// 					std::cout << "hoge" << std::endl;
// 					flags[ stack.size() ] = q;
// 				}
// 				else
// 				{
// 					std::cout << "piyo" << std::endl;
// 					flags[ stack.size() ] = -1;
// 					flag = laminar[x].second;
// 				}
// 			}
// 			flags[ stack.size() + 1 ] = -1;
// 		}
// 	}
//
// 	for ( int i = 0; i < N; ++i )
// 	{
// 		const auto p = laminar[i];
// 		std::cerr << i + 1 << " : " << ( p.first == NODE_TYPE::P ? 'P' : 'Q' ) << ' ' << p.second << std::endl;
// 	}
// 	std::cerr << std::endl;
//
// 	return std::move( laminar );
// }


class UnionFind
{
private:
	const int N;

	std::vector<int> parent;
	std::vector<int> rank;

	int groups_;
	std::vector< int > sizes;

public:
	UnionFind( int n ) : N( n ), parent( N ), rank( N, 0 ), groups_( N ), sizes( N, 1 )
	{
		std::iota( parent.begin(), parent.end(), 0 );

		return;
	}

	int find( int x )
	{
		if ( parent[x] == x )
		{
			return x;
		}
		return parent[x] = find( parent[x] );
	}

	bool same( int x, int y )
	{
		return find( x ) == find( y );
	}

	bool unite( int x, int y )
	{
		x = find( x );
		y = find( y );

		if ( x == y )
		{
			return false;
		}

		if ( rank[x] < rank[y] )
		{
			parent[x] = y;
			sizes[y] += sizes[x];
		}
		else
		{
			parent[y] = x;
			sizes[x] += sizes[y];

			if ( rank[x] == rank[y] )
			{
				++rank[x];
			}
		}

		--groups_;
		return true;
	}

	int groups() const
	{
		return groups_;
	}

	int groupSize( const int x )
	{
		return sizes[ find( x ) ];
	}
};
// UnionFind( N )
// find( x )
// same( x, y )
// unite( x, y )
// groups()
// groupSize( x )

std::vector< std::pair< MPQ_Tree::NODE_TYPE, int > > MPQ_Tree::classify( const int N, const VVI &llci )
{
// 	std::stack< int > stack;
	std::vector< std::pair< NODE_TYPE, int > > laminar( N + 1, std::make_pair( NODE_TYPE::NIL, -1 ) );
// 	VI flags( N + 1, 0 ), pi( N, -1 );
// 	std::vector< bool > opened( N );
// 	int flag = 0, p = 0, q = 0;
//
// 	for ( int i = 0; i < int( llci.size() ); ++i )
// 	{
// 		for ( const int e : llci[i] )
// 		{
// 			if ( !opened[e] )
// 			{
// 				stack.push( e );
// 				opened[e] = true;
// 				continue;
// 			}
//
// 			const int x = stack.top();
// 			stack.pop();
//
// // 			std::cout << "( e, x ) : " << e + 1 << ' ' << x + 1 << std::endl;
//
// 			if ( x == e )
// 			{
// 				if ( flags[ stack.size() + 1 ] == 0 )
// 				{
// 					const auto q_flag = std::make_pair( NODE_TYPE::Q, flag );
// 					if ( laminar[ e + 1 ] == q_flag )
// 					{
// 						laminar[e] = q_flag;
// 					}
// 					else
// 					{
// 						// algorithm 4
// 						if ( laminar[ e + 1 ] != std::make_pair( NODE_TYPE::P, p ) )
// 						{
// 							++p;
// 						}
// 						laminar[e] = std::make_pair( NODE_TYPE::P, p );
// 						// --
//
// // 						flag = -1;
// 						flag = 0;
// 					}
// 				}
// 				else
// 				{
// 					laminar[e] = std::make_pair( NODE_TYPE::Q, flags[ stack.size() + 1 ] );
// 					flags[ stack.size() ] = flags[ stack.size() + 1 ];
// 				}
// 			}
// 			else
// 			{
// 				pi[e] = x;
// // 				std::cout << "pi x : " << pi[x] + 1 << std::endl;
// 				if ( pi[x] == -1 )
// 				{
// 					//  algorithm 5
// // 					std::cout << "create Q" << std::endl;
// 					if ( laminar[e].first == NODE_TYPE::NIL )
// 					{
// 						++q;
// 						laminar[e] = std::make_pair( NODE_TYPE::Q, q );
// 						laminar[x] = laminar[e];
// 					}
// 					else
// 					{
// 						laminar[x] = laminar[e];
// 					}
// 					// --
//
// 					flags[ stack.size() ] = q;
// 				}
// 				else
// 				{
// 					flags[ stack.size() ] = 0;
// 					flag = laminar[x].second;
// 				}
// 			}
// 			flags[ stack.size() + 1 ] = 0;
// 		}
// 	}

	{
		VI intervals;
		for ( const VI &row : llci )
		{
			copy( begin( row ), end( row ), back_inserter( intervals ) );
		}
		
		UnionFind uf( N );
		for ( int i = 0; i < 2 * N; ++i )
		{
			const int j = find( begin( intervals ) + i + 1, end( intervals ), intervals[i] ) - begin( intervals );
			if ( j == 2 * N )
			{
				continue;
			}

			VI counts( N );
			for ( int k = i + 1; k < j; ++k )
			{
				++counts[ intervals[k] ];
			}

			for ( int k = 0; k < N; ++k )
			{
				if ( counts[k] % 2 )
				{
					uf.unite( intervals[i], k );
					laminar[ intervals[i] ].first = laminar[k].first = NODE_TYPE::Q;
				}
			}
		}

		VI ps;
		for ( const int i : intervals )
		{
			if ( laminar[i].first != NODE_TYPE::Q )
			{
				laminar[i].first = NODE_TYPE::P;
				ps.push_back( i );
			}
		}
		VVI poss( N );
		for ( int i = 0; i < ps.size(); ++i )
		{
			poss[ ps[i] ].push_back( i );
		}

		for ( int i = 0; i < N; ++i )
		{
			for ( int j = i + 1; j < N; ++j )
			{
				if ( poss[i].empty() || poss[j].empty() )
				{
					continue;
				}
				if ( poss[i][0] + 1 == poss[j][0] && poss[j][1] + 1 == poss[i][1] )
				{
					uf.unite( i, j );
				}
			}
		}

		for ( int i = 0; i < N; ++i )
		{
			laminar[i].second = uf.find( i );
		}
	}



// 	{
// 		VI qs;
// 		for ( int i = 0; i < int( llci.size() ); ++i )
// 		{
// 			for ( const int e : llci[i] )
// 			{
// 				if ( laminar[e].first != NODE_TYPE::P )
// 				{
// 					std::cout << "push : " << e + 1 << std::endl;
// 					qs.push_back( laminar[e].second );
// 				}
// 			}
// 		}
// 		{
// 			VI qs2( qs );
// 			std::sort( std::begin( qs2 ), std::end( qs2 ) );
// 			qs2.erase( std::unique( std::begin( qs2 ), std::end( qs2 ) ), std::end( qs2 ) );
//
// 			const auto f = [&]( const int q ){ return std::lower_bound( std::begin( qs2 ), std::end( qs2 ), q ) - std::begin( qs2 ); };
// 			std::transform( std::begin( qs ), std::end( qs ), std::begin( qs ), f );
// 			for ( int i = 0; i < N; ++i )
// 			{
// 				if ( laminar[i].first != NODE_TYPE::Q )
// 				{
// 					laminar[i].second = f( laminar[i].second );
// 				}
// 			}
// 		}
//
// 		std::cout << "qs : ";
// 		for ( const int q : qs ) std::cout << q << ' ';
// 		std::cout << std::endl;
// 		std::map< int, int > counts;
// 		std::for_each( std::begin( qs ), std::end( qs ), [&]( const int q ){ ++counts[q]; } );
//
// 		for ( int i = 0; i < int( qs.size() ); ++i )
// 		{
// 			if ( std::begin( qs ) + i != std::find( std::begin( qs ), std::end( qs ), qs[i] ) )
// 			{
// 				continue;
// 			}
// 			const int j = std::find( qs.rbegin(), qs.rend(), qs[i] ).base() - 1 - std::begin( qs );
// 			std::cout << i << ' ' << j << std::endl;
//
// 			for ( int q = 0; q < N; ++q )
// 			{
// 				if ( q == qs[i] )
// 				{
// 					continue;
// 				}
//
// 				if ( std::count( std::begin( qs ) + i + 1, std::begin( qs ) + j, q ) != counts[q] )
// 				{
// 					std::cout << "laminars of q : ";
// 					for ( int i = 0; i < N; ++i )
// 					{
// 						if ( laminar[i].first != NODE_TYPE::P )
// 						{
// 							std::cout << laminar[i].second << ' ';
// 						}
// 					}
// 					std::cout << std::endl;
// 					std::replace_if( std::begin( laminar ), std::end( laminar ),
// 							std::bind( std::equal_to< NODE_TAG >(), std::placeholders::_1, NODE_TAG( NODE_TYPE::Q, q ) ),
// 							NODE_TAG( NODE_TYPE::Q, qs[i] ) );
// 					std::transform( 
//
// 					counts.clear();
// 					std::for_each( std::begin( qs ), std::end( qs ), [&]( const int q ){ ++counts[q]; } );
// 					std::cout << "laminars of q : ";
// 					for ( int i = 0; i < N; ++i )
// 					{
// 						if ( laminar[i].first != NODE_TYPE::P )
// 						{
// 							std::cout << laminar[i].second << ' ';
// 						}
// 					}
// 					std::cout << std::endl << std::endl;
// 				}
//
// 			}
// 		}
// 	}

// 	for ( int i = 0; i < N; ++i )
// 	{
// 		auto &p = laminar[i];
// 		if ( laminar[i].first != NODE_TYPE::P )
// 		{
// 			p.first = NODE_TYPE::Q;
// 		}
//
// 		std::cerr << i + 1 << " : " << ( p.first == NODE_TYPE::NIL ? "NIL" : p.first == NODE_TYPE::P ? "P" : "Q" ) << ' ' << p.second << std::endl;
// 	}
// 	std::cerr << std::endl;

	return std::move( laminar );
}

std::shared_ptr< MPQ_Tree::Node > MPQ_Tree::link_nodes( const int N, const VVI &llci, const std::vector< MPQ_Tree::NODE_TAG > &laminar )
{
	std::vector< std::shared_ptr< Node > > nodes;
	nodes.reserve( 2 * N );
	std::generate_n( std::back_inserter( nodes ), N, []{ return std::make_shared< PNode >(); } );
	std::generate_n( std::back_inserter( nodes ), N, []{ return std::make_shared< QNode >(); } );
	// [ 0, N ) := P Node
	// [ N, 2 * N ) := Q Node

	VI opend( N ), flags( 2 * N );
	std::stack< std::shared_ptr< Node > > stack;
	stack.push( std::make_shared< PNode >() );
	for ( int i = 0; i < int( llci.size() ); ++i )
	{
		for ( const int e : llci[i] )
		{
			const int nptr_index = ( laminar[e].first == NODE_TYPE::P ? 0 : N ) + laminar[e].second;
			const std::shared_ptr< Node > &nx = nodes[ nptr_index ];
// 			std::cerr << nptr_index << std::endl;

			nx->add_interval( e );

			if ( !opend[e] )
			{
				opend[e] = true;

				if ( !flags[ nptr_index ] )
				{
					stack.top()->add_child( nx );
					stack.push( nx );
				}
				++flags[ nptr_index ];

// 				if ( flags[ nptr_index ] )
// 				{
// 					stack.top()->add_interval( e );
// 				}
// 				else
// 				{
// 					nx->add_interval( e );
// 					std::cerr << nx << std::endl;
// 					stack.top()->add_child( nx );
// 					stack.push( nx );
// 					++flags[ nptr_index ];
// 				}
			}
			else
			{
// 				if ( flags[ nptr_index ] )
// 				{
// 					stack.top()->add_interval( e );
// 					nx->add_interval( e );
// 				}
// 				std::cerr << e << std::endl;
// 				nx->add_interval( e );

				--flags[ nptr_index ];
				if ( !flags[ nptr_index ] )
				{
					stack.pop();
				}
			}
		}
		// section proseccing (?)
		// but the scope of Nx is dead
	}

	for ( int i = N; i < 2 * N; ++i )
	{
		dynamic_cast< QNode & >( *nodes[i] ).shrink();
	}

	assert( stack.size() ==1 );
	return stack.top();
}
