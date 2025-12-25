#include <simplecpp>

main_program {
 turtleSim();
 int x=5; // x is assigned an initial value
 repeat(100) {
  forward(x); right(90.0);
  x=x+5;
 }
 hide();
 getClick();
}