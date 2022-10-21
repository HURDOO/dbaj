/*
    DBAJ 암호화 구현 코드 (C/C++)
    https://github.com/HURDOO/dbaj 에서도 확인할 수 있습니다.
*/

#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <vector>
#include <math.h>
#include <string.h>
#define PRIME_RANGE 100
// #define PUBLIC_E 0x10001 // 65537
using namespace std;


/* RSA */


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


vector<unsigned char> v;
unsigned int h[5] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0};

// 함수 f 정의
int f(int t, int b, int c, int d)
{
    if (00 <= t && t <= 19) return (b & c) | (~b & d);
    if (20 <= t && t <= 39) return b ^ c ^ d;
    if (40 <= t && t <= 59) return (b & c) | (b & d) | (c & d);
    if (60 <= t && t <= 79) return b ^ c ^ d;

    // cannot reach here
    printf("f -1\n");
    return -1;
}


/* SHA */


// 상수 t 정의
int k(int t)
{
    if (00 <= t && t <= 19) return 0x5a827999;
    if (20 <= t && t <= 39) return 0x6ed9eba1;
    if (40 <= t && t <= 59) return 0x8f1bbcdc;
    if (60 <= t && t <= 79) return 0xca62c1d6;

    // cannot reach here
    printf("k -1\n");
    return -1;
}

// 좌측 순환 시프트
int rotl(int x,int n) {
    return (x << n) || (x >> 32-n);
}

// 메인 코드
char* sha(int m)
{
    printf("Starting SHA encryption\n");
    // 4byte 숫자를 1byte로 나눔
    char s[4];
    for (int i=0;i<4;i++) {
        s[i] = (m >> (24-(8*i))) & ((1<<8)-1);
    }
    int lens = strlen(s);
    printf("Input: '%s', length: %d\n", s, lens);

    // 문자를 비트로 변환하여 저장 (ASCII 코드값 사용)
    for (int i=0;i<lens;i++) {
        v.push_back(s[i] >> 8);
        v.push_back(s[i] & ((1<<8) -1));
    }
    
    // 패딩 (0을 넣어 길이 맞추기)
    v.push_back(0b10000000);
    for (int i=0;i<56-(lens*2+1)%64;i++) {
        v.push_back(0);
    }

    // 실제 평문의 길이 정보 삽입
    for (int i=0;i<8;i++)
    {
        if (sizeof(lens*16) <= 56- 8*i) v.push_back(0);
        else v.push_back(((lens*16) >> (56-8*i)) & ((1<<8) -1));
    }

    // 패딩 결과 출력
    printf("Padding complete.\n");
    for(int i=0;i<v.size();i++) {
        printf("%d ", v[i]);
    }
    printf("\nSize: %d\n", v.size());

    // 논리연산 => 해시화
    for (int i=0;i<=(v.size()-1)/64;i++) { // per block
        unsigned int w[80] = {0};
        for (int j=0;j<=15;j++) {
            int start = 64*i+4*j;
            w[j] = (v[start] << 24) + (v[start+1] << 16) + (v[start+2] << 8) + v[start+3];
        }

        for (int t=16;t<=79;t++) {
            w[t] = w[t-3] ^ w[t-8] ^ w[t-14] ^ w[t-16];
        }

        int a = h[0], b = h[1], c = h[2], d = h[3], e = h[4];
        for (int t=0;t<=79;t++) {
            int p = rotl(a, 5) + f(t,b,c,d) + e + w[t] + k(t);
            e = d;
            d = c;
            c = rotl(b, 30);
            b = a;
            a = p;
        }

        h[0] = h[0] + a;
        h[1] = h[1] + b;
        h[2] = h[2] + c;
        h[3] = h[3] + d;
        h[4] = h[4] + e;
    }
    
    // 결과 출력
    for(int i=0;i<5;i++) {
        printf("%x", h[i]);
    }
    printf("\n");
    return 0;
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