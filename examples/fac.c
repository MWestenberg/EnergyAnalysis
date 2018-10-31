int fac(int n) {
	if (n == 0) return 1;
	else return n * fac(n-1);
}

int main( int argc, const char* argv[] ) {
  return fac(3); 
}
