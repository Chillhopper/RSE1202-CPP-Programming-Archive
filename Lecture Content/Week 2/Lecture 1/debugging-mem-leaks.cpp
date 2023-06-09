/*!****************************************************************************
\file    mem-leaks.cpp
\author  Prasanna Ghali
\par     DP email: pghali\@digipen.edu
\par     Course: HLP2
\par     Section: ALL
\par     Source file for illustrating debugging memory leaks using
         Valgrind.
\date    01-04-2020

\brief
   This is an exercise in trying to simulate many of the errors caused by
   pointers and dynamic memory allocation/deallocation.
   The really bad news is that most of these tests run without crashing.
   Therefore these errors may stay un-noticed till later time, usually when
   you are presenting your program to someone. 

   "...the results are undefined, and we all know what "undefined" 
    means: it means it works during development, it works during 
    testing, and it blows up in your most important customers' 
    faces." --Scott Meyers
    
    So, what is the answer -- memory debuggers.

  The source file contains a number of simulated tests with each test
  providing a short description. For each test there is a result of running
  the executable obtained with  the following compilers:
    g++ (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
    clang version 10.0.0-4ubuntu1
    valgrind-3.15.0 
    
  Compile and link with g++:
  g++ -std=c++17 -pedantic-errors -Wall -Wextra mem-leaks.cpp
  clang++: clang++ -std=c++17 -pedantic-errors -Wall -Wextra mem-leaks.cpp
  To test for memory problems with valgrind:
  valgrind --leak-check=full -s ./a.out integer-value-between-0-and-12
*******************************************************************************/

#include <iostream> // std::cout
#include <string>   // std::string
#include <array>    // std::array
#include <cstdlib>  // std::free

// memory allocation/deallocation and/or out-of-bound read/write tests
void test0(void);  // double deletion on plain-old data types
void test1(void);  // double deletion on user-defined data types
void test2(void);  // mixing new with free
void test3(void);  // read uninitialized object on free store
void test4(void);  // writing to object pointed to by uninitialized pointer
void test5(void);  // mixing up new[] with delete
void test6(void);  // mixing new[] with free for plain-old data types
void test7(void);  // mixing new[] with delete for user-defined data types
void test8(void);  // out-of-bounds read with static array
void test9(void);  // out-of-bounds write with static array
void test10(void); // out-of-bound read to free store
void test11(void); // out-of-bound write to free store
void test12(void); // memory leak - memory allocated but not deallocated.

int main(int argc, char *argv[] ) {
  if (argc < 2) {
    std::cout << "Usage: ./a.out value-between-0-and-12\n";
    return 0;
  }
  // convert C-style string supplied as command-line parameter to int
  int test = std::stoi(std::string{argv[1]});
  if (0 < test || test > 12) {
    std::cout << "Usage: ./a.out value-between-0-and-12\n";
    return 0;
  }

  // type ptr_func is an alias for pointer to function that takes and returns void
  using ptr_func = void (*)();
  // ptr_tests is an array of pointers to functions
  std::array<ptr_func, 13> ptr_tests {
    test0, test1, test2, test3,  test4,  test5, test6,
    test7, test8, test9, test10, test11, test12
  };

  try {
      ptr_tests[test](); // call function thro' pointer ptr_tests[test]
  } catch(const char* msg) {
    std::cerr << msg << std::endl;
  }
}

// double deletion on POD!!!
// POD means "plain old data" - usually used for built in types
// Programs generated by both g++ and clang++ abort execution!!!
void test0() {
  std::cout << "test0" << "\n";
  int *pi = new int; // object in memory is default-initialized
  *pi = 2;
  delete pi;
  std::cout << "Now deleting a pointer twice..." << "\n";
  delete pi; // bad stuff here!!!
  std::cout << "Did you notice?" << "\n";
}

class TestDoubleDelete {
  int *data;
public:
  TestDoubleDelete(int d) : data{new int(d)} { /* empty by design */ }
  ~TestDoubleDelete() { delete data; }
};

// double deletion on user-defined type
// Programs generated by both g++ and clang++ abort!!!
void test1() {
  std::cout << "test1" << "\n";
  TestDoubleDelete* p = new TestDoubleDelete(11);
  std::cout << "Now deleting a pointer ..." << "\n";
  delete p;
  std::cout << "Now deleting a pointer one more time ..." << "\n";
  delete p;
  std::cout << "Did you notice?" << "\n";
}

// mixing new/delete with malloc/free
// here, we're allocating with new but deleting with free!!!
// delete is actually a free plus destructor.
// calling free instead of delete skips the destructor, which doesn't
// cause an error, but results in a memory leak!!!
void test2() {
  std::cout << "test2" << "\n";
  int *pi = new int{2};
  std::cout << "Now freeing a pointer instead of deleting it..." << "\n";
  std::free(pi);
  std::cout << "Did you notice?" << "\n";
}

// reading allocated free store memory that is uninitialized!!!
// no memory leaks.
// the program doesn't crash but its behavior is undefined!!!
// obviously, this not legal C++ code.
// the C++ standard says that a program is not supposed to use
// uninitialized memory.
// this means that memory has to be: 1) allocated (done here)
// and 2) initialized (not done here)
void test3() {
  std::cout << "test3" << "\n";
  int *pi = new int; // *pi is default-initialized. for PODs, this means
                      // the free store memory will hold same value as
                      // previous occupant
  std::cout << "Now reading uninitialized memory" << "\n";
  int j = *pi+2;
  std::cout << "Did you notice? (value was " << j << ") probably garbage" 
            << "\n";
  delete pi;
}

