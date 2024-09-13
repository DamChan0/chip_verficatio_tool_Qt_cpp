#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "registerMapping.h"
#include <QComboBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QLineEdit>
#include <algorithm>
#include <flatbuffers/flatbuffers.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void MainWindow::setScrollArea()
{
    currentActivatedButton = nullptr; // 생성자에서 명시적으로 초기화

    // RegisterTab 안에 QScrollArea 생성
    scrollArea = new QScrollArea(ui->RegisterTab);
    scrollArea->setWidgetResizable(true); // 스크롤 영역의 크기가 자동으로 조정되도록 설정
    scrollArea->setSizePolicy(QSizePolicy::Expanding,
                              QSizePolicy::Expanding); // 가로, 세로 모두 확장 가능

    // 스크롤 영역 안에 넣을 위젯 생성
    scrollAreaWidgetContents = new QWidget(); // 스크롤 영역 안에 포함될 위젯 생성
    scrollArea->setWidget(scrollAreaWidgetContents); // 위젯을 스크롤 영역에 설정

    // 스크롤 영역 안의 레이아웃 설정 (QVBoxLayout)
    registerLayout =
        new QVBoxLayout(scrollAreaWidgetContents); // 레이아웃을 스크롤 영역 위젯에 설정
    registerLayout->setAlignment(Qt::AlignTop);    // 레이아웃을 위쪽 정렬로 설정
    scrollAreaWidgetContents->setLayout(registerLayout); // 설정한 레이아웃 적용

    // RegisterTab 레이아웃에 스크롤 영역 추가
    ui->RegisterTab->layout()->addWidget(scrollArea);
    scrollArea->setMinimumHeight(ui->RegisterTab->height()); // RegisterTab과 동일한 높이로 설정
}
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setScrollArea();
    previousButton = nullptr; // 생성자에서 명시적으로 초기화

    connect(ui->generateButton, &QPushButton::clicked, this, &MainWindow::onGenerateFlatBuffer);
    connect(ui->deserializeButton, &QPushButton::clicked, [this]() {
        this->deserializeFlatBuffer("chip_config.bin");
    });
    connect(ui->functionComboBox,
            QOverload<int>::of(&QComboBox::activated),
            this,
            &MainWindow::selcetFunction);

    connect(ui->saveRegisterButton, &QPushButton::clicked, [this]() {
        if (currentActivatedButton && !currentActivatedButton->text().isEmpty())
        {
            QString functionName = this->currentActivatedButton->text();
            cout << "functionName: " << functionName.toStdString() << endl;
            std::vector<Register> registers;
            for (int i = 0; i < currentFieldName.size(); i++)
            {
                Register reg;
                reg.name = currentFieldName[i]->text();
                reg.value = currentValue[i]->text().toFloat();
                registers.push_back(reg);
            }
            settingValueMap[functionName] = registers; // 이후 로직 처리
        }
        else
        {
            cout << "currentActivatedButton is nullptr or text is empty";
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::string MainWindow::getAddressForField(const std::string &fieldName)
{
    auto it = fieldNameToAddress.find(fieldName);
    if (it != fieldNameToAddress.end())
    {
        return it->second;
    }
    else
    {
        return "0x00000000"; // Default or error case
    }
}
void MainWindow::generateFlatBuffer(const std::vector<std::string> &fieldNames,
                                    const std::vector<uint64_t> &addresses,
                                    const std::vector<float> &values)
{
    flatbuffers::FlatBufferBuilder builder(1024);

    std::vector<flatbuffers::Offset<ChipVerif::Register>> registers;

    for (size_t i = 0; i < fieldNames.size(); ++i)
    {
        auto reg = ChipVerif::CreateRegisterDirect(builder,
                                                   fieldNames[i].c_str(),
                                                   addresses[i],
                                                   values[i]);
        registers.push_back(reg);
    }

    auto registersOffset = builder.CreateVector(registers);

    auto chipFunctionName = builder.CreateString("MyChipFunction");
    auto chipConfig = ChipVerif::CreateChipConfig(builder, chipFunctionName, registersOffset);

    builder.Finish(chipConfig);

    uint8_t *buf = builder.GetBufferPointer();
    int size = builder.GetSize();

    std::ofstream outfile("chip_config.bin", std::ios::binary);
    outfile.write(reinterpret_cast<const char *>(buf), size);
    outfile.close();

    std::cout << "FlatBuffer generated" << std::endl;
}

void MainWindow::onGenerateFlatBuffer()
{
    std::vector<std::string> fieldNameInput;
    std::vector<uint64_t> addressInput;
    std::vector<float> valueInput;

    for (const auto &function : functionList)
    {
        const auto &setting = settingValueMap[QString::fromStdString(function)];
        for (size_t i = 0; i < setting.size(); ++i)
        {
            QString fieldName = setting[i].name;
            float valueStr = setting[i].value;

            if (!fieldName.isEmpty() && !valueStr != 0)
            {
                fieldNameInput.push_back(fieldName.toStdString());
                addressInput.push_back(getRegisterAddress(fieldName.toStdString()));
                valueInput.push_back(valueStr);
            }
        }
    }

    generateFlatBuffer(fieldNameInput, addressInput, valueInput);
}

void MainWindow::deserializeFlatBuffer(const std::string &fileName)
{
    std::ifstream infile("chip_config.bin", std::ios::binary);
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(infile)),
                                std::istreambuf_iterator<char>());

    auto chipConfig = ChipVerif::GetChipConfig(buffer.data());

    std::cout << "Function Name: " << chipConfig->chip_functionName()->str() << std::endl;

    for (auto reg : *chipConfig->registers())
    {
        std::cout << "Field Name: " << reg->fieldName()->str() << std::endl;
        std::cout << "Address: " << reg->address() << std::endl;
        std::cout << "Value: " << reg->value() << std::endl;
        // std::cout << "Is Valid: " << (reg->isValid() ? "True" : "False")
        //           << std::endl;
    }
}

void MainWindow::displayRegisterListForFunction(const QString &functionName,
                                                std::vector<Register> registers)
{
    std::vector<Register> currentRegisters;

    for (QLineEdit *lineEdit : currentFieldName)
    {
        registerLayout->removeWidget(lineEdit);
        delete lineEdit;
    }
    for (QLineEdit *lineEdit : currentValue)
    {
        registerLayout->removeWidget(lineEdit);
        delete lineEdit;
    }

    // 리스트 초기화
    this->currentFieldName.clear();
    this->currentValue.clear();

    for (int i = 0; i < registers.size(); i++)
    {
        Register reg = registers[i];

        // 레지스터 이름 및 값을 입력할 QLineEdit 생성
        QLineEdit *fieldLineEdit = new QLineEdit(this);
        fieldLineEdit->setText(reg.name); // 필드명을 실제 텍스트로 설정
        fieldLineEdit->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Fixed); // 가로 확장 가능, 세로 고정

        QLineEdit *valueLineEdit = new QLineEdit(this);
        if (reg.value != 0)
        {
            valueLineEdit->setText(QString::number(reg.value)); // 값이 0이 아닌 경우 텍스트로 설정
        }
        else
        {
            valueLineEdit->setPlaceholderText("Value");
        }
        valueLineEdit->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Fixed); // 가로 확장 가능, 세로 고정
        qDebug() << "Created fieldLineEdit with text:" << fieldLineEdit->text();

        // 수평 레이아웃으로 배치
        QHBoxLayout *hbox = new QHBoxLayout();
        hbox->setSpacing(10);                 // 위젯 간의 간격 설정
        hbox->setContentsMargins(0, 0, 0, 0); // 여백 설정
        hbox->addWidget(fieldLineEdit);
        hbox->addWidget(valueLineEdit);

        // registerLayout에 추가
        registerLayout->addLayout(hbox);
        currentFieldName.push_back(fieldLineEdit);
        currentValue.push_back(valueLineEdit);

        //  test 목록 저장

        connect(valueLineEdit, &QLineEdit::textChanged, this, [=](const QString &newText) {
            int copyCount = ui->spinBox->value(); // SpinBox에서 설정된 개수 가져오기
            auto currentIndex = std::find(currentValue.begin(), currentValue.end(), valueLineEdit);
            uint32_t startIndex = std::distance(currentValue.begin(), currentIndex);
            // SpinBox에 설정된 개수만큼 필드 값 복사
            if (copyCount > 0)
            {
                for (uint32_t i = 1; i <= copyCount; i++)
                {
                    uint32_t nextIndex = startIndex + i;
                    if (nextIndex >= currentValue.size() - 1)
                    {
                        nextIndex = currentValue.size() - 1;
                    }
                    QLineEdit *nextLineEdit = currentValue[nextIndex];
                    nextLineEdit->blockSignals(true);
                    nextLineEdit->setText(newText);    // 텍스트 복사
                    nextLineEdit->blockSignals(false); // 시그널 다시 활성화
                }
            }
        });
    }
    auto it = std::find(functionList.begin(), functionList.end(), functionName.toStdString());
    if (it != functionList.end())
    {
        settingValueMap[functionName] = (registers);
    }
    // 레이아웃 및 스크롤 영역 강제 갱신
    scrollArea->setMinimumHeight(700); // 스크롤 영역 최소 높이 설정
    scrollArea->setMaximumHeight(ui->FunctionTab->height()); // 스크롤 영역 최대 높이 설정
    scrollArea->update();
}

