#ifndef GRAPHER2D_H
#define GRAPHER2D_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

#include <qmath.h>

class Grapher2D : public QWidget
{
    Q_OBJECT

    static constexpr float DEFABSSHIFT      = 0.0;      /// Сдвиг по оси абсцисс по умолчанию
    static constexpr float DEFORDSHIFT      = 0.0;      /// Сдвиг по оси ординат по умолчанию
    static constexpr float DEFABSMEASURE    = 1.0;      /// Количество единиц измерения в одном делении оси абсцисс по умолчанию
    static constexpr float DEFORDMEASURE    = 1.0;      /// Количество единиц измерения в одном делении оси ординат по умолчанию
    static constexpr float DEFABSEXPAND     = 1.0;      /// Растяжение оси абсцисс по умолчанию
    static constexpr float DEFORDEXPAND     = 1.0;      /// Растяжение оси ординат по умолчанию

    static constexpr int DEFZOOMVALUE       = 10;       /// Количество пикселей в одном делении по умолчанию
    static constexpr int DEFZOOMMIN         = 1;        /// Нижняя граница зумирования по умолчанию
    static constexpr int DEFZOOMMAX         = 1000;     /// Верхняя граница зумирования по умолчанию
    static constexpr int DEFZOOMSTEP        = 4;        /// Шаг изменения зумирования по умолчанию

public:
    Grapher2D(QWidget *parent = 0);
    ~Grapher2D();

protected:
    virtual void mousePressEvent(QMouseEvent * _mEvent);
    virtual void mouseMoveEvent(QMouseEvent * _mEvent);
    virtual void mouseReleaseEvent(QMouseEvent * _mEvent);
    virtual void wheelEvent(QWheelEvent * _wEvent);
    virtual void paintEvent(QPaintEvent * _pEvent);


    /// ПАРАМЕТРЫ СИСТЕМЫ КООРДИНАТ

    /// Смещение системы координат
    inline float getCSAbsTranslate();
    inline float getCSOrdTranslate();

    /// Масштаб системы координат
    inline float getCSAbsScale();
    inline float getCSOrdScale();

    /// Отображаемые координатные углы
    bool setCSAngles(bool _first = true, bool _second = true,
                     bool _third = true, bool _fourth = true);
    inline bool *getCSAngles();


    /// ПАРАМЕТРЫ ОСИ АБСЦИСС

    /// Сдвиг относительно центра окна
    inline void setCSAbsShift(float _shift = DEFABSSHIFT);
    inline float getCSAbsShift();

    /// Количество единиц измерения в одном делении
    bool setCSAbsMeasure(float _measure = DEFABSMEASURE);
    inline float getCSAbsMeasure();

    /// Растяжение осей
    bool setCSAbsExpansion(float _expansion = DEFABSEXPAND);
    inline float getCSAbsExpansion();

    /// Отображение цифровых значений
    inline void setCSAbsValues(bool _values = true);
    inline bool isCSAbsValues();

    /// Отображение пунктирных линий
    inline void setCSAbsDashLines(bool _dashLines = true);
    inline bool isCSAbsDashLines();


    /// ПАРАМЕТРЫ ОСИ ОРДИНАТ

    /// Сдвиг относительно центра окна
    inline void setCSOrdShift(float _shift = DEFORDSHIFT);
    inline float getCSOrdShift();

    /// Количество единиц измерения в одном делении
    bool setCSOrdMeasure(float _measure = DEFORDMEASURE);
    inline float getCSOrdMeasure();

    /// Растяжение осей
    bool setCSOrdExpansion(float _expansion = DEFORDEXPAND);
    inline float getCSOrdExpansion();

    /// Отображение цифровых значений
    inline void setCSOrdValues(bool _values = true);
    inline bool isCSOrdValues();

    /// Отображение пунктирных линий
    inline void setCSOrdDashLines(bool _dashLines = true);
    inline bool isCSOrdDashLines();


    /// ПАРАМЕТРЫ ЗУМИРОВАНИЯ

