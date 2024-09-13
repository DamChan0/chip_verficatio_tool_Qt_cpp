#ifndef REGISTERMAPPING_H
#define REGISTERMAPPING_H
#include <QString>
#include <string>
#include <unordered_map>
#include <vector>
struct Register
{
    QString name;
    uint64_t address;
    float value;
};
class RegisterManager
{
  public:
    RegisterManager()
    {
        // 기능별로 레지스터 목록을 정의
        functionRegisterMap.emplace("Function A",
                                    std::vector<Register>{
                                        {"PCX_REG_A_1", 0x00000001},
                                        {"PCX_REG_A_2", 0x00000008},
                                        {"PCX_REG_A_3", 0x00000010},
                                        {"PCX_REG_A_4", 0x00000018},
                                        {"PCX_REG_A_5", 0x00000020},
                                        {"PCX_REG_A_6", 0x00000028},
                                        {"PCX_REG_A_7", 0x00000030},
                                        {"PCX_REG_A_8", 0x00000038},
                                        {"PCX_REG_A_9", 0x00000040},
                                        {"PCX_REG_A_10", 0x00000048},
                                        {"PCX_REG_A_11", 0x00000050},
                                        {"PCX_REG_A_12", 0x00000058},
                                    });
        functionRegisterMap.emplace("Function B",
                                    std::vector<Register>{{"PCX_REG_B_1", 0xB0800028},
                                                          {"PCX_REG_B_2", 0xB0800030},
                                                          {"PCX_REG_B_3", 0xB0800038},
                                                          {"PCX_REG_B_4", 0xB0800040},
                                                          {"PCX_REG_B_5", 0xB0800048},
                                                          {"PCX_REG_B_6", 0xB0800050},
                                                          {"PCX_REG_B_7", 0xB0800058},
                                                          {"PCX_REG_B_8", 0xB0800060},
                                                          {"PCX_REG_B_9", 0xB0800068},
                                                          {"PCX_REG_B_10", 0xB0800070},
                                                          {"PCX_REG_B_11", 0xB0800078},
                                                          {"PCX_REG_B_12", 0xB0800080}});
        functionRegisterMap.emplace("Function C",
                                    std::vector<Register>{{"PCX_REG_C_1", 0xB0800050},
                                                          {"PCX_REG_C_2", 0xB0800058},
                                                          {"PCX_REG_C_3", 0xB0800060},
                                                          {"PCX_REG_C_4", 0xB0800068},
                                                          {"PCX_REG_C_5", 0xB0800070},
                                                          {"PCX_REG_C_6", 0xB0800078},
                                                          {"PCX_REG_C_7", 0xB0800080},
                                                          {"PCX_REG_C_8", 0xB0800088},
                                                          {"PCX_REG_C_9", 0xB0800090},
                                                          {"PCX_REG_C_10", 0xB0800098},
                                                          {"PCX_REG_C_11", 0xB08000A0},
                                                          {"PCX_REG_C_12", 0xB08000A8}});
        functionRegisterMap.emplace("Function D",
                                    std::vector<Register>{{"PCX_REG_D_1", 0xB0800078},
                                                          {"PCX_REG_D_2", 0xB0800080},
                                                          {"PCX_REG_D_3", 0xB0800088},
                                                          {"PCX_REG_D_4", 0xB0800090},
                                                          {"PCX_REG_D_5", 0xB0800098},
                                                          {"PCX_REG_D_6", 0xB08000A0},
                                                          {"PCX_REG_D_7", 0xB08000A8},
                                                          {"PCX_REG_D_8", 0xB08000B0},
                                                          {"PCX_REG_D_9", 0xB08000B8},
                                                          {"PCX_REG_D_10", 0xB08000C0},
                                                          {"PCX_REG_D_11", 0xB08000C8},
                                                          {"PCX_REG_D_12", 0xB08000D0}});
    }

    const std::vector<Register> &getRegistersForFunction(const std::string &functionName);

  private:
    std::unordered_map<std::string, std::vector<Register>> functionRegisterMap;
};

uint64_t getRegisterAddress(const std::string &fieldName);
#endif // REGISTERMAPPING_H