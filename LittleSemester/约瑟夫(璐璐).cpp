#include<stdio.h>
#include<malloc.h>
struct node
{
	int no;//���
	struct node*next;//ָ��
};
int main()
{
	int n, k, m;
	scanf("%d,%d,%d", &n, &k, &m);
	if (n < 1 || k < 1 || m < 1)
	{
		printf("n,m,k must bigger than 0.\n");
		return 0;
	}
	if (k > n)
	{
		printf("k should not bigger than n.\n");
		return 0;
	}
	struct node*head, *p, *q;
	head = (struct node *)malloc(sizeof(struct node));
	head->no = -1;//������ͷ��㣬���ΪnoΪ-1��
	head->next = head;//�γɻ��οձ�
	for (int i = n; i > 0; i--)//�γɰ���n���ڵ��ѭ������
	{
		p = (struct node*)malloc(sizeof(struct node));
		p->next = head->next;//�ڱ�ڵ�ĺ�������½ڵ�
		p->no = i;
		head->next = p;
	}
	while (p->next != head)//����ѭ����������һ���ڵ�
		p = p->next;
	p->next = head->next;//����ѭ��
						 //p = head->next;
	for (int i = 0; i < k - 1; i++)//�ƶ�����k����
		p = p->next;
	int i;
	for (i = 0; i < n; i++)//
	{
		int flag = 0;
		for (int col = 1; col < m; col++)
			p = p->next;
		q = p->next;
		p->next = q->next;
		if (i % 10 == 0 && i != 0)
		{
			printf("\n");
		
		}
		printf("%d", q->no);
		if (i != n - 1&& i%10!=9)printf("#");
		if (i == n - 1)printf(".\n");
		free(q);
	}
}