#include "ultimate.h"

namespace rexbfs
{
	constexpr int N = 20;

	// #define F 0
	// #define NF 1
	// #define OK 2
	constexpr int F = 0;
	constexpr int NF = 1;
	constexpr int OK = 2;

	int G[N][N];

	// recognition algorithm
	int ultimate(int *order_as,int *order_as_i,int *i_as,int u,int n)
	{
		int order[N];
		int order_i[N];
		int order_p1[N];
		int order_p1_i[N];
		int order_p2[N];
		int order_p2_i[N];
		//int order_as[N];
		//int order_as_i[N];
		//int i_p1[N],i_p2[N],i_as[N];
		int i_p1[N],i_p2[N];
		int check,check2;
		int flag;

		check=LexBFS(u,order,order_i,n);
		if(check==-1) return -2;

		LexBFS_p(order_i[n-1],order,order_p1,order_p1_i,n);
		Cal_index(order_p1, order_p1_i, i_p1, n);
		//printf("index:");
		//referenceO(i_p1,n);

		LexBFS_p(order_p1_i[n-1],order_p1,order_p2,order_p2_i,n);
		Cal_index(order_p2, order_p2_i, i_p2, n);
		//printf("index:");
		//referenceO(i_p2,n);

		flag=LexBFS_as(order_p2_i[n-1],order_p1,order_p2,i_p1,i_p2,order_as,order_as_i,n);
		Cal_index(order_as,order_as_i,i_as,n);
		//printf("index:");
		//referenceO(i_as,n);

		check=is_umbrella_free(order_as,order_as_i,i_as,n);
		/*
		   if(check==1){
		   LexBFS_p(order_p2_i[n-1],order_p2,order_as,order_as_i,n);
		   Cal_index(order_as,order_as_i,i_as,n);
		   check2=is_umbrella_free(order_as,order_as_i,i_as,n);
		   if(check2!=1){
		   referenceG(n,msa);
		   fflush(msa);
		   }
		   }
		   */
		/*
		   if(check==1){
		   if(flag>=4){
		   flag-=4;
		   referenceG(n,OK_NF);
		   }
		   if(flag>=2){
		   flag-=2;
		   referenceG(n,OK_F);
		   }
		   if(flag>=1){
		   flag-=1;
		   referenceG(n,NF_F);
		   }
		   }
		   */
		return check;
	}

	int LexBFS(int u,int *order,int *order_i,int n)
	{
		int label[N];
		int i,j;
		int v;
		double x;
		int add_label,max_label;

		max_label=1;
		for(i=0 ; i<n ; i++){
			label[i]=0;
			max_label*=2;
		}
		label[u]=max_label;

		for(i=n-1 ; i>=0 ; i--){
			v=0;
			for(j=1 ; j<n ; j++){
				if(label[v]<label[j]) v=j;
			}
			order_i[n-i-1]=v;
			order[v]=n-i-1;
			//fprintf(fp,"vertex: %d\n",v);

			if(label[v]==0) return -1;

			label[v]=-max_label*2;
			add_label=1;
			for(j=0 ; j<i ; j++) add_label*=2;
			for(j=0 ; j<n ; j++){
				if(G[v][j]==1) label[j]+=add_label;
			}
		}
		return 0;
	}


	int LexBFS_p(int u,int *order,int *order_p,int *order_p_i,int n)
	{
		int label[N];
		int i,j,k;
		int v;
		int add_label,max_label;

		max_label=1;
		for(i=0 ; i<n ; i++){
			label[i]=0;
			max_label*=2;
		}
		label[u]=max_label;

		for(i=n-1 ; i>=0 ; i--){
			v=0;
			for(j=1 ; j<n ; j++){
				if(label[v]<label[j]) v=j;
				else if(label[v]==label[j]){
					//fprintf(fp,"check the preorder\n");
					if(order[v]<order[j]){
						v=j;
					}
				}
			}

			order_p[v]=n-i-1;
			order_p_i[n-i-1]=v;
			//fprintf(fp,"vertex: %d\n",v);

			label[v]=-max_label*2;
			add_label=1;
			for(j=0 ; j<i ; j++) add_label*=2;
			for(j=0 ; j<n ; j++){
				if(G[v][j]==1) label[j]+=add_label;
			}
		}
		return 0;
	}


