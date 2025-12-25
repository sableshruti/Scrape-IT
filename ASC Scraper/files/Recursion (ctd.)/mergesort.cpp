#include <iostream>
#include <cassert>
using std::cout; using std::cin; using std::cerr; using std::endl; 

void merge(const int X[], int Y[], int left, int right) {
 // merge X[left..mid] and X[mid+1..right] into Y
 int mid = (left+right)/2, lp = left, rp = mid+1;
 for(int i=left; i <= right; i++) {
  if(lp <= mid && (rp > right || X[lp] < X[rp]))
   Y[i] = X[lp++];
  else
   Y[i] = X[rp++];
 }
}

// assumes not called with left > right, and maintains that invariant
// in, out, scratch are arrays, all with left,...,right as valid indices.
void sort (const int in[], int out[], int left, int right, int scratch[]) {
 assert(left <= right);

 if (left==right) {
  out[left] = in[left];
  return;
 }
 // here left < right. then left <= mid and mid+1 <= right
 int mid = (left+right)/2;
 sort(in,scratch,left,mid,out);   // get sorted outputs in scratch
 sort(in,scratch,mid+1,right,out);
 merge(scratch,out,left,right);   // merge from scratch into out
}

int main() {
 const unsigned MAXN = 300;
 int A[MAXN], B[MAXN], C[MAXN];
 unsigned n = 0;
 do cin >> A[n]; while (A[n] > 0 && (++n)<MAXN);
 cerr << "Sorting " << n << " numbers" << endl;
 sort(A,B,0,n-1,C);
 for(int i=0; i<n; i++)
  cout << B[i] << " ";
 cout << endl;
}