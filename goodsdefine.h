#ifndef INVOICEDEFINE_H
#define INVOICEDEFINE_H

#include <QString>

struct GoodsAttribute
{
    QString name;
    QString price;
    QString attribute;
    QString count;
};

struct Goods : public GoodsAttribute
{
    QString id;
    QString shopName;
    QString date;
    int settled;

    Goods* clone()
    {
        Goods* g = new Goods();
        g->name = name;
        g->price = price;
        g->attribute = attribute;
        g->count = count;
        g->id = id;
        g->shopName = shopName;
        g->date = date;
        g->settled = settled;
        return g;
    }
};

#endif // INVOICEDEFINE_H
