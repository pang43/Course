#include<stdio.h>
int main()
{
	int i,x,count=0,sum=0;
	for(i=1;i<=10;i++)
	{
		scanf("%d",&x);
		if(x<=0)
		{
			continue;
		}
		sum+=x;
		count++;
	}
	if(count==0)
	{
		printf("����ô��û����"); 
	}
	else
	{
		printf("%d\n",sum/count);
	}
}
