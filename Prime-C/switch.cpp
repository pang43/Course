#include<stdio.h>
#include<math.h>
int main()
{
	float a,b;
	char op;
	scanf("%f %f %c",&a,&b,&op);
	switch(op)
	{
		case'+':
		printf("%.2f+%.2f=%.2f\n",a,b,a+b);break;
		case'-':
		printf("%.2f-%.2f=%.2f\n",a,b,a-b);break;
		case'*':
		printf("%.2f*%.2f=%.2f\n",a,b,a*b);break;
		case'/':
			if(b==0)
			printf("���ǹ��㣬�ⶼ���\n");
			else
			printf("%.2f/%.2f=%.2f\n",a,b,a/b);break;
		default:
			printf("�ȱ�ɶ���ٱƱ�����\n");break;
	}
 } 
