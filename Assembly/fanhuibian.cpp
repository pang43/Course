int main()
{
01011850  push        ebp  
01011851  mov         ebp,esp  
01011853  sub         esp,0E4h  
01011859  push        ebx  
0101185A  push        esi  
0101185B  push        edi  
0101185C  lea         edi,[ebp-0E4h]  
01011862  mov         ecx,39h  
01011867  mov         eax,0CCCCCCCCh  
0101186C  rep stos    dword ptr es:[edi]  
	int num = 0;
0101186E  mov         dword ptr [num],0  
	for (int i = 0; i < 5; i++)
01011875  mov         dword ptr [ebp-14h],0  
0101187C  jmp         main+37h (01011887h)  
0101187E  mov         eax,dword ptr [ebp-14h]  
01011881  add         eax,1  
01011884  mov         dword ptr [ebp-14h],eax  
01011887  cmp         dword ptr [ebp-14h],5  
0101188B  jge         main+62h (010118B2h)  
	{
		for (int j = 0; j < 5; j++)
0101188D  mov         dword ptr [ebp-20h],0  
01011894  jmp         main+4Fh (0101189Fh)  
01011896  mov         eax,dword ptr [ebp-20h]  
01011899  add         eax,1  
0101189C  mov         dword ptr [ebp-20h],eax  
0101189F  cmp         dword ptr [ebp-20h],5  
010118A3  jge         main+60h (010118B0h)  
		{
			num++;
010118A5  mov         eax,dword ptr [num]  
010118A8  add         eax,1  
010118AB  mov         dword ptr [num],eax  
		}
010118AE  jmp         main+46h (01011896h)  
	}
010118B0  jmp         main+2Eh (0101187Eh)  
	printf("%d\n", num);
010118B2  mov         eax,dword ptr [num]  
010118B5  push        eax  
010118B6  push        offset string "%d\n" (01017B30h)  
010118BB  call        _printf (0101133Eh)  
010118C0  add         esp,8  
}
010118C3  xor         eax,eax  
010118C5  pop         edi  
010118C6  pop         esi  
010118C7  pop         ebx  
010118C8  add         esp,0E4h  
010118CE  cmp         ebp,esp  
010118D0  call        __RTC_CheckEsp (01011127h)  
010118D5  mov         esp,ebp  
010118D7  pop         ebp  
010118D8  ret  