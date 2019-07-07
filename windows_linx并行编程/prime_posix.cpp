#include<stdio.h>
#include<math.h>
#include<omp.h>
#include<pthread.h>
#define THREAD_NUM 12
//ȫ�ֱ���-���ַ�Χ
int n = 0;
//��������
int prime_num = 0;
pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER; //�����ź���

//�ж�һ�����Ƿ�������
int isPrime(int n);
//���1-n��Χ�ڵ���������
void prime(int n);
//���1-n��Χ�ڵ��������������л���
void prime_parallel(int n);
void *threadFunc(void *threadid);
int main(){
	scanf_s("%d", &n);
	printf("\n");
	double startTime = omp_get_wtime();
	//prime(n);
	prime_parallel(n);
	double endTime = omp_get_wtime();
	printf("����������%d\n", prime_num);
	printf("����������ʱ��%fS\n", endTime - startTime);
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
	//��ʼ���ź���
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
			prime_num++; //�ٽ�����Դ
			pthread_mutex_unlock(&mutexsum);
		}
	}
	pthread_exit(NULL);
	return 0;
}

