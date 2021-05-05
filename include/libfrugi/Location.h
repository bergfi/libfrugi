#pragma once

#include <iostream>
#include <sstream>

namespace libfrugi {

class Location {
private:
    std::string filename;
    int first_line;
    int first_column;
    int last_line;
    int last_column;
public:
    const std::string& getFileName() const {
        return filename;
    }

    void setFileName(const std::string& filename) {
        this->filename = filename;
    }

    int getFirstLine() const {
        return first_line;
    }

    int getFirstColumn() const {
        return first_column;
    }

    int getLastLine() const {
        return last_line;
    }

    int getLastColumn() const {
        return last_column - 1;
    }

    bool isNull() const {
        return first_line == 0
               && last_line == 0
               && filename == "";
    }

    void nullify() {
        first_line = last_line = 0;
        first_column = last_column = 1;
        filename = "";
    }

    void reset() {
        first_column = first_line = 1;
        last_column = last_line = 1;
    }

    Location() :
            filename(""),
            first_line(0),
            first_column(1),
            last_line(0),
            last_column(1) {
    }

    Location(std::string filename) :
            filename(filename),
            first_line(0),
            first_column(1),
            last_line(0),
            last_column(1) {
    }

    Location(std::string filename, int first_line) :
            filename(filename),
            first_line(first_line),
            first_column(1),
            last_line(first_line),
            last_column(1) {
    }

    Location(std::string filename, int first_line, int last_line) :
            filename(filename),
            first_line(first_line),
            first_column(1),
            last_line(last_line),
            last_column(1) {
    }

    Location(std::string filename, int first_line, int first_column, int last_line, int last_column) :
            filename(filename),
            first_line(first_line),
            first_column(first_column),
            last_line(last_line),
            last_column(last_column) {
    }

    std::string toString() const {
        std::stringstream ss;
        print(ss);
        return ss.str();
    }

    Location upTo(const Location& locOther) {
        if(isNull()) {
            return locOther;
        }
        if(locOther.isNull()) {
            return *this;
        }
        Location l;
        l.filename = filename;
        l.first_line = first_line;
        l.first_column = first_column;
        l.last_line = locOther.first_line;
        l.last_column = locOther.first_column;
        return l;
    }

    Location upToAndIncluding(const Location& locOther) {
        if(isNull()) {
            return locOther;
        }
        if(locOther.isNull()) {
            return *this;
        }
        Location l;
        l.filename = filename;
        l.first_line = first_line;
        l.first_column = first_column;
        l.last_line = locOther.last_line;
        l.last_column = locOther.last_column;
        return l;
    }

    Location afterUpTo(const Location& locOther) {
        if(isNull()) {
            return locOther;
        }
        if(locOther.isNull()) {
            return *this;
        }
        Location l;
        l.filename = filename;
        l.first_line = last_line;
        l.first_column = last_column;
        l.last_line = locOther.first_line;
        l.last_column = locOther.first_column;
        return l;
    }

    Location afterUpToAndIncluding(const Location& locOther) {
        Location l;
        l.filename = filename;
        l.first_line = last_line;
        l.first_column = last_column;
        l.last_line = locOther.last_line;
        l.last_column = locOther.last_column;
        return l;
    }

    void set(const std::string& filename, int line) {
        first_column = 1;
        first_line = line;
        last_column = 1;
        last_line = line;
        this->filename = filename;
    }

    void setToLastOf(const Location& locOther) {
        first_line = last_line = locOther.last_line;
        first_column = last_column = locOther.last_column;
        filename = locOther.filename;
        //filename     = currentParser->getCurrentFileName();
    }

    void println(std::ostream& ss) const {
        print(ss);
        ss << std::endl;
    }

    void print(std::ostream& ss) const {
        ss << filename;
        if(getFirstLine() > 0) {
            ss << ":";
            if(getFirstLine() >= getLastLine()) {
                if(getFirstColumn() >= getLastColumn()) {
                    if(getFirstColumn() > 0) {
                        ss << getFirstLine();
                        ss << ".";
                        ss << getFirstColumn();
                    } else {
                        ss << getFirstLine();
                    }
                } else {
                    ss << getFirstLine();
                    ss << ".";
                    ss << getFirstColumn();
                    ss << "-";
                    ss << getLastColumn();
                }
            } else {
                if(getFirstColumn() > 0) {
                    ss << getFirstLine();
                    ss << ".";
                    ss << getFirstColumn();
                    ss << "-";
                    ss << getLastLine();
                    ss << ".";
                    ss << getLastColumn();
                } else {
                    ss << getFirstLine();
                    ss << "-";
                    ss << getLastLine();
                }
            }
        }
    }

    void step() {
        first_column = last_column;
        first_line = last_line;
    }

    void advanceCharacters(int n) {
        last_column += n;
    }

    void advanceLines(int n) {
        last_column = 1;
        last_line += n;
    }

    Location end() {
        Location l;
        l.filename = filename;
        l.first_line = last_line;
        l.first_column = last_column;
        l.last_line = last_line;
        l.last_column = last_column;
        return l;
    }

    Location begin() {
        Location l;
        l.filename = filename;
        l.first_line = first_line;
        l.first_column = first_column;
        l.last_line = first_line;
        l.last_column = first_column;
        return l;
    }

};

} // namespace libfrugi