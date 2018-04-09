#include<windows.h>
#include<iostream>
#include<stdio.h>
#include<string.h>

#define PROCESS_NUM 2
#define CUSTOMER_NUM 3

struct shareMemory
{
	int put;			//��¼�����ݵ�λ��
	int take;			//��¼ȡ���ݵ�λ��
	int number[3];		//���ݻ�����
};

//����һ�����̲�����ID
HANDLE StartClone(const char* nCloneID)
{
	char szCmdLine_P[100] = "C:\\Users\\yuheng\\Desktop\\Lab3\\Windows\\Producer\\Debug\\";
	char szCmdLine_C[100] = "C:\\Users\\yuheng\\Desktop\\Lab3\\Windows\\Customer\\Debug\\";

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi;

	BOOL bCreateOK;
	if (nCloneID == "Producer.exe")
	{
		strcat(szCmdLine_P, nCloneID);
		
		//�����ӽ���
		BOOL bCreateOK = CreateProcess(
			NULL,
			szCmdLine_P,
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			&si,
			&pi);
	}
	else
	{
		strcat(szCmdLine_C, nCloneID);

		//�����ӽ���
		bCreateOK = CreateProcess(
			NULL,
			szCmdLine_C,
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			&si,
			&pi);
	}

	if (!bCreateOK)
	{
		printf("Clone false\n");
		exit(-1);
	}

	return pi.hProcess;
}

//��������������
HANDLE MakeSharedFile()
{
	//�����ļ�ӳ�����
	HANDLE hMapping = CreateFileMapping(
		INVALID_HANDLE_VALUE,        //ʹ��ҳʽ��ʱ�ļ�
		NULL,						 //ȱʡ��ȫ��
		PAGE_READWRITE,				 //�ɶ�дȨ
		0,							 //�ļ����ߴ磨��32λ��
		sizeof(struct shareMemory),	 //�ļ����ߴ磨��32λ��
		"buffer");					 //��
									 
	//���ļ�ӳ���ϴ�����ͼ
	if (hMapping != INVALID_HANDLE_VALUE)
	{
		LPVOID pData = MapViewOfFile(
			hMapping,					//�ļ�ӳ�����Ķ���
			FILE_MAP_ALL_ACCESS,		//��ö�дȨ
			0,							//���ļ��Ŀ�ͷ������32λ����ʼ
			0,							//���ļ��Ŀ�ͷ������32λ��
			0);							//ӳ�������ļ�
		if (pData != NULL)				//�����ڴ�ռ䣬����0
		{
			ZeroMemory(pData, sizeof(struct shareMemory));
		}
		UnmapViewOfFile(pData);			//�����ļ�ӳ�����
	}
	return (hMapping);                  //���ؾ��
}


int main(int argc, char* argv[])
{
	printf("Mainprogram start:\n");

	//�����ӽ��̾����
	HANDLE ChildProcessHandel[5];

	//���������ļ�
	HANDLE hMapping = MakeSharedFile();

	//������������󣬿��ƻ�����ʻ����
	HANDLE Mutex = CreateMutex(
		NULL,			//ȱʡ��ȫ��
		FALSE,			//�����̲߳��ܻ�ȡ����������������
		"Mutex");

	//�����ź���
	HANDLE Empty = CreateSemaphore(
		NULL,			//ȱʡ��ȫ��
		3,				//�ź�����ʼΪ3
		3,				//�ź������Ϊ3
		"Empty");

	HANDLE Full = CreateSemaphore(
		NULL,
		0,
		3,
		"Full");

	//���������߽���
	for (int i = 0; i < PROCESS_NUM; i++)
	{
		HANDLE ProducerHandle = StartClone("Producer.exe");
		ChildProcessHandel[i] = ProducerHandle;
	}
	for (int i = PROCESS_NUM; i < (PROCESS_NUM + CUSTOMER_NUM); i++)
	{
		HANDLE CustomerHandle = StartClone("Customer.exe");
		ChildProcessHandel[i] = CustomerHandle;
	}

	//�ȴ��ӽ��̽���
	for (int i = 0; i < PROCESS_NUM + CUSTOMER_NUM; i++)
	{
		WaitForSingleObject(ChildProcessHandel[i], INFINITE);
		CloseHandle(ChildProcessHandel);
	}

	//�رվ��
	CloseHandle(hMapping);
	CloseHandle(Empty);
	CloseHandle(Full);
	CloseHandle(Mutex);

	printf("Mainprocess Done!\n");
	system("pause");

	return 0;

}