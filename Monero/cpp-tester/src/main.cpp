
#include <iostream>
#include <iomanip>

#include <stdio.h>

#include <wmmintrin.h>
#include <inttypes.h>



#define TABLE_ALIGN     32
#define WPOLY           0x011b
#define N_COLS          4
#define AES_BLOCK_SIZE  16
#define RC_LENGTH       (5 * (AES_BLOCK_SIZE / 4 - 2))

#if defined(_MSC_VER)
#define ALIGN __declspec(align(TABLE_ALIGN))
#elif defined(__GNUC__)
#define ALIGN __attribute__ ((aligned(16)))
#else
#define ALIGN
#endif

#define rf1(r,c) (r)
#define word_in(x,c) (*((uint32_t*)(x)+(c)))
#define word_out(x,c,v) (*((uint32_t*)(x)+(c)) = (v))

#define s(x,c) x[c]
#define si(y,x,c) (s(y,c) = word_in(x, c))
#define so(y,x,c) word_out(y, c, s(x,c))
#define state_in(y,x) si(y,x,0); si(y,x,1); si(y,x,2); si(y,x,3)
#define state_out(y,x)  so(y,x,0); so(y,x,1); so(y,x,2); so(y,x,3)
//#define round(rm,y,x,k) rm(y,x,k,0);
#define round(rm,y,x,k) rm(y,x,k,0); rm(y,x,k,1); rm(y,x,k,2); rm(y,x,k,3)
#define to_byte(x) ((x) & 0xff)
#define bval(x,n) to_byte((x) >> (8 * (n)))

#define fwd_var(x,r,c)\
     ( r == 0 ? ( c == 0 ? s(x,0) : c == 1 ? s(x,1) : c == 2 ? s(x,2) : s(x,3))\
        : r == 1 ? ( c == 0 ? s(x,1) : c == 1 ? s(x,2) : c == 2 ? s(x,3) : s(x,0))\
        : r == 2 ? ( c == 0 ? s(x,2) : c == 1 ? s(x,3) : c == 2 ? s(x,0) : s(x,1))\
        :          ( c == 0 ? s(x,3) : c == 1 ? s(x,0) : c == 2 ? s(x,1) : s(x,2)))

#define fwd_rnd(y,x,k,c)  (s(y,c) = (k)[c] ^ four_tables(x,t_use(f,n),fwd_var,rf1,c))
//#define fwd_rnd(y,x,k,c)  four_tables(x,t_use(f,n),fwd_var,rf1,c)

#define sb_data(w) {\
      w(0x63), w(0x7c), w(0x77), w(0x7b), w(0xf2), w(0x6b), w(0x6f), w(0xc5),\
      w(0x30), w(0x01), w(0x67), w(0x2b), w(0xfe), w(0xd7), w(0xab), w(0x76),\
      w(0xca), w(0x82), w(0xc9), w(0x7d), w(0xfa), w(0x59), w(0x47), w(0xf0),\
      w(0xad), w(0xd4), w(0xa2), w(0xaf), w(0x9c), w(0xa4), w(0x72), w(0xc0),\
      w(0xb7), w(0xfd), w(0x93), w(0x26), w(0x36), w(0x3f), w(0xf7), w(0xcc),\
      w(0x34), w(0xa5), w(0xe5), w(0xf1), w(0x71), w(0xd8), w(0x31), w(0x15),\
      w(0x04), w(0xc7), w(0x23), w(0xc3), w(0x18), w(0x96), w(0x05), w(0x9a),\
      w(0x07), w(0x12), w(0x80), w(0xe2), w(0xeb), w(0x27), w(0xb2), w(0x75),\
      w(0x09), w(0x83), w(0x2c), w(0x1a), w(0x1b), w(0x6e), w(0x5a), w(0xa0),\
      w(0x52), w(0x3b), w(0xd6), w(0xb3), w(0x29), w(0xe3), w(0x2f), w(0x84),\
      w(0x53), w(0xd1), w(0x00), w(0xed), w(0x20), w(0xfc), w(0xb1), w(0x5b),\
      w(0x6a), w(0xcb), w(0xbe), w(0x39), w(0x4a), w(0x4c), w(0x58), w(0xcf),\
      w(0xd0), w(0xef), w(0xaa), w(0xfb), w(0x43), w(0x4d), w(0x33), w(0x85),\
      w(0x45), w(0xf9), w(0x02), w(0x7f), w(0x50), w(0x3c), w(0x9f), w(0xa8),\
      w(0x51), w(0xa3), w(0x40), w(0x8f), w(0x92), w(0x9d), w(0x38), w(0xf5),\
      w(0xbc), w(0xb6), w(0xda), w(0x21), w(0x10), w(0xff), w(0xf3), w(0xd2),\
      w(0xcd), w(0x0c), w(0x13), w(0xec), w(0x5f), w(0x97), w(0x44), w(0x17),\
      w(0xc4), w(0xa7), w(0x7e), w(0x3d), w(0x64), w(0x5d), w(0x19), w(0x73),\
      w(0x60), w(0x81), w(0x4f), w(0xdc), w(0x22), w(0x2a), w(0x90), w(0x88),\
      w(0x46), w(0xee), w(0xb8), w(0x14), w(0xde), w(0x5e), w(0x0b), w(0xdb),\
      w(0xe0), w(0x32), w(0x3a), w(0x0a), w(0x49), w(0x06), w(0x24), w(0x5c),\
      w(0xc2), w(0xd3), w(0xac), w(0x62), w(0x91), w(0x95), w(0xe4), w(0x79),\
      w(0xe7), w(0xc8), w(0x37), w(0x6d), w(0x8d), w(0xd5), w(0x4e), w(0xa9),\
      w(0x6c), w(0x56), w(0xf4), w(0xea), w(0x65), w(0x7a), w(0xae), w(0x08),\
      w(0xba), w(0x78), w(0x25), w(0x2e), w(0x1c), w(0xa6), w(0xb4), w(0xc6),\
      w(0xe8), w(0xdd), w(0x74), w(0x1f), w(0x4b), w(0xbd), w(0x8b), w(0x8a),\
      w(0x70), w(0x3e), w(0xb5), w(0x66), w(0x48), w(0x03), w(0xf6), w(0x0e),\
      w(0x61), w(0x35), w(0x57), w(0xb9), w(0x86), w(0xc1), w(0x1d), w(0x9e),\
      w(0xe1), w(0xf8), w(0x98), w(0x11), w(0x69), w(0xd9), w(0x8e), w(0x94),\
      w(0x9b), w(0x1e), w(0x87), w(0xe9), w(0xce), w(0x55), w(0x28), w(0xdf),\
      w(0x8c), w(0xa1), w(0x89), w(0x0d), w(0xbf), w(0xe6), w(0x42), w(0x68),\
      w(0x41), w(0x99), w(0x2d), w(0x0f), w(0xb0), w(0x54), w(0xbb), w(0x16) }

