#include "mainwindow.h"
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

    // 스크롤 영역 안에 넣을 위젯 생성
    scrollAreaWidgetContents = new QWidget(); // 스크롤 영역 안에 포함될 위젯 생성
    scrollAreaWidgetContents->setMinimumSize(scrollArea->size()); // 크기를 스크롤 영역에 맞춤
    scrollArea->setWidget(scrollAreaWidgetContents); // 위젯을 스크롤 영역에 설정

    // 스크롤 영역 안의 레이아웃 설정 (QVBoxLayout)
    registerLayout =
        new QVBoxLayout(scrollAreaWidgetContents); // 레이아웃을 스크롤 영역 위젯에 설정
    registerLayout->setAlignment(Qt::AlignTop);    // 레이아웃을 위쪽 정렬로 설정
    registerLayout->setSizeConstraint(QLayout::SetMinAndMaxSize); // 레이아웃 크기 제약 설정
    scrollAreaWidgetContents->setLayout(registerLayout);          // 설정한 레이아웃 적용

    // RegisterTab 레이아웃에 스크롤 영역 추가
    ui->RegisterTab->layout()->addWidget(scrollArea);

    scrollArea->setMinimumHeight(400); // 원하는 크기로 스크롤 영역 설정
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->update(); // 스크롤 영역 업데이트
}

