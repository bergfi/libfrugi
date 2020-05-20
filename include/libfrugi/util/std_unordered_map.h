#include <unordered_map>

template<typename K, typename V>
std::ostream& operator <<(std::ostream& out, std::unordered_map<K, V> const& map) {
    bool first = true;
    out << "{";
    for(auto& kv: map) {
        out << kv.first;
        out << ": ";
        out << kv.second;
        if(!first) out << ", ";
        first = false;
    }
    out << "}";
    return out;
}