#define rc_data(w) {\
      w(0x01), w(0x02), w(0x04), w(0x08), w(0x10),w(0x20), w(0x40), w(0x80),\
      w(0x1b), w(0x36) }

#define bytes2word(b0, b1, b2, b3) (((uint32_t)(b3) << 24) | \
            ((uint32_t)(b2) << 16) | ((uint32_t)(b1) << 8) | (b0))

#define h0(x)   (x)
#define w0(p)   bytes2word(p, 0, 0, 0)
#define w1(p)   bytes2word(0, p, 0, 0)
#define w2(p)   bytes2word(0, 0, p, 0)
#define w3(p)   bytes2word(0, 0, 0, p)

#define u0(p)   bytes2word(f2(p), p, p, f3(p))
#define u1(p)   bytes2word(f3(p), f2(p), p, p)
#define u2(p)   bytes2word(p, f3(p), f2(p), p)
#define u3(p)   bytes2word(p, p, f3(p), f2(p))

#define v0(p)   bytes2word(fe(p), f9(p), fd(p), fb(p))
#define v1(p)   bytes2word(fb(p), fe(p), f9(p), fd(p))
#define v2(p)   bytes2word(fd(p), fb(p), fe(p), f9(p))
#define v3(p)   bytes2word(f9(p), fd(p), fb(p), fe(p))

#define f2(x)   ((x<<1) ^ (((x>>7) & 1) * WPOLY))
#define f4(x)   ((x<<2) ^ (((x>>6) & 1) * WPOLY) ^ (((x>>6) & 2) * WPOLY))
#define f8(x)   ((x<<3) ^ (((x>>5) & 1) * WPOLY) ^ (((x>>5) & 2) * WPOLY) ^ (((x>>5) & 4) * WPOLY))
#define f3(x)   (f2(x) ^ x)
#define f9(x)   (f8(x) ^ x)
#define fb(x)   (f8(x) ^ f2(x) ^ x)
#define fd(x)   (f8(x) ^ f4(x) ^ x)
#define fe(x)   (f8(x) ^ f4(x) ^ f2(x))

#define t_dec(m,n) t_##m##n
#define t_set(m,n) t_##m##n
#define t_use(m,n) t_##m##n

#define d_4(t,n,b,e,f,g,h) ALIGN const t n[4][256] = { b(e), b(f), b(g), b(h) }

//#define four_tables(x,tab,vf,rf,c) \
//      (tab[0][bval(vf(x,0,c),rf(0,c))]


