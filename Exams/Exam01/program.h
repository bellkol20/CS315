#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <string>
#include <cctype>
#include "Word.h"
#include "IAS.h"

namespace ose 
{
    Word negate(const Word& x){
        if (w.empty()){
            return word();
        }
        Word result = x;
        for(int i = 0; i < result.size(); i++){
            reult[i] = (result[i] =='0') ? '1' : '0';
        }
        for (int i = result.size() - 1; i >= 0; i--){
            if (result[i] == '0'){
                if(result[i] == '0'){
                    result[i] == '1';
                    break;
                }
                else
                { 
                    result[i] == '0';
                }
                }
                return result;
            }
            void LDM(IAS& computer){
                computer.MQ = computer.memory[computer.MAR];
            }
            void STA(IAS& computer){
                computer.memory[computer.MAR] = computer.AC;
            }
            void LDA(IAS& computer){
                computer.AC = computer.memory[computer.MAR];
            }
            void LSH(IAS& computer){
                computer.AC = computer.AC << 1;
            }
            void RSH(IAS& computer){
                computer.AC = computer.AC >> 1;
            }
            bool ADD(IAS& computer){
                Word oldAC = computer.AC;
                computer.AC = computer.AC + computer.memory[computer.MAR];

                bool oldSign = oldAC[0];
                bool memSign = computer.memory[computer.MAR][0];
                bool newSign = computer.AC[0];

                return(oldSign == memSign) && (newSign != oldSign);
            }
        }
    }

#endif
