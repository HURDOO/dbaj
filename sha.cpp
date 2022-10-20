/*
    SHA 암호화 구현 코드 (C/C++)
    https://github.com/HURDOO/dbaj 에서도 확인할 수 있습니다.
*/

#include <stdio.h>
#include <vector>
#include <string.h>
using namespace std;

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
int main()
{
    // 평문 입력
    char s[1000] = {0};
    printf("Enter message (<1000): ");
    scanf("%s", s);
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
