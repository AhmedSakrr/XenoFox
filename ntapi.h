#pragma once

#include "includes.h"

namespace ntapi
{
	inline NTSTATUS NTAPI pNtQueryInformationProcess(HANDLE ProcessHandle, PROCESSINFOCLASS ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength)
	{
		void * addr = GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");

		typedef NTSTATUS (NTAPI *mNtQueryInformationProcess)(
			IN HANDLE               ProcessHandle,
			IN PROCESSINFOCLASS     ProcessInformationClass,
			OUT PVOID               ProcessInformation,
			IN ULONG                ProcessInformationLength,
			OUT PULONG ReturnLength OPTIONAL
		);

		assert(addr, "NtQueryInformationProcess");

		mNtQueryInformationProcess call = (mNtQueryInformationProcess)addr;

		return call(ProcessHandle, ProcessInformationClass, ProcessInformation, ProcessInformationLength, ReturnLength);
	}
}