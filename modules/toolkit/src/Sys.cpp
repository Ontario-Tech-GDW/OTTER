#include "Sys.h"

#ifdef WINDOWS
#include "windows.h"
#include "psapi.h"
#endif

size_t System::GetMemoryUsageBytes() {
	#ifdef WINDOWS
	static PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	return pmc.WorkingSetSize;
	#endif
}

size_t System::GetPageUsageBytes()
{
	#ifdef WINDOWS
	static PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	return pmc.QuotaPagedPoolUsage;
	#endif
}

size_t System::GetPeakMemoryUsageBytes() {
	#ifdef WINDOWS
	static PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	return pmc.PeakWorkingSetSize;
	#endif
}

double System::GetCpuUsage()
{
	__Init();
	#ifdef WINDOWS
	FILETIME ftime, fsys, fuser;
	unsigned __int64 now, sys, user;
	double percent;
	
	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));

	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));
	percent = static_cast<double>(sys - lastSysCPU) + (user - lastUserCPU);
	percent /= (now - lastCPU);
	percent /= numProcessors;
	lastCPU = now;
	lastUserCPU = user;
	lastSysCPU = sys;

	return percent * 100.0;
	#endif
}

void System::__Init() {
	static bool isInit = false;
	if (!isInit) {
		SYSTEM_INFO sysInfo;
		FILETIME ftime, fsys, fuser;

		GetSystemInfo(&sysInfo);
		numProcessors = sysInfo.dwNumberOfProcessors;

		GetSystemTimeAsFileTime(&ftime);
		memcpy(&lastCPU, &ftime, sizeof(FILETIME));

		self = GetCurrentProcess();
		GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
		memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
		memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));

		isInit = true;
	}
}

int System::numProcessors;
void* System::self;
unsigned long long System::lastUserCPU;
unsigned long long System::lastSysCPU;
unsigned long long System::lastCPU;
