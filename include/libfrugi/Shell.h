/*
 * Shell.h
 * 
 * Part of a general library.
 * 
 * @author Freark van der Berg
 */

#ifndef SHELL_H
#define SHELL_H

#include <unordered_map>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <string>
#include <fstream>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
//#include <sys/wait.h>
#include "MessageFormatter.h"
#include "FileSystem.h"
#include "System.h"
//#include "yaml-cpp/yaml.h"
#include <std_unordered_map.h>

class Shell {
public:
	
	static int handleSignal(int signal) {
		if(messageFormatter) {
			ConsoleWriter& cw = messageFormatter->getConsoleWriter();
			messageFormatter->getConsoleWriter().appendPostfix();
			stringstream ss;
			ss << "Caught signal `" << signal << "', ";
			cw << " " << ConsoleWriter::Color::CyanBright << ">" << ConsoleWriter::Color::WhiteBright << "  " << ss.str();
			while(true) {
				cw << "terminate " << ConsoleWriter::Color::MagentaBright << "j" << ConsoleWriter::Color::WhiteBright << "ob or " << ConsoleWriter::Color::MagentaBright << "p" << ConsoleWriter::Color::WhiteBright << "rogram?" << ConsoleWriter::Color::Reset;
				char answer[10];
				std::cin.getline(answer,9);
				if(!strncmp("j",answer,1)) {
					break;
				} else if(!strncmp("p",answer,1)) {
					exit(signal);
				} else {
					cw << " " << ConsoleWriter::Color::CyanBright << ">" << ConsoleWriter::Color::WhiteBright << "  ";
				}
			}
			cw.appendPostfix();
		}
		return signal;
	}
	
	typedef int (*SignalHandler)(int signal);
	
	class SystemOptions {
	public:
		std::string command;
		std::string cwd;
		std::string outFile;
		std::string errFile;
		std::string statFile;
		std::string statProgram;
		std::string reportFile;
		int verbosity;
		std::function<int(int)> signalHandler;
		
		SystemOptions():
			command(""),
			cwd("."),
			outFile(""),
			errFile(""),
			statFile(""),
			statProgram(""),
			reportFile(""),
			verbosity(0),
			signalHandler(&handleSignal) {
		}
	};
	
	class RunStatistics {
	public:
		float time_user;
		float time_system;
		float time_elapsed;
		float time_monraw;
		float mem_virtual;
		float mem_resident;
		
		RunStatistics():
			time_user(0.0f),
			time_system(0.0f),
			time_elapsed(0.0f),
			time_monraw(0.0f),
			mem_virtual(0.0f),
			mem_resident(0.0f) {
		}
		
		
		void addTimeMaxMem(const RunStatistics& other) {
			time_user    += other.time_user;
			time_system  += other.time_system;
			time_elapsed += other.time_elapsed;
			time_monraw  += other.time_monraw;
			mem_virtual   = mem_virtual  > other.mem_virtual  ? mem_virtual  : other.mem_virtual;
			mem_resident  = mem_resident > other.mem_resident ? mem_resident : other.mem_resident;
		}
		
		void maxMem(const RunStatistics& other) {
			mem_virtual   = mem_virtual  > other.mem_virtual  ? mem_virtual  : other.mem_virtual;
			mem_resident  = mem_resident > other.mem_resident ? mem_resident : other.mem_resident;
		}
	};

	class StatsProgramTime {
	public:
		static std::string buildCommand(std::string command, File statsFile) {
			return "time -o " + statsFile.getFileRealPath() + " " + command;
		}
		static bool readStats(File file, RunStatistics& stats) {
			return Shell::readTimeStatistics(file, stats);
		}
		static std::string getName() {
			return "time";
		}
	};

	class StatsProgramMemTime {
	public:
		static std::string buildCommand(std::string command, File statsFile) {
			return "(memtime " + command + ") 2> " + statsFile.getFileRealPath();
			//if(options.errFile.empty()) command += " 2> " + file.getFileRealPath();
		}
		static bool readStats(File file, RunStatistics& stats) {
			return Shell::readMemtimeStatisticsFromLog(file, stats);
		}
		static std::string getName() {
			return "memtime";
		}
	};

