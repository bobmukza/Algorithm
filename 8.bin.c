#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>

#define MAX_SIZE 100

typedef struct Element {
	float weight; //무게
	float item[MAX_SIZE]; //bin에 들어있는 아이템 값
	int item_count; //bin에 들어있는 item 개수
	bool capacity; //bin에 아이템을 넣을수 있는지 아닌지 체크 
}Element;

Element bin_info[MAX_SIZE]; //bin의 정보들을 담기위한 구조체 배열을 선언

void Best_fit(int bin_num,int item_num,float arr[],float packing[]);
void Print_info(int bin_num,int item_num,float packing[]);

int main(void)
{
	char f_name[20];
	printf("Input File Name?\n");
	scanf("%s", f_name);
	FILE *fp = fopen(f_name, "r");
	int init_info[2]; //item 개수와 knapsack 크기를 담을 배열 선언
	float *arr;
	int bin_num; //bin의 개수
	int item_num; //item 개수
	float packing[MAX_SIZE];
	memset(packing,-1,MAX_SIZE); //packing -1로 초기화

	//txt파일 첫번째줄 읽어 init_info에 저장
	for (int i = 0; i < 2; i++)
		fscanf(fp, "%d", &init_info[i]);

	bin_num=init_info[0];

	// 2차원 배열 동적할당
	item_num = init_info[1];
	arr = (float*)malloc(sizeof(float)*item_num);

	//txt파일 내용 배열 저장
	//2번째줄부터 읽어들임
	for (int j = 0; j < item_num; j++)
	{
		fscanf(fp, "%f", &arr[j]);
	}
	fclose(fp);

	/*
	printf("%d %d\n",bin_num,item_num);
	for(int i=0;i<width;i++)
		printf("%0.2f ",arr[i]);
	printf("\n");
	*/

	//bin구조체에 무게 1만큼 할당
	for(int i=0;i<bin_num;i++)
	{
		bin_info[i].weight=1;
		bin_info[i].capacity=true;
	}
	for(int i=0;i<=item_num;i++)
		packing[i]=arr[i];

	//printf("초기화전 packing:");
	//for(int i=0;i<item_num;i++)
	//{
	//	printf("%0.2f ",packing[i]);
	//}
	//printf("\n");

	Best_fit(bin_num,item_num,arr,packing);

	Print_info(bin_num,item_num,packing);
	
	free(arr);
	return 0;
}

void Best_fit(int bin_num,int item_num,float arr[],float packing[])
{
	float minimum; //최소 공간 체크

	//weight - 남아 있는 공간
	for(int i=0;i<item_num;i++)
	{
		int j=0; //bin 초기값 0으로 설정
		int k=0; //bin 초기값 0으로 설정
		int count=0; //minimum값이 변했는지 안변했는지 확인하는 변수

		minimum=bin_info[j].weight; //최소공간 초기값 1번 bin 무게로 초기화

		//bin마다 남아있는 공간 체크하고 minimum 교체
		//초기값 1 1 1
		while(k<bin_num)
		{
			if(bin_info[k].capacity == true)
			{
				//현재 남아있는 bin의 무게에서 item 무게만큼 뺏을때 현재 최소공감보다 작고 뺀것이 0 이라면	
				//부동소수점 오차감안
				if( (bin_info[k].weight - arr[i] < minimum) && ( bin_info[k].weight - arr[i] >= -0.00001) )
				{
					minimum = bin_info[k].weight-arr[i];
					count++;
				}	

				//minimum이 안바꼈다면 다음 bin의 무게로 minimum 교체
				k++;
				if(count == 0)
					minimum = bin_info[k].weight;

			}else{
				k++;
				if(count == 0)
					minimum = bin_info[k].weight;
			}
		}

		//bin에 삽입하고 packing에서 뺌
		while(j<3)
		{
			//최소가치와 동등한 bin을 찾고 무게빼준후에 pack에 있는거 -1로 초기화
			if(bin_info[j].weight-arr[i]==minimum && count > 0)
			{
				bin_info[j].weight -= arr[i];
				bin_info[j].item[bin_info[j].item_count]= packing[i];
				bin_info[j].item_count= bin_info[j].item_count+1;
				packing[i] = -1;

				//물건 때려넣고 남은 무게가 0이라면 capacity 0으로 만들기
				//부동소수점때문에 오차 발생 감안
				if(bin_info[j].weight <= 0.00001)
					bin_info[j].capacity = false;
				break;
			}
			j++;	
		}
	}	
}

void Print_info(int bin_num,int item_num,float packing[])
{
	int count=0; //pack에 아이템이 있는지 없는지 검사하는 변수

	//bin안에 들어 있는 값 출력
	for(int i=0;i<bin_num;i++)
	{
		printf("%d : ",i+1);
		for(int j=0;j<bin_info[i].item_count;j++)
		{
			printf("%0.2f ",bin_info[i].item[j]);
		}
		printf("\n");
	}
	
	//packing안에 있는 것 출력
	printf("No packing :");
	
	for(int i=0;i<item_num;i++)
	{
		//printf("%0.2f ",packing[i]);
		
		//pack에 item이 남아 있다면 출력
		if(packing[i] != -1)
		{
			printf("%0.2f ",packing[i]);
			count++;
		}
		
	}
	//pack에 아이템이 없다면 None출력

	if(count==0)
		printf(" None");
	printf("\n");
	
	/*
	//각 bin의 남은 무게 출력
	for(int k=0;k<bin_num;k++)
		printf("%d : %0.2f \n",k+1,bin_info[k].weight);
	*/
}
