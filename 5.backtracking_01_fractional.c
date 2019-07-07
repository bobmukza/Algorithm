#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#pragma warning(disable:4996)

#define MAX_SIZE 100

typedef struct Element {
	int profit; //이익
	int weight; //무게
	float ppw; //무게당 이익
}Element;

Element knapsack_info[MAX_SIZE]; //knapsack의 정보들을 담기위한 구조체 배열을 선언

bool Promising(int index,int weight);
int Procedure_knapsack(int index,int profit,int weight);

int count; //함수 호출 개수 세기 위한 변수
int maxprofit; //0으로 처음에 초기화,최대이익임
int bestweight; //최대 이익에 해당하는 무게
int global_profit; //Procedure함수의 profit 변수와 혼동을 피하기 위한 변수
int item_num; //아이템 개수(textfile로 부터 읽어옴)
int knapsack_size; //가방 사이즈 변수(textfile로 부터 읽어옴)

int main(void)
{
	char f_name[20];
	printf("Input File Name?\n");
	scanf("%s", f_name);
	FILE *fp = fopen(f_name, "r");
	int init_info[2]; //item 개수와 knapsack 크기를 담을 배열 선언
				  
	//txt파일 첫번째줄 읽어 init_info에 저장
	for (int i = 0; i < 2; i++)
		fscanf(fp, "%d", &init_info[i]);

	// 2차원 배열 동적할당
	int height = 2, width = init_info[0];
	int **arr;
	arr = (int**)malloc(sizeof(int*)*height);
	for (int i = 0; i < height; i++) {
		arr[i] = (int*)malloc(sizeof(int)*width);
	}
	//txt파일 내용 배열 저장
	//2번째줄부터 읽어들임
	for (int i = 1; i < height + 1; i++) {
		for (int j = 0; j < width; j++)
			fscanf(fp, "%d", &arr[i - 1][j]);
	}
	fclose(fp);

	//txt파일 1번째 줄에서 읽어온 정보(개수,사이즈) 저장 
	item_num = init_info[0];
	knapsack_size = init_info[1];
	//knapsack_info에 item의 이익, 무게를 구조체 배열에 저장
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < item_num; j++) {
			if (i == 0)
				knapsack_info[j+1].profit = arr[i][j]; //index 1번부터 사용할거임
			else
				knapsack_info[j+1].weight = arr[i][j];
		}
	}
	//무게당 이익 넣기
	for(int i=1;i<=item_num;i++)
	{
		knapsack_info[i].ppw=(float)knapsack_info[i].profit/knapsack_info[i].weight;
		//printf("%d %f\n",knapsack_info[i].ppw);
	}

	/*
	//knapsack info 출력	
	for(int i=1;i<=item_num;i++){
		printf("profit:%d weight:%d ppw:%f\n",knapsack_info[i].profit,knapsack_info[i].weight,knapsack_info[i].ppw);
	}
	*/

	/////**********************//////
	Procedure_knapsack(0,global_profit,0);
	printf("%d %d\n%d\n",maxprofit,bestweight,count);
	/////**********************/////


	//2차원배열 동적할당 해제
	for (int i = 0; i < height; i++)
	{
		free(arr[i]);
	}
	free(arr);

	return 0;
}

bool Promising(int index,int weight)
{
	count++;
	
	int j,k;
	int totweight;
	float bound;
	if(weight >= knapsack_size) //현재 들고있는 무게가 knapsack_size보다 크다면 
		return false;
	else 
	{
		j=index+1; //다음 아이템
		bound = global_profit; //bound(기대가치)에 현재 가치 넣어주고
		totweight = weight; //총 무게에 현재 들고있는 무게 넣어줌

		while( j<=item_num  && (totweight+knapsack_info[j].weight <= knapsack_size) ) //j가 아이템개수 보다 작고 (총무게 + 담을 무게가) knapsack_size보다 작다면 
		{
			totweight = totweight + knapsack_info[j].weight; //총무게= 총무게 + index무게
			bound = bound + knapsack_info[j].profit; //기대 가치 =  기대가치 + 가치
			j++;
		}
		k = j;
		if(k<=item_num){ 	//만약 k가 아이템 숫자 보다 같거나작다면 남아 있는거 fractional knapsack해서 더해주기
			bound += (knapsack_size-totweight)*knapsack_info[k].ppw; //기대가치에 무게당 가치만큼 fractional해서 더 해줌
		}
		return bound>maxprofit;
	}

}

int Procedure_knapsack(int index,int profit,int weight)
{
	if( (weight<=knapsack_size) && (profit>maxprofit)){ //현재 무게가 knapsack_size보다 작거나 같고 현재가치가 max가치보다 크다면    
		maxprofit = profit;
		bestweight = weight;	
	}
	
	if(Promising(index,weight)){
		//1 담기	
		global_profit = profit + knapsack_info[index+1].profit;
		Procedure_knapsack(index+1,profit+knapsack_info[index+1].profit,weight+knapsack_info[index+1].weight); //1 담기
		
		//0 안담기
		global_profit = profit;
		Procedure_knapsack(index+1,profit,weight);
	}
	return 0;
}
