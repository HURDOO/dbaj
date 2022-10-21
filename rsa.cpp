/*
    RSA 암호화 구현 코드 (C/C++)
    https://github.com/HURDOO/dbaj 에서도 확인할 수 있습니다.
*/

#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <vector>
#include <math.h>
#define PRIME_RANGE 100
// #define PUBLIC_E 0x10001 // 65537
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

// 최대공약수 (유클리드 호제법)
int gcd(int n1,int n2) {
    while (n2 != 0) {
        int tmp = n1;
        n1 = n2;
        n2 = tmp % n2;
    }
    return n1;
}

// 확장된 유클리드 호제법
int extended_euclid(int r1, int r2, int s1, int s2, int t1, int t2) {
    int q = r1 / r2; // 두 수의 나눗셈의 몫
    int s = s1 - s2*q; // 첫번째 항의 곱
    int t = t1 - t2*q; // 두번째 항의 곱
    int r = r1 - r2*q; // 두 수의 나머지
    
    if (r==0)
    {
        printf("%d %d\n", s2, t2);
        return s2 > 0 ? s2 : t2; // d는 항상 양수
    }
    else return extended_euclid(r2, r, s2, s, t2, t);
}
// https://goseungduk.tistory.com/45

int rsa(int n,int m,int d) {
    int num = 1;
    for (int i=0;i<d;i++) {
        num = (num * m) % n;
    }
    return num;
}

int main()
{
    // 소수 p, q를 구한다.
    vector<int> primes = prime();
    srand(15);
    // srand((unsigned int) time(NULL));
    int p = primes[rand() % primes.size()];
    int q = primes[rand() % primes.size()];
    // int p = 17, q = 13;
    printf("p: %d, q: %d\n", p, q);

    // 오일러 파이와 공개키 n, e를 구한다.
    int pi = (p-1)*(q-1);
    int n=p*q, e;
    // for (int i=pi-1;i>1;i--) {
    for (int i=2;i<pi;i++) {
        if (gcd(pi,i) == 1) {
            e = i;
            break;
        }
        if (i==pi-1) {
            printf("Cannot find e\n");
            return 0;
        }
    }
    e = 11;

    // 비밀키 d를 구한다.
    int d = extended_euclid(pi, e, 1, 0, 0, 1);

    // 생성된 키를 출력한다.
    printf("public key N: %d, e: %d / secret key d: %d\n", n, e, d);

    // 평문을 입력받는다.
    char s[1000] = {0};
    scanf("%s", s);
    printf("Input: '%s'\n", s);

    // 암호화하여 출력한다.
    printf("Encrypted: ");
    int encrypted[1000] = {0};
    for (int i=0 ; s[i] != '\0' ; i++) {
        encrypted[i] = rsa(n, s[i], e);
        printf("%d ", encrypted[i]);
    }
    printf("\n");

    // 복호화하여 출력한다.
    printf("Decrypted: ");
    int decrypted[1000] = {0};
    for (int i=0 ; s[i] != '\0' ; i++) {

        decrypted[i] = rsa(n, encrypted[i], d);
        printf("%c ", decrypted[i]);
    }
    printf("\n");
    return 0;
}