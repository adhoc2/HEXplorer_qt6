// HEXplorer is an Asap and HEX file editor
// Copyright (C) 2011  <Christophe Hoel>
//
// This file is part of HEXplorer.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// please contact the author at : christophe.hoel@gmail.com

#include "src_addr_y.h"
#include <QMessageBox>
#include "a2lgrammar.h"

//initialise static variables
Factory<Item,SRC_ADDR_Y> SRC_ADDR_Y::itemFactory;

SRC_ADDR_Y::SRC_ADDR_Y( Node *parentNode)  : Item(parentNode)
{
    //get grammar
    A2lGrammar* gram = parentNode->lex->grammar;
    namePar = &gram->src_addr_x.namePar;
    typePar = &gram->src_addr_x.typePar;

    //Parse Mandatory PARAMETERS
    parseFixPar(typePar);
    name = (QString)"SRC_ADDR_Y";
}

SRC_ADDR_Y::~SRC_ADDR_Y()
{
    
    {
        
    }
}

QMap<QString, QString> SRC_ADDR_Y::getParameters()
{
    QMap<QString, QString> par;
    for (int i = 0; i < namePar->count(); i++)
    {
        par.insert(namePar->at(i), parameters.at(i));
    }
    return par;
}

QString SRC_ADDR_Y::getPar(QString str)
{
    int i = namePar->indexOf(str);
    return parameters.at(i);
}
