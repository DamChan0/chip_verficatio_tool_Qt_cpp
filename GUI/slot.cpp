#include "mainwindow.h"
#include <QApplication>
#include <QFile>
using namespace std;

void MainWindow::slotConnections()
{
    connect(ui->globalFixedPointCheckBox,
            &QCheckBox::stateChanged,
            this,
            &MainWindow::onGlobalFixedPointChanged);
    connect(ui->generateButton, &QPushButton::clicked, this, &MainWindow::onGenerateFlatBuffer);
    connect(ui->deserializeButton, &QPushButton::clicked, [this]() { this->onRunCompare(); });
    connect(ui->functionComboBox,
            QOverload<int>::of(&QComboBox::activated),
            this,
            &MainWindow::onSelcetFunction);
    connect(ui->saveRegisterButton, &QPushButton::clicked, this, &MainWindow::saveFunctionConfig);

    // connect(ui->saveRegisterButton, &QPushButton::clicked, [this]() {
    //     if (currentActivatedButton && !currentActivatedButton->text().isEmpty())
    //     {
    //         QString functionName = this->currentActivatedButton->text();
    //         cout << "functionName: " << functionName.toStdString() << endl;
    //         std::vector<Register> registers;
    //         for (int i = 0; i < currentFieldName.size(); i++)
    //         {
    //             Register reg;
    //             reg.name = currentFieldName[i]->text();
    //             reg.value = currentValue[i]->text().toFloat();
    //             registers.push_back(reg);
    //         }
    //         settingValueMap[functionName] = registers; // 이후 로직 처리
    //     }
    //     else
    //     {
    //         std::cout << "currentActivatedButton is nullptr or text is empty";
    //     }
    // });

    connect(ui->pushButton_inputSource,
            &QPushButton::clicked,
            this,
            &MainWindow::onSelectInputSource);
    connect(ui->spinBox_inputType,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &MainWindow::onInputTypeChanged);
    connect(ui->spinBox_outputType,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &MainWindow::onOutputTypeChanged);
    connect(ui->saveCommonRegisterButton,
            &QPushButton::clicked,
            this,
            &MainWindow::onSaveCommonRegisters);
}

void MainWindow::onGlobalFixedPointChanged(int state)
{
    bool checked = (state == Qt::Checked);
    for (QCheckBox *checkBox : currentFixedPoint)
    {
        checkBox->setChecked(checked);
    }
}

void MainWindow::onInputTypeChanged(int value)
{
    inputType = static_cast<uint32_t>(value);
}

void MainWindow::onOutputTypeChanged(int value)
{
    outputType = static_cast<uint32_t>(value);
}

void MainWindow::onSelectInputSource()
{
    QString filePath =
        QFileDialog::getOpenFileName(this, "Select Input Source", "", "All Files (*)");
    if (!filePath.isEmpty())
    {
        inputSourcePath = filePath;
        ui->lineEdit_inputSource->setText(filePath);
    }
}

void MainWindow::onGenerateFlatBuffer()
{
    std::vector<std::string> commonFieldNames;
    std::vector<uint64_t> commonAddresses;
    std::vector<float> commonValues;
    std::vector<bool> commonFixedPoints;

    for (const auto &category : CommonRegister::commonRegisterMap)
    {
        for (const auto &reg : category.second)
        {
            commonFieldNames.push_back(reg.name.toStdString());
            commonAddresses.push_back(reg.address);
            commonValues.push_back(reg.value);
            commonFixedPoints.push_back(reg.fixedPoint);
        }
    }
    std::vector<uint8_t> outputSources;

    // 입력 소스 준비
    std::vector<uint8_t> inputSources;
    if (!inputSourcePath.isEmpty())
    {
        QFile file(inputSourcePath);
        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray fileData = file.readAll();
            inputSources.assign(fileData.begin(), fileData.end());
        }
    }

    std::vector<std::string> fieldNameInput;
    std::vector<uint64_t> addressInput;
    std::vector<float> valueInput;
    std::vector<bool> fixedPointInput;

    for (const auto &function : functionList)
    {
        const auto &setting = settingValueMap[QString::fromStdString(function)];
        for (size_t i = 0; i < setting.size(); ++i)
        {
            QString fieldName = setting[i].name;
            float valueStr = setting[i].value;
            bool isFixedPoint = setting[i].fixedPoint;

            if (!fieldName.isEmpty() && valueStr != 0)
            {
                fieldNameInput.push_back(fieldName.toStdString());
                addressInput.push_back(
                    (RegisterManager::getRegisterAddress(fieldName.toStdString(), function)));
                valueInput.push_back(valueStr);
                fixedPointInput.push_back(isFixedPoint);
            }
        }
    }

    flatBuffer.generateFlatBuffer(commonFieldNames,
                                  commonAddresses,
                                  commonValues,
                                  commonFixedPoints,
                                  fieldNameInput,
                                  addressInput,
                                  valueInput,
                                  fixedPointInput,
                                  this->inputType,
                                  this->outputType,
                                  inputSources,
                                  outputSources);
}
static int previousindex = 0;

