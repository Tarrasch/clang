// RUN: %clang_cc1 -fsyntax-only -fcxx-exceptions -verify -std=c++11 %s

int align_illegal alignas(3); //expected-error {{requested alignment is not a power of 2}}
char align_big alignas(int);
int align_small alignas(1); // FIXME: this should be rejected
int align_multiple alignas(1) alignas(8) alignas(1);
alignas(4) int align_before;

struct align_member {
  int member alignas(8);
  int bitfield alignas(1) : 1; // expected-error {{}}
};

void f(alignas(1) char c) { // expected-error {{'alignas' attribute cannot be applied to a function parameter}}
  alignas(1) register char k; // expected-error {{'alignas' attribute cannot be applied to a variable with 'register' storage class}}
  try {
  } catch (alignas(4) int n) { // expected-error {{'alignas' attribute cannot be applied to a 'catch' variable}}
  }
}


template <unsigned A> struct alignas(A) align_class_template {};

// FIXME: these should not error
template <typename... T> alignas(T...) struct align_class_temp_pack_type {}; // expected-error{{pack expansions in alignment specifiers are not supported yet}}
template <unsigned... A> alignas(A...) struct align_class_temp_pack_expr {}; // expected-error{{pack expansions in alignment specifiers are not supported yet}}

typedef char align_typedef alignas(8); // expected-error {{'alignas' attribute only applies to variables, functions and tag types}}
template<typename T> using align_alias_template = align_typedef alignas(8); // expected-error {{'alignas' attribute cannot be applied to types}}

static_assert(alignof(align_big) == alignof(int), "k's alignment is wrong"); // expected-warning{{'alignof' applied to an expression is a GNU extension}}
static_assert(alignof(align_small) == 1, "j's alignment is wrong"); // expected-warning{{'alignof' applied to an expression is a GNU extension}}
static_assert(alignof(align_multiple) == 8, "l's alignment is wrong"); // expected-warning{{'alignof' applied to an expression is a GNU extension}}
static_assert(alignof(align_member) == 8, "quuux's alignment is wrong");
static_assert(sizeof(align_member) == 8, "quuux's size is wrong");
static_assert(alignof(align_class_template<8>) == 8, "template's alignment is wrong");
static_assert(alignof(align_class_template<16>) == 16, "template's alignment is wrong");
// FIXME: enable these tests
// static_assert(alignof(align_class_temp_pack_type<short, int, long>) == alignof(long), "template's alignment is wrong");
// static_assert(alignof(align_class_temp_pack_expr<8, 16, 32>) == 32, "template's alignment is wrong");
