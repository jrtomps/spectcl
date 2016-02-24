#ifndef TABBEDMULTISPECTRUMVIEW_H
#define TABBEDMULTISPECTRUMVIEW_H

#include "SpectrumView.h"

#include <QWidget>

class QPushButton;

namespace Ui {
class TabbedMultiSpectrumView;
}

namespace Viewer
{

class MultiSpectrumView;
class SpecTclInterface;

class TabbedMultiSpectrumView : public SpectrumView
{
    Q_OBJECT
    
public:
    explicit TabbedMultiSpectrumView(SpecTclInterface* pSpecTcl, QWidget *parent = 0);
    ~TabbedMultiSpectrumView();

  MultiSpectrumView* addTab(const QString& title);

  // SpectrumViewer interface
  virtual int getRowCount() const;
  virtual int getColumnCount() const;
  virtual QRootCanvas* getCurrentCanvas();
  virtual std::vector<QRootCanvas*> getAllCanvases();

signals:
  void visibleGeometryChanged(int row, int col);

public slots:
  virtual void onGeometryChanged(int row, int col);
  virtual void setCurrentCanvas(QWidget *pCanvas);
  virtual void refreshAll();
  virtual void onHistogramRemoved(HistogramBundle *pBundle);
  virtual void update(HistogramBundle *pHist);
  virtual void drawHistogram(HistogramBundle *pHist);

  void onCurrentChanged(int index);
  void onAddTab();
  void onTabCloseRequested(int index);

private:
  void updateCurrentViewToVisibleTab();

private:
    Ui::TabbedMultiSpectrumView *ui;
    MultiSpectrumView* m_pCurrentView;
    SpecTclInterface* m_pSpecTcl;
    QPushButton*      m_pAddButton;
};

}
#endif // TABBEDMULTISPECTRUMVIEW_H