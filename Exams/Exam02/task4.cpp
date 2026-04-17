//Isabella Koletic 
//04/17/25

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cctype>

static const int kMaxSeq = 4096;

static bool readCacheSize(int &out) {
    std::string line;
    if (!std::getline(std::cin, line))
        return false;
    std::istringstream iss(line);
    if (!(iss >> out))
        return false;
    int extra = 0;
    if (iss >> extra)
        return false;
    return out >= 3 && out <= 8;
}

static bool readSequence(int *blocks, int &count) {
    std::string line;
    if (!std::getline(std::cin, line))
        return false;
    std::istringstream iss(line);
    count = 0;
    int v = 0;
    while (iss >> v) {
        if (v < 0 || v > 9)
            return false;
        if (count >= kMaxSeq)
            return false;
        blocks[count++] = v;
    }
    return count > 0;
}

static std::string stateString(const int *cache, int n) {
    if (n <= 0)
        return "[]";
    std::ostringstream os;
    os << '[';
    for (int i = 0; i < n; ++i) {
        if (i)
            os << ',';
        os << cache[i];
    }
    os << ']';
    return os.str();
}

//MRU index 0
static void lruAccess(int *cache, int &n, int cap, int x) {
    int pos = -1;
    for (int i = 0; i < n; ++i) {
        if (cache[i] == x) {
            pos = i;
            break;
        }
    }
    if (pos >= 0) {
        for (int i = pos; i > 0; --i)
            cache[i] = cache[i - 1];
        cache[0] = x;
        return;
    }
    if (n >= cap) {
        for (int i = cap - 1; i > 0; --i)
            cache[i] = cache[i - 1];
        cache[0] = x;
    } else {
        for (int i = n; i > 0; --i)
            cache[i] = cache[i - 1];
        cache[0] = x;
        ++n;
    }
}

int main() {
    std::cout << "Cache size [3,8]: ";
    int cap = 0;
    if (!readCacheSize(cap)) {
        std::cerr << "Invalid cache size.\n";
        return 1;
    }
    std::cout << "Block sequence (digits 0-9, space-separated): ";
    int blocks[kMaxSeq];
    int bcount = 0;
    if (!readSequence(blocks, bcount)) {
        std::cerr << "Invalid sequence.\n";
        return 1;
    }
    std::cout << "Output file name: ";
    std::string path;
    if (!std::getline(std::cin, path) || path.empty()) {
        std::cerr << "Invalid file name.\n";
        return 1;
    }

    int cache[8];
    int n = 0;
    std::ostringstream trace;
    trace << "LRU (cache size " << cap << ")\n";
    trace << "Sequence:";
    for (int i = 0; i < bcount; ++i)
        trace << ' ' << blocks[i];
    trace << "\n\n";

    trace << "[]";
    for (int i = 0; i < bcount; ++i) {
        lruAccess(cache, n, cap, blocks[i]);
        trace << " -> " << stateString(cache, n);
    }
    trace << '\n';

    std::ofstream out(path.c_str());
    if (!out) {
        std::cerr << "Could not open output file.\n";
        return 1;
    }
    out << trace.str();
    std::cout << "Wrote trace to " << path << "\n";
    return 0;
}
