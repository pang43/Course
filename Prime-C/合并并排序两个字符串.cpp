//�����Ӵ�С������ַ�����Ҫ�ϲ���һ�𣬲��һ����ִӴ�С��˳�� 
#include<stdio.h>
char *strsequ(char *str1,char *str2)//�ַ����ϲ������� 
{
	char *str3=str1;
	for(;*str1=='\0'&& *str2=='\0';)//�����ַ�����δ����β����һֱѭ���Ƚ� 
	{
		if(*str1>*str2)
			*str3++=*str2++;//��1�봮2�Ƚϣ���С�ĸ�����3 
		else
			*str3++=*str1++;
		if(*str1=='\0')//�ж������ַ����Ƿ��ֹ ����ֹ�������һ���ַ���ʣ�µĲ����� 
		{
			while(*str3++=*str2++);
		}
		if(*str2=='\0')
		{
			while(*str3++=*str1++);
		}
	}
	return str1;
}
int main()//�����������������ַ���������������ַ��� 
{
	char a[100],b[100];
	printf("Please enter the first string\n");
	scanf("%s",a);
	printf("Please enter the second string\n");
	scanf("%s",b);
	printf("The new striing is %s",strsequ(a,b));
}
