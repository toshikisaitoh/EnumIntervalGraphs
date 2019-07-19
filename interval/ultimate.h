#pragma once

#include<stdio.h>
#include<stdlib.h>

namespace rexbfs
{
	// Number of vertices in a given graph
	// このプログラムは N 頂点までしか動かない！
	// #define N 20

	int ultimate(int *order_as,int *order_as_i,int *i_as,int u,int n);
	int LexBFS(int u,int *order,int *order_i,int n);
	int LexBFS_p(int u,int *order,int *order_p,int *order_p_i,int n);
	int LexBFS_as(int u, int *order1, int *order2, int *index1, int *index2, int *order_as, int *order_as_i, int n);
	int Cal_index(int *order,int *order_i,int *index,int n);
	int is_umbrella_free(int *order_as,int *order_as_i,int *index,int n);
}

#include <bits/stdc++.h>

using namespace std;
using VI = vector< int >;
using VVI = vector< VI >;

VI graph_to_string_form( const VVI &G );
