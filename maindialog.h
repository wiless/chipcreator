#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
namespace Ui {
class MainDialog;
}


struct JsonModule {
    int Id;
    QString Name;
    QString Desc;
    QStringList InPins;
    QStringList OutPins;
    QString Function;
//    TestNames []string
    QString ArgString;
    // FunctionName string
     QString toString();
    JsonModule(){
        Id=0;
    }
};



struct JsonPin {
    int Id;
   QString Name;
   QString Desc;
   QString DataType;
    bool InputPin;
   QString ModuleName;

   QString toString();
    JsonPin(){
        Id=0;
    }
};

class MainDialog : public QDialog
{
    Q_OBJECT
private:
    QList<JsonModule> modules;
    QList<JsonPin> pins;
    int pinid;
    int modid;
    QStringList pinlist;
public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

private slots:
    void on_pushButton_clicked();

    void on_toolButton_clicked();

    void on_pushButton_3_clicked();



    void on_listPins_itemActivated(QListWidgetItem *item);

    void on_pushButton_4_clicked();

    void on_listModules_itemActivated(QListWidgetItem *item);

    void on_btnApplyModule_clicked();



    void on_listPins_itemSelectionChanged();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_cmbDataType_activated(const QString &arg1);

    void on_listPins_currentRowChanged(int currentRow);

private:
    Ui::MainDialog *ui;
};

#endif // MAINDIALOG_H
