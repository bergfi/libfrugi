/*
 * System.cpp
 * 
 * Part of a general library.
 * 
 * @author Freark van der Berg
 */

#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <libfrugi/System.h>

namespace libfrugi {

std::string System::cwdAtStart;
std::vector<std::string> System::args;

void System::init(int argc, char* argv[]) {
    if(!args.empty()) args.clear();
    args.reserve(argc);
    for(int i = 0; i < argc; ++i) {
        new(&args[i]) std::string(argv[i]);
    }
    if(System::getArgument(0)[0] == '/') {
        cwdAtStart = FileSystem::getRealPath(System::getArgument(0));
    } else {
        cwdAtStart = FileSystem::getRealPath(std::string("./") + System::getArgument(0));
    }
    cwdAtStart = FileSystem::getDirName(cwdAtStart);
}

std::string const& System::getArgument(size_t i) {
    return args[i];
}

std::vector<std::string> const& System::getArguments() {
    return args;
}

void System::sleep(uint64_t ms) {
#ifdef WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

void System::generateUUID(size_t bytes, std::string& uuid) {
    static uint64_t lastTime;
    static uint64_t currentTime;
    static std::uniform_int_distribution<int> hex{0, 15};  // distribution that maps to the ints 1..6
    static std::default_random_engine re{};                     // the default engine	uuid.empty;
    do {
        currentTime = getCurrentTimeMicros();
    } while(currentTime == lastTime);
    lastTime = currentTime;
    re.seed(currentTime);
    uuid.clear();
    uuid.reserve(bytes * 2);
    for(unsigned int i = 0; i < bytes * 2; ++i) {
        int x = hex(re);
        if(x < 10) uuid += ('0' + x);
        else uuid += ('A' + x - 10);
    }
}

uint64_t System::getCurrentTimeMillis() {
    timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000 + now.tv_usec / 1000;
}

uint64_t System::getCurrentTimeMicros() {
    timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000000 + now.tv_usec;
}

} // namespace libfrugi
