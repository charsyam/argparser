#pragma once

#include <map>
#include <string>

namespace utils {

class ArgParser {
public:
    enum Type {
        EXISTS,
        STRING,
        BOOLEAN,
        ERROR
    };

    struct Command {
        Type type;
        std::string description;
    };

    ArgParser(int argc, char **argv) : argc_(argc), argv_(argv) {
    }

    std::map<int, struct Command> *getArgs(Type type) {
        if (type == STRING) {
            return &string_args_;
        } else if (type == BOOLEAN) {
            return &boolean_args_;
        } 

        return &exists_args_;
    }

    int getNeedParam(Type type) {
        return (type == EXISTS) ? 0 : 1;
    }

    int add(const char *option, Type type, const char *desc) {
        if (isValid(option) == 0) {
            return 0;
        }

        int opt = (int)(option[1]);
        if (isExist(opt) == 1) {
            return 0;
        }

        struct Command command;
        command.type = type;
        command.description = desc; 
        
        std::map<int, struct Command> *args = getArgs(type);
        args->insert(std::pair<int, struct Command>(opt, command));
        return 1;
    }

    int isValid(const char *option) {
        if (option != nullptr && strlen(option) == 2 && option[0] == '-') {
            return 1;
        }

        return 0;
    }

    int find(int option, std::map<int, struct Command> *args) {
        if (args->find(option) == args->end()) {
            return 0;
        }

        return 1;
    }

    Type getType(int option) {
        if (find(option, getArgs(STRING)) == 1) {
            return STRING;
        } 
        if (find(option, getArgs(BOOLEAN)) == 1) {
            return BOOLEAN;
        } 
        if (find(option, getArgs(EXISTS)) == 1) {
            return EXISTS;
        } 

        //not reached
        return ERROR;
    }

    int isExist(int option) {
        if (find(option, getArgs(STRING)) == 1) {
            return 1;
        } 
        if (find(option, getArgs(BOOLEAN)) == 1) {
            return 1;
        } 
        if (find(option, getArgs(EXISTS)) == 1) {
            return 1;
        } 
    
        return 0;
    }

    std::string getOptionString(std::map<int, struct Command> *args, int needParam) {
        std::string option;
        for (auto iter = args->begin(); iter != args->end(); iter++) {
            option += (char)((*iter).first); 
            if (needParam) {
                option += ":";
            }
        }

        return option;
    }

    void parse_opt(const char *options) {
        int opt;
        opterr = 0;
        const char *value = nullptr;

        while (-1 != (opt = getopt(argc_, argv_, options))) {
            Type type = getType(opt);
            switch (type) {
                case STRING:
                    value = optarg;
                    break;
                case BOOLEAN:
                    {
                        value = "false";
                        if (strcasecmp(optarg, "true") == 0) {
                            value = "true";
                        }
                    }
                    break;
                case EXISTS:
                    {
                        value = "true";
                    }
                    break;
                default:
                    //not reached
                    break;
            }

            argsMap_.insert(std::pair<int, std::string>(opt, value));
        } 

        makeUpForBooleanType();
    }

    void makeUpForBooleanType() {
        std::map<int, struct Command> *args = getArgs(EXISTS);
        for (auto iter = args->begin(); iter != args->end(); iter++) {
            int opt = (*iter).first;
            if (argsMap_.find(opt) == argsMap_.end()) {
                argsMap_.insert(std::pair<int, std::string>(opt, "false"));
            }
        }
    }

    void parse() {
        std::string options;
        options += getOptionString(getArgs(STRING), getNeedParam(STRING));
        options += getOptionString(getArgs(BOOLEAN), getNeedParam(BOOLEAN));
        options += getOptionString(getArgs(EXISTS), getNeedParam(EXISTS));
        parse_opt(options.c_str());
    }

    const std::map<int, std::string> &getResult() {
        return argsMap_;
    }

private:
    std::map<int, struct Command> boolean_args_; 
    std::map<int, struct Command> string_args_; 
    std::map<int, struct Command> exists_args_; 
    std::map<int, std::string> argsMap_; 
    char **argv_;
    int argc_;
};

}
