#ifndef DECODE_H
#define DECODE_H

#include "IAS.h"

namespace osl
{
    class Decode
    {
        private:
        static void LMA(IAS& machine)
        {
            //AC = MQ => machine(5) <- machine(6)
            machine(5).write(machine(6).read());
        }

        static void LDM(IAS& machine)
        {
            //MAR: 3
            //MQ = M(MAR)
            int addr = toDecimal(machine(3).read());
            machine(6).write(machine[addr].read());
        }

        static void STA(IAS& machine)
        {
            // STA: M(MAR) <- AC
            int addr = toDecimal(machine(3).read());
            machine[addr].write(machine(5).read());
        }

        static void LDA(IAS& machine)
        {
            // LDA: AC <- M(MAR)
            int addr = toDecimal(machine(3).read());
            machine(5).write(machine[addr].read());
        }

        static void LDN(IAS& machine)
        {
            // LDN: AC <- -M(MAR)
            int addr = toDecimal(machine(3).read());
            machine(5).write(negate(machine[addr].read()));
        }

        static void ALD(IAS& machine)
        {
            // ALD: AC <- |M(MAR)|
            int addr = toDecimal(machine(3).read());
            std::string valueStr = machine[addr].read();

            if(valueStr[0] == '1') {valueStr = negate(valueStr);}
            machine(5).write(valueStr);
        }

        static void ALN(IAS& machine)
        {
            // ALN: AC <- -|M(MAR)|
            int addr = toDecimal(machine(3).read());
            std::string valueStr = machine[addr].read();

            if(valueStr[0] == '1'){valueStr = negate(valueStr);}
            if(valueStr.find('1') != std::string::npos) {valueStr = negate(valueStr);}
            machine(5).write(valueStr);
        }

        static void BRL(IAS& machine)
        {
            // BRL: PC <- MAR; IR <- left(M(PC))
            int addr = toDecimal(machine(3).read());
            machine(0).write(machine(3).read());

            std::string word = machine[addr].read();
            machine(4).write(word);

            std::string left  = word.substr(0, 20);
            std::string right = word.substr(20, 20);

            machine(1).write(left.substr(0, 8));  
            machine(2).write(right);
        }

        static void BRR(IAS& machine)
        {
            // BRR: PC <- MAR; IR <- right(M(PC))
            int addr = toDecimal(machine(3).read());
            machine(0).write(machine(3).read());

            std::string word = machine[addr].read();
            machine(4).write(word);

            std::string right = word.substr(20, 20);

            machine(1).write(right.substr(0, 8));
            machine(2).write(std::string(20,'0'));
        }


        static void BPL(IAS& machine)
        {
            // BPL: if AC >= 0: PC <- MAR; IR <- left(M(PC))
            if(machine(5).read()[0] == '0')
            {
                int addr = toDecimal(machine(3).read());
                machine(0).write(machine(3).read());

                std::string word = machine[addr].read();
                machine(4).write(word);

                std::string left  = word.substr(0, 20);
                std::string right = word.substr(20, 20);

                machine(1).write(left.substr(0, 8));
                machine(2).write(right);
            }
        }

        static void BPR(IAS& machine)
        {
            // BPR: if AC >= 0: PC <- MAR; IR <- right(M(PC))
            if(machine(5).read()[0] == '0')
            {
                int addr = toDecimal(machine(3).read());
                machine(0).write(machine(3).read());

                std::string word = machine[addr].read();
                machine(4).write(word);

                std::string left  = word.substr(0, 20);
                std::string right = word.substr(20, 20);

                machine(1).write(right.substr(0, 8));
                machine(2).write(std::string(20,'0'));
            }
        }

        static void ADD(IAS& machine)
        {
            // ADD: AC <- AC + M(MAR)
            int addr = toDecimal(machine(3).read());

            std::string memStr = machine[addr].read();
            std::string acStr  = machine(5).read();

            long long mem = std::stoll(memStr, nullptr, 2);
            if(memStr[0] == '1') mem -= (1LL << 40);

            long long ac = std::stoll(acStr, nullptr, 2);
            if(acStr[0] == '1') ac -= (1LL << 40);

            long long sum = ac + mem;

            sum &= ((1LL << 40) - 1);

            std::string resultStr(40, '0');
            for(int j = 39; j >= 0; --j)
            {
                resultStr[j] = (sum & 1) ? '1' : '0';
                sum >>= 1;
            }

            machine(5).write(resultStr);
        }

