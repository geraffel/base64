#include <iostream>
#include <cstdio>
#include <cstdint>
#include <cstring>


static constexpr char tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static inline uint8_t bat(uint8_t c) {
    switch (c) {
    case 'A': return 0;
    case 'B': return 1;
    case 'C': return 2;
    case 'D': return 3;
    case 'E': return 4;
    case 'F': return 5;
    case 'G': return 6;
    case 'H': return 7;
    case 'I': return 8;
    case 'J': return 9;
    case 'K': return 10;
    case 'L': return 11;
    case 'M': return 12;
    case 'N': return 13;
    case 'O': return 14;
    case 'P': return 15;
    case 'Q': return 16;
    case 'R': return 17;
    case 'S': return 18;
    case 'T': return 19;
    case 'U': return 20;
    case 'V': return 21;
    case 'W': return 22;
    case 'X': return 23;
    case 'Y': return 24;
    case 'Z': return 25;
    case 'a': return 26;
    case 'b': return 27;
    case 'c': return 28;
    case 'd': return 29;
    case 'e': return 30;
    case 'f': return 31;
    case 'g': return 32;
    case 'h': return 33;
    case 'i': return 34;
    case 'j': return 35;
    case 'k': return 36;
    case 'l': return 37;
    case 'm': return 38;
    case 'n': return 39;
    case 'o': return 40;
    case 'p': return 41;
    case 'q': return 42;
    case 'r': return 43;
    case 's': return 44;
    case 't': return 45;
    case 'u': return 46;
    case 'v': return 47;
    case 'w': return 48;
    case 'x': return 49;
    case 'y': return 50;
    case 'z': return 51;
    case '0': return 52;
    case '1': return 53;
    case '2': return 54;
    case '3': return 55;
    case '4': return 56;
    case '5': return 57;
    case '6': return 58;
    case '7': return 59;
    case '8': return 60;
    case '9': return 61;
    case '+': return 62;
    case '/': return 63;
    default: return 64;
    } 
}


template <typename T>
void
print(T *ary, size_t N) {
    for (unsigned i=0; i<N; ++i) {
        std::printf("%c", ary[i]);
    }
    std::puts("\n");
}

bool
base64encode(const uint8_t *in, size_t insz,
             uint8_t **out, size_t *outsz) {
    int num_bits = insz * 8;
    int nxtmult = (num_bits+5)/6*6; 
    
    int num_eqs = 0;
    int nsz = nxtmult / 6;
    
    switch (nxtmult-num_bits) {
    case 0:
        *out = new uint8_t[nsz];
        break;
    case 2:
        num_eqs = 1;
        nsz += num_eqs;
        *out = new uint8_t[nsz];
        (*out)[nsz-1] = '=';
        break;
    case 4:
        num_eqs = 2;
        nsz += num_eqs;
        *out = new uint8_t[nsz];
        (*out)[nsz-1] = '=';
        (*out)[nsz-2] = '=';
        break;
    }
    *outsz = nsz;

    int a,b,c,d;
    int outpos = 0;
    for (unsigned j=0, i=0; j<insz/3; ++j, i+=3) {
        a = ((in[i] & 0xFCu) >> 2);
        (*out)[outpos++] = tab[a];

        b = ((in[i] & 0x03u) << 4) | ((in[i+1] & 0xF0u) >> 4);
        (*out)[outpos++] = tab[b];

        c = ((in[i+1] & 0x0Fu) << 2) | ((in[i+2] & 0xC0u) >> 6);
        (*out)[outpos++] = tab[c];

        d = in[i+2] & 0x3Fu;
        (*out)[outpos++] = tab[d];
    }
    switch(num_eqs) {
    case 0:
        break;
    case 1:
        a = ((in[insz-2] & 0xFCu) >> 2);
        (*out)[outpos++] = tab[a];

        b = ((in[insz-2] & 0x03u) << 4) | ((in[insz-1] & 0xF0u) >> 4);
        (*out)[outpos++] = tab[b];

        c = ((in[insz-1] & 0x0Fu) << 2) | 0;
        (*out)[outpos++] = tab[c];
        break;
    case 2:
        a = ((in[insz-1] & 0xFCu) >> 2);
        (*out)[outpos++] = tab[a];

        b = ((in[insz-1] & 0x03u) << 4) | 0;
        (*out)[outpos++] = tab[b];
        break;
    default:
        break;
    }

    return true;
}

bool
base64decode(const uint8_t *in, size_t insz,
             uint8_t **out, size_t *outsz) {

    int num_eqs = 0;
    if (in[insz-1] == '=' && in[insz-2] == '=') {
        num_eqs = 2;
    } else if (in[insz-1] == '=' && in[insz-2] != '=') {
        num_eqs = 1;
    } else {
        num_eqs = 0; // not relevant
    }

    *outsz = insz/4 * 3 - num_eqs;

    *out = new uint8_t[*outsz];

    int outpos = 0;
    for (unsigned j=0, i=0; j<insz-num_eqs/4; ++j, i+=4) {
        (*out)[outpos++] = ((bat(in[i]) & 0x3Fu) << 2) | ((bat(in[i+1]) & 0x30u) >> 4);
        (*out)[outpos++] = ((bat(in[i+1]) & 0x0Fu) << 4) | ((bat(in[i+2]) & 0x3Cu) >> 2);
        (*out)[outpos++] = ((bat(in[i+2]) & 0x03u) << 6) | ((bat(in[i+3]) & 0x3Fu));
    }
    
    return true;
}

int
main(void) {

    uint8_t in[] =
        "Man is distinguished, not only by his reason, but by this singular passion "
        "from other animals, which is a lust of the mind, that by a perseverance of "
        "delight in the continued and indefatigable generation of knowledge, exceeds "
        "the short vehemence of any carnal pleasure.";

    const uint8_t encout[] =
        "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB"
        "0aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIG"
        "x1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpb"
        "iB0aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xl"
        "ZGdlLCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=";


    
    uint8_t *out = nullptr;
    size_t outsz = 0;

    if (!base64encode(in, sizeof(in)-1, &out, &outsz)) {
        std::cout << "Error in encoding!\n";
    }
        
    if (!!std::strncmp((char *)out, (char *)encout, outsz)) {
        std::cout << "Error in encoding!\n";
        print(out, outsz);
        print(encout, sizeof(encout)-1);
    } else {
        std::cout << "Encoded: \t";
        print(out, outsz);
    }

    delete[] out;
    out = nullptr;
    outsz = 0;

    if (!base64decode(encout, sizeof(encout)-1, &out, &outsz)) {
        std::cout << "Error in decoding!\n";
    }

    if (!!std::strncmp((char *)out, (char *)in, outsz)) {
        std::cout << "Error in decoding\n";
        print(out, outsz);
        print(in, sizeof(in)-1);
    } else {
        std::cout << "Decoded: \t";
        print(out, outsz);
    }

    delete[] out;
    
    return 0;
}
