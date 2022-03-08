#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <locale.h>

#include <string.h>

void decToBin(char out[], uint32_t in, uint64_t);

void encipher(uint32_t[], uint32_t[], uint64_t);
void decipher(uint32_t[], uint32_t[], uint64_t);

int main()
{
    uint64_t block, tmp;
    uint32_t blockSize = 8, kSize =  128, i = 0, counter = 0, key[4], v[2], numRounds = 32;
    uint8_t date = 1;
    char bufor;

    key[0] = 0x01234567;
    key[1] = 0x01234567;
    key[2] = 0x01234567;
    key[3] = 0x01234567;

    FILE *out, *in;
    in = fopen("input.txt", "r");
    out = fopen("output.txt", "w");

    while(date)
    {
        int j = 0;
        block = 0;

        do
        {
            block <<= 8;
            bufor = fgetc(in);
            if((int)bufor < 0 )
                {
                    bufor = 0;
                    date = 0;
                }
            block += (int)bufor;

            j++;
        }while(j < blockSize);

        tmp = block;
        tmp >>= 32;
        v[0] =  tmp;
        tmp = (block & 0x00000000FFFFFFFF);
        v[1] = tmp;
        tmp = block;

        encipher(v, key, 32);
        decipher(v, key, 32);

        tmp = v[0];
        tmp <<= 32;
        tmp += v[1];

        for(int o = 0; o < 8; o++)
        {
            uint64_t mask= 0xFF00000000000000, t = 0;

            t = tmp & mask;
            t >>= 56;
            bufor = (char)t;
            fputc(bufor, out);

            tmp <<= 8;
        }
    }

    fclose(in);
    fclose(out);

    printf("End of encryption");

    return 0;
}

void encipher(uint32_t v[], uint32_t key[], uint64_t runds)
{
    uint32_t x = v[0], y = v[1], delta = 0x9E3779B9, sum = 0;

    for (uint64_t i = 0; i < runds; i++)
    {
        x += (((y << 4) ^ (y >> 5)) + y) ^ (sum + key[sum & 3]);
        sum += delta;
        y += (((x << 4) ^ (x >> 5)) + x) ^ (sum + key[(sum >> 11) & 3]);
    }
    v[0] = x;
    v[1] = y;
}

void decipher(uint32_t v[], uint32_t key[], uint64_t runds)
{
    uint32_t x = v[0], y =  v[1], delta = 0x9E3779B9 , sum = delta * runds;

    for (int i = 0; i < runds; i++)
    {
        y -= (((x << 4) ^ (x >> 5)) + x) ^ (sum + key[(sum >> 11) & 3]);
        sum -= delta;
        x -= (((y << 4) ^ (y >> 5)) + y) ^ (sum + key[sum & 3]);
    }
    v[0] = x;
    v[1] = y;
}
