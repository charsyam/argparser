#include <iostream>
#include "argparser.h"

int main(int argc, char *argv[]) {
    utils::ArgParser argParser(argc, argv);
    argParser.add("-m", utils::ArgParser::STRING, "memory");
    argParser.add("-d", utils::ArgParser::EXISTS, "daemon");
    argParser.add("-t", utils::ArgParser::BOOLEAN, "daemon");
    argParser.parse();

    const std::map<int, std::string> &argsMap = argParser.getResult();
    for (auto iter = argsMap.begin(); iter != argsMap.end(); iter++) {
        int opt = (*iter).first;
        std::string v = (*iter).second;
        std::cout << (char)opt << " : " << v << std::endl;
    }

    return 0;
}
