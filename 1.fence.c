#include<stdio.h>
#include<stdlib.h>

#pragma warning(disable:4996)

int Fence(int left,int right,int top,int bottom,int **arr);

int count=0;

int main(void)
{
	char f_name[20];
	printf("Input File Name?\n");
	scanf("%s",f_name);
	FILE *fp=fopen(f_name,"r");

	//Fence parameter
	int init;
	int fence_size[1];
	init = 0;

	//txt파일 첫번째줄 읽어 fence_size에 저장
	fscanf(fp,"%d",fence_size);
	//printf("%d\n",fence_size[0]);
	
	
	// 2차원 배열 동적할당
	int height=fence_size[0],width=fence_size[0];
	int **arr;
	arr = (int**)malloc(sizeof(int*)*height);
	for(int i=0;i<height;i++)
	{
    	arr[i] = (int*)malloc(sizeof(int)*width);
	}
	
	//txt파일 내용 배열 저장
	//첫번째줄은 size를 읽어들여서 2번째줄부터 받아들임
	for(int i=1;i<height+1;i++)
	{
		for(int j=0;j<width;j++)
		{
			fscanf(fp,"%d",&arr[i-1][j]);
		}
	}
	fclose(fp);

	//배열 내용 출력
	/*
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			printf("%d ",arr[i][j]);
		}
		printf("\n");
	}	
	printf("\n");
	*/

	//Fence 함수 호출 0 9 0 9
	Fence(init,fence_size[0],init,fence_size[0],arr);

	//2차원배열 동적할당 해제
	for(int i=0;i<height;i++)
	{
		free(arr[i]);
	}
	free(arr);
	
	//최소 울타리 개수 출력
  	printf("\n%d\n", count);

	return 0;
}

int Fence(int left,int right,int top,int bottom,int **arr)
{	
	int pivot=arr[top][left];
	int i,j;
	int k=(right-left)/3; // k=size를 3등분1x3x3x3x.........
	//printf("Fence 호출 %d,%d,%d,%d\n",left,right,top,bottom);

	//recursive 빠져나오는 조건값
	if(right-left<=0 || bottom-top<=0) 
		return 0;

	for(i=top;i<bottom;i++)
	{
		for(j=left;j<right;j++)
		{
			if(pivot!=arr[i][j])
			{
				//알고리즘 만들어야 할 부분
			//init 0 size 80            26           8          
				  //     81x81         27x27        9x9           
				//1번 0 26 0 26       0 8 0 8     0 2 0 2      
				Fence(left			,left+k			,top		,top+k			,arr);
			    //2번 27 53 0 26     9 17 0 8     3 5 0 2 
				Fence(left+k		,left+k*2		,top		,top+k			,arr);
				//3번 54 80 0 26     18 26 0 8    6 8 0 2   
				Fence(left+k*2		,right			,top		,top+k			,arr);
				
				//4번 0 26 27 53     0 18 9 17    0 2 3 5      
				Fence(left			,left+k			,top+k		,top+k*2		,arr);
				//5번 27 53 27 53    9 17 9 17    3 5 3 5       
				Fence(left+k		,left+k*2		,top+k		,top+k*2		,arr);
				//6번 54 80 27 53    18 26 9 17   6 8 3 5     
				Fence(left+k*2		,right			,top+k		,top+k*2		,arr); 
				
				//7번 0 26 54 80     0 18 9 17    0 2 6 8      
				Fence(left			,left+k			,top+k*2	,bottom			,arr); 
				//8번 27 53 54 80    9 17 18 26   3 5 6 8     
				Fence(left+k		,left+k*2		,top+k*2	,bottom			,arr); 
				//9번 54 80 54 80    18 26 18 26  6 8 6 8     
				Fence(left+k*2		,right			,top+k*2	,bottom			,arr); 
				
				//printf("좌표,값 (%d,%d),%d\n",i,j,arr[i][j]);
				
				count++;
				return 0;
			}
		}
	}
	return 0;
}