#define four_tables(x,tab,vf,rf,c) \
      (tab[0][bval(vf(x,0,c),rf(0,c))] \
          ^ tab[1][bval(vf(x,1,c),rf(1,c))] \
          ^ tab[2][bval(vf(x,2,c),rf(2,c))] \
          ^ tab[3][bval(vf(x,3,c),rf(3,c))])

d_4(uint32_t, t_dec(f,n), sb_data, u0, u1, u2, u3);

void print128_num2(const char *varName, int index, __m128i var) 
{
    uint32_t *v32val = (uint32_t*) &var;
    printf("%s (uint32_t)[%d] = %8x %8x %8x %8x\n",
                                        varName,
                                        index,
                                        v32val[3],
                                        v32val[2],
                                        v32val[1],
                                        v32val[0]);
}

void print128_num3(const char *varName, int index, uint32_t *v32val) 
{
    printf("%s (uint32_t)[%d] = %8x %8x %8x %8x\n",
                                        varName,
                                        index,
                                        v32val[3],
                                        v32val[2],
                                        v32val[1],
                                        v32val[0]);
}

void aesb_single_round(const uint8_t *in, uint8_t *out, uint8_t *expandedKey)
{
    uint32_t b0[4], b1[4];
    const uint32_t  *kp = (uint32_t *) expandedKey;

    state_in(b0, in);

    round(fwd_rnd,  b1, b0, kp);

    state_out(out, b1);
}

void print_line()
{
    std::cout << "+" << std::string(80, '-') << "+\n";
}

void dump_128_u8(const char *varName, uint8_t *value)
{
    const uint8_t *kp_8 = (uint8_t *)&value;
    //std::cout << "ADDR_3 = " << std::hex << (uint32_t*)value << "\n";
    for(int i=0; i<16; i++)
    {
        std::cout << std::hex
                  << std::setfill('0')
                  << std::setw(2)
                  << (unsigned int)value[i];
    }
    std::cout << "\n";
}
void dump_128(const char *varName, __m128i& value)
{
    const uint32_t *kp = (uint32_t *)&value;
    //std::cout << "ADDR_2 = " << std::hex << kp << "\n";
    std::cout << varName << " = ";
    for(int i=0; i<4; i++) {
        std::cout << "0x" << std::hex
                  << std::setfill('0')
                  << std::setw(8)
                  << (uint32_t)kp[i] << "  ";
    }
    std::cout << "\n";
}

//__m128i full_intrinsic(__m128i state, __m128i key_in)
//{
//    __m128i result;
//    __m128i in = state;
//    __m128i key = key_in;
//
//    in = _mm_xor_si128( in, key );
//    for(int i=1; i<10; i++)
//    {
//       in = _mm_aesenc_si128( in, key);
//    }
//
//    result = _mm_aesenclast_si128( in, key );
//
//    return result;
//}

__m128i calculate_intrinsic(__m128i state, __m128i key)
{
    __m128i state_copy;

    state_copy = _mm_aesenc_si128(state, key);

    return state_copy;
}

__m128i calculate_single_round(__m128i state, __m128i key)
{
    __m128i state_copy;
    state_copy = state;

    aesb_single_round( (uint8_t *) &state_copy,
                       (uint8_t *) &state_copy,
                       (uint8_t *) &key);

    return state_copy;
}

/* Based on git@github.com:kokke/tiny-AES-c.git */
typedef uint8_t state_t[4][4];
void dump_128_u83(const char *varName, uint8_t *value)
{
    const uint8_t *kp_8 = (uint8_t *)&value;
    std::cout << std::setfill(' ') << std::setw(12) << varName << " ";
    for(int i=0; i<16; i++)
    {
        std::cout << std::hex
                  << std::setfill('0')
                  << std::setw(2)
                  << (unsigned int)value[i];
    }
    std::cout << "\n";
}

// The ShiftRows() function shifts the rows in the state to the left.
// Each row is shifted with different offset.
// Offset = Row number. So the first row is not shifted.
static void ShiftRows(state_t* state)
{
  uint8_t temp;

  // Rotate first row 1 columns to left  
  temp           = (*state)[0][1];
  (*state)[0][1] = (*state)[1][1];
  (*state)[1][1] = (*state)[2][1];
  (*state)[2][1] = (*state)[3][1];
  (*state)[3][1] = temp;

  // Rotate second row 2 columns to left  
  temp           = (*state)[0][2];
  (*state)[0][2] = (*state)[2][2];
  (*state)[2][2] = temp;

  temp           = (*state)[1][2];
  (*state)[1][2] = (*state)[3][2];
  (*state)[3][2] = temp;

  // Rotate third row 3 columns to left
  temp           = (*state)[0][3];
  (*state)[0][3] = (*state)[3][3];
  (*state)[3][3] = (*state)[2][3];
  (*state)[2][3] = (*state)[1][3];
  (*state)[1][3] = temp;
}

