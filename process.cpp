#include "process.h"

std::vector<PROCESSENTRY32W> CProcess::walk_processes()
{
	if (this->attached_processes.size() != 0)
		this->release();

	std::vector<PROCESSENTRY32W> proc_list;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	PROCESSENTRY32W PE32W = { 0 };
	PE32W.dwSize = sizeof(PROCESSENTRY32W);

	bool ignoreCheck = os.GetArchitecture() == OS_X86;

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		if (Process32FirstW(hSnap, &PE32W))
		{
			do
			{
				if (PE32W.th32ProcessID == GetCurrentProcessId())
					continue;

				if (!ignoreCheck)
				{
					// we're an x64 environment. we gotta check processes whether they're running on wow64 or not.
					if (attach(PE32W.th32ProcessID, PROCESS_QUERY_INFORMATION, "__arch", false))
					{
						tAttached proc = get_attached_process("__arch");

						if (proc.handle != 0)
						{
							if (!this->is_wow64(proc.handle))
							{
								this->release("__arch");
								continue;
							}
						}

						this->release("__arch");
					}
					else
						continue;
				}

				proc_list.push_back(PE32W);

			} while (Process32NextW(hSnap, &PE32W));
		}

		CloseHandle(hSnap);
	}

	return proc_list;
}

bool CProcess::attach(DWORD pID, DWORD access_mask, const char *name OPTIONAL, bool queryinfo)
{
	struct tAttached proc = { 0 };

	proc.handle = OpenProcess(access_mask, 0, pID);

	if (proc.handle == NULL)
		return false;

	proc.id = pID;

	if (queryinfo)
	{
		if (!find_base(proc.handle, &proc))
		{
			CloseHandle(proc.handle);
			return false;
		}
	}

	if (name != nullptr)
		memcpy(proc.name, name, 32);

	this->attached_processes.push_back(proc);

	return true;
}

bool CProcess::find_base(HANDLE process, tAttached *data OUT)
{
	DWORD base = NULL;

	void * remote_peb = get_remote_peb(process);
	
	if (remote_peb == nullptr)
		return false;

	if (!ReadProcessMemory(process, (LPCVOID)((DWORD)remote_peb + 0x008), (LPVOID)&base, sizeof(DWORD), NULL))
		return false;

	data->base = base;

	return true;
}

bool CProcess::is_wow64(HANDLE process)
{
	BOOL fIs64;

	if (!IsWow64Process(process, &fIs64))
		return true;

	return fIs64;
} 

void * CProcess::get_remote_peb(HANDLE process)
{
	PROCESS_BASIC_INFORMATION pbi = { 0 };

	if (NT_SUCCESS(ntapi::pNtQueryInformationProcess(process, ProcessBasicInformation, &pbi, sizeof(PROCESS_BASIC_INFORMATION), NULL)))
	{
		return (void*)pbi.PebBaseAddress;
	}

	return nullptr;
}

tAttached CProcess::get_attached_process(const char * name)
{
	for (auto &x : this->attached_processes)
	{
		if (!lstrcmpiA(x.name, name))
			return x;
	}

	tAttached dummy = { 0 };
	return dummy;
}

void CProcess::release(const char * name)
{
	std::vector<tAttached> tmp;

	int i = 0;
	for (auto &x : this->attached_processes)
	{
		if (!lstrcmpiA(x.name, name))
		{
			CloseHandle(x.handle);
			continue;
		}

		tmp.push_back(x);

		i++;
	}

	this->attached_processes = std::move(tmp);
}

void CProcess::release()
{
	for (auto &x : this->attached_processes)
	{
		CloseHandle(x.handle);
	}

	this->attached_processes.clear();
}

std::vector<tAttached> CProcess::get_attached_processes() const
{
	return this->attached_processes;
}

