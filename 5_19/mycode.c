#include<stdio.h>
int sum(int b,int e)
{
  int ret=0;
  for(int i=b;i<=e;i++)
  {
    ret+=i;
  }
  return ret;
}

int main()
{
  printf("start:\n");
  printf("%d\nend",sum(1,2025));
  return 0;
}
