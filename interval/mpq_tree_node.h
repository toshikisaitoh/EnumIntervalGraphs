#pragma once

#include <bits/stdc++.h>

#include "./mpq_tree.h"

struct MPQ_Tree::Node : public std::enable_shared_from_this< MPQ_Tree::Node >
{
	std::weak_ptr< Node > parent;
	std::vector< std::shared_ptr< Node > > children;
	int size = 1, num_vertices = 0;

	VI canostr;

	virtual ~Node() = default;

	bool operator==( const Node &node ) const;
	bool operator!=( const Node &node ) const;
	bool operator<( const Node &node ) const;
	virtual bool compare( const Node &node ) const = 0;
	bool compare_children( const Node &node ) const;

	virtual std::shared_ptr< Node > clone() = 0;
	void clone_common( Node &result );

	void set_parent( const std::shared_ptr< Node > &p );
	bool has_parent() const;

	virtual MPQ_Tree::NODE_TYPE node_type() const = 0;
	virtual bool empty() const = 0;

	virtual void add_interval( const int e ) = 0;
	virtual void add_child( const std::shared_ptr< Node > &p ) = 0;

	virtual void normalize() = 0;
	void calculate_size();

	virtual void relabel() = 0;

	virtual VI canonical_form() = 0;
};

struct MPQ_Tree::PNode : public MPQ_Tree::Node
{
	VI intervals;

	PNode();

	virtual bool compare( const Node &node ) const;

	virtual std::shared_ptr< Node > clone() override;

	virtual MPQ_Tree::NODE_TYPE node_type() const override;
	virtual bool empty() const override;

	virtual void add_interval( const int e ) override;
	virtual void add_child( const std::shared_ptr< Node > &p ) override;

	virtual void normalize() override;
	virtual VI canonical_form() override;

	virtual void relabel() override;
};

struct MPQ_Tree::QNode : public MPQ_Tree::Node
{
	VVI lefts, rights;
	VI has_subtree;

	QNode();

	virtual bool compare( const Node &node ) const;

	virtual std::shared_ptr< Node > clone() override;

	virtual MPQ_Tree::NODE_TYPE node_type() const override;
	virtual bool empty() const override;

	virtual void add_interval( const int e ) override;
	virtual void add_child( const std::shared_ptr< Node > &p ) override;

	virtual void normalize() override;
	void reverse();

	virtual VI canonical_form() override;

	virtual void relabel() override;
	
	void shrink();
};
