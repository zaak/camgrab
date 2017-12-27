#ifndef CAMERACOMBOBOX_H
#define CAMERACOMBOBOX_H

#include <QComboBox>

class DataAwareComboBox : public QComboBox
{
    Q_OBJECT

private slots:
    void triggerVariantActivated(int index);

public:
    DataAwareComboBox(QWidget *parent = nullptr);

signals:
    void activated(const QVariant &);
};

#endif // CAMERACOMBOBOX_H
