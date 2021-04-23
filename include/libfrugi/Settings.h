#ifndef SETTINGS_H
#define SETTINGS_H

#include <cstring>
#include <string>
#include <unordered_map>
#include <sstream>

namespace libfrugi {

class SettingsValue {
public:

	SettingsValue(): _str() {
	}
	template<typename T>
	SettingsValue(const T& str) {
		std::stringstream ss;
		ss << str;
		_str = ss.str();
	}
	SettingsValue(const std::string& str): _str(str) {
	}

	template<typename T>
	SettingsValue& operator=(T&& str) {
		std::stringstream ss;
		ss << str;
		_str = ss.str();
		return *this;
	}
	
	inline bool isOn() const {
		return _str=="1" || _str=="true" || _str=="T" || _str=="on";
	}
	inline bool isOff() const {
		return _str=="0" || _str=="false" || _str=="F" || _str=="off";
	}
	
	operator bool() {
		return isOn();
	}
	operator std::string() {
		return _str;
	}

	std::string const& asString() {
		return _str;
	}

	size_t asUnsignedValue() {
		size_t r = 0;
		std::stringstream(_str) >> r;
		return r;
	}

    template<typename T>
	T as() {
		T r = 0;
		std::stringstream(_str) >> r;
		return r;
	}
private:
    std::string _str;
};

class Settings {
public:
	enum class Switch {
		UNDEFINED = 0,
		ON = 1,
		OFF = 2,
		NEITHER = 3,
		NUMBER_OF
	};

	static inline const SettingsValue NONE;

public:
	Switch asSwitch(const std::string& setting) {
		auto it = _map.find(setting);
		if(it != _map.end()) {
			const SettingsValue& value = it->second;
			if(value.isOn()) {
				return Switch::ON;
			} else if(value.isOff()) {
				return Switch::OFF;
			} else {
				return Switch::NEITHER;
			}
		} else {
			return Switch::UNDEFINED;
		}
	}

    static Settings& global() {
        static Settings settings;
        return settings;
    }

    void insertKeyValue(const char* s) {
        const char* v = strchr(s, '=');
        if(v) {
            (*this)[std::string(s, v-s)] = std::string(v+1);
        } else {
            (*this)[std::string(s)] = "true";
        }
    }

    Settings getSubSection(const std::string& section) {
	    Settings s;
	    for(auto& kv: _map) {
	        if(!strcasecmp(kv.first.c_str(), section.c_str()) && kv.first.at(section.length()) == '.') {
	            std::string k = kv.first.substr(section.length() + 1);
	            s[k] = kv.second;
	        }
	    }
	    return s;
	}

    SettingsValue& operator[](std::string const& key) {
        return _map[key];
    }

    SettingsValue const& operator[](std::string const& key) const {
        auto it = _map.find(key);
        if(it != _map.end()) {
            return it->second;
        } else {
            return NONE;
        }
    }
private:
    std::unordered_map<std::string, SettingsValue> _map;

};

}

#endif
