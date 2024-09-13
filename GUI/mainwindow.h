#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "registerMapping.h"
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton> // Add this line to include QPushButton
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>
#include <config_generated.h> // Generated from config.fbs
#include <flatbuffers/flatbuffers.h>
#include <unordered_map>

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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setScrollArea();
    std::vector<std::string> functionList;
    RegisterManager registerManager;
    QString currentFunctionName;
    QString toSavefunctionName;

  private slots:
    void onGenerateFlatBuffer(); // Slot for button click
    void deserializeFlatBuffer(const std::string &fileName);
    void displayRegisterListForFunction(const QString &funtioncName,
                                        std::vector<Register> registers);
    void loadRegisterSet(const QString &functionName);
    void onClickFuntionButton();

  private:
    Ui::MainWindow *ui;                        // Add this line to declare ui
    QVBoxLayout *registerLayout;               // 레지스터 탭의 레이아웃
    std::vector<QLineEdit *> currentFieldName; // 동적으로 생성된 레지스터 입력 필드 저장소
    std::vector<QLineEdit *> currentValue; // 동적으로 생성된 레지스터 입력 필드 저장소
    QScrollArea *scrollArea;               // 스크롤 영역 멤버 변수
    QWidget *scrollAreaWidgetContents; // 스크롤 영역의 내부 위젯
    QPushButton *previousButton;       // 이전에 클릭된 버튼을 저장하는 포인터
    std::string getAddressForField(const std::string &fieldName); // Address mapping logic
    std::unordered_map<QString, std::vector<Register>> settingValueMap;
    std::vector<QPushButton *> functionButtons;
    QPushButton *currentActivatedButton;

    void generateFlatBuffer(const std::vector<std::string> &fieldNames,
                            const std::vector<uint64_t> &addresses,
                            const std::vector<float> &values);

    void selcetFunction(int index);
};
extern std::unordered_map<std::string, std::string> fieldNameToAddress;
#endif // MAINWINDOW_H