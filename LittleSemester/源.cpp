#include<stdio.h>
#include<malloc.h>
struct node
{
	int no;//���
	struct node*next;//ָ��
};
void main()
{
	int n, k, m;
	scanf("%d%d%d", &n, &k, &m);
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
	p = head->next;
	for(int i=1;i<k;i++)//�ƶ�����k����
		p = p->next;
	for (int i = 0; i < n; i++)//
	{
		for (int col = 1; col < m - 1; col++)
			p = p->next;
		q = p -> next;
		p->next = q->next;
		printf("%d", q->no);
		free(q);
	}
}