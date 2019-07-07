#include<stdio.h>
#include<math.h>
#include<omp.h>

#define THREAD_NUM 8
//全局变量-数字范围
int n = 0;
//素数个数
int prime_num = 0;

//判断一个数是否是素数
int isPrime(int n);
//输出1-n范围内的所有素数
void prime(int n);
//输出1-n范围内的所有素数（并行化）
void prime_parallel(int n);

int main(){
	printf("请输入范围：");
	scanf_s("%d", &n);
	printf("\n");
	double startTime = omp_get_wtime();
	prime(n);
	//prime_parallel(n);
	double endTime = omp_get_wtime();
	printf("素数个数：%d\n", prime_num);
	printf("程序运行用时：%fS\n", endTime - startTime);
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