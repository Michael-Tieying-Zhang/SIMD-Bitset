#include "bitset.h"

/*g++ -std=c++0x test.cpp*/

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

  // bitset1.Count(63);

  int count1 = bitset1.Count();
  int count2 = bitset2.Count();

  int and_count = bitset1.CountAnd(bitset2);

  std::cout << "count1: " << count1 << "; count2: " << count2 << std::endl;
  std::cout << "and_count: " << and_count << std::endl;

  // Create a bitset with 100 bits
  Bitset bitset3;
  Bitset bitset4;

  // Set bitset3
  std::vector<int> wid3;
  wid3.push_back(2);
  wid3.push_back(2);
  wid3.push_back(2);
  wid3.push_back(2);
  wid3.push_back(2);
  wid3.push_back(2);
  wid3.push_back(2);

  bitset3.Resize(3);
  bitset3.Set(wid3);

  // Set bitset
  std::vector<int> wid4;
  wid4.push_back(2);

  bitset4.Resize(3);
  bitset4.Set(wid4);

  count1 = bitset3.Count();
  count2 = bitset4.Count();

  and_count = bitset3.CountAnd(bitset4);

  std::cout << "count3: " << count1 << "; count4: " << count2 << std::endl;
  std::cout << "and_count34: " << and_count << std::endl;

  return 0;
}
