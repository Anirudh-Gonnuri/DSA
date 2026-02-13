#include <stdio.h>
#include <stdlib.h>
#define MAX 10
#define INF 99999
void printGraph(char hubs[MAX][50],int graph[MAX][MAX],int n){
	int i,j;
	printf("\t");
	for(i=0;i<n;i++){
		printf("%s\t",hubs[i]);
	}
	for(i=0;i<n;i++){
		printf("\n%s\t",hubs[i]);
		for(j=0;j<n;j++){
			printf("%d\t",graph[i][j]);
		}
	}
}
// Fetches minimum key keeps track of visited nodes
int findMin(int arr[],int n,int visited[]){
	int i,min=-1;
	for(i=0;i<n;i++){
		if(!visited[i] && (min==-1 || arr[i]<arr[min])){
			min=i;
		}
	}
	return min;
}
// Computing network with minimum weighted edges
void prim(int graph[MAX][MAX],char hubs[MAX][50], int n){
	int key[n],visited[n],parent[n];
	int i,count,total=0,u,cost;
	for(i=0;i<n;i++){
		key[i]=INF;
		visited[i]=0;
	}
	key[0]=0;
	parent[0]=-1;

	for(count=0;count<n-1;count++){
		u = findMin(key,n,visited);
		visited[u]=1;
		printf("\nChosen Vertex: %s",hubs[u]);
		for(i=0;i<n;i++){
			if(graph[u][i] && !visited[i] && graph[u][i] < key[i]){
				parent[i] = u;
				key[i] = graph[u][i];
				printf("\n==updating node key %s",hubs[i]);
			}
		}
	}

	printf("\n%s is the start node: ",hubs[0]);
	for(i=1;i<n;i++){
		printf("\nParent of %s is %s",hubs[i],hubs[parent[i]]);
		total+=key[i];
	}
	printf("\nTotal cost: %d",total);
}

int main(){
	int i,j,graph[MAX][MAX],ch,cost,n=0;
	char hubs[MAX][50];
	while(1){
		printf("\nMenu:\n");
		printf("1. Create Graph\n");
		printf("2. Display Graph\n");
		printf("3. Compute Optimal connection network\n");
		printf("4. Exit\n");
		printf("Enter choice: ");
		scanf("%d",&ch);

		switch(ch){
			case 1:
				printf("Enter number of hubs: ");
				scanf("%d",&n);
				for(i=0;i<n;i++){
					printf("Enter name of hub %d: ",i);
					scanf("%s",hubs[i]);
				}
				for(i=0;i<n;i++){
					for(j=0;j<n;j++){
						if(i==j) graph[i][j]=0;
						else if(i<j){
							printf("Cost from %s to %s: ",hubs[i],hubs[j]);
							scanf("%d",&cost);
							graph[i][j]=cost;
							graph[j][i]=cost;
						}
					}
				}
				break;

			case 2:
				printGraph(hubs,graph,n);
				break;

			case 3:
				prim(graph,hubs,n);
				break;

			case 4:
				exit(0);

			default:
				printf("Invalid choice");
		}
	}
}
