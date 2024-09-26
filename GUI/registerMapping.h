#ifndef REGISTERMAPPING_H
#define REGISTERMAPPING_H

#include "register.h"
#include <QString>
#include <QTableWidget>
#include <array>
#include <string>
#include <unordered_map>
#include <vector>
typedef struct ioRegisterInfo
{
    IP_INFO_t ipInfo;
    INTERRUPT_t interrupt;
    ERRORS_t errors;
    OP_SEQUENCE_t opSequence;
    std::vector<PC_INFO_t> src{4};
    std::vector<PC_INFO_t> dst{4};
    PC_CONCAT_INFO_t pcConcatInfo;
    PC_SOR_DATA_INFO_t pcSorDataInfo;
    DMA_PARAMS_1_t dmaParams1;
    std::vector<PC_SRC_SCALING_AND_OFFSET_t> pcSrcScalingAndOffset{4};
    DMA_MONITORING_t dmaMonitoring;

    ioRegisterInfo()
    {
        // ipInfo.ip_name = 0x00000000;
        // ipInfo.ip_version_patch = 0x00;
        // ipInfo.ip_version_minor = 0x00;
        // ipInfo.ip_version_major = 0x00;
        // ipInfo.reserved = 0x00;

        // interrupt.status = 0x00;
        // interrupt.mask = 0x00;
        // interrupt.clear = 0x00;

        // errors.op_error_code = 0x00;
        // errors.pced_error_code = 0x00;
        // errors.voxel_error_code = 0x00;
        // errors.reserved = 0x00;

        // opSequence.op_start = 0x00;
        // opSequence.OP_SEQ_0.op_seq_0 = 0x00;
        // opSequence.OP_SEQ_0.op_seq_1 = 0x00;
        // opSequence.OP_SEQ_0.op_seq_2 = 0x00;
        // opSequence.OP_SEQ_0.op_seq_3 = 0x00;
        // opSequence.OP_SEQ_0.op_seq_4 = 0x00;
        // opSequence.OP_SEQ_0.op_seq_5 = 0x00;
        // opSequence.OP_SEQ_0.reserved = 0x00;
        // opSequence.OP_SEQ_1.op_seq_6 = 0x00;
        // opSequence.OP_SEQ_1.op_seq_7 = 0x00;
        // opSequence.OP_SEQ_1.op_seq_8 = 0x00;
        // opSequence.OP_SEQ_1.op_seq_9 = 0x00;
        // opSequence.OP_SEQ_1.op_seq_10 = 0x00;
        // opSequence.OP_SEQ_1.op_seq_11 = 0x00;
        // opSequence.OP_SEQ_1.reserved = 0x00;

        // src[0].info_1.width = 0x00;
        // src[0].info_2.height = 0x00;
        // src[0].info_3.address_l = 0x00;
        // src[0].info_4.address_h = 0x00;
        // src[0].info_5.capacity = 0x00;
    }
} ioRegisterInfo_t;

struct Register
{
    QString name;
    uint64_t address;
    float value;
    bool fixedPoint;
};

