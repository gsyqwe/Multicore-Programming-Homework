#include<stdio.h>
#include<math.h>
#include<omp.h>
#include<windows.h>

#define THREAD_NUM 8
//ȫ�ֱ���-���ַ�Χ
int n = 0;
//��������
int prime_num = 0;
CRITICAL_SECTION g_csVar; //�����ؼ���cs
HANDLE semphore; //�����ź���
//�ж�һ�����Ƿ�������
int isPrime(int n);
//���1-n��Χ�ڵ���������
void prime(int n);
//���1-n��Χ�ڵ��������������л���
void prime_parallel(int n);
//�̺߳���
DWORD WINAPI ThreadFunc(LPVOID);

int main(){
	scanf_s("%d", &n);
	printf("\n");
	double startTime = omp_get_wtime();
	prime_parallel(n);
	double endTime = omp_get_wtime();
	printf("����������%d\n", prime_num);
	printf("����������ʱ��%fS\n",endTime-startTime);
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
	//��ʼ��
	InitializeCriticalSection(&g_csVar);
	semphore = CreateSemaphore(NULL, 0, 1, NULL);
	//�����߳�
	HANDLE hThread[THREAD_NUM];
	for (int i = 0; i < THREAD_NUM; i++)
	{
		hThread[i] = CreateThread(NULL, 0, ThreadFunc, (LPVOID)i, 0, NULL);
	}
	//�ȴ��߳����н���
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
			prime_num++; //�ٽ�����Դ
			LeaveCriticalSection(&g_csVar);
			ReleaseSemaphore(semphore, 1, NULL);
		}
	}
	return 0;
}