        static void AAD(IAS& machine)
        {
            // AAD: AC <- AC + |M(MAR)|
            int addr = toDecimal(machine(3).read());

            std::string memStr = machine[addr].read();
            std::string acStr  = machine(5).read();

            long long mem = std::stoll(memStr, nullptr, 2);
            if(memStr[0] == '1')
                mem -= (1LL << 40);

            long long ac = std::stoll(acStr, nullptr, 2);
            if(acStr[0] == '1')
                ac -= (1LL << 40);

            if(mem < 0)
                mem = -mem;

            long long sum = ac + mem;

            sum &= ((1LL << 40) - 1);

            std::string resultStr(40, '0');
            for(int j = 39; j >= 0; --j)
            {
                resultStr[j] = (sum & 1) ? '1' : '0';
                sum >>= 1;
            }

            machine(5).write(resultStr);
        }

        static void SUB(IAS& machine)
        {
            // SUB: AC <- AC - M(MAR)
            int addr = toDecimal(machine(3).read());

            std::string memStr = machine[addr].read();
            std::string acStr  = machine(5).read();

            long long mem = std::stoll(memStr, nullptr, 2);
            if(memStr[0] == '1')
                mem -= (1LL << 40);

            long long ac = std::stoll(acStr, nullptr, 2);
            if(acStr[0] == '1')
                ac -= (1LL << 40);

            long long diff = ac - mem;

            diff &= ((1LL << 40) - 1);

            std::string resultStr(40, '0');
            for(int j = 39; j >= 0; --j)
            {
                resultStr[j] = (diff & 1) ? '1' : '0';
                diff >>= 1;
            }

            machine(5).write(resultStr);
        }

        static void ASB(IAS& machine)
        {
            // ASB: AC <- AC - |M(MAR)|
            int addr = toDecimal(machine(3).read());

            std::string memStr = machine[addr].read();
            std::string acStr  = machine(5).read();

            long long mem = std::stoll(memStr, nullptr, 2);
            if(memStr[0] == '1')
                mem -= (1LL << 40);

            long long ac = std::stoll(acStr, nullptr, 2);
            if(acStr[0] == '1')
                ac -= (1LL << 40);

            if(mem < 0)
                mem = -mem;

            long long diff = ac - mem;

            diff &= ((1LL << 40) - 1);

            std::string resultStr(40, '0');
            for(int j = 39; j >= 0; --j)
            {
                resultStr[j] = (diff &  1) ? '1' : '0';
                diff >>= 1;
            }

            machine(5).write(resultStr);
        }

        static void MUL(IAS& machine)
        {
            // MUL: AC:MQ <- MQ * M(MAR)
            int addr = toDecimal(machine(3).read());

            std::string memStr = machine[addr].read();
            std::string mqStr  = machine(6).read();

            long long mem = std::stoll(memStr, nullptr, 2);
            if(memStr[0] == '1')
                mem -= (1LL << 40);

            long long mq = std::stoll(mqStr, nullptr, 2);
            if(mqStr[0] == '1')
                mq -= (1LL << 40);

            long long product = mem * mq;

            long long highPart = (product >> 40);
            long long lowPart  = product & ((1LL << 40) - 1);

            highPart &= ((1LL << 40) - 1);

            std::string highStr(40, '0');
            std::string lowStr(40, '0');

            for(int j = 39; j >= 0; --j)
            {
                highStr[j] = (highPart & 1) ? '1' : '0';
                highPart >>= 1;

                lowStr[j] = (lowPart & 1) ? '1' : '0';
                lowPart >>= 1;
            }

            machine(5).write(highStr);
            machine(6).write(lowStr);
        }

