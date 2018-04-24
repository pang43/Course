#include<Windows.h>
#include<iostream>
#include<stdio.h>
#include<Shlwapi.h>
#include<string.h>
#include<stdlib.h>
#include<TlHelp32.h>		//����ToolHelp API������PROCESSENTRY32�ṹ��
#include<iomanip>
/*
iomanip����������
setbase(int n)������ת��Ϊn����
setw(n) Ԥ�趨���
*/

//���Ӷ�̬���ӿ�
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Kernel32.lib")

using namespace std;

void GetMemoryInfo()
{
	SYSTEM_INFO si;
	ZeroMemory(&si, sizeof(si));
	GetSystemInfo(&si);

	//setw(20)�������ÿ�ȣ�����
	cout << setw(20) << "����������: " << si.dwActiveProcessorMask << endl
		<< setw(20) << "����������: " << si.dwNumberOfProcessors << endl
		<< setw(20) << "��������ҳ��С: " << si.dwPageSize << endl
		<< setw(20) << "����������: " << si.dwProcessorType << endl
		<< setw(20) << "���Ѱַ��Ԫ: " << si.lpMaximumApplicationAddress << endl
		<< setw(20) << "��СѰַ��Ԫ: " << si.lpMinimumApplicationAddress << endl
		<< setw(20) << "�������ȼ�: " << si.wProcessorLevel << endl
		<< setw(20) << "�������汾: " << si.wProcessorRevision << endl;
}

void GetMemoryUsing()
{
	MEMORYSTATUSEX stat;
	stat.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&stat);
	long int DIV = 1024 * 1024;

	cout << setw(20) << "�ڴ�ռ��:" << stat.dwMemoryLoad << "%" << endl
		<< setw(20) << "�������ڴ�:" << stat.ullTotalPhys / DIV << "MB" << endl
		<< setw(20) << "���������ڴ�:" << stat.ullAvailPhys / DIV << "MB" << endl
		<< setw(20) << "��ҳ�ļ�����:" << stat.ullTotalPageFile / DIV << "MB" << endl
		<< setw(20) << "���з�ҳ�ļ���:" << stat.ullAvailPageFile / DIV << "MB" << endl
		<< setw(20) << "�����ڴ�����:" << stat.ullTotalVirtual / DIV << "MB" << endl
		<< setw(20) << "���������ڴ�����:" << stat.ullAvailVirtual / DIV << "MB" << endl;
}

//��ʾ��ǰ��ҳ�汣����ʽ
void ShowProtection(DWORD dwTarget)
{
	printf(",");
	if (dwTarget == PAGE_READONLY)printf("PAGE_READONLY");
	if (dwTarget == PAGE_READWRITE)printf("PAGE_READWRITE");
	if (dwTarget == PAGE_WRITECOPY)printf("PAGE_WRITECOPY");
	if (dwTarget == PAGE_EXECUTE)printf("PAGE_EXECUTE");
	if (dwTarget == PAGE_EXECUTE_READ)printf("PAGE_EXECUTE_READ");
	if (dwTarget == PAGE_EXECUTE_READWRITE)printf("PAGE_EXECUTE_READWRITE");
	if (dwTarget == PAGE_EXECUTE_WRITECOPY)printf("PAGE_EXECUTE_WRITECOPY");
	if (dwTarget == PAGE_GUARD)printf("PAGE_GUARD");
	if (dwTarget == PAGE_NOACCESS)printf("PAGE_NOACCESS");
	if (dwTarget == PAGE_NOCACHE)printf("PAGE_NOCACHE");
}

