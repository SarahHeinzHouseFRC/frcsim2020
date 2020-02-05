/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_ARGUMENTPARSER_H
#define ROBOT_SIM_ARGUMENTPARSER_H

#include <map>
#include <vector>
#include <string>


/**
 * Simple argument parser which assumes argument list consists of flags (which start with "-" or "--") and values (which
 * start with any other character). This parser assumes each flag only has 0 or 1 values immediately succeeding it
 * separated by a space. For example, the following is supported:
 *
 * ./robot_sim --verbose --debug-view --config robotConfig.yml
 *
 * But the following are not:
 *
 * ./robot_sim --initial-pose 0 5 --verbose
 * ./robot_sim --config=robotConfig.yml
 *
 * In the second case, 5 will simply be skipped over since it is not supported. Additionally, this parse does not
 * complain about unrecognized arguments. E.g., if the user passes --erbose instead of --verbose, the application will
 * launch with the default value for verbosity.
 */
class ArgumentParser
{
public:
    /**
     * Constructor from argument list
     */
    ArgumentParser(int argc, char** argv);

    /**
     * Returns whether or not the given flag was in the argument list
     */
    bool contains(const std::string& flag) { return (_parsedArgs.find(flag) != _parsedArgs.end()); }

    /**
     * Returns the value associated with the given flag
     */
    std::string getValue(const std::string& flag) { return contains(flag) ? _parsedArgs[flag] : ""; }

private:
    /**
     * Determines whether the given string is a flag or a value
     */
    bool isFlag(const std::string& flag) { return flag[0] == '-'; }

    std::map<std::string, std::string> _parsedArgs; // Pairs of flags and values
};


#endif //ROBOT_SIM_ARGUMENTPARSER_H
