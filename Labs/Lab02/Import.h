#ifndef IMPORT_H
#define IMPORT_H

#include "IAS.h"
#include <fstream>
#include <sstream>

namespace osl
{
    void Import(IAS& machine, std::string filename)
    {
        std::ifstream file(filename);
        std::string line;
        int cellIndex = 0;

        // read file line by line, stop at 4096 cells
        while(cellIndex < 4096 && std::getline(file, line))
        {
            // stop if line is empty, too long, or not valid hex
            if(line.empty() || line.length() > 10 || !isHex(line)) { break; }

            // convert hex to binary and write to memory cell
            std::istringstream ss(line);
            ss >> machine[cellIndex];

            // activate cell only if value is non-zero
            std::string cellValue = machine[cellIndex].read();
            if(cellValue != std::string(40, '0')) { machine[cellIndex].change(true); }

            cellIndex += 1;
        }
    }
}

#endif
