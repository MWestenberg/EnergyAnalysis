int main(int argc, const char * argv[]) {
    int a[] = { 1 , 2 , 3 , 4  };
    int s = sizeof a / sizeof *a;
    return s;

}