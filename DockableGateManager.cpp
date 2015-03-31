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

static const char* Copyright = "(C) Copyright Michigan State University 2015, All rights reserved";
#include "DockableGateManager.h"
#include "ui_DockableGateManager.h"
#include "GateBuilderDialog.h"
#include "GateBuilder1DDialog.h"
#include "SpectrumViewer.h"
#include "SpecTclInterface.h"
#include "QRootCanvas.h"
#include "GSlice.h"
#include "SliceTableItem.h"
#include <QListWidget>
#include <QMessageBox>
#include "TCutG.h"
#include <TH1.h>
#include <TH2.h>

DockableGateManager::DockableGateManager(const SpectrumViewer& viewer,
                                         SpecTclInterface* pSpecTcl,
                                         QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockableGateManager),
    m_view(viewer),
    m_pSpecTcl(pSpecTcl)
{
    ui->setupUi(this);
    connect(ui->addButton,SIGNAL(clicked()),this, SLOT(launchAddGateDialog()));
    connect(ui->editButton,SIGNAL(clicked()),this, SLOT(launchEditGateDialog()));
}

DockableGateManager::~DockableGateManager()
{
    delete ui;
}

void DockableGateManager::launchAddGateDialog()
{
    auto pCanvas = m_view.getCurrentFocus();
    auto histPkg = m_view.getCurrentHist();

    if (histPkg->hist()->InheritsFrom(TH2::Class())) {

        GateBuilderDialog* dialog = new GateBuilderDialog(*pCanvas, *histPkg);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        connect(dialog, SIGNAL(completed(TCutG*)),this,SLOT(registerGate(TCutG*)));
        connect(dialog, SIGNAL(finished(int)), dialog, SLOT(close()));

        dialog->show();
        dialog->raise();
    } else {
        GateBuilder1DDialog* dialog = new GateBuilder1DDialog(*pCanvas, *histPkg);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        connect(dialog, SIGNAL(completed(GSlice*)),
                this, SLOT(registerSlice(GSlice*)));

        dialog->show();
        dialog->raise();
    }
}

void DockableGateManager::launchEditGateDialog()
{
    auto pCanvas = m_view.getCurrentFocus();
    auto histPkg = m_view.getCurrentHist();

    auto selection = ui->gateList->selectedItems();
    if (selection.size()==1) {
        auto pItem = selection.at(0);

        if (auto pSlItem = dynamic_cast<SliceTableItem*>(pItem)) {
            auto pCut = pSlItem->getSlice();
            GateBuilder1DDialog* dialog = new GateBuilder1DDialog(*pCanvas, *histPkg, pCut);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            connect(dialog, SIGNAL(completed(GSlice*)),
                    this, SLOT(editSlice(GSlice*)));

            dialog->show();
            dialog->raise();

        } else {
            QVariant cut = pItem->data(Qt::UserRole);
            TCutG* pCut = reinterpret_cast<TCutG*>(cut.value<void*>());
            GateBuilderDialog* dialog = new GateBuilderDialog(*pCanvas, *histPkg, pCut);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            connect(dialog, SIGNAL(completed(TCutG*)),
                    this, SLOT(editGate(TCutG*)));

            dialog->show();
            dialog->raise();
        }
    } else {
        QMessageBox::warning(0, "Invalid selection", "User must select one gate to edit.");
    }
}

void DockableGateManager::registerGate(TCutG* pCut)
{
    Q_ASSERT(pCut!=nullptr);

    QListWidgetItem* pItem = new QListWidgetItem(QString(pCut->GetName()),
                                                 ui->gateList,Qt::UserRole);
    QVariant var = QVariant::fromValue(reinterpret_cast<void*>(pCut));
    pItem->setData(Qt::UserRole, var);

    ui->gateList->addItem(pItem);

    if (m_pSpecTcl) {
        m_pSpecTcl->addGate(*pCut);
    }
}


void DockableGateManager::registerSlice(GSlice *pSlice)
{
    Q_ASSERT(pSlice != nullptr);

    QString name = pSlice->getName();

    SliceTableItem* pItem = new SliceTableItem(name,
                                               ui->gateList,
                                               Qt::UserRole,
                                               pSlice);
    ui->gateList->addItem(pItem);

    if (m_pSpecTcl) {
        m_pSpecTcl->addGate(*pSlice);
    }
}


void DockableGateManager::editGate(TCutG* pCut)
{
    Q_ASSERT(pCut!=nullptr);

    if (m_pSpecTcl) {
        m_pSpecTcl->editGate(*pCut);
    }
}


void DockableGateManager::editSlice(GSlice *pSlice)
{
    Q_ASSERT(pSlice != nullptr);

    if (m_pSpecTcl) {
        m_pSpecTcl->editGate(*pSlice);
    }
}


