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

#ifndef GATELISTREQUESTHANDLER_H
#define GATELISTREQUESTHANDLER_H

#include <GateInfo.h>

#include <QObject>

#include <vector>
#include <memory>

class QNetworkReply;
class QNetworkAccessManager;

namespace Viewer
{

class HistogramView;

class GateListRequestHandler : public QObject
{
    Q_OBJECT
public:
    explicit GateListRequestHandler(QObject *parent = 0);
    

public slots:
    void get();
    void finishedSlot(QNetworkReply* reply);

signals:
    void parseCompleted(std::vector<SpJs::GateInfo*> gateList);

private:
    std::vector<SpJs::GateInfo*> 
      deuniquify_vector_contents(std::vector<std::unique_ptr<SpJs::GateInfo>>& vect);

    QNetworkReply*         m_pReply;
    QNetworkAccessManager* m_pNAM;
    HistogramView*         m_view;
};

} // end of namespace

#endif // GATELISTREQUESTHANDLER_H