//��õ����ڴ��ʹ�����
void LookVM(HANDLE hProcess)
{
	SYSTEM_INFO si;
	ZeroMemory(&si, sizeof(si));
	GetSystemInfo(&si);				//���ϵͳ��Ϣ

	LPCVOID start = (LPVOID)si.lpMinimumApplicationAddress;
	LPCVOID end = (LPCVOID)si.lpMaximumApplicationAddress;

	MEMORY_BASIC_INFORMATION mbi;	//���������ڴ�ռ�û�����Ϣ�ṹ
	ZeroMemory(&mbi, sizeof(mbi));	//���任��ȥ�����ڱ�����Ϣ
									/*typedef struct MEMORY_BASIC_INFORMATION {
									PVOID BaseAddress;       // �������ַ��
									PVOID AllocationBase;    // �������ַ��
									DWORD AllocationProtect; // ���򱻳��α���ʱ����ı������ԡ�
									SIZE_T RegionSize;       // �����С�����ֽ�Ϊ������λ����
									DWORD State;             // ״̬��MEM_FREE��MEM_RESERVE�� MEM_COMMIT����
									DWORD Protect;           // �������ԡ�
									DWORD Type;              // ���͡�
									} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;*/

	//ѭ�������������ó���õ�ַ�ռ�
	while (start < end)
	{
		//�����һ�������ڴ�����Ϣ
		if (VirtualQueryEx(hProcess, start, &mbi, sizeof(mbi)) == sizeof(mbi))
		{
			//����ÿ����ý�β������
			LPCVOID pEnd = (PBYTE)start + mbi.RegionSize;

			//��ʾ���ַ�ͳ���
			char szSize[MAX_PATH];
			StrFormatByteSizeA(mbi.RegionSize, (PSTR)szSize, MAX_PATH);
			printf("���ַ��%08x-%08x ��%s,", (DWORD)start, (DWORD)pEnd, szSize);
			
			//��ʾ���״̬
			if (mbi.State == MEM_COMMIT)printf("COMMIT");
			if (mbi.State == MEM_FREE)printf("FREE");
			if (mbi.State == MEM_RESERVE)printf("RESERVE");

			//��ʾ���� 
			if (mbi.Protect == 0 && mbi.State != MEM_FREE) {
				mbi.Protect = PAGE_READONLY;
			}
			ShowProtection(mbi.Protect);

			//��ʾ���� �ڽ�ҳ������洢������ָ�����������ַ����ҳ����ͬ�Ĵ洢������
			if (mbi.Type == MEM_IMAGE)printf(",Image");
			if (mbi.Type == MEM_MAPPED)printf(",Mapped");
			if (mbi.Type == MEM_PRIVATE)printf(",Private");

			printf("\n");
			//�ƶ���ָ���Ի����һ���� 
			start = pEnd;
		}
	}

}

void SearchProcessID()
{
	int id = 0;
	cout << "��ѯ����PID:" << endl;
	cin >> id;

	//�򿪽��̶���
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, id);
	LookVM(hProcess);
}

void GetProcessInfo()
{
	PROCESSENTRY32 pro;//���������Ϣ�ṹ
	pro.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hProc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);    //��ȡ�����б����һ��������0,����ϵͳ��ǰ���̿���

	BOOL flag = Process32First(hProc, &pro);//�ӵ�һ�����̿�ʼ
	//�������̿���
	while (flag)
	{
		printf("�������ƣ�%35ls\t", pro.szExeFile);//��ţ�������
		printf("PID : %d\t", pro.th32ProcessID);//PID
		printf("�̸߳��� : %d\n", pro.cntThreads);//�˽��̿������߳���
		flag = Process32Next(hProc, &pro);
	}

	SearchProcessID();
	CloseHandle(hProc);
}

int main()
{
	char temp;
	cout << "�ڴ����" << endl;

	while (1)
	{
		cout << "��ѡ����:" << endl;
		cout << "1 - �鿴�ڴ�����" << endl;
		cout << "2 - �鿴�ڴ�ʹ��" << endl;
		cout << "3 - �鿴��ǰ����" << endl;
		cout << "0 - �˳�" << endl;

		cin >> temp;
		switch (temp)
		{
		case'1':
			GetMemoryInfo();
			break;
		case'2':
			GetMemoryUsing();
			break;
		case'3':
			GetProcessInfo();
			break;
		case'0':
			cout << "ByeBye" << endl;
			return 0;
		default:
			cout << "�Ƿ�����" << endl;
			break;
		}
	}
	return 0;
}