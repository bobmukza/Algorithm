#include<stdio.h>
#include<stdlib.h>

#pragma warning(disable:4996)

#define MAX_SIZE 100
int C[MAX_SIZE];
int C_sum[MAX_SIZE];

void Count_sort(int *A,int *B,int min,int max, int n);

int main(void)
{
	int max; //max값
	int min; //min값
	int A[MAX_SIZE]={0,};
	int B[MAX_SIZE]={0,};
	int n=0; //아이템개수

	char f_name[20];
	printf("Input File Name?\n");
	scanf("%s", f_name);
	FILE *fp = fopen(f_name, "r");
				 	
	while(1)
	{
		fscanf(fp,"%d",&A[n]);
		if(feof(fp))
			break;     
		printf("%d ", A[n]);
		if(A[n]>max)
			max=A[n];
		else if(A[n]<min)
			min=A[n];
		n++;
	}	

	printf("\n%d\n%d %d\n",n,min,max);
	Count_sort(A,B,min,max,n);

	for(int i=1;i<n;i++)
		printf("%d ",B[i]);

	printf("\n");
	return 0;
}


void Count_sort(int *A,int *B,int min,int max,int n)
{
	int i;
	
	for(i=0;i<n;i++)
	   	C[A[i]-min]++;
	for (i=1;i<=k;i++)
		C_sum[i] = C_sum[i-1]+C[i]; // prefix sums
	for (i=n;i>=1;i--){
		B[C_sum[A[i]]] = A[i];
		C_sum[A[i]]--;
	}
}

