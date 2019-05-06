#include "BloomFilter.h"
#include "HashSet.h"

#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

void usage() {
  std::cerr << "USAGE: ./bloom [options] <file1> [file2]\n\n";
  std::cerr << "If two file names are given, all lines from the first file are inserted and all lines from the second file are looked up.\n";
  std::cerr << "If only one file name is given, the first 1,000 lines are inserted and then all lines are looked up.\n\n";
  std::cerr << "Available options:\n";
  std::cerr << "  -c      Disable coloration of the output.\n";
  std::cerr << "  -f      Print only the false positive rate (overrides -t).\n";
  std::cerr << "  -i <I>  Use method I to hash (reduce) integers.  Methods are:\n";
  std::cerr << "            division   - use the division method\n";
  std::cerr << "            reciprocal - use the multiplication method with fractions\n";
  std::cerr << "            squareroot - use the multiplication method with square roots (default)\n";
  std::cerr << "  -k <K>  Use K hash functions in the Bloom filter (default = 10).\n";
  std::cerr << "  -m <M>  Use M bits in the Bloom filter (default = 15,000).\n";
  std::cerr << "  -n <N>  Insert at most N lines from file1.\n";
  std::cerr << "  -s <S>  Use method S to hash strings to integers.  Methods are:\n";
  std::cerr << "            jenkins - use Jenkins' one-at-a-time hashing (default)\n";
  std::cerr << "            pearson - use 8-bit Pearson hashing\n";
  std::cerr << "  -t      Print the summary in tabular (CSV) format.\n";
  std::cerr << "  -v      Increase verbosity (may be repeated).\n";
}


int parseint(char opt, std::string arg) {
  try {
    /*
     * size_t是一种“整型”类型，里面保存的是一个整数，就像int, long那样。它这种整数用来记录一个大小(size)。
     * size_t的全称应该是size type，就是说“一种用来记录大小的数据类型”。
     */
    size_t index;
    //stoi（字符串，起始位置，2~32进制），将n进制的字符串转化为十进制。
    int result = std::stoi(arg, &index);
    if(index == arg.length() && result > 0) {
      return result;
    }
  }
  catch(std::exception&) {
    // Fall through.
  }

  std::cerr << "Option -" << opt << " requires a positive integer argument.\n\n";
  usage();
  exit(1);
}

std::string color(std::string text, int code, bool c) {
  if(c) return "\e[" + std::to_string(code) + "m" + text + "\e[0m";
  return text;
}

