#include <iostream>
#include <gtest/gtest.h>
#include "../argparser.h"

TEST(ArgParserTest1, StringOnlyTest) {
    int argc = 3;
    char *argv[] = {
        (char *)"gtest",
        (char *)"-m",
        (char *)"64m",
        nullptr
    };

    utils::ArgParser argParser(argc, argv);
    argParser.add("-m", utils::ArgParser::STRING, "memory");

    const std::map<int, std::string> &argsMap = argParser.parse();
    EXPECT_EQ(1, argsMap.size());
    auto iter = argsMap.begin();
    EXPECT_EQ((int)'m', (*iter).first);
    EXPECT_EQ("64m", (*iter).second);
}

TEST(ArgParserTest, BooleanOnlyTest1) {
    int argc = 3;
    char *argv[] = {
        (char *)"gtest",
        (char *)"-t",
        (char *)"true",
        nullptr
    };

    utils::ArgParser argParser(argc, argv);
    argParser.add("-t", utils::ArgParser::BOOLEAN, "type");
    const std::map<int, std::string> &argsMap = argParser.parse();

    EXPECT_EQ(1, argsMap.size());
    auto iter = argsMap.begin();
    EXPECT_EQ((int)'t', (*iter).first);
    EXPECT_EQ("true", (*iter).second);
}

TEST(ArgParserTest, BooleanOnlyTest2) {
    int argc = 3;
    char *argv[] = {
        (char *)"gtest",
        (char *)"-t",
        (char *)"false",
        nullptr
    };

    utils::ArgParser argParser(argc, argv);
    argParser.add("-t", utils::ArgParser::BOOLEAN, "type");

    const std::map<int, std::string> &argsMap = argParser.parse();
    EXPECT_EQ(1, argsMap.size());
    auto iter = argsMap.begin();
    EXPECT_EQ((int)'t', (*iter).first);
    EXPECT_EQ("false", (*iter).second);
}

TEST(ArgParserTest, ExistsOnlyTest1) {
    int argc = 2;
    char *argv[] = {
        (char *)"gtest",
        (char *)"-d",
        nullptr
    };

    utils::ArgParser argParser(argc, argv);
    argParser.add("-d", utils::ArgParser::EXISTS, "type");

    const std::map<int, std::string> &argsMap = argParser.parse();
    EXPECT_EQ(1, argsMap.size());
    auto iter = argsMap.begin();
    EXPECT_EQ((int)'d', (*iter).first);
    EXPECT_EQ("true", (*iter).second);
}

TEST(ArgParserTest, ExistsOnlyTest2) {
    int argc = 1;
    char *argv[] = {
        (char *)"gtest",
        nullptr
    };

    utils::ArgParser argParser(argc, argv);
    argParser.add("-d", utils::ArgParser::EXISTS, "type");

    const std::map<int, std::string> &argsMap = argParser.parse();
    EXPECT_EQ(1, argsMap.size());
    auto iter = argsMap.begin();
    EXPECT_EQ((int)'d', (*iter).first);
    EXPECT_EQ("false", (*iter).second);
}

static std::string find(int option, const std::map<int, std::string> *args) {
    auto iter = args->find(option);
    if (iter == args->end()) {
        return "";
    }

    return ((*iter).second);
}

TEST(ArgParserTest, CombineTest1) {
    int argc = 5;
    char *argv[] = {
        (char *)"gtest",
        (char *)"-m",
        (char *)"64m",
        (char *)"-t",
        (char *)"true",
        nullptr
    };

    utils::ArgParser argParser(argc, argv);
    argParser.add("-m", utils::ArgParser::STRING, "memory");
    argParser.add("-t", utils::ArgParser::BOOLEAN, "type");
    const std::map<int, std::string> &argsMap = argParser.parse();

    EXPECT_EQ(2, argsMap.size());

    std::string value = ::find((int)'m', &argsMap);
    EXPECT_EQ("64m", value);

    value = ::find((int)'t', &argsMap);
    EXPECT_EQ("true", value);
}

TEST(ArgParserTest, CombineTest2) {
    int argc = 4;
    char *argv[] = {
        (char *)"gtest",
        (char *)"-m",
        (char *)"64m",
        (char *)"-d",
        nullptr
    };

    utils::ArgParser argParser(argc, argv);
    argParser.add("-m", utils::ArgParser::STRING, "memory");
    argParser.add("-d", utils::ArgParser::EXISTS, "type");
    const std::map<int, std::string> &argsMap = argParser.parse();

    EXPECT_EQ(2, argsMap.size());

    std::string value = ::find((int)'m', &argsMap);
    EXPECT_EQ("64m", value);

    value = ::find((int)'d', &argsMap);
    EXPECT_EQ("true", value);
}

TEST(ArgParserTest, CombineTest3) {
    int argc = 3;
    char *argv[] = {
        (char *)"gtest",
        (char *)"-m",
        (char *)"64m",
        nullptr
    };

    utils::ArgParser argParser(argc, argv);
    argParser.add("-m", utils::ArgParser::STRING, "memory");
    argParser.add("-d", utils::ArgParser::EXISTS, "type");
    const std::map<int, std::string> &argsMap = argParser.parse();

    EXPECT_EQ(2, argsMap.size());

    std::string value = ::find((int)'m', &argsMap);
    EXPECT_EQ("64m", value);

    value = ::find((int)'d', &argsMap);
    EXPECT_EQ("false", value);
}
