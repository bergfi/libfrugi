/*
 * System.h
 * 
 * Part of a general library.
 * 
 * @author Freark van der Berg
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#define SYSTEM_TIMER_BACKEND_MONOTONIC     1
#define SYSTEM_TIMER_BACKEND_MONOTONIC_RAW 2
#define SYSTEM_TIMER_BACKEND_WINDOWS       3

#include <cassert>
#include <ctime>
#include <string>
#include <random>
#include <vector>

#include <libfrugi/FileSystem.h>

class System {
public:
	class Timer {
	private:
	#if WIN32
		LARGE_INTEGER freq;
		LARGE_INTEGER start;
	#else
		timespec start;
	#endif
	public:
		Timer();
		Timer* create();
		void reset();
		double getElapsedSeconds();
	};

	static void init(int argc, char* argv[]);

	static void sleep(uint64_t ms);
	
	static void generateUUID(size_t bytes,std::string& uuid);
	
	static uint64_t getCurrentTimeMillis();
	static uint64_t getCurrentTimeMicros();

	static std::string getBinaryLocation() {
		if(!cwdAtStart.empty()) return cwdAtStart;
		assert(0);
	}

	static std::string const& getArgument(size_t i);
	static std::vector<std::string> const& getArguments();

private:
	static std::string cwdAtStart;
	static std::vector<std::string> args;
};

#endif
