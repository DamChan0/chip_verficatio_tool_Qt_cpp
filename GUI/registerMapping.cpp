#include "registerMapping.h"
#include "mainwindow.h"
std::unordered_map<std::string, std::vector<Register>> RegisterManager::functionRegisterMap;
std::unordered_map<std::string, std::vector<Register>> CommonRegister::commonRegisterMap;

// TODO: Add more registers

const std::vector<Register> &RegisterManager::getRegistersForFunction(
    const std::string &functionName)
{
    return functionRegisterMap.at(functionName);
}

// 특정 함수에 대한 특정 레지스터의 주소를 가져오는 메서드
uint64_t RegisterManager::getRegisterAddress(const std::string &fieldName,
                                             const std::string &functionName)
{
    for (const auto &reg : functionRegisterMap[functionName])
    {
        if (reg.name.toStdString() == fieldName)
        {
            return reg.address;
        }
    }
    return 0;
}

std::vector<std::string> CommonRegister::getCategories() const
{
    std::vector<std::string> categories;
    categories.reserve(commonRegisterMap.size());
    for (const auto &pair : commonRegisterMap)
    {
        categories.push_back(pair.first);
    }

    return categories;
}

std::vector<Register> CommonRegister::getRegistersForCategory(const std::string &category) const
{
    auto it = commonRegisterMap.find(category);
    if (it != commonRegisterMap.end())
    {
        return it->second;
    }
    return {}; // 카테고리가 존재하지 않을 경우 빈 벡터 반환
}

void CommonRegister::updateCommonRegisterValues(QTableWidget *commonRegisterTable)
{
    for (int row = 0; row < commonRegisterTable->rowCount(); ++row)
    {
        QString category = commonRegisterTable->item(row, 0)->text();
        QString registerName = commonRegisterTable->item(row, 1)->text();
        QLineEdit *valueEdit = qobject_cast<QLineEdit *>(commonRegisterTable->cellWidget(row, 3));
        if (valueEdit)
        {
            bool ok;
            float value = valueEdit->text().toFloat(&ok);
            if (ok)
            {
                // 여기서 CommonRegister::commonRegisterMap을 직접 업데이트합니다.
                for (auto &reg : CommonRegister::commonRegisterMap[category.toStdString()])
                {
                    if (reg.name == registerName)
                    {
                        reg.value = value;
                        break;
                    }
                }
            }
        }
    }
}

void MainWindow::saveFunctionConfig()
{
    if (currentActivatedButton && !currentActivatedButton->text().isEmpty())
    {
        QString functionName = currentActivatedButton->text();
        std::cout << "Saving function: " << functionName.toStdString() << std::endl;

        FunctionConfig &config = functionConfigs[functionName];

        // 기능별 레지스터 저장
        config.registers.clear();
        for (int i = 0; i < currentFieldName.size(); i++)
        {
            Register reg;
            reg.name = currentFieldName[i]->text();
            reg.value = currentValue[i]->text().toFloat();
            reg.fixedPoint = currentFixedPoint[i]->isChecked();
            config.registers.push_back(reg);
        }

        // 공통 레지스터 저장
        config.commonRegisters.clear();
        for (const auto &categoryPair : CommonRegister::commonRegisterMap)
        {
            for (const auto &reg : categoryPair.second)
            {
                Register updatedReg = reg; // 기존 정보 복사

                // UI에서 현재 값 가져오기
                for (int row = 0; row < commonRegisterTable->rowCount(); ++row)
                {
                    if (commonRegisterTable->item(row, 1)->text() == reg.name)
                    {
                        QLineEdit *valueEdit =
                            qobject_cast<QLineEdit *>(commonRegisterTable->cellWidget(row, 3));
                        if (valueEdit)
                        {
                            updatedReg.value = valueEdit->text().toFloat();
                        }
                        break;
                    }
                }

                config.commonRegisters.push_back(updatedReg);
            }
        }

        // 기타 설정 저장
        config.inputType = ui->spinBox_inputType->value();
        config.outputType = ui->spinBox_outputType->value();
        config.inputSourcePath = ui->lineEdit_inputSource->text();

        std::cout << "Function config saved successfully" << std::endl;
    }
    else
    {
        std::cout << "Error: No function selected or function name is empty" << std::endl;
    }
}

void MainWindow::updateCommonRegisterTable(const QString &functionName)
{
    if (functionConfigs.find(functionName) != functionConfigs.end())
    {
        const auto &config = functionConfigs[functionName];

        for (int row = 0; row < commonRegisterTable->rowCount(); ++row)
        {
            QString regName = commonRegisterTable->item(row, 1)->text();
            QLineEdit *valueEdit =
                qobject_cast<QLineEdit *>(commonRegisterTable->cellWidget(row, 3));
            if (valueEdit)
            {
                auto it =
                    std::find_if(config.commonRegisters.begin(),
                                 config.commonRegisters.end(),
                                 [&regName](const Register &reg) { return reg.name == regName; });
                if (it != config.commonRegisters.end())
                {
                    if (it->value == 0)
                    {
                        valueEdit->clear();
                        valueEdit->setPlaceholderText("Value");
                    }
                    else
                    {
                        valueEdit->setText(QString::number(it->value));
                    }
                }
            }
        }
    }
}