void MainWindow::onSelcetFunction(int selectedIndex)
{
    QVBoxLayout *testListLayout = dynamic_cast<QVBoxLayout *>(ui->TestLists->widget()->layout());
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // functionComboBox에서 선택한 기능 이름을 가져옴
    QString functionName = ui->functionComboBox->itemText(selectedIndex);

    // 새 버튼을 TestLists 영역에 추가
    QPushButton *functionSelectButton = new QPushButton(functionName, this);
    functionSelectButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    functionButtons.push_back(functionSelectButton);
    previousindex = selectedIndex;

    QPushButton *functionDeleteButton = new QPushButton("Delete", this);
    functionDeleteButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    functionDeleteButton->setMaximumWidth(functionSelectButton->width() / 3);

    buttonLayout->addWidget(functionSelectButton, 3); // 4의 stretch factor
    buttonLayout->addWidget(functionDeleteButton, 1); // 1의 stretch factor

    testListLayout->addLayout(buttonLayout);

    // 버튼 클릭 시 해당 레지스터 목록을 표시하도록 연결
    connect(functionSelectButton, &QPushButton::clicked, this, &MainWindow::onClickFuntionButton);
    connect(
        functionDeleteButton,
        &QPushButton::clicked,
        [this, functionSelectButton, functionDeleteButton, buttonLayout, testListLayout]() mutable {
            this->onDeleteFunction(functionSelectButton,
                                   functionDeleteButton,
                                   buttonLayout,
                                   testListLayout);
            // 람다 캡처된 포인터들을 nullptr로 설정
            functionSelectButton = nullptr;
            functionDeleteButton = nullptr;
            buttonLayout = nullptr;
        });

    ui->TestLists->widget()->adjustSize();
    ui->TestLists->update();
}

void MainWindow::onSaveCommonRegisters()
{
    commonRegister.updateCommonRegisterValues(this->commonRegisterTable);
    // 여기에 저장 완료 메시지를 표시하거나 다른 작업을 수행할 수 있습니다.
    std::cout << "Common Registers saved";
}

void MainWindow::onRunCompare()
{
    // TODO : add open expected file and compare logic
    std::cout << "Run Compare";
}
void MainWindow::onDeleteFunction(QPushButton *functionSelectButton, QPushButton *deleteButton,
                                  QHBoxLayout *buttonLayout, QVBoxLayout *testListLayout)
{
    // functionButtons 벡터에서 해당 버튼 제거
    if (!functionSelectButton || !deleteButton || !buttonLayout || !testListLayout)
    {
        std::cout << "Invalid pointers provided to onDeleteFunction" << std::endl;
        return;
    }

    std::cout << functionSelectButton->text().toStdString() << " Function deleted" << std::endl;

    // functionButtons 벡터에서 해당 버튼 제거
    auto it = std::find(functionButtons.begin(), functionButtons.end(), functionSelectButton);
    if (it != functionButtons.end())
    {
        functionButtons.erase(it);
    }

    // 버튼들과 연결된 모든 시그널/슬롯 연결 해제
    functionSelectButton->disconnect();
    deleteButton->disconnect();

    // 레이아웃에서 버튼들 제거
    buttonLayout->removeWidget(functionSelectButton);
    buttonLayout->removeWidget(deleteButton);

    // 버튼들 삭제 예약
    functionSelectButton->deleteLater();
    deleteButton->deleteLater();

    // 레이아웃 제거 및 삭제 예약
    testListLayout->removeItem(buttonLayout);
    buttonLayout->deleteLater();

    // UI 업데이트
    ui->TestLists->widget()->adjustSize();
    ui->TestLists->update();
    QApplication::processEvents();

    std::cout << functionSelectButton->text().toStdString() << " deleted" << std::endl;
}