#include <stdio.h>
#include <vector>
#include <string.h>
using namespace std;

vector<unsigned char> v;
unsigned int h[5] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0};
    
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
int rotl(int x,int n) {
    return (x << n) || (x >> 32-n);
}
void print_v() {
    for(int i=0;i<v.size();i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}
void print_h() {
    for(int i=0;i<5;i++) {
        printf("%x ", h[i]);
    }
    printf("\n");
}
int main()
{
    char s[1000] = {0};
    scanf("%s", s);
    int lens = strlen(s);
    printf("Input: '%s', length: %d\n", s, lens);

    for (int i=0;i<lens;i++) {
        v.push_back(s[i] >> 8);
        v.push_back(s[i] & ((1<<8) -1));
    }

    print_v();
    
    v.push_back(0b10000000);
    for (int i=0;i<56-(lens*2+1)%64;i++) {
        v.push_back(0);
    }

    print_v();

    for (int i=0;i<8;i++)
    {
        // printf("i: %d, %d, %d, %d\n", i, lens*16, 56-8*i, (lens*16) >> (56- 8*i));
        if (sizeof(lens*16) <= 56- 8*i) v.push_back(0);
        else v.push_back(((lens*16) >> (56-8*i)) & ((1<<8) -1));
    }

    print_v();
    printf("%d\n", v.size());

    for (int i=0;i<=(v.size()-1)/64;i++) { // per block
        unsigned int w[80] = {0};
        for (int j=0;j<=15;j++) {
            int start = 64*i+4*j;
            w[j] = (v[start] << 24) + (v[start+1] << 16) + (v[start+2] << 8) + v[start+3];
            printf("start: %d / w[%d]: %d / %d %d %d %d\n", start, j, w[j], v[start] << 24, v[start+1] << 16, v[start+2] << 8, v[start+3]);
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
        print_h();
    }
    return 0;
}