    /// Количество пикселей в одном делении
    bool setCSZoom(int _zoom = DEFZOOMVALUE);
    inline int getCSZoom();

    /// Нижняя граница зумирования
    bool setCSZoomMin(int _zoomMin = DEFZOOMMIN);
    inline int getCSZoomMin();

    /// Верхняя граница зумирования
    bool setCSZoomMax(int _zoomMax = DEFZOOMMAX);
    inline int getCSZoomMax();

    /// Шаг изменения зума
    bool setCSZoomStep(int _zoomStep = DEFZOOMSTEP);
    inline int getCSZoomStep();

    /// Зумирование относительно центра системы координат
    inline void setCSZoomCenter(bool _zoomCenter = false);
    inline bool isCSZoomCenter();


    /// ПАРАМЕТРЫ КУРСОРА МЫШИ

    /// Последняя позиция курсора мыши при нажатии
    inline QPoint getMPosClick();

    /// Последняя позиция курсора мыши без нажатия
    inline QPoint getMPosNoClick();

    /// Тип курсора
    inline void setMCursorShape(Qt::CursorShape _cursorShape = Qt::OpenHandCursor);
    inline Qt::CursorShape getMCursorShape();

private:
    void translocationCoordinateSystem();   /// Смещение системы координат

    struct CoordinateSystem     /// Структура системы координат (СК)
    {
        bool angles[4];         /// Координатные углы

        struct Axis             /// Структура координатной оси
        {
            float shift;        /// Сдвиг относительно центра окна
            float measure;      /// Количество единиц измерения в одном делении
            float expansion;    /// Коэффициентр растяжения
            bool values;        /// Флаг отображения цифровых значений
            bool dashLines;     /// Флаг отображения пунктирных линий
        } abs, ord;             /// Оси абсцисс и ординат

        struct Zoom             /// Структура зумирования
        {
            int value;          /// Количество пикселей в одном делении
            int min;            /// Нижняя граница зумирования
            int max;            /// Верхняя граница зумирования
            int step;           /// Шаг изменения зума
            bool center;        /// Зумирование относительно центра СК
        } zoom;
    } coordSystem;

    struct Mouse                        /// Структура курсора мыши
    {
        struct Pos                      /// Структура последней позиции курсора
        {
            QPoint click;               /// Позиция курсора при нажатии
            QPoint noClick;             /// Позиция курсора без нажатия
        } pos;

        Qt::CursorShape cursorShape;    /// Текущий тип курсора
    } mouse;
};


/// ПАРАМЕТРЫ СИСТЕМЫ КООРДИНАТ

/// Текущее смещение системы координат по оси абсцисс
float Grapher2D::getCSAbsTranslate()
{
    return width() / 2.0 + coordSystem.abs.shift;
}

/// Текущее смещение системы координат по оси ординат
float Grapher2D::getCSOrdTranslate()
{
    return height() / 2.0 + coordSystem.ord.shift;
}

/// Текущий масштаб системы координат по оси абсцисс
float Grapher2D::getCSAbsScale()
{
    return coordSystem.zoom.value * coordSystem.abs.expansion / coordSystem.abs.measure;
}

/// Текущий масштаб системы координат по оси ординат
float Grapher2D::getCSOrdScale()
{
    return - coordSystem.zoom.value * coordSystem.ord.expansion / coordSystem.ord.measure;
}

/// Отображаемые координатные углы
bool* Grapher2D::getCSAngles()
{
    return coordSystem.angles;
}


/// ПАРАМЕТРЫ ОСИ АБСЦИСС

/// Установка сдвига относительно центра окна по оси абсцисс
void Grapher2D::setCSAbsShift(float _shift)
{
    coordSystem.abs.shift = _shift;
}

/// Текущий сдвиг относительно центра окна по оси абсцисс
float Grapher2D::getCSAbsShift()
{
    return coordSystem.abs.shift;
}

/// Текущее количество единиц измерения в одном делении по оси абсцисс
float Grapher2D::getCSAbsMeasure()
{
    return coordSystem.abs.measure;
}