	class StatsProgram {
	public:
		virtual std::string buildCommand(std::string command, File file) = 0;
		virtual bool readStats(File file, RunStatistics& stats) = 0;
		virtual std::string getName() = 0;
	public:
		static std::unordered_map<std::string, StatsProgram*> statsBinaries;
		static void findStatsBinaries();
		static StatsProgram* findStatsBinary(std::string statsProgram) {
			findStatsBinaries();
			if(statsProgram.empty() && statsBinaries.size() > 0) {
				return statsBinaries.begin()->second;
			}
			std::cout << statsBinaries;
			assert(statsBinaries[statsProgram]);
			return statsBinaries[statsProgram];
		}
	private:
		template<typename S>
		static void findStatsBinary(std::string s) {
			auto it = statsBinaries.find(s);
			if(it == statsBinaries.end()) {
				File f;
				if(!FileSystem::findBinary(s, f)) {
					statsBinaries[s] = new StatsProgram_Impl<S>();
				}
			}
		}
	};

	template<typename StatProgramHandler>
	class StatsProgram_Impl: public StatsProgram {
	public:
		virtual std::string buildCommand(std::string command, File file) {
			return StatProgramHandler::buildCommand(command, file);
		}
		virtual bool readStats(File file, RunStatistics& stats) {
			return StatProgramHandler::readStats(file, stats);
		}
		virtual std::string getName() {
			return StatProgramHandler::getName();
		}
	};

	static MessageFormatter* messageFormatter;

	/**
	 * Gets the contents of the specified environment variable.
	 * @param var The contents will be written to this string.
	 * @return whether or not the environment variable exists.
	 */
	static bool getenv(std::string& var) {
		const char* varResult = ::getenv(var.c_str());
		if(varResult) {
			var = string(varResult);
		}
		return !varResult;
	}

    static std::string buildCommand(const SystemOptions& options, RunStatistics* stats = nullptr);

	/**
	 * Execute the specified command, in the specified working directory.
	 * Pipe stdout to the file specified by outFile and stderr to the file
	 * specified by errFile.
	 * Returns the return code of the command or some error code in case of an error.
	 * @param command The command to execute.
	 * @param cwd The directory in which the command will be executed.
	 * @param outFile The file to which stdout will be piped.
	 * @param errFile The file to which stderr will be piped.
	 * @return The return code of the command or some error code in case of an error.
	 */
	static int system(std::string command, std::string cwd=".", std::string outFile="", std::string errFile="", int verbosity=0, RunStatistics* stats = NULL);
	static int system(std::string command, int verbosity=0, RunStatistics* stats = NULL);
	static int system(const SystemOptions& options, RunStatistics* stats = NULL);
	
	static bool memtimeAvailable() {
		vector<File> memtimes;
		int n = FileSystem::findInPath(memtimes,File("memtime"));
		return n>0;
	}
	
	static bool readMemtimeStatisticsFromLog(File logFile, Shell::RunStatistics& stats);
	static bool readMemtimeStatistics(File file, RunStatistics& stats);
	static bool readMemtimeStatistics(const string& contents, RunStatistics& stats);
	static bool readMemtimeStatistics(const char* contents, RunStatistics& stats);

	static bool readTimeStatistics(File file, RunStatistics& stats) {
		string* contents = FileSystem::load(file);
		
		if(contents) {
			// Format example: 0.00 user, 0.00 system, 0.10 elapsed -- Max VSize = 4024KB, Max RSS = 76KB
			int result = sscanf(contents->c_str(),"\nreal %f\nuser %f\nsys %f",
				   &stats.time_user,
				   &stats.time_system,
				   &stats.time_elapsed
			);
			delete contents;
			if(result == 3) {
				stats.mem_virtual = 0;
				stats.mem_resident = 0;
				return false;
			} else {
				return true;
			}
		} else {
			return true;
		}
	}
};

class ArgParser {
public:
	ArgParser(int argc, char** argv)
	: _argc(argc)
	, _argv(argv)
	{
		for(int a = 0; a < argc; +=argv) {
			_arguments.emplace_back(argv);
		}
	}
private:
	std::vector<std::string> _arguments;
	int _argc;
	char** _argv;
};

#endif // SHELL_H