class CommonRegister
{
  public:
    CommonRegister()
    {
        commonRegisterMap["IP Information"] = {{"IP_NAME", 0x0}, {"IP_VERSION", 0x4}};

        // Interrupt
        commonRegisterMap["Interrupt"] = {{"INTERRUPT_STATUS", 0x10},
                                          {"INTERRUPT_MASK", 0x14},
                                          {"INTERRUPT_CLEAR", 0x18}};

        // Errors
        commonRegisterMap["Errors"] = {{"ERRORS", 0x20}};

        // Operation Sequence
        commonRegisterMap["Operation Sequence"] = {{"OP_START", 0x100},
                                                   {"OP_SEQ_0", 0x108},
                                                   {"OP_SEQ_1", 0x10c}};

        // Point Cloud Source Information
        std::vector<Register> pcSrcInfo;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 1; j <= 5; ++j)
            {
                pcSrcInfo.push_back({"PC_SRC_" + QString::number(i) + "_INFO_" + QString::number(j),
                                     static_cast<uint64_t>(0x114 + i * 0x14 + (j - 1) * 0x4)});
            }
        }
        commonRegisterMap["Point Cloud Source Information"] = pcSrcInfo;

        // Point Cloud Destination Information
        std::vector<Register> pcDstInfo;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 1; j <= 5; ++j)
            {
                pcDstInfo.push_back({"PC_DST_" + QString::number(i) + "_INFO_" + QString::number(j),
                                     static_cast<uint64_t>(0x1b4 + i * 0x14 + (j - 1) * 0x4)});
            }
        }
        commonRegisterMap["Point Cloud Destination Information"] = pcDstInfo;

        // Point Cloud Concatenation Information
        commonRegisterMap["Point Cloud Concatenation Information"] = {{"PC_CONCAT_INFO_1", 0x254},
                                                                      {"PC_CONCAT_INFO_2", 0x258},
                                                                      {"PC_CONCAT_INFO_3", 0x25c},
                                                                      {"PC_CONCAT_INFO_4", 0x260},
                                                                      {"PC_CONCAT_INFO_5", 0x264}};

        // Point Cloud SOR Data Information
        commonRegisterMap["Point Cloud SOR Data Information"] = {{"PC_SOR_DATA_INFO_1", 0x268},
                                                                 {"PC_SOR_DATA_INFO_2", 0x26c},
                                                                 {"PC_SOR_DATA_INFO_3", 0x270},
                                                                 {"PC_SOR_DATA_INFO_4", 0x274},
                                                                 {"PC_SOR_DATA_INFO_5", 0x278}};

        // DMA Parameters
        commonRegisterMap["DMA Parameters"] = {{"DMA_PARAMS_1", 0x280}};

        // Point Cloud Source Scaling and Offset
        std::vector<Register> pcSrcScalingAndOffset;
        for (int i = 0; i < 4; ++i)
        {
            pcSrcScalingAndOffset.push_back({"PC_SRC" + QString::number(i) + "_AZM_SCL_FAC",
                                             static_cast<uint64_t>(0x284 + i * 0x1c)});
            pcSrcScalingAndOffset.push_back({"PC_SRC" + QString::number(i) + "_ELV_SCL_FAC",
                                             static_cast<uint64_t>(0x288 + i * 0x1c)});
            pcSrcScalingAndOffset.push_back({"PC_SRC" + QString::number(i) + "_POS_SCL_FAC",
                                             static_cast<uint64_t>(0x28c + i * 0x1c)});
            pcSrcScalingAndOffset.push_back({"PC_SRC" + QString::number(i) + "_INTEN_SCL_FAC",
                                             static_cast<uint64_t>(0x290 + i * 0x1c)});
            pcSrcScalingAndOffset.push_back({"PC_SRC" + QString::number(i) + "_AZM_OFFSET",
                                             static_cast<uint64_t>(0x294 + i * 0x1c)});
            pcSrcScalingAndOffset.push_back({"PC_SRC" + QString::number(i) + "_ELV_OFFSET",
                                             static_cast<uint64_t>(0x298 + i * 0x1c)});
            pcSrcScalingAndOffset.push_back({"PC_SRC" + QString::number(i) + "_POS_OFFSET",
                                             static_cast<uint64_t>(0x29c + i * 0x1c)});
        }
        commonRegisterMap["Point Cloud Source Scaling and Offset"] = pcSrcScalingAndOffset;

        // DMA Monitoring
        commonRegisterMap["DMA Monitoring"] = {{"DMA_READ_MONITOR", 0x2f4},
                                               {"DMA_READ_FIFO", 0x2f8},
                                               {"DMA_READ_FIFO_MON", 0x2fc},
                                               {"DMA_RREQ_MON_L", 0x300},
                                               {"DMA_RREQ_MON_H", 0x304},
                                               {"DMA_READ_STATE_MON", 0x308},
                                               {"DMA_WREQ_MON_L", 0x310},
                                               {"DMA_WREQ_MON_H", 0x314},
                                               {"DMA_WRITE_STATE_MON", 0x318},
                                               {"DMA_WRESP_STATE_MON", 0x320}};

        // Sync Select Mode
        commonRegisterMap["Sync Select Mode"] = {{"SYNC_SEL_MODE", 0x360}};
    }

    std::vector<std::string> getCategories() const;
    std::vector<Register> getRegistersForCategory(const std::string &category) const;
    static std::unordered_map<std::string, std::vector<Register>> commonRegisterMap;
    void updateCommonRegisterValues(QTableWidget *commonRegisterTable);

  private:
};

class RegisterManager
{
  public:
    RegisterManager()
    {
        // 기능별로 레지스터 목록을 정의
        functionRegisterMap.emplace("Function A",
                                    std::vector<Register>{
                                        {"PCX_REG_A_1", 0x00000001},  {"PCX_REG_A_2", 0x00000008},
                                        {"PCX_REG_A_3", 0x00000010},  {"PCX_REG_A_4", 0x00000018},
                                        {"PCX_REG_A_5", 0x00000020},  {"PCX_REG_A_6", 0x00000028},
                                        {"PCX_REG_A_7", 0x00000030},  {"PCX_REG_A_8", 0x00000038},
                                        {"PCX_REG_A_9", 0x00000040},  {"PCX_REG_A_10", 0x00000048},
                                        {"PCX_REG_A_11", 0x00000050}, {"PCX_REG_A_12", 0x00000058},
                                        {"PCX_REG_A_13", 0x00000062}, {"PCX_REG_A_14", 0x00000070},
                                        {"PCX_REG_A_15", 0x00000078}, {"PCX_REG_A_16", 0x00000080},
                                        {"PCX_REG_A_17", 0x00000088}, {"PCX_REG_A_18", 0x00000090},
                                        {"PCX_REG_A_19", 0x00000098}, {"PCX_REG_A_20", 0x000000A0},
                                        {"PCX_REG_A_21", 0x000000A8}, {"PCX_REG_A_22", 0x000000B0},
                                        {"PCX_REG_A_23", 0x000000B8}, {"PCX_REG_A_24", 0x000000C0},
                                        {"PCX_REG_A_25", 0x000000C8}, {"PCX_REG_A_26", 0x000000D0},
                                        {"PCX_REG_A_27", 0x000000D8}, {"PCX_REG_A_28", 0x000000E0},
                                        {"PCX_REG_A_29", 0x000000E8}, {"PCX_REG_A_30", 0x000000F0},
                                        {"PCX_REG_A_31", 0x000000F8}, {"PCX_REG_A_32", 0x00000100},
                                        {"PCX_REG_A_33", 0x00000108}, {"PCX_REG_A_34", 0x00000110},
                                        {"PCX_REG_A_35", 0x00000118}, {"PCX_REG_A_36", 0x00000120},
                                        {"PCX_REG_A_37", 0x00000128}, {"PCX_REG_A_38", 0x00000130},
                                        {"PCX_REG_A_39", 0x00000138}, {"PCX_REG_A_40", 0x00000140},
                                        {"PCX_REG_A_41", 0x00000148}, {"PCX_REG_A_42", 0x00000150},
                                        {"PCX_REG_A_43", 0x00000158},
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
    static uint64_t getRegisterAddress(const std::string &fieldName,
                                       const std::string &functionName);

  private:
    static std::unordered_map<std::string, std::vector<Register>> functionRegisterMap;
};

// uint64_t getRegisterAddress(const std::string &fieldName);
#endif // REGISTERMAPPING_H