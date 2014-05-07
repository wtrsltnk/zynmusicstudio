#ifndef VIEWSCALE_H
#define VIEWSCALE_H

#include <QObject>

class ViewScale : public QObject
{
    Q_OBJECT

public:
    ViewScale();

    int vertical() const;
    int horizontal() const;

public slots:
    void setVerticalScale(int scale);
    void setHorizontalScale(int scale);

signals:
    void verticalScaleChanged(int scale);
    void horizontalScaleChanged(int scale);

protected:
    int _verticalScale;
    int _horizontalScale;

};

#endif // VIEWSCALE_H