static const uint8_t rsbox[256] = {
  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };
/*
static uint8_t getSBoxValue(uint8_t num)
{
  return sbox[num];
}
*/
#define getSBoxValue(num) (sbox[(num)])
/*
static uint8_t getSBoxInvert(uint8_t num)
{
  return rsbox[num];
}
*/
#define getSBoxInvert(num) (rsbox[(num)])

// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
static void InvSubBytes(state_t* state)
{
  uint8_t i, j;
  for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      (*state)[j][i] = getSBoxInvert((*state)[j][i]);
    }
  }
}

// The lookup-tables are marked const so they can be placed in read-only storage instead of RAM
// The numbers below can be computed dynamically trading ROM for RAM - 
// This can be useful in (embedded) bootloader applications, where ROM is often limited.
static const uint8_t sbox[256] = {
  //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };
// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
static void SubBytes(state_t* state)
{
  uint8_t i, j;
  for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      (*state)[j][i] = getSBoxValue((*state)[j][i]);
    }
  }
}

static uint8_t xtime(uint8_t x)
{
  return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

// MixColumns function mixes the columns of the state matrix
static void MixColumns(state_t* state)
{
  uint8_t i;
  uint8_t Tmp, Tm, t;
  for (i = 0; i < 4; ++i)
  {  
    t   = (*state)[i][0];
    Tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3] ;
    Tm  = (*state)[i][0] ^ (*state)[i][1] ; Tm = xtime(Tm);  (*state)[i][0] ^= Tm ^ Tmp ;
    Tm  = (*state)[i][1] ^ (*state)[i][2] ; Tm = xtime(Tm);  (*state)[i][1] ^= Tm ^ Tmp ;
    Tm  = (*state)[i][2] ^ (*state)[i][3] ; Tm = xtime(Tm);  (*state)[i][2] ^= Tm ^ Tmp ;
    Tm  = (*state)[i][3] ^ t ;              Tm = xtime(Tm);  (*state)[i][3] ^= Tm ^ Tmp ;
  }
}

#define AES_BLOCKLEN 16 //Block length in bytes AES is 128b block only
static void XorWithIv(uint8_t* buf, uint8_t* Iv)
{
  uint8_t i;
  for (i = 0; i < AES_BLOCKLEN; ++i) // The block in AES is always 128bit no matter the key size
  {
    buf[i] ^= Iv[i];
  }
}

// Cipher is the main function that encrypts the PlainText.
static void Cipher2(state_t* state, uint8_t* RoundKey)
{
    uint8_t round = 0;

    ShiftRows(state);
    SubBytes(state);
    MixColumns(state);
    XorWithIv((uint8_t*)state, RoundKey);
}
__m128i calculate_c(__m128i state, __m128i key2)
{
    __m128i result;

    uint8_t key[16];
    uint8_t  in[16];

    _mm_store_si128( (__m128i*) &in, state );
    _mm_store_si128( (__m128i*)&key, key2);

    Cipher2((state_t*)in, key);

    result = _mm_set_epi8(in[15], in[14], in[13], in[12],
                          in[11], in[10], in[ 9], in[ 8],
                          in[ 7], in[ 6], in[ 5], in[ 4],
                          in[ 3], in[ 2], in[ 1], in[ 0]);

    return result;
}

int main(int arc, char **argv)
{
    print_line();
    std::cout << "Monero preprocessor  ---\n";
    std::cout << "\n\n";

    __m128i key;
    __m128i state; 
    __m128i exp_state;

    key        =  _mm_setr_epi32(0xab09cac1, 0x09cfb622, 0xb5d5f13c, 0xda3ed5e2);
    state      =  _mm_setr_epi32(0x4fa33a00, 0x1f15431d, 0x09ad8239, 0x669eb8ea);
    exp_state  =  _mm_setr_epi32(0xe7140a8f, 0xf5f9adcb, 0x7ee4e566, 0x9388a334);

    // Dump initial State
    print_line();
    dump_128("Input key:", key);
    dump_128("Input state:", state);
    print_line();

    // Perform calculations
    __m128i     act_int  =  calculate_intrinsic(state, key);
    __m128i  act_single  =  calculate_single_round(state, key);
    __m128i       act_c  =  calculate_c(state, key);

    // Display Results
    print_line();
    std::cout << "Results:\n";
    std::cout << "--\n";
    print_line();
    dump_128("EXPECTED      ", exp_state);
    dump_128("actual_int    ", act_int);
    dump_128("actual_single ", act_single);
    dump_128("actual_c      ", act_c);
    print_line();

    std::cout << "\n\n";
    print_line();

    return 0;
}
