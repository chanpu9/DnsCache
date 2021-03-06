#include "pch.h"
#include <iostream>
#include <windows.h>
#include <WinDNS.h>
#include <string>
#include <vector>
using namespace std;

struct CachedDnsRecord
{
	wstring name;
	int type;
};

#define INET_ADDRSTRLEN     22
#define INET6_ADDRSTRLEN    65


typedef struct _DNS_CACHE_ENTRY
{
	struct _DNS_CACHE_ENTRY* pNext; // Pointer to next entry
	PWSTR pszName; // DNS Record Name
	unsigned short wType; // DNS Record Type
	unsigned short wDataLength; // Not referenced
	unsigned long dwFlags; // DNS Record Flags
} DNSCACHEENTRY, *PDNSCACHEENTRY;

typedef int(WINAPI *DNS_GET_CACHE_DATA_TABLE)(PDNSCACHEENTRY);

vector<CachedDnsRecord> getDnsCache()
{
	vector<CachedDnsRecord> results;

	PDNSCACHEENTRY pEntry = (PDNSCACHEENTRY)malloc(sizeof(DNSCACHEENTRY));
	HINSTANCE hLib = LoadLibrary(TEXT("DNSAPI.dll"));
	DNS_GET_CACHE_DATA_TABLE DnsGetCacheDataTable =
		(DNS_GET_CACHE_DATA_TABLE)GetProcAddress(hLib, "DnsGetCacheDataTable");

	int stat = DnsGetCacheDataTable(pEntry);
	pEntry = pEntry->pNext;
	while (pEntry)
	{
		CachedDnsRecord record;
		record.name = wstring(pEntry->pszName);
		record.type = pEntry->wType;
		results.push_back(record);
		pEntry = pEntry->pNext;
	}
	free(pEntry);

	return results;
}

int main()
{
	auto cache = getDnsCache();
	wprintf(L"[Type]: [Name]\n");

	for (auto record : cache)
	{
		switch (record.type)
		{
		case 1:
			wprintf(L"A: ");
			break;
		case 2:
			wprintf(L"NS: ");
			break;
		case 5:
			wprintf(L"CNAME: ");
			break;
		case 6:
			wprintf(L"SOA: ");
			break;
		case 12:
			wprintf(L"PTR: ");
			break;
		case 15:
			wprintf(L"MX: ");
			break;
		case 16:
			wprintf(L"TXT: ");
			break;
		case 28:
			wprintf(L"AAAA: ");
			break;
		case 33:
			wprintf(L"SRV: ");
			break;
		case 255:
			wprintf(L"*: ");
			break;
		default:
			wprintf(L"%d:", record.type);
			break;
		}
		wprintf(L"%s\n", record.name.c_str());
	}
	return 0;
}