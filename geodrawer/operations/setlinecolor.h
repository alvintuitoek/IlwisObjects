#ifndef SETLINECOLOR_H
#define SETLINECOLOR_H

namespace Ilwis {
namespace Ui {

class SetLineColor : public DrawerOperation
{
public:
    SetLineColor();

    SetLineColor(quint64 metaid, const Ilwis::OperationExpression &expr);

    bool execute(ExecutionContext *ctx,SymbolTable& symTable);
    static Ilwis::OperationImplementation *create(quint64 metaid,const Ilwis::OperationExpression& expr);
    Ilwis::OperationImplementation::State prepare(ExecutionContext *ctx, const SymbolTable &);

    static quint64 createMetadata();

    NEW_OPERATION(SetLineColor);

private:
    QColor _lineColor = true;
    int _index = iUNDEF;
    QString _code = sUNDEF;
    LayerModel  *_layer;
};
}
}

#endif // SETLINECOLOR_H
