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

    std::map<int, struct Command> *getArgs() {
        return &args_;
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
        
        std::map<int, struct Command> *args = getArgs();
        args->insert(std::pair<int, struct Command>(opt, command));
        return 1;
    }

    int isValid(const char *option) {
        if (option != nullptr && strlen(option) == 2 && option[0] == '-') {
            return 1;
        }

        return 0;
    }

    struct Command *find(int option, std::map<int, struct Command> *args) {
        auto iter = args->find(option);
        if (iter == args->end()) {
            return nullptr;
        }

        return &((*iter).second);
    }

    int isExist(int option) {
        return (find(option, getArgs()) != nullptr);
    }

    std::string getOptionString(std::map<int, struct Command> *args) {
        std::string option = "";
        int needParam = 0;
        for (auto iter = args->begin(); iter != args->end(); iter++) {
            option += (char)((*iter).first); 
            needParam = getNeedParam(((*iter).second).type);
            if (needParam) {
                option += ":";
            }
        }

        return option;
    }

    Type getType(int option) {
        struct Command *command = find(option, getArgs());
        return (command != nullptr) ? command->type : ERROR;
    }

    void parse_opt(const char *options) {
        int opt;
        opterr = 0;
        //getopt is not repeatable so we must init getopt before starting
        optind = 1;
        const char *value = nullptr;

        while (-1 != (opt = getopt(argc_, argv_, (char *)options))) {
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

        makeUpForExistsType();
    }

    void makeUpForExistsType() {
        std::map<int, struct Command> *args = getArgs();
        for (auto iter = args->begin(); iter != args->end(); iter++) {
            int opt = (*iter).first;
            if ((*iter).second.type == EXISTS) {
                if (argsMap_.find(opt) == argsMap_.end()) {
                    argsMap_.insert(std::pair<int, std::string>(opt, "false"));
                }
            }
        }
    }

    const std::map<int, std::string> &parse() {
        std::string options;
        options = getOptionString(getArgs());
        parse_opt(options.c_str());
        return argsMap_;
    }

private:
    std::map<int, struct Command> args_; 
    std::map<int, std::string> argsMap_; 
    char **argv_;
    int argc_;
};

}
