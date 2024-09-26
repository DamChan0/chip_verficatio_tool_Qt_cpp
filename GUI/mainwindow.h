#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "./ui_mainwindow.h"
#include "protocol.h"
#include "registerMapping.h"
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton> // Add this line to include QPushButton
#include <QScrollArea>
#include <QSpinBox>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <config_generated.h> // Generated from config.fbs
#include <flatbuffers/flatbuffers.h>
#include <qfiledialog.h>
#include <unordered_map>
struct FunctionConfig
{
    uint32_t inputType;
    uint32_t outputType;
    QString inputSourcePath;
    std::vector<Register> registers;
    std::vector<Register> commonRegisters;
};

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    void setupCommonRegisterTable();
    void resizeEvent(QResizeEvent *event);
    explicit MainWindow(QWidget *parent = nullptr);
    void setScrollArea();
    ~MainWindow();

    RegisterManager registerManager;
    CommonRegister commonRegister;
    QString currentFunctionName;
    QString toSavefunctionName;
    QScrollArea *commonRegisterScrollArea;
    QTableWidget *commonRegisterTable;
    FlatBuffer flatBuffer;
    QVBoxLayout *registerLayout;       // 레지스터 탭의 레이아웃
    QScrollArea *scrollArea;           // 스크롤 영역 멤버 변수
    QWidget *scrollAreaWidgetContents; // 스크롤 영역의 내부 위젯
    QPushButton *previousButton;       // 이전에 클릭된 버튼을 저장하는 포인터

  private slots:
    void onGenerateFlatBuffer(); // Slot for button click
    void deserializeFlatBuffer(const std::string &fileName);
    void displayRegisterListForFunction(const QString &funtioncName,
                                        std::vector<Register> registers);
    void loadRegisterSet(const QString &functionName);
    void onClickFuntionButton();
    void onGlobalFixedPointChanged(int state);
    void onSelectInputSource();
    void onInputTypeChanged(int value);
    void onOutputTypeChanged(int value);
    void onSaveCommonRegisters();
    void onRunCompare();
    void onDeleteFunction(QPushButton *functionSelectButton, QPushButton *deleteButton,
                          QHBoxLayout *buttonLayout, QVBoxLayout *testListLayout);
    void saveFunctionConfig();
    void onSelcetFunction(int index);

  private:
    void setupConnections();
    void slotConnections();
    void updateFunctionConfigUI(const QString &functionName);
    void updateCommonRegisterTable(const QString &functionName);

    Ui::MainWindow *ui; // Add this line to declare ui
    std::vector<std::string> functionList;
    std::vector<QLineEdit *> currentFieldName; // 동적으로 생성된 레지스터 입력 필드 저장소
    std::vector<QLineEdit *> currentValue; // 동적으로 생성된 레지스터 입력 필드 저장소
    std::vector<QCheckBox *> currentFixedPoint; // 동적으로 생성된 레지스터 입력 필드 저장소
    std::vector<QWidget *> currentWidgets; // 동적으로 생성된 레지스터 입력 필드 저장소
    std::unordered_map<QString, std::vector<Register>> settingValueMap;
    std::vector<QPushButton *> functionButtons;
    QPushButton *currentActivatedButton;
    uint32_t inputType;
    uint32_t outputType;
    QString inputSourcePath;
    std::unordered_map<QString, FunctionConfig> functionConfigs;
};

#endif // MAINWINDOW_H