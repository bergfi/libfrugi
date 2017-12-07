/*
 * MessageFormatter.cpp
 * 
 * Part of a general library.
 * 
 * @author Freark van der Berg
 */

#include "libfrugi/MessageFormatter.h"

const MessageFormatter::MessageType MessageFormatter::MessageType::Message(MessageType::MESSAGE);
const MessageFormatter::MessageType MessageFormatter::MessageType::Notify (MessageType::NOTIFY);
const MessageFormatter::MessageType MessageFormatter::MessageType::NotifyH(MessageType::NOTIFYH);
const MessageFormatter::MessageType MessageFormatter::MessageType::Action (MessageType::ACTION);
const MessageFormatter::MessageType MessageFormatter::MessageType::Action2(MessageType::ACTION2);
const MessageFormatter::MessageType MessageFormatter::MessageType::Action3(MessageType::ACTION3);
const MessageFormatter::MessageType MessageFormatter::MessageType::Warning(MessageType::WARNING);
const MessageFormatter::MessageType MessageFormatter::MessageType::Success(MessageType::SUCCESS);
const MessageFormatter::MessageType MessageFormatter::MessageType::Failure(MessageType::FAILURE);
const MessageFormatter::MessageType MessageFormatter::MessageType::Note   (MessageType::NOTE);
const MessageFormatter::MessageType MessageFormatter::MessageType::Error  (MessageType::ERR);
const MessageFormatter::MessageType MessageFormatter::MessageType::File   (MessageType::FILE);
const MessageFormatter::MessageType MessageFormatter::MessageType::Title  (MessageType::TITLE);

const int MessageFormatter::VERBOSITY_DEFAULT = 0;

void MessageFormatter::print(const MSG& msg) {

	const Location& loc = msg.loc;
	const std::string& str = msg.message;
	const MessageType& mType = msg.type;

	consoleWriter << msg.pid << "|";

	for(int i = msg.indent; i--;) {
		consoleWriter << "  ";
	}

	if(mType.isError()) {
		consoleWriter << ConsoleWriter::Color::Error;
	} else if (mType.isWarning()) {
		consoleWriter << ConsoleWriter::Color::Warning;
	} else if (mType.isNotify()) {
		consoleWriter << ConsoleWriter::Color::Notify;
	} else if (mType.isAction() || mType.isTitle()) {
		consoleWriter << ConsoleWriter::Color::Action;
	} else if (mType.isReport()) {
		if(mType == MessageType::Success) {
			consoleWriter << ConsoleWriter::Color::Proper;
		} else if(mType == MessageType::Failure) {
			consoleWriter << ConsoleWriter::Color::Error;
		} else {
			consoleWriter << ConsoleWriter::Color::Notify2;
		}
	} else if (mType.isFile()) {
	} else {
		consoleWriter << ConsoleWriter::Color::Message;
	}

	loc.print(consoleWriter.ss());

	if(mType.isError()) {
		consoleWriter << ":error:";
	} else if (mType.isWarning()) {
		consoleWriter << ":warning:";
	} else if (mType.isNotify()) {
		consoleWriter << ":: ";
		if(mType==MessageFormatter::MessageType::Notify)
			consoleWriter << ConsoleWriter::Color::Notify2;
		else
			consoleWriter << ConsoleWriter::Color::NotifyH;
	} else if (mType.isAction() || mType.isTitle()) {
		if(mType==MessageFormatter::MessageType::Action) {
			consoleWriter << " > ";
			consoleWriter << ConsoleWriter::Color::Notify2;
		} else if(mType==MessageFormatter::MessageType::Action2) {
			consoleWriter << "   > ";
			consoleWriter << ConsoleWriter::Color::Notify2;
		} else {
			consoleWriter << "   - ";
			consoleWriter << ConsoleWriter::Color::Reset;
		}
	} else if (mType.isMessage()) {
	} else if (mType.isReport()) {
		if(mType == MessageType::Success) {
			consoleWriter << " o ";
		} else if(mType == MessageType::Failure) {
			consoleWriter << " x ";
		} else {
			consoleWriter << " - ";
		}
		consoleWriter << ConsoleWriter::Color::Reset;
	} else if (mType.isFile()) {
	} else {
		consoleWriter << ":";
	}

	consoleWriter << str;

	consoleWriter << ConsoleWriter::Color::Reset;

	if (mType.isTitle()) {
		consoleWriter << ":";
	}
	
	consoleWriter << consoleWriter.applypostfix;

}

void MessageFormatter::reportErrorAt(Location loc, const std::string& str, const size_t& messageClassIndex) {
	messageAt(loc,str,MessageType::Error,getMessageClass(messageClassIndex));
	errors++;
}