int main(int argc, char** argv) {
  const char* i = "squareroot";
  const char* s = "jenkins";

  bool c = true;
  bool f = false;
  int  k = 10;
  int  m = 15000;
  int  n = 0;
  bool t = false;
  int  v = 0;
  int  opt;

/**
 * getopt()用来分析命令行参数。参数argc和argv分别代表参数个数和内容，跟main（）函数的命令行参数是一样的。
 * 参数optstring为选项字符串， 告知getopt()可以处理哪个选项以及哪个选项需要参数，如果选项字符串里的字母后接着冒号“:”，
 * 则表示还有相关的参数，全域变量optarg 即会指向此额外参数。
 * 如果在处理期间遇到了不符合optstring指定的其他选项getopt()将显示一个错误消息，并将全域变量optopt设为“?”字符，
 * 如果不希望getopt()打印出错信息，则只要将全域变量opterr设为0即可。
 */

  while((opt = getopt(argc, argv, "cfi:k:m:n:s:tv")) != -1) {
    switch(opt) {
    case 'c':
      c = false;
      break;
    case 'f':
      f = true;
      break;
    case 'i':
      i = optarg;
      break;
    case 'k':
      k = parseint('k', optarg);
      break;
    case 'm':
      m = parseint('m', optarg);
      break;
    case 'n':
      n = parseint('n', optarg);
      break;
    case 's':
      s = optarg;
      break;
    case 't':
      t = true;
      break;
    case 'v':
      v += 1;
      break;
    case '?':
      std::cerr << "Unknown option -" << optopt << "\n\n";
      usage();
      exit(1);
    default:
      std::cerr << "Something has gone very wrong...\n\n";
      exit(1);
    }
  }

  const char* storefile;
  const char* checkfile;

/*
 * 在处理命令行参数时，用到一个变量 optind, 原来是系统定义的。可以在命令行中，通过 man optind 来看相关信息
 * optind: the index of the next element to be processed in the argv.  The system initializes it to 1.
 * The caller can reset it to 1 to restart scanning of the same argv or scanning a new argument vector.
 * 当调用 getopt() , getopt_long() 之类的函数时， optind 的值会变化。如：
 * 执行 $  ./a.out -ab         当调用  一次  getopt() , 则 optind 值会 +1
 */
  if(optind == argc - 2) {
    storefile = argv[optind];
    checkfile = argv[optind + 1];
  }
  else if(optind == argc - 1) {
    if(n == 0) n = 1000;
    storefile = argv[optind];
    checkfile = argv[optind];
  }
  else {
    std::cerr << "Exactly one or two filenames are required.\n\n";
    usage();
    exit(1);
  }

  const std::string ADDL = "[" + color("INSERT", 34, c) + "] ";
  const std::string TPOS = "[" + color("T. POS", 32, c) + "] ";
  const std::string TNEG = "[" + color("T. NEG", 36, c) + "] ";
  const std::string FPOS = "[" + color("F. POS", 33, c) + "] ";
  const std::string FNEG = "[" + color("F. NEG", 31, c) + "] ";

  std::string line;
  BloomFilter filter(k, m, s, i);
  HashSet hashset;

  int nstores = 0;
  int nchecks = 0;
  int tpos = 0;
  int tneg = 0;
  int fpos = 0;
  int fneg = 0;

  // Insert lines from storefile
  std::ifstream store(storefile);
  if(store.fail()) {
    std::cerr << "Unable to open file: " << storefile << '\n';
    exit(1);
  }

  while(std::getline(store, line)) {
    if(v > 1) std::cout << ADDL << line << '\n';
    hashset.insert(line);
    filter.insert(line);

    nstores += 1;
    if(nstores == n) {
      break;
    }
  }

  if(v > 1) std::cout << '\n';
  store.close();

  // Look up lines from checkfile
  std::ifstream check(checkfile);
  if(check.fail()) {
    std::cerr << "Unable to open file: " << checkfile << '\n';
    exit(1);
  }

  while(std::getline(check, line)) {
    bool sfound = hashset.lookup(line);
    bool ffound = filter.lookup(line);
    nchecks += 1;

    if(sfound) {
      if(ffound) {
        if(v > 0) std::cout << TPOS << line << '\n';
        tpos += 1;
      }
      else {
        if(v > 0) std::cout << FNEG << line << '\n';
        fneg += 1;
      }
    }
    else {
      if(ffound) {
        if(v > 0) std::cout << FPOS << line << '\n';
        fpos += 1;
      }
      else {
        if(v > 0) std::cout << TNEG << line << '\n';
        tneg += 1;
      }
    }
  }

  if(v > 0) std::cout << '\n';
  check.close();

  // Summarize the results
  if(f) {
    std::cout << double(fpos) / (fpos + tneg) << '\n';
  }
  else if(t) {
    std::cout << nstores << ',';
    std::cout << nchecks << ',';
    std::cout << tpos << ',';
    std::cout << tneg << ',';
    std::cout << fpos << ',';
    std::cout << fneg << '\n';
  }
  else {
    std::cout << "Inserted " << nstores << " items.\n";
    std::cout << "Looked up " << nchecks << " items.\n";
    std::cout << " - True Positives:  " << color(std::to_string(tpos), 32, c) << '\n';
    std::cout << " - True Negatives:  " << color(std::to_string(tneg), 36, c) << '\n';
    std::cout << " - False Positives: " << color(std::to_string(fpos), 33, c) << '\n';
    std::cout << " - False Negatives: " << color(std::to_string(fneg), 31, c) << '\n';
  }

  return 0;
}
