#include <bits/stdc++.h>

#include "./mpq_tree.h"
#include "./mpq_tree_node.h"

template < typename T >
inline std::ostream& operator<<( std::ostream &s, const std::vector< T > &v )
{
	for ( int i = 0; i < int( v.size() ); ++i )
	{
		s << "{ ";
		s << ( " " + !i ) << v[i];
		s << " } ";
	}
	return s;
}

void dump( const std::string &name, const VI &v )
{
	std::cout << name << " : ";
	for ( const int a : v )
	{
		std::cout << a << ' ';
	}
	std::cout << std::endl;
	return;
}

bool MPQ_Tree::Node::operator==( const Node &node ) const
{
	return !( *this < node ) && !( node < *this );
}

bool MPQ_Tree::Node::operator!=( const Node &node ) const
{
	return !( *this == node );
}

bool MPQ_Tree::Node::operator<( const Node &node ) const
{
	if ( size != node.size )
	{
		return size < node.size;
	}

	if ( node_type() != node.node_type() )
	{
		return node.node_type() == NODE_TYPE::Q;
	}

	if ( num_vertices != node.num_vertices )
	{
		return num_vertices < node.num_vertices;
	}

// 	if ( children.size() != node.children.size() )
// 	{
// 		return children.size() < node.children.size();
// 	}

	return compare( node );
}

bool MPQ_Tree::Node::compare_children( const Node &node ) const
{
	return std::lexicographical_compare( std::begin( children ), std::end( children ), 
			std::begin( node.children ), std::end( node.children ),
			[]( const auto &p, const auto &q ){ return *p < *q; } );
}

void MPQ_Tree::Node::clone_common( Node &result )
{
	std::transform( std::begin( children ), std::end( children ), std::back_inserter( result.children ),
			std::mem_fn( &Node::clone ) );
	std::for_each( std::begin( result.children ), std::end( result.children ),
			std::bind( std::mem_fn( &Node::set_parent ), std::placeholders::_1, result.shared_from_this() ) );

	result.size = size;
	result.num_vertices = num_vertices;

	result.canostr = canostr;

	return;
}

void MPQ_Tree::Node::set_parent( const std::shared_ptr< Node > &p )
{
	parent = p;
	return;
}

bool MPQ_Tree::Node::has_parent() const
{
	return !parent.expired();
}

void MPQ_Tree::Node::calculate_size()
{
	std::for_each( std::begin( children ), std::end( children ), std::mem_fn( &Node::calculate_size ) );
	size = 1 + std::accumulate( std::begin( children ), std::end( children ), 0,
			[&]( const int acc, const auto &p ){ return acc + p->size; } );
	return;
}

MPQ_Tree::PNode::PNode() : Node()
{
	return;
}

bool MPQ_Tree::PNode::compare( const Node &node ) const
{
	const auto &pnode = dynamic_cast< const PNode & >( node );

	if ( children.size() != pnode.children.size() )
	{
		return children.size() < pnode.children.size();
	}

	return compare_children( node );
}

std::shared_ptr< MPQ_Tree::Node > MPQ_Tree::PNode::clone()
{
	const std::shared_ptr< PNode > result = std::make_shared< PNode >();

	clone_common( *result );

	result->intervals = intervals;

	return result;
}

MPQ_Tree::NODE_TYPE MPQ_Tree::PNode::node_type() const
{
	return MPQ_Tree::NODE_TYPE::P;
}

bool MPQ_Tree::PNode::empty() const
{
	return intervals.empty() && children.empty();
}

void MPQ_Tree::PNode::add_interval( const int e )
{
	if ( std::find( std::begin( intervals ), std::end( intervals ), e ) == std::end( intervals ) )
	{
		intervals.push_back( e );
		++num_vertices;
	}
	return;
}

void MPQ_Tree::PNode::add_child( const std::shared_ptr< Node > &p )
{
	p->set_parent( shared_from_this() );
	children.push_back( p );
	return;
}

void MPQ_Tree::PNode::normalize()
{
	std::for_each( std::begin( children ), std::end( children ), std::mem_fn( &Node::normalize ) );

	for ( auto child : children )
	{
		if ( child->children.size() == 1 )
		{
			child = child->children[0];
			child->set_parent( shared_from_this() );
		}
	}

	std::sort( std::begin( children ), std::end( children ),
			[]( const auto &p, const auto &q ){ return *p < *q; } );
	return;
}

void MPQ_Tree::PNode::relabel()
{
	std::transform( std::begin( intervals ), std::end( intervals ), std::begin( intervals ),
			&MPQ_Tree::relabel );
	for ( const auto &child : children )
	{
		child->relabel();
	}
	return;
}

VI MPQ_Tree::PNode::canonical_form()
{
	canostr.clear();
	std::copy( std::begin( intervals ), std::end( intervals ), std::back_inserter( canostr ) );

	for ( const auto &child : children )
	{
		child->canonical_form();
		std::copy( std::begin( child->canostr ), std::end( child->canostr ), std::back_inserter( canostr ) );
	}

	std::copy( intervals.rbegin(), intervals.rend(), std::back_inserter( canostr ) );

	return canostr;
}

