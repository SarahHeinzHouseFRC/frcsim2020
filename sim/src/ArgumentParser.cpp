/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include "ArgumentParser.h"

using namespace std;


ArgumentParser::ArgumentParser(int argc, char **argv)
{
    if (argc < 2) { return; }

    // Store into straight vector
    vector<string> args(argc);
    for (int i=1; i<argc; i++)
    {
        args.emplace_back(argv[i]);
    }

    // Store into map
    for (int i=0; i<args.size(); i++)
    {
        if (isFlag(args[i]))
        {
            // Assume flags only ever have one following value
            string flag = args[i];
            string value;
            int j = i+1;
            if (j < args.size() && !isFlag(args[j]))
            {
                value = args[j];
                i++;
            }
            _parsedArgs.insert({ flag, value });
        }
    }
}
