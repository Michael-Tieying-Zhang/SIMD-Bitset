#include "bitset.h"

int main() {
  // Create a bitset with 100 bits
  Bitset bitset1(100);
  Bitset bitset2(100);

  // Set bitset1
  bitset1.Set(3);
  bitset1.Set(8);
  bitset1.Set(14);
  bitset1.Set(35);
  bitset1.Set(87);

  // Set bitset2
  bitset2.Set(3);
  bitset2.Set(10);
  bitset2.Set(14);
  bitset2.Set(64);
  bitset2.Set(87);

  int count1 = bitset1.Count();
  int count2 = bitset2.Count();

  int and_count = bitset1.CountAnd(bitset2);

  std::cout << "count1: " << count1 << "; count2: " << count2 << std::endl;
  std::cout << "and_count: " << and_count << std::endl;

  return 0;
}