MPQ_Tree::QNode::QNode() : Node()
{
	return;
}

bool MPQ_Tree::QNode::compare( const Node &node ) const
{
	const auto &qnode = dynamic_cast< const QNode & >( node );

	assert( children.size() == lefts.size() );
	assert( lefts.size() == rights.size() );
	if ( children.size() != qnode.children.size() )
	{
		return children.size() < qnode.children.size();
	}

	VVI lhs( children.size(), VI( children.size() ) ), rhs( lhs );
	for ( int i = 0; i < children.size(); ++i )
	{
		for ( const int k : rights[i] )
		{
			const int j = find_if( begin( lefts ), end( lefts ),
					[&]( const VI &l ){ return find( begin( l ), end( l ), k ) != end( l ); } ) - begin( lefts );
			++lhs[i][j];
		}
		for ( const int k : qnode.rights[i] )
		{
			const int j = find_if( begin( qnode.lefts ), end( qnode.lefts ),
					[&]( const VI &l ){ return find( begin( l ), end( l ), k ) != end( l ); } ) - begin( qnode.lefts );
			++rhs[i][j];
		}
	}
	for ( int i = 0; i < children.size(); ++i )
	{
		lhs[i].push_back( lefts[i].size() );
		rhs[i].push_back( qnode.lefts[i].size() );
	}
	if ( lhs != rhs )
	{
		return lhs < rhs;
	}

	return compare_children( node );
}

std::shared_ptr< MPQ_Tree::Node > MPQ_Tree::QNode::clone()
{
	const std::shared_ptr< QNode > result = std::make_shared< QNode >();

	clone_common( *result );

	result->lefts = lefts;
	result->rights = rights;
	result->has_subtree = has_subtree;

	return result;
}

MPQ_Tree::NODE_TYPE MPQ_Tree::QNode::node_type() const
{
	return MPQ_Tree::NODE_TYPE::Q;
}

bool MPQ_Tree::QNode::empty() const
{
	return false;
}

void MPQ_Tree::QNode::add_interval( const int e )
{
	children.push_back( std::make_shared< PNode >() );
	children.back()->set_parent( shared_from_this() );
	lefts.emplace_back();
	rights.emplace_back();
	has_subtree.emplace_back();

	bool opened = false;
	for ( int i = 0; i < int( lefts.size() ); ++i )
	{
		opened |= std::find( std::begin( lefts[i] ), std::end( lefts[i] ), e ) != std::end( lefts[i] );
	}

	( opened ? rights[ rights.size() - 2 ] : lefts.back() ).push_back( e );
	num_vertices += !opened;

	return;
}

void MPQ_Tree::QNode::add_child( const std::shared_ptr< Node > &p )
{
	assert( !children.empty() );

	children.back()->add_child( p );
	has_subtree.back() = 1;

	return;
}

void MPQ_Tree::QNode::normalize()
{
	assert( 3 <= children.size() );

	std::for_each( std::begin( children ), std::end( children ), std::mem_fn( &Node::normalize ) );

	{
		VVI forward( children.size(), VI( children.size() ) ), backward( forward );
		for ( int i = 0; i < children.size(); ++i )
		{
			for ( const int k : rights[i] )
			{
				const int j = find_if( begin( lefts ), end( lefts ),
						[&]( const VI &l ){ return find( begin( l ), end( l ), k ) != end( l ); } ) - begin( lefts );
				++forward[i][j];
				++backward[ children.size() - 1 - j ][ children.size() - 1 - i ];
			}
		}
		for ( int i = 0; i < children.size(); ++i )
		{
			forward[i].push_back( lefts[i].size() );
			backward[ children.size() - 1 - i ].push_back( rights[i].size() );
		}

// 		std::reverse( begin( backward ), end( backward ) );
// 		if ( forward != backward )
// 		{
// 			if ( forward < backward )
// 			{
// 				reverse();
// 			}
// 			return;
// 		}
// 		if ( std::lexicographical_compare( begin( forward ), end( forward ),
// 					backward.rbegin(), backward.rend() ) )
// 		{
// 			reverse();
// 			return;
// 		}
// 		if ( std::lexicographical_compare( backward.rbegin(), backward.rend(),
// 					begin( forward ), end( forward ) ) )
// 		{
// 			return;
// 		}

		if ( forward != backward )
		{
			if ( forward < backward )
			{
				reverse();
			}
			return;
		}
	}

	if ( !std::equal( std::begin( children ), std::end( children ),
				children.rbegin(), children.rend(),
				[]( const auto &p, const auto &q ){ return *p == *q; } ) )
	{
		if ( std::lexicographical_compare( children.rbegin(), children.rend(),
					std::begin( children ), std::end( children ),
					[]( const auto &p, const auto &q ){ return *p < *q; } ) )
		{
			reverse();
		}
		return;
	}

	assert( std::equal( begin( has_subtree ), end( has_subtree ),
				has_subtree.rbegin(), has_subtree.rend() ) );

// 	const auto qnode2 = this->clone();
// 	dynamic_cast< QNode & >( *qnode2 ).reverse();
// 	assert( *this == *qnode2 );

// 	const auto qnode2 = this->clone();
// 	dynamic_cast< QNode & >( *qnode2 ).reverse();
// 	if ( *this != *qnode2 )
// 	{
// 		if ( *this < *qnode2 )
// 		{
// 			reverse();
// 		}
// 		return;
// 	}

	return;
}

