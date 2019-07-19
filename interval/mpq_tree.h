#pragma once

#include <bits/stdc++.h>

using VI = std::vector< int >;
using VVI = std::vector< VI >;

class MPQ_Tree
{
	static VI label;
	static int n;
	static int relabel( const int v );

	enum struct NODE_TYPE
	{
		P, Q, NIL
	};

	using NODE_TAG = std::pair< NODE_TYPE, int >;

	struct Node;
	struct PNode;
	struct QNode;

	const int N;

	const std::shared_ptr< Node > root;

public:
	MPQ_Tree( const VI &intervals );
	MPQ_Tree( const MPQ_Tree &t );

	MPQ_Tree& normalize();
	MPQ_Tree& relabel_nodes();
	VI canonical_form() const;

private:
	static VVI compactization( const VI &ci );
	static VVI ll_reorder( const int N, const VVI &ci );
	static std::vector< NODE_TAG > classify( const int N, const VVI &llci );
	static std::shared_ptr< Node > link_nodes( const int N, const VVI &llci, const std::vector< NODE_TAG > &laminar );
};
