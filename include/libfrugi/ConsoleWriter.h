/*
 * CompilerContext.h
 * 
 * Part of a general library.
 * 
 * @author Freark van der Berg
 */

#ifndef CONSOLEWRITER_H
#define CONSOLEWRITER_H

#include "FileWriter.h"

/**
 * The ConsoleWriter is a FileWriter with support to colour the output if the
 * specified output stream is std::cout or std::cerr.
 */
class ConsoleWriter: public FileWriter {
public:
	class Color {
	public:
		char code[4];
		Color(char code): code{code,0,0,0} {
		}
		Color(char code1, char code2): code{code1,code2,0,0} {
		}
		Color(char code1, char code2, char code3): code{code1,code2,code3,0} {
		}
		Color(char code1, char code2, char code3, char code4): code{code1,code2,code3,code4} {
		}
	public:
		
		static const ConsoleWriter::Color Error;
		static const ConsoleWriter::Color Warning;
		static const ConsoleWriter::Color Notify;
		static const ConsoleWriter::Color Notify2;
		static const ConsoleWriter::Color NotifyH;
		static const ConsoleWriter::Color Action;
		static const ConsoleWriter::Color Proper;
		static const ConsoleWriter::Color Message;
		static const ConsoleWriter::Color Reset;
		static const ConsoleWriter::Color ResetBright;
		static const ConsoleWriter::Color Bright;
		
		//static const ConsoleWriter::Color Black;
		static const ConsoleWriter::Color Red;
		static const ConsoleWriter::Color Green;
		static const ConsoleWriter::Color Blue;
		static const ConsoleWriter::Color Yellow;
		static const ConsoleWriter::Color Magenta;
		static const ConsoleWriter::Color Cyan;
		static const ConsoleWriter::Color White;
		//static const ConsoleWriter::Color BlackBright;
		static const ConsoleWriter::Color RedBright;
		static const ConsoleWriter::Color GreenBright;
		static const ConsoleWriter::Color BlueBright;
		static const ConsoleWriter::Color YellowBright;
		static const ConsoleWriter::Color MagentaBright;
		static const ConsoleWriter::Color CyanBright;
		static const ConsoleWriter::Color WhiteBright;

		//static const ConsoleWriter::Color Black;
		static const ConsoleWriter::Color BgRed;
		static const ConsoleWriter::Color BgGreen;
		static const ConsoleWriter::Color BgBlue;
		static const ConsoleWriter::Color BgYellow;
		static const ConsoleWriter::Color BgMagenta;
		static const ConsoleWriter::Color BgCyan;
		static const ConsoleWriter::Color BgWhite;
		//static const ConsoleWriter::Color BlackBright;
		static const ConsoleWriter::Color BgRedBright;
		static const ConsoleWriter::Color BgGreenBright;
		static const ConsoleWriter::Color BgBlueBright;
		static const ConsoleWriter::Color BgYellowBright;
		static const ConsoleWriter::Color BgMagentaBright;
		static const ConsoleWriter::Color BgCyanBright;
		static const ConsoleWriter::Color BgWhiteBright;

		int getColorCode() const { return code[0]; }
	};
private:
	std::ostream& out;
	int kindOfStream;
	std::stack<Color> colorStack;
	bool ignoreColors;
	bool lastWasEndLine;
public:

	/**
	 * Creates a new ConsoleWriter object.
	 * @param out The stream to output to.
	 */
	ConsoleWriter(std::ostream& out);
	
	virtual ~ConsoleWriter() {
		
	}
	
	/**
	 * Returns the topmost stream on the stack stream.
	 */
	virtual ostream& ss() {
		if(sss.size()==1) return out;
		return *sss.top();
	}

	/**
	 * Stream a Color object. The next object streamed will be of the specified
	 * colour. Often followed by push().
	 * @param color The Color the next streamed object will have.
	 * @return The Conwo
	 */
	virtual ConsoleWriter& operator<<(ConsoleWriter::Color color);

	virtual ConsoleWriter& appendLine(const string& s) {
		preAddHook();
		appendPrefix();
		append(s);
		appendPostfix();
		return *this;
	}

	virtual ConsoleWriter& append(const string& s) {
		ss() << s;
		return *this;
	}

	virtual ConsoleWriter& append(int i) {
		ss() << i;
		return *this;
	}

	virtual ConsoleWriter& append(unsigned int i) {
		ss() << i;
		return *this;
	}

	virtual ConsoleWriter& operator<<(const string& s) {
		preAddHook();
		ss() << s;
		return *this;
	}

	virtual ConsoleWriter& operator<<(int i) {
		preAddHook();
		ss() << i;
		return *this;
	}

	virtual ConsoleWriter& operator<<(unsigned int i) {
		preAddHook();
		ss() << i;
		return *this;
	}

	virtual ConsoleWriter& operator<<(long int i) {
		preAddHook();
		ss() << i;
		return *this;
	}

	virtual ConsoleWriter& operator<<(long unsigned int i) {
		preAddHook();
		ss() << i;
		return *this;
	}

	virtual ConsoleWriter& operator<<(float f) {
		preAddHook();
		ss() << f;
		return *this;
	}

	virtual ConsoleWriter& operator<<(double d) {
		preAddHook();
		ss() << d;
		return *this;
	}

	virtual ConsoleWriter& operator<<(long double ld) {
		preAddHook();
		ss() << ld;
		return *this;
	}

	virtual ConsoleWriter& operator<<(const FileWriterOption& option) {
		preAddHook();
		FileWriter::operator<<(option);
		return *this;
	}

	virtual ConsoleWriter& operator<<(const FileWriter& other) {
		preAddHook();
		ss() << other.toString();
		return *this;
	}

	virtual ConsoleWriter& operator<<(std::ostream&(*f)(std::ostream&)) {
		preAddHook();
		ss() << f;
		lastWasEndLine = true;
		return *this;
	}

	virtual ConsoleWriter& appendPrefix() {
		append(applyprefix);
		return *this;
	}

	virtual ConsoleWriter& appendPostfix() {
		append(postfix);
		return *this;
	}
	
	virtual void setIgnoreColors(bool ignoreColors) {
		this->ignoreColors = ignoreColors;
	}

	virtual void preAddHook() {
		if(lastWasEndLine) {
			appendPrefix();
			lastWasEndLine = false;
		}
	}

};

#endif //CONSOLEWRITER_H


