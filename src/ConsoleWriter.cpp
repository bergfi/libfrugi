/*
 * CompilerContext.cpp
 * 
 * Part of a general library.
 * 
 * @author Freark van der Berg
 */

#include "libfrugi/ConsoleWriter.h"

#include <iostream>

namespace libfrugi {

#ifdef WIN32
#include <windows.h>
const ConsoleWriter::Color ConsoleWriter::Color::Error  (FOREGROUND_RED   );
const ConsoleWriter::Color ConsoleWriter::Color::Warning(FOREGROUND_RED   | FOREGROUND_GREEN);
const ConsoleWriter::Color ConsoleWriter::Color::Notify (FOREGROUND_BLUE  | FOREGROUND_INTENSITY);
const ConsoleWriter::Color ConsoleWriter::Color::Notify2(FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
const ConsoleWriter::Color ConsoleWriter::Color::NotifyH(FOREGROUND_RED   | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
const ConsoleWriter::Color ConsoleWriter::Color::Action (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
const ConsoleWriter::Color ConsoleWriter::Color::Proper (FOREGROUND_GREEN | FOREGROUND_INTENSITY);
const ConsoleWriter::Color ConsoleWriter::Color::Message(0);
const ConsoleWriter::Color ConsoleWriter::Color::Reset  (0);
const ConsoleWriter::Color ConsoleWriter::Color::ResetBright  (FOREGROUND_INTENSITY);

const ConsoleWriter::Color ConsoleWriter::Color::Red          (FOREGROUND_RED   );
const ConsoleWriter::Color ConsoleWriter::Color::Green        (FOREGROUND_GREEN );
const ConsoleWriter::Color ConsoleWriter::Color::Blue         (FOREGROUND_BLUE  );
const ConsoleWriter::Color ConsoleWriter::Color::Yellow       (FOREGROUND_RED   | FOREGROUND_GREEN     );
const ConsoleWriter::Color ConsoleWriter::Color::Cyan         (FOREGROUND_GREEN | FOREGROUND_BLUE      );
const ConsoleWriter::Color ConsoleWriter::Color::Magenta      (FOREGROUND_RED   | FOREGROUND_BLUE      );
const ConsoleWriter::Color ConsoleWriter::Color::White        (FOREGROUND_RED   | FOREGROUND_GREEN     | FOREGROUND_BLUE      );

const ConsoleWriter::Color ConsoleWriter::Color::RedBright    (FOREGROUND_RED   | FOREGROUND_INTENSITY );
const ConsoleWriter::Color ConsoleWriter::Color::GreenBright  (FOREGROUND_GREEN | FOREGROUND_INTENSITY );
const ConsoleWriter::Color ConsoleWriter::Color::BlueBright   (FOREGROUND_BLUE  | FOREGROUND_INTENSITY );
const ConsoleWriter::Color ConsoleWriter::Color::YellowBright (FOREGROUND_RED   | FOREGROUND_GREEN     | FOREGROUND_INTENSITY );
const ConsoleWriter::Color ConsoleWriter::Color::CyanBright   (FOREGROUND_GREEN | FOREGROUND_BLUE      | FOREGROUND_INTENSITY );
const ConsoleWriter::Color ConsoleWriter::Color::MagentaBright(FOREGROUND_RED   | FOREGROUND_BLUE      | FOREGROUND_INTENSITY );
const ConsoleWriter::Color ConsoleWriter::Color::WhiteBright  (FOREGROUND_RED   | FOREGROUND_GREEN     | FOREGROUND_BLUE      | FOREGROUND_INTENSITY);

const ConsoleWriter::Color ConsoleWriter::Color::BgRed          (BACKGROUND_RED   );
const ConsoleWriter::Color ConsoleWriter::Color::BgGreen        (BACKGROUND_GREEN );
const ConsoleWriter::Color ConsoleWriter::Color::BgBlue         (BACKGROUND_BLUE  );
const ConsoleWriter::Color ConsoleWriter::Color::BgYellow       (BACKGROUND_RED   | BACKGROUND_GREEN     );
const ConsoleWriter::Color ConsoleWriter::Color::BgCyan         (BACKGROUND_GREEN | BACKGROUND_BLUE      );
const ConsoleWriter::Color ConsoleWriter::Color::BgMagenta      (BACKGROUND_RED   | BACKGROUND_BLUE      );
const ConsoleWriter::Color ConsoleWriter::Color::BgWhite        (BACKGROUND_RED   | BACKGROUND_GREEN     | BACKGROUND_BLUE      );

const ConsoleWriter::Color ConsoleWriter::Color::BgRedBright    (BACKGROUND_RED   | BACKGROUND_INTENSITY );
const ConsoleWriter::Color ConsoleWriter::Color::BgGreenBright  (BACKGROUND_GREEN | BACKGROUND_INTENSITY );
const ConsoleWriter::Color ConsoleWriter::Color::BgBlueBright   (BACKGROUND_BLUE  | BACKGROUND_INTENSITY );
const ConsoleWriter::Color ConsoleWriter::Color::BgYellowBright (BACKGROUND_RED   | BACKGROUND_GREEN     | BACKGROUND_INTENSITY );
const ConsoleWriter::Color ConsoleWriter::Color::BgCyanBright   (BACKGROUND_GREEN | BACKGROUND_BLUE      | BACKGROUND_INTENSITY );
const ConsoleWriter::Color ConsoleWriter::Color::BgMagentaBright(BACKGROUND_RED   | BACKGROUND_BLUE      | BACKGROUND_INTENSITY );
const ConsoleWriter::Color ConsoleWriter::Color::BgWhiteBright  (BACKGROUND_RED   | BACKGROUND_GREEN     | BACKGROUND_BLUE      | BACKGROUND_INTENSITY);

#else

#include <cstdio>
#include <cstdlib>

const ConsoleWriter::Color ConsoleWriter::Color::Error(31);
const ConsoleWriter::Color ConsoleWriter::Color::Warning(33);
const ConsoleWriter::Color ConsoleWriter::Color::Notify(1, 34);
const ConsoleWriter::Color ConsoleWriter::Color::Notify2(1, 37);
const ConsoleWriter::Color ConsoleWriter::Color::NotifyH(1, 35);
const ConsoleWriter::Color ConsoleWriter::Color::Action(1, 36);
const ConsoleWriter::Color ConsoleWriter::Color::Proper(1, 32);
const ConsoleWriter::Color ConsoleWriter::Color::Message(0);
const ConsoleWriter::Color ConsoleWriter::Color::Reset(0);
const ConsoleWriter::Color ConsoleWriter::Color::ResetBright(1);
const ConsoleWriter::Color ConsoleWriter::Color::Bright(1);

const ConsoleWriter::Color ConsoleWriter::Color::Red(31);
const ConsoleWriter::Color ConsoleWriter::Color::Green(32);
const ConsoleWriter::Color ConsoleWriter::Color::Blue(34);
const ConsoleWriter::Color ConsoleWriter::Color::Yellow(33);
const ConsoleWriter::Color ConsoleWriter::Color::Cyan(36);
const ConsoleWriter::Color ConsoleWriter::Color::Magenta(35);
const ConsoleWriter::Color ConsoleWriter::Color::White(37);

const ConsoleWriter::Color ConsoleWriter::Color::RedBright(1, 31);
const ConsoleWriter::Color ConsoleWriter::Color::GreenBright(1, 32);
const ConsoleWriter::Color ConsoleWriter::Color::BlueBright(1, 34);
const ConsoleWriter::Color ConsoleWriter::Color::YellowBright(1, 33);
const ConsoleWriter::Color ConsoleWriter::Color::CyanBright(1, 36);
const ConsoleWriter::Color ConsoleWriter::Color::MagentaBright(1, 35);
const ConsoleWriter::Color ConsoleWriter::Color::WhiteBright(1, 37);

const ConsoleWriter::Color ConsoleWriter::Color::BgRed(41);
const ConsoleWriter::Color ConsoleWriter::Color::BgGreen(42);
const ConsoleWriter::Color ConsoleWriter::Color::BgBlue(44);
const ConsoleWriter::Color ConsoleWriter::Color::BgYellow(43);
const ConsoleWriter::Color ConsoleWriter::Color::BgCyan(46);
const ConsoleWriter::Color ConsoleWriter::Color::BgMagenta(45);
const ConsoleWriter::Color ConsoleWriter::Color::BgWhite(47);

const ConsoleWriter::Color ConsoleWriter::Color::BgRedBright(1, 41);
const ConsoleWriter::Color ConsoleWriter::Color::BgGreenBright(1, 42);
const ConsoleWriter::Color ConsoleWriter::Color::BgBlueBright(1, 44);
const ConsoleWriter::Color ConsoleWriter::Color::BgYellowBright(1, 43);
const ConsoleWriter::Color ConsoleWriter::Color::BgCyanBright(1, 46);
const ConsoleWriter::Color ConsoleWriter::Color::BgMagentaBright(1, 45);
const ConsoleWriter::Color ConsoleWriter::Color::BgWhiteBright(1, 47);
#endif

ConsoleWriter::ConsoleWriter(std::ostream& out)
        : FileWriter(), out(out), ignoreColors(false), lastWasEndLine(false) {

    if(&out == &std::cout) {
        kindOfStream = 1;
    } else if(&out == &std::cerr) {
        kindOfStream = 2;
    } else {
        kindOfStream = 0;
    }

    if(kindOfStream > 0) {
#ifdef WIN32
        CONSOLE_SCREEN_BUFFER_INFO out;
        HANDLE hConsole = GetStdHandle(kindOfStream==1?STD_OUTPUT_HANDLE:STD_ERROR_HANDLE);
        GetConsoleScreenBufferInfo(hConsole,&out);
        colorStack.push(Color(out.wAttributes));
#else
        colorStack.push(ConsoleWriter::Color::Reset);
#endif
    }
}

ConsoleWriter& ConsoleWriter::operator<<(ConsoleWriter::Color color) {
    if(ignoreColors) { return *this; }
#ifdef WIN32
#error to implement
#else
    out << "\033[";
    out << int(color.code[0]);
    int i = 1;
    while(i < 4 && color.code[i]) {
        out << ";" << int(color.code[i]);
        ++i;
    }
    out << "m";
    i = 0;
#endif
    return *this;
}

} // namespace libfrugi
