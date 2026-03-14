#include "Import.h"
#include "Decode.h"
#include <fstream>

using namespace osl;

void simulate(IAS& machine, std::ofstream& logStream)
{
    try
    {
        while(true)
        {
            //Fetch Left Instruction

            // MAR <- PC
            machine(3).write(machine(0).read());
            logStream << machine << "\n";

            // MBR <- M(MAR)
            machine(4).write(machine[toDecimal(machine(3).read())].read());
            logStream << machine << "\n";

            // IBR <- MBR[20:39]
            machine(2).write(machine(4).read().substr(20, 20));
            logStream << machine << "\n";

            // IR <- MBR[0:7]
            machine(1).write(machine(4).read().substr(0, 8));
            logStream << machine << "\n";

            // MAR <- MBR[8:19]
            machine(3).write(machine(4).read().substr(8, 12));
            logStream << machine << "\n";

            // PC <- PC + 1
            std::string inc = toBinary(toDecimal(machine(0).read()) + 1);
            machine(0).write(std::string(12 - inc.length(), '0') + inc);
            logStream << machine << "\n";

            // execute left instruction
            Decode::execute(machine);
            logStream << machine << "\n";

            //Fetch Right Instruction

            // IR <- IBR[0:7]
            machine(1).write(machine(2).read().substr(0, 8));
            logStream << machine << "\n";

            // MAR <- IBR[8:19]
            machine(3).write(machine(2).read().substr(8, 12));
            logStream << machine << "\n";

            // execute right instruction
            Decode::execute(machine);
            logStream << machine << "\n";
        }
    }
    catch(std::runtime_error&)
    {
        // HLT / end of simulation
    }
}

int main()
{
    IAS machine;
    std::ofstream logStream("output.txt");

    Import(machine, "program.hex");
    simulate(machine, logStream);

    return 0;
}
