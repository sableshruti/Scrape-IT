#include <iostream>
#include <fstream>
#include <set>
#include <chrono>

using std::string;
using std::time_t;

struct record {
    int score;
    time_t when;
    string name;
    // higher up (less) if score is higher, or scores are equal but when is earlier
    bool operator< (const record& r) const { return (score > r.score || (score == r.score && when < r.when)) ; }
    void setTime() {
        typedef std::chrono::system_clock clock;
        when = clock::to_time_t(clock::now());
    }
};

istream& operator>> (istream& in, record& r) {
    in >> r.score >> r.when;
    std::getline(in >> std::ws, r.name);
 if(r.name=="") r.name = "-";
    return in;
}

ostream& operator<< (ostream& out, const record& r) {
    out << r.score << " " <<  r.when << " " << r.name << std::endl;
    return out;
}

class history {
    string filename; 
    int maxsize;
    std::set<record> records; // read from the file into this set
public:
    history(const string& fname, int n=10) : filename(fname), maxsize(n) {
        // if file exists, read it into the set records
        std::ifstream infile (filename);
        while(infile) {
            record r;
            infile >> r;
            if(infile) 
                records.insert(r);
        }
        if(records.size() > maxsize)  // we will not shrink the size
            maxsize = records.size();
    }

    ~history() { flush(); }

    void flush() {
        std::ofstream outfile (filename,ios::trunc);
        if(outfile) 
            for (auto& r : records) 
                outfile << r;
  if(!outfile)
   cerr << "Error writing to file " << filename << endl;
    }

    void addRecord(record& r) {
        records.insert(r);
        if(records.size() > maxsize)
            records.erase(--records.end());
    }

    bool eligible(int score) {
        return (records.size() < maxsize || (records.rbegin())->score < score);
    }
};