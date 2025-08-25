#include "stdafx.h"
#include "TodoAppOnQt.h" // UI ���谡 ���Ե� �ڵ� ���� ���

// ������ ����
TodoAppOnQt::TodoAppOnQt(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TodoAppOnQtClass())
{
    ui->setupUi(this); // UI ������ ���� TodoAppOnQt�� ����

    // Qt Designer���� ��ư�� objectName�� "addButton"���� �����ߴٸ� ������ ���� ����
    connect(ui->addButton, &QPushButton::clicked, this, &TodoAppOnQt::on_addButton_clicked);

    // Qt Designer���� ��ư�� objectName�� "deleteButton"���� �����ߴٸ� ������ ���� ����
    connect(ui->deleteButton, &QPushButton::clicked, this, &TodoAppOnQt::on_deleteButton_clicked);

    // Qt Designer���� QLineEdit�� objectName�� "taskLineEdit"���� �����ߴٸ� ������ ���� ���
    ui->taskLineEdit->setPlaceholderText("�� �� ���� �Է��ϼ���...");

    // �� ���� �� ����� �� �� ��� �ҷ�����
    loadTasksFromFile();
}

// �Ҹ��� ����
TodoAppOnQt::~TodoAppOnQt()
{
    // �� ���� �� ���� �� �� ��� ����
    saveTasksToFile();
    delete ui;
}

// "�� �� �߰�" ��ư Ŭ�� �� ���� �Լ�
void TodoAppOnQt::on_addButton_clicked()
{
    // Qt Designer���� QLineEdit�� objectName�� "taskLineEdit"���� �����ߴٸ� ������ ���� ���
    QString taskText = ui->taskLineEdit->text().trimmed();

    if (!taskText.isEmpty()) {
        // Qt Designer���� QListWidget�� objectName�� "taskListWidget"���� �����ߴٸ� ������ ���� ���
        ui->taskListWidget->addItem(taskText);
        ui->taskLineEdit->clear();
        qDebug() << "�� �� �߰���: " << taskText;
        saveTasksToFile(); // �� �� �߰� �� ��� ����
    }
    else {
        qDebug() << "�Էµ� �� ���� �����ϴ�.";
    }
}

// "���� �׸� ����" ��ư Ŭ�� �� ���� �Լ�
void TodoAppOnQt::on_deleteButton_clicked()
{
    // Qt Designer���� QListWidget�� objectName�� "taskListWidget"���� �����ߴٸ� ������ ���� ���
    QListWidgetItem* selectedItem = ui->taskListWidget->currentItem();

    if (selectedItem) {
        int row = ui->taskListWidget->row(selectedItem);
        delete ui->taskListWidget->takeItem(row);
        qDebug() << "�� �� ������: " << selectedItem->text();
        saveTasksToFile(); // �� �� ���� �� ��� ����
    }
    else {
        qDebug() << "������ �� ���� ���õ��� �ʾҽ��ϴ�.";
    }
}

// �� �� ���� ��θ� ��ȯ�ϴ� �Լ�
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


// �� �� �����͸� ���Ͽ� �����ϴ� �Լ�
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
        qDebug() << "�� �� ���� �Ϸ�: " << filePath;
    }
    else {
        qDebug() << "�� �� ���� ����: ������ �� �� �����ϴ�. " << saveFile.errorString();
    }
}

// ���Ͽ��� �� �� �����͸� �ҷ����� �Լ�
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
            qDebug() << "�� �� �ҷ����� �Ϸ�: " << filePath;
        }
        else {
            qDebug() << "����� ������ ��ȿ�� JSON �迭�� �ƴմϴ�.";
        }
    }
    else {
        qDebug() << "�� �� �ҷ����� ����: ������ �� �� �����ϴ�. " << loadFile.errorString();
        qDebug() << "���ο� �� �� ����� �����˴ϴ�.";
    }
}


