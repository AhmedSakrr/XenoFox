#pragma once

#include "includes.h"

// structure of attached processes
struct tAttached
{
	char name[32];
	HANDLE handle;
	DWORD id;
	DWORD base;
};

class CProcess
{
public:

	// Collects all running x86 processes containing the information as PROCESSENTRY32W
	std::vector<PROCESSENTRY32W> walk_processes();

	// Opens handle safely to a process.
	bool attach(DWORD pID, DWORD access_mask, const char *name OPTIONAL, bool queryinfo = true);

	// Tries to find the base address of the process
	bool find_base(HANDLE process, tAttached *data OUT);

	// Determine whether the desired process runs on wow64 or not
	bool is_wow64(HANDLE process);

	// Get remote PEB of the process
	void * get_remote_peb(HANDLE process);

	// Get attached process by name identifier
	tAttached get_attached_process(const char *name);

	// Free attached process by name identifier
	void release(const char * name);

	// Free all process handles
	void release();

private:

	// List of all attached processes
	std::vector<tAttached> attached_processes;
};

extern CProcess process;