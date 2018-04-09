#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//�����1-100
int my_random()
{
	srand((unsigned)(GetCurrentProcessId() + time(NULL)));
	int t = rand() % 100 + 1;
	return t;
}

int main()
{
	SYSTEMTIME systime;
	
	//���ļ�ӳ����󣬳ɹ������ļ�ӳ������������򷵻�0
	HANDLE hMapping = OpenFileMapping(FILE_MAP_WRITE, FALSE, "buffer");
	//���ļ�����ӳ�䵽���̵�ַ�ռ䣬�ɹ������ļ�ӳ�����ڴ��е���ʼ��ַ�����򷵻�0
	LPVOID pfile = MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 0, 0);
	//�����ַָ�룬ָ���ڴ��׵�ַ
	int* pData = reinterpret_cast<int*>(pfile);
	//���ź������󣬳ɹ������ź��������������򷵻�FALSE
	HANDLE EMPTY_Handle = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Empty");
	HANDLE FULL_Handle = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Full");
	//�򿪻�������󣬳ɹ����ػ���������������򷵻�FALSE
	HANDLE MUTEX_Handle = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "Mutex");

	for (int i = 0; i < 4; i++)
	{
		//����ȴ�
		Sleep(1000);
		//�Ի������FULL�ź�������P����
		WaitForSingleObject(FULL_Handle, INFINITE);
		WaitForSingleObject(MUTEX_Handle, INFINITE);

		printf("Customer����");
		//�ó����ݵ�ʱ��
		GetLocalTime(&systime);
		printf("Using time: %02d:%02d:%02d  ", systime.wHour, systime.wMinute, systime.wSecond);
		//�ó�����
		int get = *(pData + 4);
		printf("gets: %d  ", *(pData + get * 4 + 8));
		*(pData + get * 4 + 8) = 0;
		if (++get > 2)get = 0;
		*(pData + 4) = get;
		printf("Buffer: %d %d %d\n", *(pData + 8), *(pData + 12), *(pData + 16));

		//�Ի������EMPTY�ź�������V����
		ReleaseSemaphore(EMPTY_Handle, 1, NULL);
		ReleaseMutex(MUTEX_Handle);
	}

	//�رվ��
	CloseHandle(hMapping);
	CloseHandle(EMPTY_Handle);
	CloseHandle(MUTEX_Handle);
	CloseHandle(FULL_Handle);
	return 0;
}