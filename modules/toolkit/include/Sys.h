#pragma once
#include <cstdint>

class System
{
public:
	static size_t GetMemoryUsageBytes();
	inline static double GetMemoryUsageKB() { return GetMemoryUsageBytes() / 1024.0; }
	inline static double GetMemoryUsageMB()  { return GetMemoryUsageKB() / 1024.0; }
	inline static double GetMemoryUsageGB() { return GetMemoryUsageMB() / 1024.0; }
	
	static size_t GetPageUsageBytes();
	inline static double GetPageUsageKB() { return GetPageUsageBytes() / 1024.0; }
	inline static double GetPageUsageMB() { return GetPageUsageKB() / 1024.0; }
	inline static double GetPageUsageGB() { return GetPageUsageMB() / 1024.0; }

	static size_t GetPeakMemoryUsageBytes();
	inline static double GetPeakMemoryUsageKB() { return GetPeakMemoryUsageBytes() / 1024.0; }
	inline static double GetPeakMemoryUsageMB() { return GetPeakMemoryUsageKB() / 1024.0; }
	inline static double GetPeakMemoryUsageGB() { return GetPeakMemoryUsageMB() / 1024.0; }

	static double GetCpuUsage();
		
private:
	static void __Init();
	
	static unsigned long long lastCPU, lastSysCPU, lastUserCPU;
	static int      numProcessors;
	static void*    self;
};
