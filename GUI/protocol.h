#ifndef PROTOCOL_H
#define PROTOCOL_H
#include "mainwindow.h"

class FlatBuffer
{
  public:
    void generateFlatBuffer(
        const std::vector<std::string> &commonFieldNames,
        const std::vector<uint64_t> &commonAddresses, const std::vector<float> &commonValues,
        const std::vector<bool> &commonFixedPoints, const std::vector<std::string> &fieldNames,
        const std::vector<uint64_t> &addresses, const std::vector<float> &values,
        const std::vector<bool> &fixedPoints, const uint32_t inputType, const uint32_t outputType,
        const std::vector<uint8_t> &inputSources, const std::vector<uint8_t> &outputSources);
    void printFlatBufferContent(const uint8_t *buf, int size);
};
#endif // PROTOCOL_H