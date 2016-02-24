//    This software is Copyright by the Board of Trustees of Michigan
//    State University (c) Copyright 2015.
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//    Authors:
//    Jeromy Tompkins 
//    NSCL
//    Michigan State University
//    East Lansing, MI 48824-1321
#include "SpecTclRESTInterface.h"
#include "GlobalSettings.h"
#include "GSlice.h"
#include "GateEditComHandler.h"
#include "GateEditRequest.h"
#include "GateDeleteRequest.h"
#include "CommonResponseHandler.h"
#include "GateListRequestHandler.h"
#include "HistogramList.h"
#include "QRootCanvas.h"

#include <QString>
#include <QTimer>
#include <QUrl>

#include <TLine.h>
#include <TCanvas.h>
#include <TList.h>
#include <TH1.h>

#include <iostream>

using namespace std;


namespace Viewer
{


SpecTclRESTInterface::SpecTclRESTInterface()
    : SpecTclInterface(),
    m_pGateList(new GateList),
    m_pHistList(new HistogramList(this)),
    m_pGateEditCmd(new GateEditComHandler),
    m_pCommonHandler(new CommonResponseHandler),
    m_pGateListCmd(new GateListRequestHandler),
    m_pHistListCmd(new ListRequestHandler),
    m_pHistContentCmd(new ContentRequestHandler(m_pHistList.get())),
    pollGates(false),
    pollHistInfo(false)
{

  connect(m_pGateListCmd.get(), SIGNAL(parseCompleted(std::vector<SpJs::GateInfo*>)),
      this, SLOT(onGateListReceived(std::vector<SpJs::GateInfo*>)));

  connect(m_pHistListCmd.get(), SIGNAL(parseCompleted(std::vector<SpJs::HistInfo>)),
      this, SLOT(onHistogramListReceived(std::vector<SpJs::HistInfo>)));

  connect(m_pHistContentCmd.get(), SIGNAL(parsingComplete(HistogramBundle*)),
          this, SLOT(onHistogramContentUpdated(HistogramBundle*)));
}

void SpecTclRESTInterface::addGate(const GSlice &slice)
{
    // at the moment editing and creating use the same command
    editGate(slice);
}

void SpecTclRESTInterface::editGate(const GSlice &slice)
{
    GateEditRequest req(slice);

//    cout << "edit gate" << endl;

    //cout << req.toUrl().toString().toStdString() << endl;

    m_pGateEditCmd->makeRequest(req.toUrl());
}

void SpecTclRESTInterface::deleteGate(const GSlice &slice)
{
  GateDeleteRequest req(slice);

//  cout << req.toUrl().toString().toStdString() << endl;
  m_pCommonHandler->makeRequest(req.toUrl());
}

void SpecTclRESTInterface::addGate(const GGate &gate)
{
    editGate(gate);
}

void SpecTclRESTInterface::editGate(const GGate &gate)
{

  GateEditRequest req(gate);

//  cout << "edit gate" << endl;

//  cout << req.toUrl().toString().toStdString() << endl;
  m_pGateEditCmd->makeRequest(req.toUrl());
}

void SpecTclRESTInterface::deleteGate(const GGate &slice)
{
  GateDeleteRequest req(slice);

//  cout << "delete gate" << endl;

//  cout << req.toUrl().toString().toStdString() << endl;
  m_pCommonHandler->makeRequest(req.toUrl());
}

void SpecTclRESTInterface::deleteGate(const QString& name) 
{
  GateDeleteRequest req(name);

//  cout << "delete gate" << endl;

//  cout << req.toUrl().toString().toStdString() << endl;
  m_pCommonHandler->makeRequest(req.toUrl());

}


void SpecTclRESTInterface::listGates() 
{
//  cout << "list gates" << endl;
  m_pGateListCmd->get(); 
}

void SpecTclRESTInterface::listHistogramInfo()
{
//  cout << "list hists" << endl;
  m_pHistListCmd->get();
}

void 
SpecTclRESTInterface::onGateListReceived(std::vector<SpJs::GateInfo*> gates)
{

  if (! pollGates) {
      // free the gates... they have done their job
      for (auto ptr : gates) { delete ptr; }

      return;
  }

//  cout << "gates received" << endl;
  // synchronize our list of gates to the list that we are being passed
  // from SpecTcl
  bool gatesChanged = m_pGateList->synchronize(gates);
//  cout << "gates changed" << endl;

  // only update everything else if something actually changed.
  if (gatesChanged) {
//      cout << "gates changed" << endl;

      // now update the histograms so that we know they only reference gates
      // that exist after the synchronization
//      cout << "gates synchronized" << endl;
      // tell the world that things have changed.
      emit gateListChanged();

  }

  // schedule the next update
  QTimer::singleShot(GlobalSettings::getPollInterval(), this, SLOT(listGates()));

  // free the gates... they have done their job
  for (auto ptr : gates) { delete ptr; }

}

void SpecTclRESTInterface::enableGatePolling(bool enable)
{
  // don't double schedule... only start the polling if it has not already been
  // started.
  if (pollGates != enable) {
      pollGates = enable;
      if (enable) {
          listGates();
        }
  }

}

void SpecTclRESTInterface::enableHistogramInfoPolling(bool enable)
{
  // don't double schedule... only start the polling if it has not already been
  // started.
  if (pollHistInfo != enable) {
      pollHistInfo = enable;
      if (enable) {
          listHistogramInfo();
      }
  }

}

void SpecTclRESTInterface::requestHistContentUpdate(QRootCanvas* pCanvas)
{

  Q_ASSERT( pCanvas != nullptr );

  // update all histograms in this canvas
  requestHistContentUpdate(pCanvas->getCanvas());
}

void SpecTclRESTInterface::requestHistContentUpdate(TPad* pPad)
{

  Q_ASSERT( pPad != nullptr );

  int padCount = 0;
  // update all histograms in this canvas
  auto pList = pPad->GetListOfPrimitives();
  TObject *pObject = nullptr;
  TIter it(pList);
  while (( pObject = it.Next() )) {
      if (pObject->InheritsFrom(TPad::Class()) && padCount < 1) {
          requestHistContentUpdate(dynamic_cast<TPad*>(pObject));
          ++padCount;
      } else if (pObject->InheritsFrom(TH1::Class())) {
          auto pHist = dynamic_cast<TH1*>(pObject);

          requestHistContentUpdate(QString(pHist->GetName()));
      }
  }
}

void SpecTclRESTInterface::requestHistContentUpdate(const QString& name)
{
  auto host = GlobalSettings::getServerHost();
  auto port = GlobalSettings::getServerPort();

  QString reqUrl("http://%1:%2/spectcl/spectrum/contents?name=%3");
  auto reqUrlTmp = reqUrl.arg(host).arg(port).arg(name);

  m_pHistContentCmd->get(QUrl(reqUrlTmp));
}

void
SpecTclRESTInterface::onHistogramListReceived(std::vector<SpJs::HistInfo> hists)
{

//  cout << "hist list received" << endl;
  if (! pollHistInfo) {

      return;
  }

//  cout << "hist list updated..." << flush;
  // synchronize our list of gates to the list that we are being passed
  // from SpecTcl
  bool histInfoChanged = m_pHistList->update(hists);
//  cout << "done" << endl;

  // only update everything else if something actually changed.
  if (histInfoChanged) {

//      cout << "hist list changed..." << flush;
      // tell the world that things have changed.
      emit histogramListChanged();
//      cout << "done" << endl;

  }

  // schedule the next update
  QTimer::singleShot(GlobalSettings::getPollInterval(), this, SLOT(listHistogramInfo()));

}

void
SpecTclRESTInterface::onHistogramContentUpdated(HistogramBundle *pBundle)
{
  emit histogramContentUpdated(pBundle);
}

} // end of namespace