void MainWindow::loadRegisterSet(const QString &functionName)
{
    auto it = std::find(functionList.begin(), functionList.end(), functionName.toStdString());

    if (it != functionList.end())
    {
        std::vector<Register> registers = settingValueMap[functionName];
        if (registers.empty())
        {
            registers = registerManager.getRegistersForFunction(functionName.toStdString());
        }

        displayRegisterListForFunction(functionName, registers);
    }
    else
    {
        std::vector<Register> registers =
            registerManager.getRegistersForFunction(functionName.toStdString());
        displayRegisterListForFunction(functionName, registers);
    }

    if (it != functionList.end())
    {
    }
    else
    {
        this->functionList.push_back(functionName.toStdString());
    }
}

static int previousindex = NULL;
void MainWindow::selcetFunction(int selectedIndex)
{
    static int previousindex = NULL;
    bool loadData = false;
    QVBoxLayout *testListLayout = dynamic_cast<QVBoxLayout *>(ui->TestLists->widget()->layout());

    // functionComboBox에서 선택한 기능 이름을 가져옴
    QString functionName = ui->functionComboBox->itemText(selectedIndex);

    // 새 버튼을 TestLists 영역에 추가
    QPushButton *newButton = new QPushButton(functionName, this);
    functionButtons.push_back(newButton);
    previousindex = selectedIndex;

    testListLayout->addWidget(newButton);

    // 버튼 클릭 시 해당 레지스터 목록을 표시하도록 연결
    connect(newButton, &QPushButton::clicked, this, &MainWindow::onClickFuntionButton);

    ui->TestLists->widget()->adjustSize();
    ui->TestLists->update();
}
// 레이아웃 및 스크롤 영역 갱신

// ComboBox에서 선택된 항목이 변경될 때 selcetFunction 함수 호출

void MainWindow::onClickFuntionButton()
{
    // sender()를 사용하여 클릭된 버튼을 식별
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());

    if (!clickedButton)
    {
        qDebug() << "Error: clickedButton is null.";
        return; // clickedButton이 nullptr이면 함수 종료
    }

    QString functionName = clickedButton->text();

    if (functionName.isEmpty())
    {
        qDebug() << "Error: Button text is empty.";
        return; // 버튼의 텍스트가 비어 있으면 함수 종료
    }

    // 이전에 클릭된 버튼이 있다면, 색을 원래대로 되돌립니다.
    if (previousButton)
    {
        previousButton->setStyleSheet(""); // 스타일 시트를 초기화하여 원래 색으로 돌림
    }

    // 클릭된 버튼의 색을 변경합니다.
    clickedButton->setStyleSheet("background-color: lightblue;");

    // 현재 버튼을 이전 버튼으로 저장
    previousButton = clickedButton;

    this->currentActivatedButton = clickedButton;
    // 버튼의 텍스트 출력
    qDebug() << "Clicked button text:" << clickedButton->text();

    // 선택한 버튼의 기능을 실행
    loadRegisterSet(functionName);
}