/// Текущее растяжение оси абсцисс
float Grapher2D::getCSAbsExpansion()
{
    return coordSystem.abs.expansion;
}

/// Установка флага отображения цифровых значений по оси абсцисс
void Grapher2D::setCSAbsValues(bool _values)
{
    coordSystem.abs.values = _values;
}

/// Текущее значение флага отображения цифровых значений по оси абсцисс
bool Grapher2D::isCSAbsValues()
{
    return coordSystem.abs.values;
}

/// Установка флага отображения пунктирных линий по оси абсцисс
void Grapher2D::setCSAbsDashLines(bool _dashLines)
{
    coordSystem.abs.dashLines = _dashLines;
}

/// Текущее значение флага отображения пунктирных линий по оси абсцисс
bool Grapher2D::isCSAbsDashLines()
{
    return coordSystem.abs.dashLines;
}


/// ПАРАМЕТРЫ ОСИ ОРДИНАТ

/// Установка сдвига относительно центра окна по оси ординат
void Grapher2D::setCSOrdShift(float _shift)
{
    coordSystem.ord.shift = _shift;
}

/// Текущий сдвиг относительно центра окна по оси ординат
float Grapher2D::getCSOrdShift()
{
    return coordSystem.ord.shift;
}

/// Текущее количество единиц измерения в одном делении по оси ординат
float Grapher2D::getCSOrdMeasure()
{
    return coordSystem.ord.measure;
}

/// Текущее растяжение оси ординат
float Grapher2D::getCSOrdExpansion()
{
    return coordSystem.ord.expansion;
}

/// Установка флага отображения цифровых значений по оси ординат
void Grapher2D::setCSOrdValues(bool _values)
{
    coordSystem.ord.values = _values;
}

/// Текущее значение флага отображения цифровых значений по оси ординат
bool Grapher2D::isCSOrdValues()
{
    return coordSystem.ord.values;
}

/// Установка флага отображения пунктирных линий по оси ординат
void Grapher2D::setCSOrdDashLines(bool _dashLines)
{
    coordSystem.ord.dashLines = _dashLines;
}

/// Текущее значение флага отображения пунктирных линий по оси ординат
bool Grapher2D::isCSOrdDashLines()
{
    return coordSystem.ord.dashLines;
}


/// ПАРАМЕТРЫ ЗУМИРОВАНИЯ

/// Текущее количество пикселей в одном делении
int Grapher2D::getCSZoom()
{
    return coordSystem.zoom.value;
}

/// Нижняя граница зумирования
int Grapher2D::getCSZoomMin()
{
    return coordSystem.zoom.min;
}

/// Верхняя граница зумирования
int Grapher2D::getCSZoomMax()
{
    return coordSystem.zoom.max;
}

/// Текущий шаг изменения зума
int Grapher2D::getCSZoomStep()
{
    return coordSystem.zoom.step;
}

/// Установка флага зумирования относительно центра системы координат
void Grapher2D::setCSZoomCenter(bool _zoomCenter)
{
    coordSystem.zoom.center = _zoomCenter;
}

/// Текущее значение флага зумирования относительно центра системы координат
bool Grapher2D::isCSZoomCenter()
{
    return coordSystem.zoom.center;
}


/// ПАРАМЕТРЫ КУРСОРА МЫШИ

/// Последняя позиция курсора мыши при нажатии
QPoint Grapher2D::getMPosClick()
{
    return mouse.pos.click;
}

/// Последняя позиция курсора мыши без нажатия
QPoint Grapher2D::getMPosNoClick()
{
    return mouse.pos.noClick;
}

/// Установка типа курсора
void Grapher2D::setMCursorShape(Qt::CursorShape _cursorShape)
{
    mouse.cursorShape = _cursorShape;
}

/// Текущий тип курсора
Qt::CursorShape Grapher2D::getMCursorShape()
{
    return mouse.cursorShape;
}

#endif // GRAPHER2D_H
