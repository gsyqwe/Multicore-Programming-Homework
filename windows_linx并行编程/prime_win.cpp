#include<stdio.h>
#include<math.h>
#include<omp.h>
#include<windows.h>

#define THREAD_NUM 8
//全局变量-数字范围
int n = 0;
//素数个数
int prime_num = 0;
CRITICAL_SECTION g_csVar; //创建关键段cs
HANDLE semphore; //定义信号量
//判断一个数是否是素数
int isPrime(int n);
//输出1-n范围内的所有素数
void prime(int n);
//输出1-n范围内的所有素数（并行化）
void prime_parallel(int n);
//线程函数
DWORD WINAPI ThreadFunc(LPVOID);

int main(){
	scanf_s("%d", &n);
	printf("\n");
	double startTime = omp_get_wtime();
	prime_parallel(n);
	double endTime = omp_get_wtime();
	printf("素数个数：%d\n", prime_num);
	printf("程序运行用时：%fS\n",endTime-startTime);
	return 0;
}

int isPrime(int n){
	int flag = 1;
	if (n == 2) return flag;
	int range = ceil(sqrt(n+0.0));
	for (int i = 2; i <= range;i++){
		if (n % i == 0){
			flag = 0;
			break;
		}
	}
	return flag;
}

void prime(int n){
	for (int i = 2; i <= n; i++){
		if (isPrime(i)){
			prime_num++;
		}
	}
}

void prime_parallel(int n){
	//初始化
	InitializeCriticalSection(&g_csVar);
	semphore = CreateSemaphore(NULL, 0, 1, NULL);
	//创建线程
	HANDLE hThread[THREAD_NUM];
	for (int i = 0; i < THREAD_NUM; i++)
	{
		hThread[i] = CreateThread(NULL, 0, ThreadFunc, (LPVOID)i, 0, NULL);
	}
	//等待线程运行结束
	for (int i = 0; i < THREAD_NUM; ++i)
	{
		if (WaitForSingleObject(hThread[i], INFINITE))
		{
			CloseHandle(hThread[i]);
		}
	}
}

DWORD WINAPI ThreadFunc(LPVOID lpPara){
	int begin = 2 + (int)lpPara;
	for (int i = begin; i <= n;i=i+THREAD_NUM){
		if (isPrime(i)){
			EnterCriticalSection(&g_csVar);
			prime_num++; //临界区资源
			LeaveCriticalSection(&g_csVar);
			ReleaseSemaphore(semphore, 1, NULL);
		}
	}
	return 0;
}
