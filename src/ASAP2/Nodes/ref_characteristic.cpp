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

#include "Nodes/ref_characteristic.h"
#include <QMessageBox>
#include <typeinfo>
#include <QHash>
#include "a2lgrammar.h"

//initialise static variables
Factory<Node,REF_CHARACTERISTIC> REF_CHARACTERISTIC::nodeFactory;

// Functions (Predicate)
bool nodeLessThan( const Node *a, const Node *b );
bool itemLessThan( const Item *a, const Item *b );

REF_CHARACTERISTIC::REF_CHARACTERISTIC(Node *parentNode)
    : Node(parentNode, parentNode->lex, parentNode->errorList)
{
    //get grammar
    A2lGrammar* gram = parentNode->lex->grammar;
    namePar = &gram->ref_characteristic.namePar;
    typePar = &gram->ref_characteristic.typePar;

    //Set the line where the Node starts in ASAP file
    a2lLine = lex->getLine();

    //Parse Mandatory PARAMETERS
    //parseFixPar(typePar, namePar ,in);
    name = (QString)"REF_CHARACTERISTIC";

    //Parse optional PARAMETERS
    //TokenTyp token = parseOptPar(in);

    TokenTyp token = parseListChar();

    //fianlize parsing
    if (token == BlockEnd)
    {
        token = nextToken();
        if (token == Keyword && lex->getLexem() == "REF_CHARACTERISTIC")
        {
            //Sort the childNodes
            //std::sort(this->childNodes.begin(), this->childNodes.end(), nodeLessThan);

            //Sort The childItems
            //std::sort(this->optItems.begin(), this->optItems.end(), itemLessThan);

            //sort the charList
            std::sort(charList.begin(), charList.end());
        }
        else
        {
            QString s(lex->toString(token));
            this->showError("expected token : BlockEnd REF_CHARACTERISTIC\nfind token : " + s);
        }
    }
    else
    {
        QString s1(lex->toString(token));
        QString s2(lex->getLexem());
        this->showError("expected end REF_CHARACTERISTIC\nfind : " + s1 + " " + s2);
    }
}

REF_CHARACTERISTIC::~REF_CHARACTERISTIC()
{
    
    {
        
    }
}

QMap<QString, QString> *REF_CHARACTERISTIC::getParameters()
{
    QMap<QString, QString> *par = new QMap<QString, QString>;
    for (int i = 0; i < namePar->count(); i++)
    {
        par->insert(namePar->at(i), parameters.at(i));
    }
    return par;
}

QString  REF_CHARACTERISTIC::pixmap()
{
    return ":/icones/CHAR.bmp";
}

TokenTyp REF_CHARACTERISTIC::parseListChar()
{
    TokenTyp token = lex->getNextToken();

    while (token == Identifier)
    {
        charList.append(lex->getLexem());
        token = lex->getNextToken();
    }

    return token;
}

QStringList REF_CHARACTERISTIC::getCharList()
{
    QStringList list;
    foreach(QString str, charList)
    {
        list.append(str);
    }
    return list;
}

QString REF_CHARACTERISTIC::getPar(QString str)
{
    int i = namePar->indexOf(str);
    return parameters.at(i);
}
