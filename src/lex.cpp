#include <iostream>
#include <fstream>
#include <string>




bool lex(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "You should provide a file name." << std::endl;
        return false;
    }

    std::ifstream input_file(argv[1]);
    if (!input_file)
    {
        std::cerr << "I can't read " << argv[1] << std::endl;
        return false;
    }
    
    else
    {
        std::cout << "Read input file '" << argv[1] << "' successfully" << std::endl;
    }

	std::string output_file_name = std::string(argv[1])+".tokens";
	std::ofstream output_file(output_file_name.c_str());
	if (!output_file)
    {
        std::cerr << "I can't write " << argv[1] << ".tokens" << std::endl;
        return false;
    }

    std::string line;
    for (int line_no = 1; std::getline(input_file, line); ++line_no)
    {
		for (size_t i = 0; i < line.size();)
		{
			// comments
			if (line[i] == '/')
			{
				++i;
				if ((i == line.size()) || (line[i] != '/'))
				{
					std::cerr << "LINE " << line_no
						<< ": a single / is not allowed" << std::endl;
					return -1;
				}
				break; // skip the rest of the line by exiting the loop
			}

			// spaces
			if ((line[i] == ' ') || (line[i] == '\t')
				|| (line[i] == '\r') || (line[i] == '\n'))
			{
				++i; // skip this space character
				continue; // skip the rest of the iteration
			}

			// SINGLE
			if ((line[i] == '(') || (line[i] == ')')
				|| (line[i] == '[') || (line[i] == ']')
				|| (line[i] == ':') || (line[i] == ';')
				|| (line[i] == ',') || (line[i] == '='))
			{
				output_file << "SINGLE " << line[i] << std::endl;
				++i; // we consumed this character
				continue; // skip the rest of the iteration
			}

			// NAME
			if (((line[i] >= 'a') && (line[i] <= 'z'))       // a to z
				|| ((line[i] >= 'A') && (line[i] <= 'Z'))    // A to Z
				|| (line[i] == '_'))
			{
				size_t name_begin = i;
				for (++i; i < line.size(); ++i)
				{
					if (!(((line[i] >= 'a') && (line[i] <= 'z'))
						|| ((line[i] >= 'A') && (line[i] <= 'Z'))
						|| ((line[i] >= '0') && (line[i] <= '9'))
						|| (line[i] == '_') || (line[i] == '$')))
					{
						break; // [name_begin, i) is the range for the token
					}
				}
				output_file << "NAME "
					<< line.substr(name_begin, i-name_begin) << std::endl;
				continue;
			}

			// NUMBER
			if ((line[i] >= '0')&&(line[i] <= '9'))
			{
				size_t number_begin = i;
				for (++i; i < line.size();++i)
				{
					if(!((line[i] >= '0')&&(line[i] <= '9')))
					{
						break;
					}
				}
				output_file << "NUMBER " 
					<< line.substr(number_begin, i - number_begin) << std::endl;
			}


		    else
			{
				std::cerr << "LINE " << line_no
					<< ": invalid character '" << line[i] << "'" << std::endl;
				return false;
			}
		}
	}

    return true;
}
