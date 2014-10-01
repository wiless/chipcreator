#include "maindialog.h"
#include "ui_maindialog.h"
#include <QListWidgetItem>
#include <QFileDialog>
#include <qdebug.h>
#include <QTextStream>


QString appendString(QString str)
{
    return '"'+str+'"';
}
QString appendBool(bool value)
{

    if (value)
        return "true";
    else
        return "false";

}
QString appendNum(int number)
{
    QString str;
    str=str.sprintf("%d",number);
    return str;
}


QString appendStringList(QStringList strlist)
{

    QString result;
    result="[";
    for (int k=0;k<strlist.length();k++) {
        strlist[k]=appendString(strlist[k]);

    }
    result+=strlist.join(" ");
    result+="]";
    return result;
}
MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
    pinid=0;
    modid=0;

}
QString JsonModule::toString(){
    QString str;
    str="{\n";
    str+=appendString("Id")+":"+appendNum(Id)+",\n";
    str+=appendString("Name")+":"+appendString(Name)+",\n";
    str+=appendString("InPins")+":"+appendStringList(InPins)+",\n";
    str+=appendString("OutPins")+":"+appendStringList(OutPins)+",\n";
    str+=appendString("Function")+":"+appendString(Function);
    str+="\n}";
    return str;
}

QString JsonPin::toString(){
    QString str;
    str="{\n";
    str+=appendString("Id")+":"+appendNum(Id)+",\n";
    str+=appendString("Name")+":"+appendString(Name)+",\n";
    str+=appendString("Desc")+":"+appendString(Desc)+",\n";
    str+=appendString("DataType")+":"+appendString(DataType)+",\n";
    str+=appendString("InputPin")+":"+appendBool(InputPin);
    str+="\n}";
    return str;
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::on_pushButton_clicked()
{
    JsonPin pin;
    pin.Id=0;
    pin.Name=ui->txtPinName->text();
    pin.Desc=ui->txtDesc->toPlainText();
    pin.InputPin= ui->chkInput->isChecked();
    pin.DataType=ui->cmbDataType->currentText();
    pins.append(pin);
    ui->listPins->addItem(pin.Name);
}

void MainDialog::on_toolButton_clicked()
{
    ui->txtFullText->clear();

    QString chipstr;
    chipstr.sprintf("{\n");
    chipstr+=appendString("Name")+":";
    chipstr+=appendString(ui->lineChipName->text())+",";
    ui->txtFullText->append(chipstr);
    /// Generate json for pins
    ///
    ui->txtFullText->append("\"Pins\": [");
    for(int k=0;k<pins.count();k++)
    {
        pins[k].Id=k;
        QString pinstr=pins[k].toString();
        if(k<pins.count()-1)
            pinstr+=",";
        ui->txtFullText->append(pinstr);


    }
    ui->txtFullText->append("]");


    /// FINISH PINS..
    /// ADD MODULE STRINGS
    ui->txtFullText->append(",");
    ui->txtFullText->append("\"Modules\": [");
    for(int k=0;k<modules.count();k++)
    {
        modules[k].Id=k;
        QString modstr=modules[k].toString();
        if(k<modules.count()-1)
            modstr+=",";
        ui->txtFullText->append(modstr);


    }
    ui->txtFullText->append("]");


    /// FINISH MODULES
    chipstr="\n}";
    ui->txtFullText->append(chipstr);
}

void MainDialog::on_pushButton_3_clicked()
{

       ui->listPins->clear();
       ui->listModules->clear();
       pins.clear();
       modules.clear();
    JsonPin pin;
    int inPins,outPins;
    inPins=ui->inPins->value();
    outPins=ui->outPins->value();
    for(int k=0;k<inPins;k++)
    {
        pin.Id=k;
        pin.Name.sprintf("inputPin%d",k);
        pin.Desc="";
        pin.InputPin= true;
        pin.DataType="BitChannel";
        pins.append(pin);
        QListWidgetItem *item=new QListWidgetItem(pin.Name);
        item->setData(Qt::UserRole,pins.length()-1);
        ui->listPins->addItem(item);
    }
    for(int k=0;k<outPins;k++)
    {
        pin.Id=k+inPins;
        pin.Name.sprintf("outputPin%d",k);
        pin.Desc="";
        pin.InputPin= false;
        pin.DataType="Complex128Channel";
        pins.append(pin);


        QListWidgetItem *item=new QListWidgetItem(pin.Name);
        //        QVariant vdata;
        item->setData(Qt::UserRole,pins.length()-1);
        ui->listPins->addItem(item);
    }

    int Nmodules=ui->nModules->value();
    for(int k=0;k<Nmodules;k++)
    {
        JsonModule mod;
        if(Nmodules==1)
        {
            mod.Name.sprintf("default");
               mod.Function="ModuleFn";
        }
        else
        {
            mod.Name.sprintf("Module%d",k);
            mod.Function.sprintf("ModulateFn%d",k);

        }
        mod.Id=k;

        modules.append(mod);
        QListWidgetItem *item=new QListWidgetItem(mod.Name);
        //        QVariant vdata;
        item->setData(Qt::UserRole,modules.length()-1);
        ui->listModules->addItem(item);
    }



}
void MainDialog::on_listModules_itemActivated(QListWidgetItem *item)
{
    JsonModule mod=modules[item->data(Qt::UserRole).toInt()];
    ui->txtModuleName->setText( mod.Name);
    ui->txtFunctionName->setText(mod.Function);
    ui->txtInPins->setText(mod.InPins.join(" "));
    ui->txtOutPins->setText(mod.OutPins.join(" "));
}

void MainDialog::on_listPins_itemActivated(QListWidgetItem *item)
{
    JsonPin pin=pins[item->data(Qt::UserRole).toInt()];
    ui->txtPinName->setText( pin.Name);
    ui->txtDesc->setPlainText(pin.Desc);
    ui->chkInput->setChecked(pin.InputPin);
    int indx=ui->cmbDataType->findText(pin.DataType);
    ui->cmbDataType->setCurrentIndex(indx); // Text(indx,pin.DataType);
}

void MainDialog::on_pushButton_4_clicked()
{
    //    JsonPin pin;
    int indx=ui->listPins->currentItem()->data(Qt::UserRole).toInt();
    JsonPin pin=pins[indx];
    //    pin.Id=0;
    pin.Name=ui->txtPinName->text();
    pin.Desc=ui->txtDesc->toPlainText();
    pin.InputPin= ui->chkInput->isChecked();
    pin.DataType=ui->cmbDataType->currentText();
    pins[indx]=pin;

    ui->listPins->currentItem()->setText(pin.Name);
}


void MainDialog::on_btnApplyModule_clicked()
{
    int indx=ui->listModules->currentItem()->data(Qt::UserRole).toInt();
    JsonModule mod=modules[indx];

    mod.Name=ui->txtModuleName->text();
    mod.Function=ui->txtFunctionName->text();
    mod.InPins= ui->txtInPins->text().split(" ");
    mod.OutPins=ui->txtOutPins->text().split(" ");
    modules[indx]=mod;

    ui->listModules->currentItem()->setText(mod.Name);
}


void MainDialog::on_listPins_itemSelectionChanged()
{
    pinlist.clear();
    QList <QListWidgetItem*>  items=    ui->listPins->selectedItems();
    foreach (QListWidgetItem *item, items) {
        pinlist << item->text();
    }

}

void MainDialog::on_pushButton_2_clicked()
{
    ui->txtInPins->setText(pinlist.join(" "));
}

void MainDialog::on_pushButton_5_clicked()
{
     ui->txtOutPins->setText(pinlist.join(" "));
}

void MainDialog::on_pushButton_6_clicked()
{
    /// Save the file
    ///
    QString fname = QFileDialog::getSaveFileName(this, tr("Save Json"),
                               ui->lineChipName->text()+".json",
                               "JSON  (*.json)");
//    QString fname=QFileDialog::getSaveFileName(this,"Save Json File to","","*.json");
    QFile file(fname);
    file.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Truncate);
    QTextStream ts(&file);
    ts << ui->txtFullText->toPlainText();
    file.close();
}

void MainDialog::on_cmbDataType_activated(const QString &arg1)
{

}

void MainDialog::on_listPins_currentRowChanged(int currentRow)
{

}
