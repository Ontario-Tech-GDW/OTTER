#include "Logging.h"
#include <sstream>

#include "spdlog/common.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/ansicolor_sink.h"

#ifdef WINDOWS
#include <Windows.h>
#include <DbgHelp.h>
#endif

std::shared_ptr<spdlog::logger> Logger::myLogger;
bool Logger::isInitialized = false;

void Logger::Init(const LoggerSettings& settings) {
	if (!isInitialized) {
		// Set our spd logging pattern
		spdlog::set_pattern("%^[%l] %n: %v%$");

		// Create a new color logger
		if (settings.OutputToConsole) {
			myLogger = spdlog::stdout_color_mt("APP", spdlog::color_mode::automatic);
		}
		if (settings.OutputToFile) {
			myLogger->sinks().emplace(
				myLogger->sinks().begin(),
				std::make_shared<spdlog::sinks::basic_file_sink<spdlog::details::null_mutex>>(
					settings.LogFileName.empty() ? "logs.txt" : settings.LogFileName)
			);
		}

		// Our log level is set to trace (the highest) by default
		myLogger->set_level(spdlog::level::trace);
		// The default color for trace is the same as info, so we get our color output
		auto console_sink = dynamic_cast<spdlog::sinks::stdout_color_sink_mt*>(myLogger->sinks().back().get());
		// and make trace cyan instead
		console_sink->set_color(spdlog::level::trace, console_sink->CYAN);

		#ifdef WINDOWS 
		// Get the process handle
		HANDLE process = GetCurrentProcess();

		// Initialize the symbol handler for our process
		SymInitialize(process, NULL, TRUE);
		SymSetOptions(SYMOPT_LOAD_LINES);
		#endif

		isInitialized = true;
	}
}

void Logger::Uninitialize()
{
	if (isInitialized) {
		#ifdef WINDOWS 
		HANDLE process = GetCurrentProcess();
		SymCleanup(process);
		#endif
		myLogger = nullptr;
		spdlog::shutdown();
	}
}

std::string Logger::DumpStackTrace()
{
	std::stringstream ss;

	// Windows implementation taken from http://www.rioki.org/2017/01/09/windows_stacktrace.html
#ifdef WINDOWS
	// We will always assume x64, since that is our only supported project architecture
	DWORD machine = IMAGE_FILE_MACHINE_AMD64;

	// Get the process handle
	HANDLE process = GetCurrentProcess();
	// Get the current thread handle
	HANDLE thread = GetCurrentThread();
	// Prep our context info
	CONTEXT context = {};
	context.ContextFlags = CONTEXT_FULL;
	// Capture our device context (registers and everything)
	RtlCaptureContext(&context);

	// Prepare our stack frame
	STACKFRAME frame = {};
	frame.AddrPC.Offset = context.Rip; // This is our instruction pointer in register RIP
	frame.AddrPC.Mode = AddrModeFlat;
	frame.AddrFrame.Offset = context.Rbp; // This is the address of our frame, stored in RBP
	frame.AddrFrame.Mode = AddrModeFlat;
	frame.AddrStack.Offset = context.Rsp; // This is the address of the stack, stored in RSP
	frame.AddrStack.Mode = AddrModeFlat;

	// Bypass this frame
	StackWalk(machine, process, thread, &frame, &context, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL);

	// Iterate over all frames in the stack
	while (StackWalk(machine, process, thread, &frame, &context, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL))
	{
		// The address of the function
		DWORD64 functionAddress;
		// Stores the output information
		std::string moduleName, functionName, file;
		// Stores the line
		unsigned int line = 0;

		// Extract the function address from the current frame
		functionAddress = frame.AddrPC.Offset;

		// Get the module that the address is part of
		DWORD64 moduleBase = SymGetModuleBase(process, frame.AddrPC.Offset);
		// Get the name of the module, store it in buffer
		char moduleBuff[MAX_PATH];
		if (moduleBase && GetModuleFileNameA(reinterpret_cast<HINSTANCE>(moduleBase), moduleBuff, MAX_PATH)) {
			moduleName = moduleBuff;
		}

		// Prepare a buffer to hold the symbol and it's name
		char symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 255];
		PIMAGEHLP_SYMBOL symbol = (PIMAGEHLP_SYMBOL)symbolBuffer;
		symbol->SizeOfStruct = (sizeof IMAGEHLP_SYMBOL) + 255;
		symbol->MaxNameLength = 254; // one byte for null terminator

		// Get the symbol info for the function address, and extract the name
		if (SymGetSymFromAddr(process, functionAddress, NULL, symbol)) {
			functionName = symbol->Name;
		}

		// Prepare a line info structure
		DWORD offset = 0;
		IMAGEHLP_LINE hLine;
		hLine.SizeOfStruct = sizeof(IMAGEHLP_LINE);

		// Get the file and line info for the function
		if (SymGetLineFromAddr(process, functionAddress, &offset, &hLine)) {
			file = hLine.FileName;
			line = hLine.LineNumber;
		}

		// Append to the output line
		ss << "\t" << functionName << "@" << file << " line " << line << std::endl;
	}
#endif

	// Return the result
	return ss.str();
}

