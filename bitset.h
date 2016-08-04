#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <x86intrin.h>

#define SIMD_SIZE 128
#define ALIGNED_SIZE 16
#define CHAR_SIZE 8

class Bitset {
 public:
  // Bitset is using char* to store data
  Bitset(int size) : num_bits_(size) {
    // The total memory is larger than # of bits
    num_bytes_ = num_bits_ / CHAR_SIZE + 1;

    // bits must be aligned. For 128-bit SIMD, it is 16
    bits_ = (char*)aligned_alloc(ALIGNED_SIZE, num_bytes_);

    // Init the memory with zero
    memset(bits_, 0, num_bytes_);
  }

  // Support null bitset.
  Bitset() : num_bits_(0), num_bytes_(0), bits_(nullptr) {}

  ~Bitset() {
    if (bits_ != nullptr) {
      free(bits_);
    }
  }

  // Use resize to allocate bitset
  void Resize(int size) : num_bits_(size) {
    // The total memory is larger than # of bits
    num_bytes_ = num_bits_ / CHAR_SIZE + 1;

    // bits must be aligned. For 128-bit SIMD, it is 16
    bits_ = (char*)aligned_alloc(ALIGNED_SIZE, num_bytes_);

    // Init the memory with zero
    memset(bits_, 0, num_bytes_);
  }

  // For exampel, bits are (7, 10, 13, 45, 100)
  // and bitset are (000000000000000). This func set bitset with
  // (00000100100100001)
  void Set(std::vector<int>& bits) {
    // Iterate the input region
    for (auto& bit : bits) {
      int idx = bit / CHAR_SIZE;
      int offbit = bit % CHAR_SIZE;

      char offset = 0x01;
      offset = offset << offbit;

      std::cout << "bit--" << bit << ":: idx--" << idx << " offbit--" << offbit
                << " offset--" << (unsigned short)offset << std::endl;

      bits_[idx] |= offset;
    }
  }

  void Set(int bit) {
    int idx = bit / CHAR_SIZE;
    int offbit = bit % CHAR_SIZE;

    char offset = 0x01;
    offset = offset << offbit;

    std::cout << "bit--" << bit << ":: idx--" << idx << " offbit--" << offbit
              << " offset--" << (unsigned short)offset << std::endl;

    bits_[idx] |= offset;
  }

  // Pass another bitset, computer AND for each bit and return a new char*
  // (bitset). The passing by bitset should have the same size, and the caller
  // should guarantee this.
  // Note: this new bitset is using new, the caller should delete it later.
  char* AND(Bitset& rh_bitset) {
    // The SIMD vector
    __m128 A, B, C;

    // The char* of right hand bitset
    char* rbits = rh_bitset.Get();

    // The return char*
    char* result = (char*)aligned_alloc(ALIGNED_SIZE, num_bytes_);

    // How many chars a SIMD vector contains
    // For example, 128-bit SIMD, and char is 8, so it processes 16 bytes each
    // time (span = 16)
    int span = SIMD_SIZE / CHAR_SIZE;

    for (int i = 0; i < num_bytes_; i += span) {
      A = _mm_load_ps((float*)&bits_[i]);
      B = _mm_load_ps((float*)&rbits[i]);
      C = _mm_and_ps(A, B);
      _mm_store_ps((float*)&result[i], C);
    }

    return result;
  }

  char* OR(Bitset& rh_bitset) {
    // The SIMD vector
    __m128 A, B, C;

    // The char* of right hand bitset
    char* rbits = rh_bitset.Get();

    // The return char*
    char* result = (char*)aligned_alloc(ALIGNED_SIZE, num_bytes_);

    // How many chars a SIMD vector contains
    int span = SIMD_SIZE / CHAR_SIZE;

    for (int i = 0; i < num_bytes_; i += span) {
      A = _mm_load_ps((float*)&bits_[i]);
      B = _mm_load_ps((float*)&rbits[i]);
      C = _mm_or_ps(A, B);
      _mm_store_ps((float*)&result[i], C);
    }

    return result;
  }

  // Return the number of bits (which are equal to 1)
  int Count() {
    // Return count
    int count = 0;

    // The SIMD vector
    __m128 A;

    // How many chars a SIMD vector contains
    int span = SIMD_SIZE / CHAR_SIZE;

    for (int i = 0; i < num_bytes_; i += span) {
      A = _mm_load_ps((float*)&bits_[i]);
      count += popcnt128((__m128i)A);
    }

    return count;
  }

  // Return the count for AND operation
  int CountAnd(Bitset& rh_bitset) {
    // Return count
    int count = 0;

    // The SIMD vector
    __m128 A, B, C;

    // The char* of right hand bitset
    char* rbits = rh_bitset.Get();

    // How many chars a SIMD vector contains
    // For example, 128-bit SIMD, and char is 8, so it processes 16 bytes each
    // time (span = 16)
    int span = SIMD_SIZE / CHAR_SIZE;

    for (int i = 0; i < num_bytes_; i += span) {
      A = _mm_load_ps((float*)&bits_[i]);
      B = _mm_load_ps((float*)&rbits[i]);
      C = _mm_and_ps(A, B);
      count += popcnt128((__m128i)C);
    }

    return count;
  }

  char* Get() { return bits_; }

 private:
  inline int popcnt128(__m128i n) {
    const __m128i n_hi = _mm_unpackhi_epi64(n, n);
#ifdef _MSC_VER
    return __popcnt64(_mm_cvtsi128_si64(n)) +
           __popcnt64(_mm_cvtsi128_si64(n_hi));
#else
    return __popcntq(_mm_cvtsi128_si64(n)) + __popcntq(_mm_cvtsi128_si64(n_hi));
#endif
  }

 private:
  // Bitset value
  char* bits_;

  // number of bits of this bitset
  int num_bits_;

  // number of bytes of this bitset
  // size_ = num_bits_/CHAR_SIZE + 1;
  int num_bytes_;
};