        static void DIV(IAS& machine)
        {
            // DIV: MQ <- AC / M(MAR); AC <- AC % M(MAR)
            int addr = toDecimal(machine(3).read());

            std::string divStr = machine[addr].read();
            std::string acStr  = machine(5).read();

            long long divisor = std::stoll(divStr, nullptr, 2);
            if(divStr[0] == '1')
                divisor -= (1LL << 40);

            long long dividend = std::stoll(acStr, nullptr, 2);
            if(acStr[0] == '1')
                dividend -= (1LL << 40);

            if(divisor == 0)
                return;

            long long quotient = dividend / divisor;
            long long remainder = dividend % divisor;

            quotient &= ((1LL << 40) - 1);
            remainder &= ((1LL << 40) - 1);

            std::string qStr(40, '0');
            std::string rStr(40, '0');

            for(int j = 39; j >= 0; --j)
            {
                qStr[j] = (quotient & 1) ? '1' : '0';
                quotient >>= 1;

                rStr[j] = (remainder & 1) ? '1' : '0';
                remainder >>= 1;
            }

            machine(6).write(qStr);
            machine(5).write(rStr);
        }

        static void LSH(IAS& machine)
        {
            // LSH: AC <- AC << 1
            std::string acStr = machine(5).read();

            long long ac = std::stoll(acStr, nullptr, 2);
            if(acStr[0] == '1')
                ac -= (1LL << 40);

            ac = ac << 1;

            ac &= ((1LL << 40) - 1);

            std::string resultStr(40, '0');
            for(int j = 39; j >= 0; --j)
            {
                resultStr[j] = (ac & 1) ? '1' : '0';
                ac >>= 1;
            }

            machine(5).write(resultStr);
        }

        static void RSH(IAS& machine)
        {
            // RSH: AC <- AC >> 1
            std::string acStr = machine(5).read();

            long long ac = std::stoll(acStr, nullptr, 2);
            if(acStr[0] == '1')
                ac -= (1LL << 40);

            ac = ac >> 1;

            ac &= ((1LL << 40) - 1);

            std::string resultStr(40, '0');
            for(int j = 39; j >= 0; --j)
            {
                resultStr[j] = (ac & 1) ? '1' : '0';
                ac >>= 1;
            }

            machine(5).write(resultStr);
        }

        static void STL(IAS& machine)
        {
            // STL: M(MAR)[8:19]  <- AC[28:39]
            int addr = toDecimal(machine(3).read());

            std::string acStr  = machine(5).read();
            std::string memStr = machine[addr].read();

            std::string segment = acStr.substr(28, 12);

            memStr.replace(8, 12, segment);

            machine[addr].write(memStr);
        }

        static void STR(IAS& machine)
        {
            // STR: M(MAR)[28:39] <- AC[28:39]
            int addr = toDecimal(machine(3).read());

            std::string acStr  = machine(5).read();
            std::string memStr = machine[addr].read();

            std::string segment = acStr.substr(28, 12);

            memStr.replace(28, 12, segment);

            machine[addr].write(memStr);
        }

        static void HLT(IAS& machine)
        {
            //Halts
            throw std::runtime_error("HLT");
        }

        public:
        static void execute(IAS& machine)
        {
            int opcode = toDecimal(machine(1).read());

            if(opcode == 10) { LMA(machine); }          // 0A
            else if(opcode == 9) { LDM(machine); }      // 09
            else if(opcode == 33) { STA(machine); }     // 21
            else if(opcode == 1) { LDA(machine); }      // 01
            else if(opcode == 2) { LDN(machine); }      // 02
            else if(opcode == 3) { ALD(machine); }      // 03
            else if(opcode == 4) { ALN(machine); }      // 04
            else if(opcode == 13) { BRL(machine); }     // 0D
            else if(opcode == 14) { BRR(machine); }     // 0E
            else if(opcode == 15) { BPL(machine); }     // 0F
            else if(opcode == 16) { BPR(machine); }     // 10
            else if(opcode == 5) { ADD(machine); }      // 05
            else if(opcode == 7) { AAD(machine); }      // 07
            else if(opcode == 6) { SUB(machine); }      // 06
            else if(opcode == 8) { ASB(machine); }      // 08
            else if(opcode == 11) { MUL(machine); }     // 0B
            else if(opcode == 12) { DIV(machine); }     // 0C
            else if(opcode == 20) { LSH(machine); }     // 14
            else if(opcode == 21) { RSH(machine); }     // 15
            else if(opcode == 18) { STL(machine); }     // 12
            else if(opcode == 19) { STR(machine); }     // 13
            else if(opcode == 0) { HLT(machine); }      // 00
        }
    };
}

#endif