void MessageFormatter::reportErrorAt(Location loc, const std::string& str, const MessageFormatter::MessageClass& messageClass) {
	messageAt(loc,str,MessageType::Error,messageClass);
	errors++;
}

void MessageFormatter::reportWarningAt(Location loc, const std::string& str, const size_t& messageClassIndex) {
	messageAt(loc,str,MessageType::Warning,getMessageClass(messageClassIndex));
	warnings++;
}

void MessageFormatter::reportWarningAt(Location loc, const std::string& str, const MessageFormatter::MessageClass& messageClass) {
	messageAt(loc,str,MessageType::Warning,messageClass);
	warnings++;
}

void MessageFormatter::reportActionAt(Location loc, const std::string& str, const size_t& messageClassIndex) {
	messageAt(loc,str,MessageType::Action,getMessageClass(messageClassIndex));
}

void MessageFormatter::reportActionAt(Location loc, const std::string& str, const MessageFormatter::MessageClass& messageClass) {
	messageAt(loc,str,MessageType::Action,messageClass);
}

void MessageFormatter::reportAction2At(Location loc, const std::string& str, const size_t& messageClassIndex) {
	messageAt(loc,str,MessageType::Action2,getMessageClass(messageClassIndex));
}

void MessageFormatter::reportAction2At(Location loc, const std::string& str, const MessageFormatter::MessageClass& messageClass) {
	messageAt(loc,str,MessageType::Action2,messageClass);
}

void MessageFormatter::reportAction3At(Location loc, const std::string& str, const size_t& messageClassIndex) {
	messageAt(loc,str,MessageType::Action3,getMessageClass(messageClassIndex));
}

void MessageFormatter::reportAction3At(Location loc, const std::string& str, const MessageFormatter::MessageClass& messageClass) {
	messageAt(loc,str,MessageType::Action3,messageClass);
}

void MessageFormatter::reportError(const std::string& str, const size_t& messageClassIndex) {
	message(str,MessageType::Error,getMessageClass(messageClassIndex));
	errors++;
}

void MessageFormatter::reportError(const std::string& str, const MessageFormatter::MessageClass& messageClass) {
	message(str,MessageType::Error,messageClass);
	errors++;
}

void MessageFormatter::reportWarning(const std::string& str, const size_t& messageClassIndex) {
	message(str,MessageType::Warning,getMessageClass(messageClassIndex));
	warnings++;
}

void MessageFormatter::reportWarning(const std::string& str, const MessageFormatter::MessageClass& messageClass) {
	message(str,MessageType::Warning,messageClass);
	warnings++;
}

void MessageFormatter::reportAction(const std::string& str, const size_t& messageClassIndex) {
	message(str,MessageType::Action,getMessageClass(messageClassIndex));
}

void MessageFormatter::reportAction(const std::string& str, const MessageFormatter::MessageClass& messageClass) {
	message(str,MessageType::Action,messageClass);
}

void MessageFormatter::reportAction2(const std::string& str, const size_t& messageClassIndex) {
	message(str,MessageType::Action2,getMessageClass(messageClassIndex));
}

void MessageFormatter::reportAction2(const std::string& str, const MessageFormatter::MessageClass& messageClass) {
	message(str,MessageType::Action2,messageClass);
}

void MessageFormatter::reportAction3(const std::string& str, const size_t& messageClassIndex) {
	message(str,MessageType::Action3,getMessageClass(messageClassIndex));
}

void MessageFormatter::reportAction3(const std::string& str, const MessageFormatter::MessageClass& messageClass) {
	message(str,MessageType::Action3,messageClass);
}

void MessageFormatter::reportFile(const std::string& fileName, const std::string& contents, const size_t& messageClassIndex) {
	message(fileName,MessageType::Title,getMessageClass(messageClassIndex));
	message(contents,MessageType::File,getMessageClass(messageClassIndex));
}

void MessageFormatter::reportFile(const std::string& fileName, const std::string& contents, const MessageFormatter::MessageClass& messageClass) {
	message(fileName,MessageType::Title,messageClass);
	message(contents,MessageType::File,messageClass);
}

void MessageFormatter::reportSuccess(const std::string& str, const size_t& messageClassIndex) {
	message(str,MessageType::Success,getMessageClass(messageClassIndex));
}

void MessageFormatter::reportSuccess(const std::string& str, const MessageFormatter::MessageClass& messageClass) {
	message(str,MessageType::Success,messageClass);
}

void MessageFormatter::reportFailure(const std::string& str, const size_t& messageClassIndex) {
	message(str,MessageType::Failure,getMessageClass(messageClassIndex));
}