	int LexBFS_as(int u,int *order1,int *order2,int *index1,int *index2,int *order_as,int *order_as_i,int n)
	{
		int label[N];
		int i,j,k;
		int a,b,v;
		int add_label,max_label;
		int attribute[2];
		int flag1=0,flag2=0,flag3=0,flag=0;

		max_label=1;
		for(i=0 ; i<n ; i++){
			label[i]=0;
			max_label*=2;
		}
		label[u]=max_label;

		for(i=n-1 ; i>=0 ; i--){
			// Select Vertex
			v=0; a=0; b=0;
			for(j=1 ; j<n ; j++){
				if(label[v]<label[j]){
					a=j;	b=j;	v=j;
				}
				else if(label[v]==label[j]){
					//fprintf(fp,"check the preorder\n");
					if(order1[a]<order1[j]) a=j;
					if(order2[b]<order2[j]) b=j;
				}
			}

			attribute[0]=OK;
			attribute[1]=OK;
			for(j=0 ; j<n ; j++){
				if(label[j]>=0){
					if(label[j]<label[a] && G[j][a]==1){
						attribute[0]=F;
						//printf("a is fly\n");
						break;
					}
					if(label[j]==label[a] && G[j][a]==1){
						attribute[0]=NF;
						//printf("a is NF\n");
					}	
					if(label[j]<label[b] && G[j][b]==1){
						attribute[1]=F;
						//printf("b is fly\n");
					}
					if(label[j]==label[b] && G[j][b]==1 && attribute[1]==OK){
						attribute[1]=NF;
						//printf("b is NF\n");
					}
				}
			}

			if(a==b) v=b;
			else if(attribute[0]==F) v=b;
			else if(attribute[1]==OK) v=b;
			else if(attribute[0]==NF && attribute[1]==NF) v=b;
			else{
				v=a;
				if(attribute[0]==NF) flag1=1;
				else if(attribute[1]==F) flag2=1;
				else flag3=1;
				flag++;
			}

			order_as[v]=n-i-1;
			order_as_i[n-i-1]=v;
			//fprintf(fp,"a=%d, b=%d\n",a,b);
			//fprintf(fp,"vertex: %d\n",v);

			label[v]=-max_label*2;
			add_label=1;
			for(j=0 ; j<i ; j++) add_label*=2;
			for(j=0 ; j<n ; j++){
				if(G[v][j]==1) label[j]+=add_label;
			}
		}
		if(flag>=2) flag=flag1+2*flag2+4*flag3;
		else flag=0;
		return flag;
	}


	int Cal_index(int *order,int *order_i,int *index,int n)
	{
		int i,j,k;
		int max;

		for(i=0 ; i<n ; i++){
			max=order[i];
			for(j=0 ; j<n ; j++)
				if(G[i][j]==1 && max<order[j]) max=order[j];
			index[i]=max;
		}
		return 0;
	}


	int is_umbrella_free(int *order_as,int *order_as_i,int *index,int n)
	{
		int i,j;
		for(i=0 ; i<n ; i++){
			for(j=order_as[i]+1 ; j<=index[i] ; j++){
				if(G[i][order_as_i[j]]==0) return -1;
			}
		}
		return 1;
	}
}

VI graph_to_string_form( const VVI &G )
{
	const int N = G.size();

	for ( int i = 0; i < N; ++i )
	{
		for ( int j = 0; j < i; ++j )
		{
			rexbfs::G[i][j] = rexbfs::G[j][i] = G[i][j];
		}
	}

  int order_as[N], order_as_i[N], i_as[N];
  int flag = rexbfs::ultimate(order_as, order_as_i, i_as, 0, N );

  if ( flag != 1 )
  {
	  return {};
  }

  VVI lefts( N ), rights( N );
  for ( int i = 0; i < N; ++i )
  {
	  lefts[ order_as[i] ].push_back( i );
	  rights[ i_as[i] ].push_back( i );
  }

  VI res;
  for ( int i = 0; i < N; ++i )
  {
	  copy( begin( lefts[i] ), end( lefts[i] ), back_inserter( res ) );
	  copy( begin( rights[i] ), end( rights[i] ), back_inserter( res ) );
  }
  return move( res );
}
