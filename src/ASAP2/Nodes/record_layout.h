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

#ifndef RECORD_LAYOUT_H
#define RECORD_LAYOUT_H

#include "node.h"
#include "basefactory.h"
#include "Items/no_axis_pts_x.h"
#include "Items/no_axis_pts_y.h"
#include "Items/axis_pts_x.h"
#include "Items/axis_pts_y.h"
#include "Items/fnc_values.h"
#include "Items/src_addr_x.h"
#include "Items/src_addr_y.h"
#include "Items/static_record_layout.h"


class RECORD_LAYOUT : public Node
{
    public:
        RECORD_LAYOUT( Node *parentNode);
        ~RECORD_LAYOUT();

        QMap<QString, QString> *getParameters();
        static Factory<Node,RECORD_LAYOUT> nodeFactory;
        QString pixmap();
        QString getPar(QString str);

    private:
        // Fix parameters
        QList<TokenTyp> *typePar;
        QList<QString> *namePar;
        QList<QString> parameters;

        // Opt parameters
        QMap<QString, FactoryPlant<Node> *>  *factoryOptNode;
        QMap<QString, FactoryPlant<Item> *>  *factoryOptItem;

        void parseFixPar(QList<TokenTyp> *typePar);
        TokenTyp parseOptPar();
};

#endif // RECORD_LAYOUT_H
