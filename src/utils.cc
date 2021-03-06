#include <set>
#include <string>
#include <vector>
#include <random>
#include <fstream>
#include <iostream>
#include <algorithm>

#define ADD "add"
#define REM "rem"
#define SEARCH "search"

enum COLOR { RED, GREEN, BLUE, GRAY};

std::string string_color(const std::string &s, COLOR color = GRAY) {
  std::string ret;
  if (color == RED)
    ret = "\033[0;31m";
  if (color == GREEN)
    ret = "\033[0;32m";
  if (color == BLUE)
    ret = "\033[0;34m";
  if (color == GRAY)
    ret = "\033[0;37m";

  return ret + s + "\033[0m";

}

bool file_exists(const std::string &filename) {
  // std::cout << "Looking for " << filename << std::endl;
  std::ifstream ifile(filename, std::ifstream::binary);
  bool good = !ifile.fail();
  ifile.close();
  return good;
}

class fenwick_tree {
  std::vector<long long> v;
  int maxSize;
  public:
  fenwick_tree () {}
  fenwick_tree(int _maxSize) : maxSize(_maxSize+1) {
    v = std::vector<long long>(maxSize, 0LL);
  }
  void add(int where, long long what) {
    for (where++; where <= maxSize; where += where & -where){
      v[where] += what;
    }
  }
  long long query(int where) {
    long long sum = v[0];
    for (where++; where > 0; where -= where & -where){
      sum += v[where];
    }
    return sum;
  }
  long long query(int from, int to) {
    return query(to) - query(from-1);
  }
};

namespace totient {
  class entry {
    public:
      std::string tracker_url, name;
      int piece_length, length;
      size_t total;
      std::vector<std::string> pieces;
      std::set<std::string> downloaded;

      entry() {}

      entry (const std::string &filename) {
        std::ifstream totient_file(filename);
        totient_file >> tracker_url >> name >> piece_length >> length;
        total = (length + piece_length - 1 ) / piece_length;
        pieces.resize(total);
        for (size_t i = 0; i < pieces.size(); ++i)
          totient_file >> pieces[i];
      }

      std::string next() {

        if (pieces.size() == 0)
          return "";

        std::random_device generator;
        std::uniform_int_distribution<int> distribution(0, pieces.size() - 1);
        int index = distribution(generator);
        std::swap(pieces[index], pieces[pieces.size() - 1]);
        std::string ans = pieces.back();
        pieces.pop_back();
        return ans;
      }

      void add_piece(std::string hash) {
        downloaded.insert(hash);
      }

      bool finish() {
        return downloaded.size() == total;
      }
  };

}
/*
   namespace tracker {

   struct peer {
   std::string id, ip, port;
   peer() {}
   std::string encode() {}
   static peer decode(const std::string &benc) {}
   };

   struct request {
   std::string info_hash, peer_id, ip, port;
   std::string encode() {}
   static request decode(const std::string &benc) {}
   };

   struct response {
   int interval;
   std::vector<peer> peers;
   std::string encode() {}
   static request decode(const std::string &benc) {}
   };
   }

   namespace totient {

   struct metainfo {
   std::string announce, name, pieces;
   int piece_lenght, length;
   metainfo() {}
   std::string encode() {}
   static metainfo decode(const std::string &benc) {}
   };
   }
   */
