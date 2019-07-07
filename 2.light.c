#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#pragma warning(disable:4996)

#define MAX_SIZE 1001
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define SWAP(x,y,temp) ((temp)=(x),(x)=(y),(y)=(temp))
#define INF 1000000000  //최대 전력값

int D[MAX_SIZE],W[MAX_SIZE],TOTAL[MAX_SIZE][MAX_SIZE][2];
int wSum[MAX_SIZE];
int N; //가로등 개수
int M; //좌표
int nM; //가로등 위치
int result; //최소 전력량

/*
왼쪽애 L개 오른쪽에 R개 가로등이 남아있고
손흥민이 어느쪽에 가있는지가 onWhich로 주어질때 1이면오른쪽 0이 왼쪽
남은 가로등을 다 끌때까지 소비되는 최소 전력 소비량
*/

int minWaste(int L,int R,bool onWhich);
void selection_sort();

int main(void)
{

	char f_name[20];
	printf("Input File Name?\n");
	scanf("%s",f_name);
	FILE *fp=fopen(f_name,"r");

	//가로등 개수와 초기 위치
	int init_info[2];

	//txt파일 첫번째줄 읽어 init_info에 저장
	for(int i=0;i<2;i++)
		fscanf(fp,"%d",&init_info[i]);
	//printf("가로등 개수:%d 초기 위치:%d\n",init_info[0],init_info[1]);
	
	// 2차원 배열 동적할당
	int height=2,width=init_info[0];
	int **arr;
	arr = (int**)malloc(sizeof(int*)*height);
	for(int i=0;i<height;i++)
	{
    	arr[i] = (int*)malloc(sizeof(int)*width);
	}

	//txt파일 내용 배열 저장
	//첫번째줄은 size를 읽어들여서 2번째줄부터 받아들임
	for(int i=2;i<height+2;i++)
	{
		for(int j=0;j<width;j++)
		{
			fscanf(fp,"%d",&arr[i-2][j]);
		}
	}
	fclose(fp);

	/*
	//배열 내용 출력
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
	
	N=init_info[0];
	M=init_info[1];
	wSum[0]=0;
	
	//배열 값 채우기
	for(int k=1;k<=2;k++){
		for(int z=1;z<=N;z++){
			if(k==1)
				D[z]=arr[k-1][z-1];
			if(k==2)
				W[z]=arr[k-1][z-1];
		}
	}
	//배열 끝에 나의 현재좌표에 해당하는 값과 전력량 넣어줌 전력량은 방문중인 가로등이니 0임  
	D[N+1]=M;
	W[N+1]=0;
	
	//해당 위치에 정렬
	selection_sort();
	
	
	/*
	//정렬 잘됫는지 확인	
	for(int i=1;i<=N+1;i++)
		printf("%d %d\n",D[i],W[i]);
	
	*/


	//M의 index를 찾아서 저장한다 nM에 저장
	for(int i=1;i<=N+1;i++){
		if(D[i]==M)
			nM=i;
	}
	
	//왼쪽 오른쪽 가면서 켜진 가로등 값 미리 저장(누적)
	//i는 w[1] + w[2] + ... w[i+1]
	for(int i=1;i<=N+1;i++){
        wSum[i] = wSum[i-1] + W[i];
    }
	
	/*
	//배열에 값들 다 잘 들어갔는지 확인
	for(int i=1;i<=N+1;i++)
		printf("가로등 위치%d 전력%d  누적 합%d\n",D[i],W[i],wSum[i]);
	*/

	//total -1로 초기화	
	memset(TOTAL, -1, sizeof(TOTAL));


	//2차원배열 동적할당 해제
	for(int i=0;i<height;i++)
	{
		free(arr[i]);
	}
	free(arr);

	//최소 전력량
	result = minWaste(nM,nM,false);
	printf("%d\n",result);

	return 0;
}

int minWaste(int L, int R,bool onWhich){

	if(L == 1 && R == N+1)
		return 0;
	if(TOTAL[L][R][onWhich] != -1){
		return TOTAL[L][R][onWhich];		
	}

	int nsec,ncost;
	int ans = INF;

	if(L-1 >=1){
		//이동거리
		//onWhich가 0이면 왼쪽에 위치
		if(!onWhich)
			nsec = D[L] - D[L-1]; //왼쪽에서 왼쪽으로
		else
			nsec = D[R] - D[L-1]; //왼쪽에서 오른쪽으로 선택
		ncost = nsec * (wSum[L-1] + (wSum[N+1]-wSum[R]) ); //켜져있는 가로등 값들 누적합
		ans = MIN(ans, minWaste(L-1,R,false) + ncost);

	}
	if(R+1<= N+1){
		if(!onWhich)
			nsec = D[R+1] - D[L]; //오른쪽에서 왼쪽
		else
			nsec = D[R+1] - D[R]; //오른쪽에서 오른쪽
		ncost = nsec*(wSum[L-1] + (wSum[N+1]-wSum[R]) );
		ans = MIN(ans, minWaste(L,R+1,true) + ncost);
	}
	
	
	return ans;
}

void selection_sort()
{
	int least,temp;
	//마지막 숫자는 자동으로 정렬되기 때문에 (배열개수-1)만큼
	//현재 나의 배열 개수는 N+1이고 배열 index 1부터 쓰기때문에
	for(int i=1;i<=N;i++){
		least = i;

		//최소값 탐색
		for(int j=i+1;j<=N+1;j++){
			if(D[j]<D[least])
				least = j;
		}

		//최소값이 자기 자신이면 자료 이동을 하지 않는다.
		if(i != least){
			SWAP(D[i],D[least],temp);
			SWAP(W[i],W[least],temp);
		}
	}
}
