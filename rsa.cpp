#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <vector>
#define PRIME_RANGE 100
#define PUBLIC_E 0x10001 // 65537
using namespace std;

// 에라토스테네스의 체
vector<int> prime()
{
    bool arr[PRIME_RANGE+1] = {true, true, false};

    for (int i=2;i<=PRIME_RANGE;i++) {
        if(arr[i]) continue;
        for (int j=i*2;j<=PRIME_RANGE;j+=i) {
            arr[j] = true;
        }
    }

    vector<int> primes;
    for(int i=2;i<=PRIME_RANGE;i++) {
        if(!arr[i]) primes.push_back(i);
    }

    return primes;
}

// 확장된 유클리드 호제법
int extended_uclid(int n,int e) {

}

// 최대공약수 (유클리드 호제법)
int gcd(int n1,int n2) {
    while (n2 != 0) {
        int tmp = n1;
        n1 = n2;
        n2 = tmp % n2;
    }
    return n1;
}

// 개인키 구하기
int get_private_key(int p,int q, int N, int e)
{
    
}

int main()
{
    // 소수 p, q를 구한다.
    vector<int> primes = prime();
    srand(10);
    int p = primes[rand() % primes.size()];
    int q = primes[rand() % primes.size()];
    printf("p: %d, q: %d\n", p, q);

    // 공개키 N,e를 구한다.
    int N = p*q;
    int d = rsa(p, q, N, PUBLIC_E);

    return 0;
}