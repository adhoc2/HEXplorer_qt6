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

#ifndef ITEM_H
#define ITEM_H

#include <QMap>
#include "lexer.h"
#include "QMutex"

class Node;

class Item
{
    public:
        Item(Node *parentNod);
        virtual ~Item();

        virtual QMap<QString, QString> getParameters()
        {
            QMap<QString, QString> map;
            return map;
        }
        TokenTyp nextToken();
        void parseFixPar(QList<TokenTyp> *typePar);
        QString getName() const
        {
            QString str = name;
            return str;
        }
        QString name;

    protected:
        Node *parentNode;
        void showError(QString str);
        QList<QString> parameters;
    };

#endif // ITEM_H
