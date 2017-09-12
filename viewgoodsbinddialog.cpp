#include <QDebug>
#include <QStandardItemModel>
#include "viewgoodsbinddialog.h"
#include "ui_viewgoodsbinddialog.h"
#include "store.h"
#include "orderdefine.h"
#include "goodsdefine.h"

ViewGoodsBindDialog::ViewGoodsBindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewGoodsBindDialog)
{
    ui->setupUi(this);

    init();
}

ViewGoodsBindDialog::~ViewGoodsBindDialog()
{
    delete ui;
}

void ViewGoodsBindDialog::setOrder(Order* order)
{
    m_order = order;
}

void ViewGoodsBindDialog::load()
{
    QStandardItemModel* model = (QStandardItemModel*)ui->goodsView->model();
    if (NULL == model)
    {
        qDebug() << "empty model";
        return;
    }

    m_goodsID = m_order->goods_id.split(",");

    for (int i = 0; i < m_goodsID.size(); i++)
    {
        Goods* goods = Store::instance()->getGoodsByID(m_goodsID[i]);
        if (NULL != goods)
        {
            int col = 0;
            model->setItem(i, col++, new QStandardItem(goods->name));
            model->setItem(i, col++, new QStandardItem(goods->price));
            model->setItem(i, col++, new QStandardItem(goods->attribute));
            model->setItem(i, col++, new QStandardItem(goods->count));
        }
    }
}

void ViewGoodsBindDialog::on_detachButton_clicked()
{
    int row = ui->goodsView->currentIndex().row();
    if (0 > row || row >= m_goodsID.size())
    {
        return;
    }

    if (false == Store::instance()->orderDetachGoods(m_order, m_goodsID[row]))
    {
        qDebug() << "detach goods from order failed";
        // QMessageBox
        return;
    }

    m_goodsID.removeAt(row);
    QString goodsID;
    for (int i = 0; i < m_goodsID.size(); i++)
    {
        goodsID += m_goodsID[i];
        goodsID += ",";
    }
    goodsID = goodsID.left(goodsID.length()-1);
    m_order->goods_id = goodsID;

    QAbstractItemModel* model = ui->goodsView->model();
    model->removeRow(row);
}

void ViewGoodsBindDialog::init()
{
    QStandardItemModel* goodsModel = new QStandardItemModel(0, 4);
    ui->goodsView->setModel(goodsModel);
    int col = 0;
    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("商品名"));
    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("价格"));
    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("属性"));
    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("数量"));

    ui->goodsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->goodsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
