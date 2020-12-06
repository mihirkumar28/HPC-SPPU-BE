#include<iostream>
#include<cstdlib>
#include<chrono>

using namespace std;
using namespace std::chrono;

int binarysearch(int *arr,int val,int l,int r)
{
	if(l<r)
	{
		int mid = (l+r)/2;
		
		if(arr[mid] == val)
			return(mid);

			
		else if(arr[mid] > val)
			return(binarysearch(arr,val,l,mid-1));
		
		else
			return(binarysearch(arr,val,mid+1,r));
	}
	
	return(-1);
}

int parallel_binarysearch(int *arr,int val,int l,int r)
{
	int ans = -1,side;

	if(l<r)
	{
		int mid = (l+r)/2;
		
		if(arr[mid] == val)
			return(mid);

		if(arr[mid]>val)
			side = 0;
		else
			side = 1;

		#pragma omp parallel sections
		{		
			#pragma omp section
			{	
				if(side == 0)
				ans = parallel_binarysearch(arr,val,l,mid-1);
			}
			
			
			
			#pragma omp section
			{
				if(side == 1)
				ans = parallel_binarysearch(arr,val,mid+1,r);
			}
			
		}
		
	}
	
	return(ans);
}

int main()
{

	int *arr,i,j;
	
	srand(time(0));
	
	arr = new int[100000];
	
	for(i=0;i<100000;i++)
		arr[i] = i;
		
	auto s = high_resolution_clock::now();
	
	for(i=0;i<100000;i++)
		j = binarysearch(arr,rand()%100000,0,100000-1);
		
	auto e = high_resolution_clock::now();

	cout << "Time linear : " << duration_cast<microseconds>(e-s).count() << "\n";

	s = high_resolution_clock::now();
	
	for(i=0;i<100000;i++)
		j = parallel_binarysearch(arr,rand()%100000,0,100000-1);
		
	e = high_resolution_clock::now();


	cout << "Time parallel : " << duration_cast<microseconds>(e-s).count() << "\n";
	
	if(j==-1)
		cout<<endl<<"Not Found"<<endl;
	else
		cout<<"Found"<<endl;

}





