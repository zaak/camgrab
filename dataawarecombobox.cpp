#include "dataawarecombobox.h"

DataAwareComboBox::DataAwareComboBox(QWidget *parent)
    :QComboBox(parent)
{
    connect(this, SIGNAL(activated(int)), this, SLOT(triggerVariantActivated(int)));
}

void DataAwareComboBox::triggerVariantActivated(int index)
{
    emit activated(itemData(index));
}
