#ifndef PIXELWIDGET_H
#define PIXELWIDGET_H

#include <QImage>
#include <QSize>
#include <QWidget>

class PixelWidget : public QWidget
{
  Q_OBJECT
public:
  explicit PixelWidget(QWidget *parent = nullptr);

  QSize sizeHint() const override;

  void clear(const QColor& color = Qt::transparent);
  void writePixel(int x, int y, const QColor& color);
  QColor readPixel(int x, int y) const;

  void setBufferSize(int width, int height);
  QSize bufferSize() const;
  bool contains(int x, int y) const;

  int zoom() const;

public slots:
  void setZoom(int zoom);
  void drawSplash();

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  QImage m_image;
  int m_zoom;
};

#endif // PIXELWIDGET_H
