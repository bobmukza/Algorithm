#include<stdio.h>
#include<stdlib.h>

#pragma warning(disable:4996)

#define MAX_SIZE 100
typedef struct Element{
	int index; //해당 인덱스
	int profit; //이익
	int weight; //무게
	float k; //무게당이익
}Element;

void bubble_sort(Element *arr,int n);

void into_knapsack(Element *arr,int *pocket,int n,int ks);

int main(void)
{
	int i,j,n,ks;
   	int knapsack_info[MAX_SIZE]={0,};
	char f_name[20];
	printf("Input File Name?\n");
	scanf("%s",f_name);
	FILE *fp=fopen(f_name,"r");

	//item 개수와 knapsack 크기를 담을 배열 선언
	int init_info[2];

	//txt파일 첫번째줄 읽어 init_info에 저장
	for(int i=0;i<2;i++)
		fscanf(fp,"%d",&init_info[i]);
	//printf("아이템개수:%d knapsack size:%d\n",init_info[0],init_info[1]);

    
	// 2차원 배열 동적할당
	int height=2,width=init_info[0];
	int **arr;
	arr = (int**)malloc(sizeof(int*)*height);
	for(int i=0;i<height;i++){
    	arr[i] = (int*)malloc(sizeof(int)*width);
	}	
	
	//txt파일 내용 배열 저장
	//2번째줄부터 읽어들임
	for(int i=1;i<height+1;i++){
		for(int j=0;j<width;j++)
			fscanf(fp,"%d",&arr[i-1][j]);
	}
	fclose(fp);


	Element mknapsack[init_info[0]];
	n=init_info[0]; //아이템 개수
	ks=init_info[1]; //최대로 들 수 있는 무게
	for(int i=0;i<height;i++){
		for(int j=0;j<n;j++){
			if(i==0)
				mknapsack[j].profit=arr[i][j];
			else
				mknapsack[j].weight=arr[i][j];
		}
	}
	/*
	for(i=0;i<n;i++)
	{
		printf("이익 무게\n");
		printf("%d %d\n",mknapsack[i].profit,mknapsack[i].weight);
	}
	printf("\n무게당이익\n");
	*/
	for(i=0;i<n;i++)
	{
		mknapsack[i].index=i;
		mknapsack[i].k=(float)mknapsack[i].profit/mknapsack[i].weight;
		//printf("%d %f\n",mknapsack[i].index,mknapsack[i].k);
	}
	
	//무게당 가치가 제일 높은순으 오름차순 정렬
	bubble_sort(mknapsack,n);
	
	/*
	printf("정렬한후");
	for(i=0;i<n;i++)
	{
		printf("아이템순서:%d 무게당가치%f 무게:%d\n",mknapsack[i].index+1,mknapsack[i].k,mknapsack[i].weight);
	}	
	*/
	
	//my knapsack에 주어 담기
	into_knapsack(mknapsack,knapsack_info,n,ks);
	
	//knapsack 정보 출력
	for(i=0;i<n;i++)
	{
		printf("%d ",knapsack_info[i]);
	}
	printf("\n");
	return 0;
}

void bubble_sort(Element *arr,int n)
{
	int i,j,temp1,temp2,temp3;
	float temp4;
	for(i=0;i<n;i++){
		for(j=0;j<n-(i+1);j++){
			if(arr[j].k >arr[j+1].k){

				//아이템 순서
				temp1 = arr[j+1].index;
				arr[j+1].index = arr[j].index;
				arr[j].index = temp1;

				//이익
                temp2 = arr[j+1].profit;
                arr[j+1].profit = arr[j].profit;
                arr[j].profit = temp2;

 				//무게
                temp3 = arr[j+1].weight;
                arr[j+1].weight = arr[j].weight;
                arr[j].weight = temp3;

				//무게당이익
	 			temp4 = arr[j+1].k;
				arr[j+1].k=arr[j].k;
			   	arr[j].k = temp4;
			}
		}
	}
}

void into_knapsack(Element *arr,int *myknapsack,int n,int ks)
{
	int i,j;
	for(i=n-1;i>=0;i--){
		if(arr[i].weight< ks){
			myknapsack[arr[i].index]=arr[i].weight;
			ks = ks - arr[i].weight;
		}else{
			myknapsack[arr[i].index]=ks;
			return;
		}
	}
}

