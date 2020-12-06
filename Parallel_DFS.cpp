#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<mpi.h>
#define send_data_tag 2001
#define return_data_tag 2002

//Inorder traversal
void printInorder(int* arr,int start,int end)
{
	if(start > end) 
    return; 
  
  // print left subtree 
  printInorder(arr, start*2 + 1, end); 
  
  // print root 
  printf("%d\t", arr[start]); 
  
  // print right subtree 
  printInorder(arr, start*2 + 2, end); 
}

void buildTree(int* arr,int start,int end,int*,int,int,int);

int main(int argc, char const *argv[])
{
	int N = 1024;
	int arr[N];
	int tree[N];

	int slaveArray[N];
	int i;
	int turns = 1;

	int children =4;
	int ierr;
	
    MPI_Status status;

    int my_id, root_process, num_procs,
         an_id, num_elements_to_receive, 
         sender, num_elements_receive , num_rows_to_send;

    
	double start,finish;

	ierr = MPI_Init(&argc, &argv); 
  
	start = MPI_Wtime();
      
    root_process = 0;

    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if(my_id == root_process)
    {
    	turns = 1;
    	/*
      For loop is constructing every child branch and giving it to the slave.
    	*/
    	for(;turns<=children;turns++)
	    {
			int offset = (turns-1)*N;
	
			buildTree(arr,0,N,tree,0,offset,N);

			ierr = MPI_Send(&N,1,MPI_INT,turns-1,send_data_tag,MPI_COMM_WORLD);

			ierr = MPI_Send(&tree,N,MPI_INT,turns-1,send_data_tag,MPI_COMM_WORLD);
    	}


    	/*Waiting for slaves to finish*/

	}
	else
	{
		ierr = MPI_Recv(&num_elements_to_receive,1,MPI_INT,root_process,send_data_tag,MPI_COMM_WORLD,&status);

		int received = num_elements_to_receive;

        ierr = MPI_Recv(&slaveArray,num_elements_to_receive,MPI_INT,root_process,send_data_tag,MPI_COMM_WORLD,&status);

        printInorder(slaveArray,0,received-1);
	}
	finish = MPI_Wtime();
	printf("Execution time = %f seconds",(finish-start));
	MPI_Finalize();

}


void buildTree(int* arr,int start,int end,int* tree,int k,int offset,int N)
{
	
	if(start<=end && k<=N-1)
	{
	int mid = (end+start+1)/2;
	tree[k] = mid+offset;
	
	buildTree(arr,start,mid-1,tree, k*2+1,offset,N);
	buildTree(arr,mid+1,end,tree,k*2+2,offset,N);
   }

}
"""
