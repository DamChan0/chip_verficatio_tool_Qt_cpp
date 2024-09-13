#include "registerMapping.h"

// TODO: Add more registers
std::unordered_map<std::string, uint64_t> fieldNameToAddress = {
    {"PCX_REG_A_1", 0x00000001},  {"PCX_REG_A_2", 0x00000008},  {"PCX_REG_A_3", 0x00000010},
    {"PCX_REG_A_4", 0x00000018},  {"PCX_REG_A_5", 0x00000020},  {"PCX_REG_A_6", 0x00000028},
    {"PCX_REG_A_7", 0x00000030},  {"PCX_REG_A_8", 0x00000038},  {"PCX_REG_A_9", 0x00000040},
    {"PCX_REG_A_10", 0x00000048}, {"PCX_REG_A_11", 0x00000050}, {"PCX_REG_A_12", 0x00000058},
    {"PCX_REG_B_1", 0x00000028},  {"PCX_REG_B_2", 0x00000030},  {"PCX_REG_B_3", 0x00000038},
    {"PCX_REG_B_4", 0x00000040},  {"PCX_REG_B_5", 0x00000048},  {"PCX_REG_B_6", 0x00000050},
    {"PCX_REG_B_7", 0x00000058},  {"PCX_REG_B_8", 0x00000060},  {"PCX_REG_B_9", 0x00000068},
    {"PCX_REG_B_10", 0x00000070}, {"PCX_REG_B_11", 0x00000078}, {"PCX_REG_B_12", 0x00000080},
    {"PCX_REG_C_1", 0x00000050},  {"PCX_REG_C_2", 0x00000058},  {"PCX_REG_C_3", 0x00000060},
    {"PCX_REG_C_4", 0x00000068},  {"PCX_REG_C_5", 0x00000070},  {"PCX_REG_C_6", 0x00000078},
    {"PCX_REG_C_7", 0x00000080},  {"PCX_REG_C_8", 0x00000088},  {"PCX_REG_C_9", 0x00000090},
    {"PCX_REG_C_10", 0x00000098}, {"PCX_REG_C_11", 0x000000A0}, {"PCX_REG_C_12", 0x000000A8},
    {"PCX_REG_D_1", 0x00000078},  {"PCX_REG_D_2", 0x00000080},  {"PCX_REG_D_3", 0x00000088},
    {"PCX_REG_D_4", 0x00000090},  {"PCX_REG_D_5", 0x00000098},  {"PCX_REG_D_6", 0x000000A0},
    {"PCX_REG_D_7", 0x000000A8},  {"PCX_REG_D_8", 0x000000B0},  {"PCX_REG_D_9", 0x000000B8},
    {"PCX_REG_D_10", 0x000000C0}, {"PCX_REG_D_11", 0x000000C8}, {"PCX_REG_D_12", 0x000000D0},
};

const std::vector<Register> &RegisterManager::getRegistersForFunction(
    const std::string &functionName)
{

    return functionRegisterMap.at(functionName);
}

uint64_t getRegisterAddress(const std::string &fieldName)
{
    // List를 순회해서 해당 필드 이름을 찾아서 주소를 반환
    for (auto &pair : fieldNameToAddress)
    {
        if (pair.first == fieldName)
        {
            return pair.second;
        }
    }
    return 0;
}