void MPQ_Tree::QNode::reverse()
{
	std::swap( lefts, rights );
	std::reverse( std::begin( lefts ), std::end( lefts ) );
	std::reverse( std::begin( rights ), std::end( rights ) );
	std::reverse( std::begin( children ), std::end( children ) );
	std::reverse( std::begin( has_subtree ), std::end( has_subtree ) );

	const auto rev = []( VI &v ){ std::reverse( std::begin( v ), std::end( v ) ); };
	for_each( std::begin( lefts ), std::end( lefts ), rev );
	for_each( std::begin( rights ), std::end( rights ), rev );

	return;
}

void MPQ_Tree::QNode::relabel()
{
	for ( int i = 0; i < int( children.size() ); ++i )
	{
		for ( int &l : lefts[i] )
		{
			l = MPQ_Tree::relabel( l );
		}
		children[i]->relabel();
		for ( int &r : rights[i] )
		{
			r = MPQ_Tree::relabel( r );
		}
	}
	return;
}

VI MPQ_Tree::QNode::canonical_form()
{
	canostr.clear();

	for ( int i = 0; i < int( children.size() ); ++i )
	{
		std::for_each( std::begin( lefts[i] ), std::end( lefts[i] ), [&]( const int l ){ canostr.push_back( l ); } );
		children[i]->canonical_form();
		std::copy( std::begin( children[i]->canostr ), std::end( children[i]->canostr ),
				std::back_inserter( canostr ) );
// 		assert( is_sorted( std::begin( rights[i] ), std::end( rights[i] ), std::greater< int >() ) );
		std::for_each( std::begin( rights[i] ), std::end( rights[i] ), [&]( const int r ){ canostr.push_back( r ); } );
	}

	return canostr;
}

void MPQ_Tree::QNode::shrink()
{
	if ( children.empty() )
	{
		return;
	}

	assert( lefts.back().empty() );
	assert( !has_subtree.back() );

	lefts.pop_back();
	rights.pop_back();
	children.pop_back();
	has_subtree.pop_back();

// 	std::cout << "###########################" << std::endl;
// 	std::cout << lefts << std::endl;
// 	std::cout << rights << std::endl;
// 	std::cout << has_subtree << std::endl << std::endl;

	for ( int i = 0; i + 1 < int( children.size() ); ++i )
	{
// 		if ( ( has_subtree[i] || has_subtree[ i + 1 ] ) && ( !rights[i].empty() || !lefts[ i + 1 ].empty() ) )
		if ( !rights[i].empty() && !lefts[ i + 1 ].empty()
				|| !rights[i].empty() && has_subtree[ i + 1 ]
				|| !lefts[ i + 1 ].empty() && has_subtree[i] )
		{
			continue;
		}

		assert( !( rights[i].empty() && lefts[ i + 1 ].empty() ) );

		for_each( std::begin( children[ i + 1 ]->children ), std::end( children[ i + 1 ]->children ), [&]( const auto &p ){ children[i]->add_child( p ); } ); 
		std::copy( std::begin( lefts[ i + 1 ] ), std::end( lefts[ i + 1 ] ), std::back_inserter( lefts[i] ) );
		std::copy( std::begin( rights[ i + 1 ] ), std::end( rights[ i + 1 ] ), std::back_inserter( rights[i] ) );

		children.erase( std::begin( children ) + i + 1 );
		lefts.erase( std::begin( lefts ) + i + 1 );
		rights.erase( std::begin( rights ) + i + 1 );
		has_subtree[i] |= has_subtree[ i + 1 ];
		has_subtree.erase( std::begin( has_subtree ) + i + 1 );
		--i;
	}

// 	std::cout << lefts << std::endl;
// 	std::cout << rights << std::endl;
// 	std::cout << has_subtree << std::endl << std::endl;

// 	std::cout << lefts << std::endl;
// 	std::cout << rights << std::endl;
// 	std::cout << has_subtree << std::endl << std::endl;

// 	for_each( std::begin( lefts ), std::end( lefts ), []( VI &l ){ std::sort( std::begin( l ), std::end( l ) ); } );
// 	for_each( std::begin( rights ), std::end( rights ), []( VI &r ){ std::sort( std::begin( r ), std::end( r ), std::greater< int >() ); } );

	for ( auto child : children )
	{
		if ( child->children.size() == 1 )
		{
			child = child->children[0];
			child->set_parent( shared_from_this() );
		}
	}

	assert( 2 < children.size() );
	assert( has_subtree[0] );
	assert( has_subtree.back() );

	return;
}