void MainWindow::setupCommonRegisterTable()
{
    // 스크롤 영역 설정
    commonRegisterScrollArea = new QScrollArea(ui->commonRegisterTab);
    commonRegisterScrollArea->setWidgetResizable(true); // 위젯 크기를 스크롤 영역 크기에 맞춤
    commonRegisterScrollArea->setMinimumHeight(400); // 스크롤 영역의 최소 높이 설정 (적절히
    // 조정)
    ui->commonRegisterTab->layout()->addWidget(commonRegisterScrollArea);
    // ui->commonRegisterTab->layout()->addWidget(commonRegisterScrollArea);
    // 테이블 위젯 생성 및 설정
    commonRegisterTable = new QTableWidget(commonRegisterScrollArea);
    commonRegisterTable->setColumnCount(4); // 4개 열로 설정
    commonRegisterTable->setHorizontalHeaderLabels(
        {"Category", "Register Name", "Address", "Value"});
    commonRegisterTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    commonRegisterScrollArea->setWidget(commonRegisterTable);

    // CommonRegister 데이터로 테이블 채우기 (역순으로)
    int row = 0;
    auto categories = commonRegister.getCategories();
    for (auto categoryIt = categories.rbegin(); categoryIt != categories.rend();
         ++categoryIt) // 역순 순회
    {
        const auto &category = *categoryIt;
        auto registers = commonRegister.getRegistersForCategory(category);
        for (auto regIt = registers.rbegin(); regIt != registers.rend();
             ++regIt) // 레지스터도 역순 순회
        {
            const auto &reg = *regIt;

            commonRegisterTable->insertRow(row);
            commonRegisterTable->setItem(row,
                                         0,
                                         new QTableWidgetItem(QString::fromStdString(category)));
            commonRegisterTable->setItem(row, 1, new QTableWidgetItem(reg.name));
            commonRegisterTable->setItem(
                row,
                2,
                new QTableWidgetItem(QString("0x%1").arg(reg.address, 0, 16)));

            // 수정 가능한 QLineEdit 추가
            QLineEdit *valueEdit = new QLineEdit(this);
            if (reg.value == 0)
            {
                valueEdit->setPlaceholderText("Value");
            }
            else
            {
                valueEdit->setText(QString::number(reg.value));
            }
            commonRegisterTable->setCellWidget(row, 3, valueEdit);

            // Connect textChanged signal to update placeholder visibility
            connect(valueEdit, &QLineEdit::textChanged, [valueEdit](const QString &text) {
                if (text.isEmpty())
                {
                    valueEdit->setPlaceholderText("Value");
                }
            });

            row++;
        }
    }

    commonRegisterTable->resizeColumnsToContents();
    commonRegisterTable->resizeRowsToContents();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (commonRegisterTable && commonRegisterScrollArea)
    {
        commonRegisterTable->setMinimumSize(commonRegisterScrollArea->viewport()->size());
    }
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setScrollArea();
    setupCommonRegisterTable();
    previousButton = nullptr; // 생성자에서 명시적으로 초기화

    slotConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::deserializeFlatBuffer(const std::string &fileName)
{
    // std::ifstream infile("chip_config.bin", std::ios::binary);
    // std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(infile)),
    //                             std::istreambuf_iterator<char>());

    // auto chipConfig = ChipVerif::GetchipTestConfig(buffer.data());

    // std::cout << "Function Name: " << chipConfig->chipfunctionname()->str() << std::endl;
    // std::cout << "Input Type: " << chipConfig->inputtype() << std::endl;
    // std::cout << "Output Type: " << chipConfig->outputtype() << std::endl;

    // for (auto reg : *chipConfig->registers())
    // {
    //     std::cout << "Field Name: " << reg->fieldname()->str() << std::endl;
    //     std::cout << "Address: " << reg->address() << std::endl;
    //     std::cout << "Value: " << reg->value() << std::endl;
    //     std::cout << "Fixed Point: " << (reg->fixedpoint() ? "True" : "False") << std::endl;
    // }
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
    for (QWidget *widget : currentWidgets)
    {
        registerLayout->removeWidget(widget);
        delete widget;
    }

    // 리스트 초기화
    this->currentFieldName.clear();
    this->currentValue.clear();
    this->currentFixedPoint.clear();
    this->currentWidgets.clear();

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

        QCheckBox *fixedPointCheckBox = new QCheckBox("Fixed Point", this);
        fixedPointCheckBox->setChecked(reg.fixedPoint);
        // 수평 레이아웃으로 배치
        QHBoxLayout *hbox = new QHBoxLayout();
        hbox->setSpacing(10);                 // 위젯 간의 간격 설정
        hbox->setContentsMargins(0, 0, 0, 0); // 여백 설정
        hbox->addWidget(fieldLineEdit);
        hbox->addWidget(valueLineEdit);
        hbox->addWidget(fixedPointCheckBox);

        // registerLayout에 추가
        registerLayout->addLayout(hbox);

        currentFixedPoint.push_back(fixedPointCheckBox);
        currentFieldName.push_back(fieldLineEdit);
        currentValue.push_back(valueLineEdit);
        currentWidgets.push_back(fixedPointCheckBox);

        //  test 목록 저장
        connect(valueLineEdit, &QLineEdit::textChanged, this, [=](const QString &newText) {
            int copyCount = ui->spinBox_multiInput->value(); // SpinBox에서 설정된 개수 가져오기
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
    scrollArea->setMaximumHeight(ui->FunctionTab->height()); // 스크롤 영역 최대 높이 설정
    scrollArea->update();
}

// void MainWindow::loadRegisterSet(const QString &functionName)
// {
//     auto it = std::find(functionList.begin(), functionList.end(), functionName.toStdString());

//     // 기존에 로드된 레지스터 목록이 있다면 해당 목록을 표시
//     if (it != functionList.end())
//     {
//         std::vector<Register> registers = settingValueMap[functionName];
//         if (registers.empty())
//         {
//             registers = registerManager.getRegistersForFunction(functionName.toStdString());
//         }

//         displayRegisterListForFunction(functionName, registers);
//     }
//     else
//     {
//         std::vector<Register> registers =
//             registerManager.getRegistersForFunction(functionName.toStdString());
//         displayRegisterListForFunction(functionName, registers);
//         this->functionList.push_back(functionName.toStdString());
//     }
// }

void MainWindow::loadRegisterSet(const QString &functionName)
{
    auto it = functionConfigs.find(functionName);

    if (it != functionConfigs.end())
    {
        // 기존에 저장된 FunctionConfig가 있는 경우
        const FunctionConfig &config = it->second;
        displayRegisterListForFunction(functionName, config.registers);
    }
    else
    {
        // 새로운 함수인 경우, 새 FunctionConfig 생성
        FunctionConfig newConfig;
        newConfig.registers = registerManager.getRegistersForFunction(functionName.toStdString());

        // 공통 레지스터 초기화
        for (const auto &categoryPair : CommonRegister::commonRegisterMap)
        {
            newConfig.commonRegisters.insert(newConfig.commonRegisters.end(),
                                             categoryPair.second.begin(),
                                             categoryPair.second.end());
        }
        functionConfigs[functionName] = newConfig;
        displayRegisterListForFunction(functionName, newConfig.registers);
    }
    updateCommonRegisterTable(functionName);
    updateFunctionConfigUI(functionName);

    // functionList 업데이트 (필요한 경우)
    if (std::find(functionList.begin(), functionList.end(), functionName.toStdString()) ==
        functionList.end())
    {
        functionList.push_back(functionName.toStdString());
    }
}

static int previousindex = 0;
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
    if (previousButton &&
        std::find(functionButtons.begin(), functionButtons.end(), previousButton) !=
            functionButtons.end())
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

    // 선택한 버튼의 기능의 레지스터 목록을 로드
    loadRegisterSet(functionName);
    // updateFunctionConfigUI(functionName);
    // updateCommonRegisterTable(functionName);
}

void MainWindow::updateFunctionConfigUI(const QString &functionName)
{
    if (functionConfigs.find(functionName) != functionConfigs.end())
    {
        const auto &config = functionConfigs[functionName];

        ui->spinBox_inputType->setValue(config.inputType == 0 ? 1 : config.inputType);
        ui->spinBox_outputType->setValue(config.outputType == 0 ? 1 : config.outputType);
        ui->lineEdit_inputSource->setText(config.inputSourcePath);
    }
    else
    {
        // 새로운 함수의 경우 기본값 설정
        ui->spinBox_inputType->setValue(1);
        ui->spinBox_outputType->setValue(1);
        ui->lineEdit_inputSource->clear();

        // 새 함수의 경우 공통 레지스터 초기화
        FunctionConfig newConfig;

        // CommonRegister에서 모든 카테고리의 레지스터를 가져옵니다.
        auto categories = commonRegister.getCategories();
        for (const auto &category : categories)
        {
            auto registers = commonRegister.getRegistersForCategory(category);
            newConfig.commonRegisters.insert(newConfig.commonRegisters.end(),
                                             registers.begin(),
                                             registers.end());
        }

        functionConfigs[functionName] = newConfig;
    }
}
