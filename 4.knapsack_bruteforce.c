#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#pragma warning(disable:4996)

#define MAX_SIZE 100

typedef struct Element {
	int index; //초기 인덱스
	int profit; //이익
	int weight; //무게
}Element;


void brute_force(Element *arr, int n, int ks, int *best_profit ,int *best_index); //모든 조합계산해서 집어 넣는 함수

int main(void)
{
	int item_num, knapsack_size; //아이템 개수, knapsack size
	Element knapsack_info[MAX_SIZE]; //knapsack의 정보들을 담기위한 구조체 배열 선언
	

	//최고의 조합(index,profit)
	int best_index[10]={0, };
	int best_profit;
	
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
	//knapsack_info에 item의 index,이익, 무게를 구조체 배열에 저장
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < item_num; j++) {
			knapsack_info[j].index = j;
			if (i == 0)
				knapsack_info[j].profit = arr[i][j];
			else
				knapsack_info[j].weight = arr[i][j];
		}
	}
	/*
	//knapsack info 출력	
	for(int i=0;i<item_num;i++){
		printf("index:%d profit:%d weight:%d\n",knapsack_info[i].index,knapsack_info[i].profit,knapsack_info[i].weight);
	}
	*/
	/////**********************//////
	

	//knapsack에 주어 담기 모든 경우의 수
	brute_force(knapsack_info, item_num, knapsack_size,&best_profit,best_index);
	
	//최대 이익 출력	
	printf("--------\n%d ",best_profit);

	//최고 조합 출력
	for(int k = item_num-1 ; k>=0 ; k--)
		printf("%d",best_index[k]);
	printf("\n");

	//2차원배열 동적할당 해제
	for (int i = 0; i < height; i++)
	{
		free(arr[i]);
	}
	free(arr);

	return 0;
}

void brute_force(Element *arr, int item_num, int knapsack_size,int *best_profit, int *best_index)
{
	// 총 이익 총 무게,최고 이익,최고 조합 초기화
	int total_profit=0; 
	int total_weight=0;
	int position = 0;
	int p; //2진수 출력에 쓰일 변수
	*best_profit = total_profit;
	int binary[10] = {0,};

	for(int i=0; i < (1<<(item_num)); i++){ //비트 연산 조건문 2^n
		
		//i값 배열을 이용해서 2진수로 저장
		p=i;
		while(1)
		{
			binary[position] = p%2;
			p = p / 2;
			position++;
			if(p==0)
				break;
		}
		//역순으로 출력
		for(int k=item_num-1; k>=0 ; k--)
			printf("%d",binary[k]);

		//경우의 수
		for(int j=0; j < item_num ; j++){
			if( i & (1 << j)){
				//printf("%d ",arr[j].index);
				//경우의 수 누적
				total_profit = total_profit + arr[j].profit; 
				total_weight = total_weight + arr[j].weight;
			}
		}
		//총 무게가 knapsack_size 보다 크면 -999넣기
		if(total_weight > knapsack_size) 
			total_profit = -999;
		//총 이익 출력
		printf(" %d",total_profit);
		
		//최고 조합,최대 이익 
		if(*best_profit < total_profit){
			memcpy(best_index,binary,sizeof(binary));
			*best_profit = total_profit;
		}
		printf("\n");
		total_profit = 0; //0으로 초기화
		total_weight = 0; //0으로 초기화
		position = 0; //2진수 저장 index 초기화
	}
}
