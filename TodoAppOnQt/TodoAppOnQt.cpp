#include "stdafx.h"
#include "TodoAppOnQt.h" // UI 설계가 포함된 자동 생성 헤더

// 생성자 구현
TodoAppOnQt::TodoAppOnQt(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TodoAppOnQtClass())
{
    ui->setupUi(this); // UI 설정을 현재 TodoAppOnQt에 적용

    // Qt Designer에서 버튼의 objectName을 "addButton"으로 설정했다면 다음과 같이 연결
    connect(ui->addButton, &QPushButton::clicked, this, &TodoAppOnQt::on_addButton_clicked);

    // Qt Designer에서 버튼의 objectName을 "deleteButton"으로 설정했다면 다음과 같이 연결
    connect(ui->deleteButton, &QPushButton::clicked, this, &TodoAppOnQt::on_deleteButton_clicked);

    // Qt Designer에서 QLineEdit의 objectName을 "taskLineEdit"으로 설정했다면 다음과 같이 사용
    ui->taskLineEdit->setPlaceholderText("새 할 일을 입력하세요...");

    // 앱 시작 시 저장된 할 일 목록 불러오기
    loadTasksFromFile();
}

// 소멸자 구현
TodoAppOnQt::~TodoAppOnQt()
{
    // 앱 종료 시 현재 할 일 목록 저장
    saveTasksToFile();
    delete ui;
}

// "할 일 추가" 버튼 클릭 시 슬롯 함수
void TodoAppOnQt::on_addButton_clicked()
{
    // Qt Designer에서 QLineEdit의 objectName을 "taskLineEdit"으로 설정했다면 다음과 같이 사용
    QString taskText = ui->taskLineEdit->text().trimmed();

    if (!taskText.isEmpty()) {
        // Qt Designer에서 QListWidget의 objectName을 "taskListWidget"으로 설정했다면 다음과 같이 사용
        ui->taskListWidget->addItem(taskText);
        ui->taskLineEdit->clear();
        qDebug() << "할 일 추가됨: " << taskText;
        saveTasksToFile(); // 할 일 추가 후 즉시 저장
    }
    else {
        qDebug() << "입력된 할 일이 없습니다.";
    }
}

// "선택 항목 삭제" 버튼 클릭 시 슬롯 함수
void TodoAppOnQt::on_deleteButton_clicked()
{
    // Qt Designer에서 QListWidget의 objectName을 "taskListWidget"으로 설정했다면 다음과 같이 사용
    QListWidgetItem* selectedItem = ui->taskListWidget->currentItem();

    if (selectedItem) {
        int row = ui->taskListWidget->row(selectedItem);
        delete ui->taskListWidget->takeItem(row);
        qDebug() << "할 일 삭제됨: " << selectedItem->text();
        saveTasksToFile(); // 할 일 삭제 후 즉시 저장
    }
    else {
        qDebug() << "삭제할 할 일이 선택되지 않았습니다.";
    }
}

// 할 일 파일 경로를 반환하는 함수
QString TodoAppOnQt::getTodoFilePath() const
{
    QString appDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (appDataLocation.isEmpty()) {
        qDebug() << "Warning: AppDataLocation not found, using current directory.";
        return QCoreApplication::applicationDirPath() + "/todos.json";
    }

    QDir appDataDir(appDataLocation);
    if (!appDataDir.exists()) {
        appDataDir.mkpath(".");
    }

    return appDataLocation + "/todos.json";
}


// 할 일 데이터를 파일에 저장하는 함수
void TodoAppOnQt::saveTasksToFile()
{
    QJsonArray taskArray;

    for (int i = 0; i < ui->taskListWidget->count(); ++i) {
        QListWidgetItem* item = ui->taskListWidget->item(i);
        taskArray.append(item->text());
    }

    QJsonDocument doc(taskArray);
    QString filePath = getTodoFilePath();
    QFile saveFile(filePath);

    if (saveFile.open(QIODevice::WriteOnly)) {
        saveFile.write(doc.toJson(QJsonDocument::Indented));
        saveFile.close();
        qDebug() << "할 일 저장 완료: " << filePath;
    }
    else {
        qDebug() << "할 일 저장 실패: 파일을 열 수 없습니다. " << saveFile.errorString();
    }
}

// 파일에서 할 일 데이터를 불러오는 함수
void TodoAppOnQt::loadTasksFromFile()
{
    QString filePath = getTodoFilePath();
    QFile loadFile(filePath);

    if (loadFile.open(QIODevice::ReadOnly)) {
        QByteArray saveData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        loadFile.close();

        if (loadDoc.isArray()) {
            QJsonArray taskArray = loadDoc.array();
            ui->taskListWidget->clear();

            for (const QJsonValue& value : taskArray) {
                ui->taskListWidget->addItem(value.toString());
            }
            qDebug() << "할 일 불러오기 완료: " << filePath;
        }
        else {
            qDebug() << "저장된 파일이 유효한 JSON 배열이 아닙니다.";
        }
    }
    else {
        qDebug() << "할 일 불러오기 실패: 파일을 열 수 없습니다. " << loadFile.errorString();
        qDebug() << "새로운 할 일 목록이 생성됩니다.";
    }
}


