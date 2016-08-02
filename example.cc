#include "argparser.h"

int main(int argc, char *argv[]) {
    utils::ArgParser argParser(argc, argv);
    argParser.add("-m", utils::ArgParser::STRING, "memory");
    argParser.add("-d", utils::ArgParser::EXISTS, "daemon");
    argParser.add("-t", utils::ArgParser::BOOLEAN, "daemon");
    argParser.parse();
    return 0;
}
