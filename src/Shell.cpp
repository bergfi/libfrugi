/*
 * Shell.cpp
 * 
 * Part of a general library.
 * 
 * @author Freark van der Berg
 */

#include "libfrugi/Shell.h"

class statsBinaries;
MessageFormatter* Shell::messageFormatter = NULL;
std::unordered_map<std::string, Shell::StatsProgram*> Shell::StatsProgram::statsBinaries;


int Shell::system(std::string command, std::string cwd, std::string outFile, std::string errFile, int verbosity, RunStatistics* stats) {
	SystemOptions sysOps;
	sysOps.command = command;
	sysOps.cwd = cwd;
	sysOps.outFile = outFile;
	sysOps.errFile = errFile;
	sysOps.verbosity = verbosity;
	return system(sysOps,stats);
}

int Shell::system(std::string command, int verbosity, RunStatistics* stats) {
	return system(command,".","","",verbosity,stats);
}

void Shell::StatsProgram::findStatsBinaries() {
	findStatsBinary<StatsProgramTime>("time");
	findStatsBinary<StatsProgramMemTime>("memtime");
}

std::string Shell::buildCommand(const SystemOptions& options, RunStatistics* stats) {

	std::string command = options.command;

	// Pipe stdout to /dev/null if no outFile is specified
	command += " > ";
	if(options.outFile.empty()) {
#ifdef WIN32
		command += "NUL";
#else
		command += "/dev/null";
#endif

	// If there is an outFile specified, pipe stdout to it
	} else {
		command += options.outFile;
	}
	command += " 2> ";

	// Pipe stderr to /dev/null if no errFile is specified
	if(options.errFile.empty()) {
#ifdef WIN32
		command += "NUL";
#else
		command += "/dev/null";
#endif

	// If there is an errFile specified, pipe stderr to it
	} else {
		command += options.errFile;
	}

    return command;
}

int Shell::system(const SystemOptions& options, RunStatistics* stats) {

	StatsProgram* statsProgramHandler;
	std::string command = Shell::buildCommand(options, stats);

	bool removeTmpFile = false;
	bool useStatFile = false;

	// If no statProgram is specified, use this default
	statsProgramHandler = StatsProgram::findStatsBinary(options.statProgram);

	// If no statFile was specified, use a temporary
	File statFile = File(options.statFile);
	if(stats && options.statFile.empty()) {
		char buffer[L_tmpnam];
		tmpnam(buffer);
		statFile = File(string(buffer));
		removeTmpFile = true;
	}

	// If statistics are requested, set up the command
	if(stats) {
		assert(statsProgramHandler);
		*stats = RunStatistics();
		useStatFile = true;
	}
	if(useStatFile) {
		command = statsProgramHandler->buildCommand(command, statFile);
	}

	// Obtain the real path of the specified cwd and enter it
	string realCWD = FileSystem::getRealPath(options.cwd);
	PushD dir(realCWD);
	if(messageFormatter) messageFormatter->reportAction("Entering directory: `" + realCWD + "'", MessageFormatter::MessageClass(options.verbosity));
	
	// Execute the command
	int result = 0;
	if(messageFormatter) messageFormatter->reportAction("Executing: " + command, MessageFormatter::MessageClass(options.verbosity));
	
	System::Timer timer;
	result = ::system( command.c_str() );
	if(stats) stats->time_monraw = (float)timer.getElapsedSeconds();
	
	if(messageFormatter) {
		std::stringstream str;
		str << "Process exited with result: ";
		str << result;
		messageFormatter->reportAction(str.str(), MessageFormatter::MessageClass(options.verbosity));
	}
	
	// Obtain statistics
	if(stats) {
		statsProgramHandler->buildCommand(command, statFile);
		if(messageFormatter) messageFormatter->reportAction("Read time statistics (" + statsProgramHandler->getName() + ")", MessageFormatter::MessageClass(options.verbosity));
	}
	
	// Leave the cwd
	dir.popd();
	if(messageFormatter) messageFormatter->reportAction("Exiting directory: `" + realCWD + "'", MessageFormatter::MessageClass(options.verbosity));
	
	if(stats && removeTmpFile && FileSystem::exists(statFile)) {
		FileSystem::remove(statFile);
	}
	
	// Check if the command was killed, e.g. by ctrl-c
#ifdef WIN32
#else
	if (options.signalHandler && WIFSIGNALED(result)) {
		result = options.signalHandler(result);
	}
#endif
	
	// Return the result of the command
	return result;
}

bool Shell::readMemtimeStatisticsFromLog(File logFile, Shell::RunStatistics& stats) {
	if(!FileSystem::exists(logFile)) {
		return true;
	}
	
	std::ifstream log(logFile.getFileRealPath());
	if(!log.is_open()) {
		return true;
	}
	
	static const int MAX_CHARS = 200;
	
	char buffer[MAX_CHARS];
	Shell::RunStatistics statsTemp;
	while(true) {
		
		// Read the next line
		log.getline(buffer,MAX_CHARS);
		
		// Read the stats from the current line
		if(!Shell::readMemtimeStatistics(buffer,statsTemp)) {
			stats.addTimeMaxMem(statsTemp);
		}
		
		// Abort if EOF found
		if(log.eof()) {
			break;
		}
		
		// Handle a fail
		if(log.fail()) {
			
			// If the fail is caused by a line that is too long
			if(log.gcount()==MAX_CHARS-1) {
				
				// Clear the error and continue
				log.clear();
			
			// If the fail was because of something else
			} else {
				
				// Abort
				break;
			}
		}
		
	}
	return false;
}

bool Shell::readMemtimeStatistics(File file, RunStatistics& stats) {
	string* contents = FileSystem::load(file);
	
	if(contents) {
		bool result = readMemtimeStatistics(*contents,stats);
		delete contents;
		return result;
	} else {
		return true;
	}
}

bool Shell::readMemtimeStatistics(const string& contents, RunStatistics& stats) {
	return readMemtimeStatistics(contents.c_str(),stats);
}

bool Shell::readMemtimeStatistics(const char* contents, RunStatistics& stats) {
	// Format example: 0.00 user, 0.00 system, 0.10 elapsed -- Max VSize = 4024KB, Max RSS = 76KB
	int result = sscanf( contents,"%f user, %f system, %f elapsed -- Max VSize = %fKB, Max RSS = %fKB"
	                   , &stats.time_user
	                   , &stats.time_system
	                   , &stats.time_elapsed
	                   , &stats.mem_virtual
	                   , &stats.mem_resident
	                   );
	return result != 5;
}
