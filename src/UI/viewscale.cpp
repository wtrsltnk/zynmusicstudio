#include "viewscale.h"

ViewScale::ViewScale()
    : _verticalScale(10), _horizontalScale(10)
{ }

int ViewScale::vertical() const
{
    return this->_verticalScale;
}

void ViewScale::setVerticalScale(int scale)
{
    this->_verticalScale = scale;
    this->verticalScaleChanged(scale);
}

int ViewScale::horizontal() const
{
    return this->_horizontalScale;
}

void ViewScale::setHorizontalScale(int scale)
{
    this->_horizontalScale = scale;
    this->horizontalScaleChanged(scale);
}