// this is the worst possible thing done so far!!!
// here, we're trying to write to an object *pi that wasn't even allocated!!!
// what happens is that pi contains garbage, then this garbage is treated
// as an address, and we attempt to write to that address.
// as expected, behavior is undefined - the program may run, may crash,
// who knows.
// the good thing is that both compilers generate warning messages.
// the bad news is that you've to use -Wall option.
void test4() {
  std::cout << "test4" << "\n";
  int *pi;
  std::cout << "Now writing to uninitialized pointer (un-allocated memory) pi = "
            << pi << "\n";
  *pi = 100;
  std::cout << "Did you notice?" << "\n";
}

// mixing up new[] with delete
// both programs run.
// g++ and clang++ provide appropriate warnings with -Wall option.
// delete[] is actually quite complicated. It calls a destructor on each
// element of the array, and than frees the array itself.  Thus, calling
// delete instead of delete [] doesn't produce an error, but it skips all
// destructors, which may be a memory leak (if the objects in the array had
// dynamically allocated memory as with objects of type std::string).
// this particular example doesn't have a leak, since there is no dynamically 
// allocated memory inside int objects. But it's still illegal code.
void test5() {
  std::cout << "test5" << "\n";
  int *pi = new int[10];
  std::cout << "Let's delete instead of delete [] " << "\n";
  delete pi;
  std::cout << "Did you notice?" << "\n";
}

class CTest1 {
  int *pi;
public:
  CTest1() : pi{new int {2}} { /* empty by design */ }
  ~CTest1() { std::cout << "In ~CTest1()" << "\n"; delete pi; }
};

// mixing new[] with free!!!
// programs generated by g++ and clang++ both abort!!!
// this is a continuation of test5.
// there is definitely a leak. Why?
// No destructor is called, so all 10 dynamically allocated ints
// are still on the free store.
void test6() {
  std::cout << "test6" << "\n";
  CTest1 *p = new CTest1[10];
  std::cout << "Let's free instead of delete [] " << "\n";
  free(p);
  std::cout << "Did you notice?" << "\n";
}

class CTest2 {
  int i;
public:
  CTest2() : i{2} {}
  ~CTest2() { std::cout << "In ~CTest2()" << "\n"; }
};

// mixing new[] with delete!!!
// this is a continuation of test5.
// programs generated by both g++ and clang++ abort!!!
// Again, there MAY be run-time crashes, and definitely there is a leak. 
// What happens? when delete is called, it calls a SINGLE destructor on the 
// pointer p, and then calls free.
// to prove that only one destructor is called, change delete to delete[]
// and you'll see 10 destructors.
void test7() {
  std::cout << "test7" << "\n";
  CTest2 *p = new CTest2[10];
  std::cout << "Let's delete instead of delete [] " << "\n";
  delete p;
  std::cout << "Did you notice?" << "\n";
}

// no memory allocation/deallocation problems.
// here, we're making an out-of-bounds read!!!
// this is the most dangerous!!!
// g++ doesn't flag this out-of-bounds read, while clang++ does.
// valgrind doesn't catch this!!!
void test8() {
  std::cout << "test8" << "\n";
  int ar[10] = {0};
  std::cout << "Let's read out of bounds " << "\n";
  std::cout << "ar[10] = " << ar[10] << "\n";
  std::cout << "Did you notice?" << "\n";
}

// no memory allocation/deallocation problems.
// this is continuation of test8
// here, we're making an out-of-bounds write!!!
// this is the most dangerous!!!
// g++ crashes with this report: *** stack smashing detected ***: terminated 
// clang++ flags out-of-bounds read!!!
// valgrind doesn't catch the out-of-bounds read!!!
void test9() {
  std::cout << "test9" << "\n";
  int ar[10] = {0};
  std::cout << "Let's write out of bounds ";
  ar[10] = 7;
  std::cout << "ar[10]: " << ar[10] << "\n";
  std::cout << "Did you notice?" << "\n";
}

// no memory leaks.
// but, out-of-bound read to free store
// neither g++ nor clang++ flags this out-of-bounds read.
// valgrind flags this error!!!
void test10() {
  std::cout << "test10" << "\n";
  int *ar = new int[3]{0}; // initialize all elements to zero
  std::cout << "Let's read out of bounds " << "\n";
  std::cout << "ar[3] = " << ar[3] << "\n";
  std::cout << "Did you notice?" << "\n";
  delete [] ar;
}

// no memory leaks.
// but, out-of-bounds write to free store
// neither g++ nor clang++ flags this out-of-bounds read.
// valgrind flags this error!!!
void test11() {
  std::cout << "test11" << "\n";
  int *ar = new int[3];
  std::cout << "Let's write out of bounds " << "\n";
  ar[3] = 7;
  std::cout << "Did you notice?" << "\n";
  delete [] ar;
}

// memory leak - memory allocated but not deallocated.
// neither g++ nor clang++ flag this memory leak.
// valgrind comes to the rescue by reporting the memory leak!!!
void test12() {
  std::cout << "test12" << "\n";
  int *pi = new int;
  std::cout << "Let's leak a pointer to int" << "\n";
  *pi = 303;
}
