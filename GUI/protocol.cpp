#include "mainwindow.h"
void FlatBuffer::generateFlatBuffer(
    const std::vector<std::string> &commonFieldNames, const std::vector<uint64_t> &commonAddresses,
    const std::vector<float> &commonValues, const std::vector<bool> &commonFixedPoints,
    const std::vector<std::string> &fieldNames, const std::vector<uint64_t> &addresses,
    const std::vector<float> &values, const std::vector<bool> &fixedPoints,
    const uint32_t inputType, const uint32_t outputType, const std::vector<uint8_t> &inputSources,
    const std::vector<uint8_t> &outputSources)
{
    flatbuffers::FlatBufferBuilder builder(1024);

    std::vector<flatbuffers::Offset<ChipVerif::commonRegisterInfo>> commonRegisters;
    for (size_t i = 0; i < commonFieldNames.size(); ++i)
    {
        auto reg = ChipVerif::CreatecommonRegisterInfo(builder,
                                                       builder.CreateString(commonFieldNames[i]),
                                                       commonAddresses[i],
                                                       commonValues[i],
                                                       commonFixedPoints[i]);
        commonRegisters.push_back(reg);
    }
    auto commonRegistersOffset = builder.CreateVector(commonRegisters);

    std::vector<flatbuffers::Offset<ChipVerif::registerInfo>> registers;
    for (size_t i = 0; i < fieldNames.size(); ++i)
    {
        auto reg = ChipVerif::CreateregisterInfo(builder,
                                                 builder.CreateString(fieldNames[i]),
                                                 addresses[i],
                                                 values[i],
                                                 fixedPoints[i]);
        registers.push_back(reg);
    }
    auto registersOffset = builder.CreateVector(registers);

    auto chipFunctionName = builder.CreateString("MyChipFunction");
    auto inputSourcesOffset = builder.CreateVector(inputSources);
    auto outputSourcesOffset = builder.CreateVector(outputSources);

    auto chipConfig = ChipVerif::CreatechipTestConfig(builder,
                                                      chipFunctionName,
                                                      commonRegistersOffset,
                                                      registersOffset,
                                                      inputType,
                                                      outputType,
                                                      inputSourcesOffset,
                                                      outputSourcesOffset);

    builder.Finish(chipConfig);

    uint8_t *buf = builder.GetBufferPointer();
    int size = builder.GetSize();

    std::ofstream outfile("chip_config.bin", std::ios::binary);
    outfile.write(reinterpret_cast<const char *>(buf), size);
    outfile.close();

    std::cout << "FlatBuffer generated" << std::endl;
    printFlatBufferContent(buf, size);
    // Deserialize FlatBuffer
}

void FlatBuffer::printFlatBufferContent(const uint8_t *buf, int size)
{
    auto chipConfig = ChipVerif::GetchipTestConfig(buf);

    std::cout << "FlatBuffer Content:" << std::endl;
    std::cout << "Chip Function Name: " << chipConfig->chipFunctionName()->str() << std::endl;

    std::cout << "\nCommon Registers:" << std::endl;
    for (const auto &reg : *chipConfig->commonRegister())
    {
        std::cout << "  Field Name: " << reg->fieldName()->str() << ", Address: 0x" << std::hex
                  << reg->address() << ", Value: " << std::dec << reg->value()
                  << ", Fixed Point: " << (reg->fixedPoint() ? "Yes" : "No") << std::endl;
    }

    std::cout << "\nRegisters:" << std::endl;
    for (const auto &reg : *chipConfig->registers())
    {
        std::cout << "  Field Name: " << reg->fieldName()->str() << ", Address: 0x" << std::hex
                  << reg->address() << ", Value: " << std::dec << reg->value()
                  << ", Fixed Point: " << (reg->fixedPoint() ? "Yes" : "No") << std::endl;
    }

    std::cout << "\nInput Type: " << chipConfig->inputType() << std::endl;
    std::cout << "Output Type: " << chipConfig->outputType() << std::endl;

    std::cout << "\nInput Sources:" << std::endl;
    for (const auto &source : *chipConfig->inputSources())
    {
        std::cout << "  " << static_cast<int>(source) << std::endl;
    }

    std::cout << "\nOutput Sources:" << std::endl;
    for (const auto &source : *chipConfig->outputSources())
    {
        std::cout << "  " << static_cast<int>(source) << std::endl;
    }
}