void MessageFormatter::reportFailure(const std::string& str, const MessageFormatter::MessageClass& messageClass) {
	message(str,MessageType::Failure,messageClass);
}

void MessageFormatter::reportNote(const std::string& str, const size_t& messageClassIndex) {
	message(str,MessageType::Note,getMessageClass(messageClassIndex));
}

void MessageFormatter::reportNote(const std::string& str, const MessageFormatter::MessageClass& messageClass) {
	message(str,MessageType::Note,messageClass);
}

void MessageFormatter::notify(const std::string& str, const size_t& messageClassIndex) {
	message(str,MessageType::Notify,getMessageClass(messageClassIndex));
}

void MessageFormatter::notify(const std::string& str, const MessageFormatter::MessageClass& messageClass) {
	message(str,MessageType::Notify,messageClass);
}

void MessageFormatter::notifyHighlighted(const std::string& str, const size_t& messageClassIndex) {
	message(str,MessageType::NotifyH,getMessageClass(messageClassIndex));
}

void MessageFormatter::notifyHighlighted(const std::string& str, const MessageFormatter::MessageClass& messageClass) {
	message(str,MessageType::NotifyH,messageClass);
}

void MessageFormatter::message(const std::string& str, const size_t& messageClassIndex) {
	message(str,MessageType::Message,getMessageClass(messageClassIndex));
}

void MessageFormatter::message(const std::string& str, const MessageFormatter::MessageClass& messageClass) {
	message(str,MessageType::Message,messageClass);
}

void MessageFormatter::message(const std::string& str, const MessageType& mType, const size_t& messageClassIndex) {
	messageAt(Location(),str,mType,getMessageClass(messageClassIndex));
}

void MessageFormatter::message(const std::string& str, const MessageType& mType, const MessageFormatter::MessageClass& messageClass) {
	messageAt(Location(),str,mType,messageClass);
}

void MessageFormatter::messageAt(Location loc, const std::string& str, const MessageType& mType, const size_t& messageClassIndex) {
	messageAt(loc,str,mType,getMessageClass(messageClassIndex));
}

void MessageFormatter::messageAt(Location loc, const std::string& str, const MessageType& mType, const MessageFormatter::MessageClass& messageClass) {
	static int n=1;
	
	if(!messageClass.isEnabled()) {
		return;
	}
	
	if(messageClass.getVerbosity()>verbosity) {
		return;
	}
	
	if(m_autoFlush) {
		flush();
		print(MSG(n++,loc,_indent,str,mType));
	} else {
		messages.insert(MessageFormatter::MSG(n++,loc,_indent,str,mType));
	}
}

void MessageFormatter::reportErrors() {

	consoleWriter << consoleWriter.applyprefix;
	consoleWriter << ConsoleWriter::Color::Notify << ":: ";
	consoleWriter << ConsoleWriter::Color::Notify2  << "Finished. ";
	{
		if(errors==0) {
			consoleWriter << ConsoleWriter::Color::Proper;
		} else {
			consoleWriter << ConsoleWriter::Color::Error;
		}
		consoleWriter << errors << " errors";
	}
	consoleWriter << ConsoleWriter::Color::Notify2  << " and ";
	{
		if(warnings==0) {
			consoleWriter << ConsoleWriter::Color::Proper;
		} else {
			consoleWriter << ConsoleWriter::Color::Warning;
		}
		consoleWriter << warnings << " warnings";
	}
	consoleWriter << ConsoleWriter::Color::Notify2 << "." << consoleWriter.applypostfix;
	consoleWriter << ConsoleWriter::Color::Reset;
}

void MessageFormatter::flush() {
	std::set<MSG>::iterator it = messages.begin();
	for(;it!=messages.end(); ++it) {
		print(*it);
	}
	messages.clear();
}

MessageFormatter::MessageClass& MessageFormatter::getMessageClass(size_t classIndex) {
	if(classIndex>=messageClasses.size()) messageClasses.resize(classIndex+1);
	return messageClasses[classIndex];
}

MessageFormatter::MessageClass& MessageFormatter::getMessageClass(std::string className) {
	auto it = messageClassIndex.find(className);
	if(it==messageClassIndex.end()) {
		return getMessageClass(messageClasses.size());
	} else {
		return getMessageClass(it->second);
	}
}

MessageFormatter::MessageClass& MessageFormatter::newMessageClass(size_t classIndex, std::string classname) {
	messageClasses.resize(classIndex);
	messageClassIndex.insert(std::pair<std::string,size_t>(classname,classIndex));
	return messageClasses[classIndex];
}
