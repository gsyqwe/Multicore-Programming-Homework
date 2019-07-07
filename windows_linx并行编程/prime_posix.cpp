#include<stdio.h>
#include<math.h>
#include<omp.h>
#include<pthread.h>
#define THREAD_NUM 12
//全局变量-数字范围
int n = 0;
//素数个数
int prime_num = 0;
pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER; //互斥信号量

//判断一个数是否是素数
int isPrime(int n);
//输出1-n范围内的所有素数
void prime(int n);
//输出1-n范围内的所有素数（并行化）
void prime_parallel(int n);
void *threadFunc(void *threadid);
int main(){
	scanf_s("%d", &n);
	printf("\n");
	double startTime = omp_get_wtime();
	//prime(n);
	prime_parallel(n);
	double endTime = omp_get_wtime();
	printf("素数个数：%d\n", prime_num);
	printf("程序运行用时：%fS\n", endTime - startTime);
	pthread_exit(NULL);
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
	//初始化信号量
	pthread_mutex_init(&mutexsum, NULL);
	pthread_t threads[THREAD_NUM];
	int rc, i;
	for (i = 0; i<THREAD_NUM; i++){
		rc = pthread_create(&threads[i], NULL, threadFunc, (void *)i);
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
		}
	}
	for (int i = 0; i < THREAD_NUM; i++){
		pthread_join(threads[i], NULL);
	}
	pthread_mutex_destroy(&mutexsum);
}

void *threadFunc(void *threadid){
	int tid = (int)threadid;
	for (int i = tid+2; i <= n; i = i + THREAD_NUM){
		if (isPrime(i)){
			pthread_mutex_lock(&mutexsum);
			prime_num++; //临界区资源
			pthread_mutex_unlock(&mutexsum);
		}
	}
	pthread_exit(NULL);
	return 0;
}

