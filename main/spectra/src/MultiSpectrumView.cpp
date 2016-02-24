#include "MultiSpectrumView.h"
#include "HistogramBundle.h"
#include "QRootCanvas.h"
#include "SpecTclInterface.h"

#include <TPad.h>

#include <QGridLayout>
#include <QSpacerItem>
#include <QMouseEvent>
#include <QPainter>
#include <QBrush>
#include <QColor>

#include <iostream>
#include <stdexcept>

using namespace std;

namespace Viewer
{

MultiSpectrumView::MultiSpectrumView(SpecTclInterface* pSpecTcl, QWidget *parent) :
    SpectrumView(parent),
    m_pLayout(new QGridLayout(this)),
    m_histMap(),
    m_pSpecTcl(pSpecTcl),
    m_pCurrentCanvas(new QRootCanvas),
    m_currentNRows(1),
    m_currentNColumns(1)
{
    setLayout(m_pLayout.get());

    m_pLayout->addWidget(m_pCurrentCanvas, 0, 0);
    m_pCurrentCanvas->cd();

    connect(m_pCurrentCanvas, SIGNAL(mousePressed(QWidget*)),
            this, SLOT(setCurrentCanvas(QWidget*)));

}

int MultiSpectrumView::getRowCount() const
{
    return m_currentNRows;
}

int MultiSpectrumView::getColumnCount() const
{
    return m_currentNColumns;
}

vector<QRootCanvas*> MultiSpectrumView::getAllCanvases()
{
  vector<QRootCanvas*> canvases;

  for (int col=0; col<m_pLayout->columnCount(); col++) {
      for (int row=0; row<m_pLayout->rowCount(); row++) {
          auto pItem = m_pLayout->itemAtPosition(row, col);
          if (pItem) {
              auto pCanvas = dynamic_cast<QRootCanvas*>(pItem->widget());
              if (pCanvas) {
                  canvases.push_back(pCanvas);
              } else {
                  throw runtime_error("MultiSpectrumView::getAllCanvases() Failed to upcast widget to QRootCanvas*");
              }
          }
       }
    }

  return canvases;
}


void MultiSpectrumView::onGeometryChanged(int nRows, int nCols)
{
    // figure out the current configuation...
    if (nRows == m_pLayout->rowCount() && nCols == m_pLayout->columnCount()) {
        // if the same, do nothing
        return;
    }

    auto prevCanvasLoc = findLocation(m_pCurrentCanvas);

    // if different, then figure out how to change
    int currentNRows = m_currentNRows;
    int currentNCols = m_currentNColumns;

    int maxRows = std::max(nRows, currentNRows);
    int maxCols = std::max(nCols, currentNCols);
    for (int row=0; row<maxRows; ++row) {
        for (int col=0; col<maxCols; ++col) {

            // get rid of current rows that are marked for destruction
            if ( row >= nRows ) {
               auto pItem = m_pLayout->itemAtPosition(row, col);
               if (pItem) {
                   m_pLayout->removeItem(pItem);
                   delete pItem->widget();
                   delete pItem;
               }
            } else if ( col >= nCols ) {
                auto pItem = m_pLayout->itemAtPosition(row, col);
                if (pItem) {
                    m_pLayout->removeItem(pItem);
                    delete pItem->widget();
                    delete pItem;
                }
            }

            // add empty spaces
            if (row >= currentNRows) {
                auto pCanvas = new QRootCanvas;
                m_pLayout->addWidget(pCanvas, row, col);
                connect(pCanvas, SIGNAL(mousePressed(QWidget*)),
                        this, SLOT(setCurrentCanvas(QWidget*)));
            } else if (col >= currentNCols) {
               auto pCanvas = new QRootCanvas;
                m_pLayout->addWidget(pCanvas, row, col);
                connect(pCanvas, SIGNAL(mousePressed(QWidget*)),
                        this, SLOT(setCurrentCanvas(QWidget*)));
            }
        }
    }

    m_currentNRows = nRows;
    m_currentNColumns = nCols;

    // update our current canvas
    int newCurrentCanvasRow = prevCanvasLoc.first;
    int newCurrentCanvasCol = prevCanvasLoc.second;

    if (prevCanvasLoc.first >= m_currentNRows) {
        newCurrentCanvasRow = m_currentNRows-1;
    }
    if (prevCanvasLoc.second >= m_currentNColumns) {
        newCurrentCanvasCol = m_currentNColumns-1;
    }

    auto pItem = m_pLayout->itemAtPosition(newCurrentCanvasRow, newCurrentCanvasCol);
    setCurrentCanvas(pItem->widget());

    SpectrumView::update();

}

void MultiSpectrumView::setCurrentCanvas(QWidget *pWidget)
{
  if (auto pCanvas = dynamic_cast<QRootCanvas*>(pWidget)) {
      m_pCurrentCanvas = pCanvas;
      m_pCurrentCanvas->cd();

      setFocus();
      QWidget::update();
  }
}

void MultiSpectrumView::keyPressEvent(QKeyEvent *key)
{
  auto location = findLocation(m_pCurrentCanvas);
  if (location == std::pair<int,int>(-1,-1))
    return;

    int newRow = location.first;
    int newCol = location.second;

    cout << "rows=" << m_currentNRows << "  cols=" << m_currentNColumns << endl;
    int keyId = key->key();
    if (keyId == Qt::Key_Up) {
        if (location.first == 0) {
            newRow = m_currentNRows-1;
            if (location.second == 0) {
              newCol = m_currentNColumns-1;
            } else {
              newCol = location.second - 1;
            }
        } else {
            newRow = location.first-1;
        }
    } else if (keyId == Qt::Key_Right) {
        if (location.second == m_currentNColumns-1) {
            newCol = 0;
            if (location.first == m_currentNRows-1) {
                newRow = 0;
              } else {
                newRow = location.first + 1;
              }
        } else {
            newCol = location.second+1;
        }
    } else if (keyId == Qt::Key_Down) {
        if (location.first == m_currentNRows-1) {
            newRow = 0;
            if (location.second == m_currentNColumns-1) {
              newCol = 0;
            } else {
              newCol = location.second + 1;
            }
        } else {
            newRow = location.first+1;
        }
    } else if (keyId == Qt::Key_Left) {
        if (location.second == 0) {
            newCol = m_currentNColumns-1;
            if (location.first == 0) {
              newRow = m_currentNRows-1;
            } else {
              newRow = location.first - 1;
            }    
        } else {
            newCol = location.second-1;
        }
    }

    auto pNewCanvas = m_pLayout->itemAtPosition(newRow, newCol)->widget();
    setCurrentCanvas(pNewCanvas);

}

std::pair<int,int> MultiSpectrumView::findLocation(QWidget *pWidget)
{
  for (int col=0; col<m_pLayout->columnCount(); col++) {
      for (int row=0; row<m_pLayout->rowCount(); row++) {
          auto pItem = m_pLayout->itemAtPosition(row,col);
          if (pItem) {
              if (pItem->widget() == pWidget) {
                  return std::make_pair(row,col);
                }
            }
        }
    }
  return std::make_pair(-1, -1);
}

void MultiSpectrumView::paintEvent(QPaintEvent *evt)
{
  QPainter painter(this);
  QPen pen(Qt::DotLine);
  pen.setColor(Qt::red);
  pen.setWidth(2);
  painter.setPen(pen);
  auto frame = m_pCurrentCanvas->geometry();

  painter.drawRect(frame.x()-2, frame.y()-2,
                   frame.width()+3, frame.height()+3);

}


QRootCanvas* MultiSpectrumView::getCurrentCanvas()
{
    return m_pCurrentCanvas;
}

void MultiSpectrumView::update(HistogramBundle* pBundle)
{
  if (pBundle) {
      getCurrentCanvas()->cd();
      if (pBundle->hist() && histogramInCanvas(pBundle, getCurrentCanvas())) {
          if (m_pSpecTcl) {
              pBundle->synchronizeGates(m_pSpecTcl->getGateList());
          }
          pBundle->draw();
        }
    }
    setFocus();
    refreshAll();
}


void MultiSpectrumView::drawHistogram(HistogramBundle* pBundle)
{
  if (pBundle) {
      getCurrentCanvas()->cd();
      if (pBundle->hist()) {
          if (m_pSpecTcl) {
              pBundle->synchronizeGates(m_pSpecTcl->getGateList());
          }
          pBundle->draw();
        }
    }
    setFocus();
    refreshAll();
}


void MultiSpectrumView::mouseDoubleClickEvent(QMouseEvent* evt)
{
//  auto pWidget = childAt(evt->x(), evt->y());
 
//  setCurrentCanvas(pWidget);
//  onGeometryChanged(1,1);
}

void MultiSpectrumView::refreshAll()
{
    // if different, then figure out how to change
    int currentNRows = m_pLayout->rowCount();
    int currentNCols = m_pLayout->columnCount();

    for (int row=0; row<currentNRows; ++row) {
        for (int col=0; col<currentNCols; ++col) {

            auto pItem = m_pLayout->itemAtPosition(row, col);
            if (pItem) {
                auto pCanvas = dynamic_cast<QRootCanvas*>(pItem->widget());
                if (pCanvas) {
                    pCanvas->Modified();
                    pCanvas->Update();
                }
            }
        }
    }
    m_pCurrentCanvas->cd();
}

void MultiSpectrumView::onHistogramRemoved(HistogramBundle *pHistBundle)
{
    std::cout << "Removed hist @ " << static_cast<void*>(pHistBundle) << std::endl;
}

bool MultiSpectrumView::histogramVisible(HistogramBundle *pHist)
{
  for (int row = 0; row<m_pLayout->rowCount(); ++row) {
      for (int col = 0; col<m_pLayout->columnCount(); ++col) {

          auto pItem = m_pLayout->itemAtPosition(row,col);

          if (histogramInCanvas(pHist, dynamic_cast<QRootCanvas*>(pItem->widget()))) {
              return true;
          }
      }
  }

  return false;
}

bool MultiSpectrumView::histogramInCanvas(HistogramBundle* pHist, QRootCanvas* pCanvas)
{
  return (pCanvas->findObject(pHist->hist()) != nullptr);
}

} // end of namespace