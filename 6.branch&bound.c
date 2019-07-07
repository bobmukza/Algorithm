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

typedef struct Node{
	int level;
	int profit;
	int weight;
	float bound;
}node;

/*****************************************************************************/
//								우선순위 큐									 //

typedef struct priority_queue {
    node heap[MAX_SIZE];
    int size;
}priority_queue;

priority_queue PQ; //우선순위 큐 선언

//size초기화 함수
void init(){
	PQ.size = 0;
}

//구조체 노드 swap
void swap(node *a, node *b) {
    node tmp = *a;
    *a = *b;
    *b = tmp;
}

int push(node k) {
    if (PQ.size + 1 > MAX_SIZE) {
        return 0;
    }
    
    PQ.heap[PQ.size].level = k.level;
    PQ.heap[PQ.size].profit = k.profit;
    PQ.heap[PQ.size].weight = k.weight;
    PQ.heap[PQ.size].bound = k.bound;
    
    
    
    
    int current = PQ.size;
    int parent = (PQ.size - 1) / 2;
    
    while (current > 0 && PQ.heap[current].bound > PQ.heap[parent].bound) {
        swap(&PQ.heap[current], &PQ.heap[parent]);
        current = parent;
        parent = (parent - 1) / 2;
    }
    
    PQ.size++;
    
    return 1;
}

node pop() {
    
	if (PQ.size <= 0)
        exit(1);
    
    node ret = PQ.heap[0];
    PQ.size--;
    
    PQ.heap[0] = PQ.heap[PQ.size];
    int current = 0;
    int leftChild = current * 2 + 1;
    int rightChild = current * 2 + 2;
    int maxNode = current;
    
    while (leftChild < PQ.size) {
        if (PQ.heap[maxNode].bound < PQ.heap[leftChild].bound) {
            maxNode = leftChild;
        }
        if (rightChild < PQ.size && PQ.heap[maxNode].bound < PQ.heap[rightChild].bound) {
            maxNode = rightChild;
        }
        
        if (maxNode == current) {
            break;
        }
        else {
            swap(&PQ.heap[current], &PQ.heap[maxNode]);
            current = maxNode;
            leftChild = current * 2 + 1;
            rightChild = current * 2 + 2;
        }
    }
    
    return ret;
}

int empty() {
    if (PQ.size == 0) {
        return 1;
    }
    else {
        return 0;
    }
}
/*****************************************************************************/

float Bound(node u);
void Branch_and_bound_01knapsack();

int count; //함수 호출 개수 세기 위한 변수
int maxprofit; //0으로 처음에 초기화,최대이익임
int bestweight; //최대 이익에 해당하는 무게
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


	Branch_and_bound_01knapsack();
	printf("%d %d\n%d\n",maxprofit,bestweight,count);
	
	//2차원배열 동적할당 해제
	for (int i = 0; i < height; i++)
	{
		free(arr[i]);
	}
	free(arr);
	

	return 0;
}


float Bound(node u)
{
	count++;
	
	int j,k;
	int totweight;
	float bound;
	if( u.weight >= knapsack_size) //현재 들고있는 무게가 knapsack_size보다 크다면 
		return 0;
	else 
	{
		j= u.level+1; //다음 아이템
		bound = u.profit; //bound(기대가치)에 현재 가치 넣어주고
		totweight = u.weight; //총 무게에 현재 들고있는 무게 넣어줌

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
		return bound;
	}

}

void Branch_and_bound_01knapsack()
{

	node u,v;

	//root 노드 설정
	v.level = 0;
	v.profit = 0;
	v.weight = 0;
	v.bound = Bound(v);

	//우선순위큐 초기화
	init();

	//삽입
	push(v);

	while(!empty())
	{
		//제일 위에 있는노드 꺼내서 v에 저장하고 루트삭제
		v=pop();
	
		//기대가치 조사
		if(v.bound > maxprofit)
		{
			// 1 선택함
			u.level = v.level +1;
			u.weight = v.weight + knapsack_info[u.level].weight;
			u.profit = v.profit + knapsack_info[u.level].profit;

			if(u.weight <= knapsack_size && u.profit > maxprofit)
			{
				maxprofit = u.profit;
				bestweight = u.weight;
				//printf("[level]:%3d price:%d weight:%d]\n",u.level,u.profit,u.weight);
			}
			u.bound = Bound(u);
			if(u.bound > maxprofit)
				push(u);

			// 0 선택하지 않음
			u.weight = v.weight;
			u.profit = v.profit;

			u.bound = Bound(u);
			if(u.bound > maxprofit)
				push(u);
		}

	}

}
