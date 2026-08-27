#include <stdio.h>
int main()
{
	int a=0;
	int b=0;
	scanf("%d %d",&a,&b);
	int m=(a>b?a:b);
	printf("%d",m);
	return 0; 
}
