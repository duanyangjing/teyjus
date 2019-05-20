sig arith.
#lib arith.

regcl fact, add, sine.

extern type fact fact_wrapper int -> int -> o.
extern type add  add_wrapper  int -> int -> int -> o.
extern type sine sine_wrapper real -> real -> o.
