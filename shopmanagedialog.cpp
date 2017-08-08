#include <QStandardItemModel>
#include "shopmanagedialog.h"
#include "ui_shopmanagedialog.h"
#include "shopnamestore.h"

ShopManageDialog::ShopManageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShopManageDialog)
{
    ui->setupUi(this);

    init();
}

ShopManageDialog::~ShopManageDialog()
{
    delete ui;
}

void ShopManageDialog::on_saveButton_clicked()
{
    const QString& name = ui->nameEdit->text();
    if (!name.isEmpty())
    {
        ShopNameStore::instance()->insert(name);

        QStandardItemModel* model = (QStandardItemModel*)ui->shopView->model();
        if (NULL == model)
        {
            // TODO: Message Box
            return;
        }

        int col = 0;
        int row = model->rowCount();
        model->setItem(row, col++, new QStandardItem(name));
    }
}

void ShopManageDialog::init()
{
    QStandardItemModel* model = new QStandardItemModel(0,1);
    ui->shopView->setModel(model);
    int col = 0;
    model->setHeaderData(col++, Qt::Horizontal, tr("商场名称"));

    const QVector<QString>& names = ShopNameStore::instance()->getNames();
    for (int i = 0; i < names.size(); ++i)
    {
        model->setItem(i, 0, new QStandardItem(names[i]));
    }
}
