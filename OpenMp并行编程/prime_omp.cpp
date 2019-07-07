#include<stdio.h>
#include<math.h>
#include<omp.h>

#define THREAD_NUM 8
//ȫ�ֱ���-���ַ�Χ
int n = 0;
//��������
int prime_num = 0;

//�ж�һ�����Ƿ�������
int isPrime(int n);
//���1-n��Χ�ڵ���������
void prime(int n);
//���1-n��Χ�ڵ��������������л���
void prime_parallel(int n);

int main(){
	printf("�����뷶Χ��");
	scanf_s("%d", &n);
	printf("\n");
	double startTime = omp_get_wtime();
	prime(n);
	//prime_parallel(n);
	double endTime = omp_get_wtime();
	printf("����������%d\n", prime_num);
	printf("����������ʱ��%fS\n", endTime - startTime);
	return 0;
}

int isPrime(int n){
	int flag = 1;
	if (n == 2) return flag;
	int range = ceil(sqrt(n + 0.0));
	for (int i = 2; i <= range; i++){
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
#pragma omp parallel for num_threads(THREAD_NUM)
	for (int i = 2; i <= n; i++){
		if (isPrime(i)){
#pragma omp critical
			prime_num++;
		}
	}
}