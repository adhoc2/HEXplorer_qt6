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

#include "Nodes/calibration_handle.h"
#include <QMessageBox>
#include <typeinfo>
#include "a2lgrammar.h"

//initialise static variables
Factory<Node,CALIBRATION_HANDLE> CALIBRATION_HANDLE::nodeFactory;

// Functions (Predicate)
bool nodeLessThan( const Node *a, const Node *b );
bool itemLessThan( const Item *a, const Item *b );

CALIBRATION_HANDLE::CALIBRATION_HANDLE( Node *parentNode)
    : Node(parentNode, parentNode->lex, parentNode->errorList)
{
    //get grammar
    A2lGrammar* gram = parentNode->lex->grammar;
    namePar = &gram->calibration_handle.namePar;
    typePar = &gram->calibration_handle.typePar;
    factoryOptNode = &gram->calibration_handle.factoryOptNode;
    factoryOptItem = &gram->calibration_handle.factoryOptItem;

    //opt parameters
    occOptPar = new QMap<QString, Occurence>;
    occOptPar->insert("CALIBRATION_HANDLE_TEXT", ZeroOrOne);

    //Set the line where the Node starts in ASAP file
    this->a2lLine = lex->getLine();

    //Parse Mandatory PARAMETERS
    //parseFixPar(typePar ,in);
    name = (QString)"calibration_handle";


    //Parse optional PARAMETERS (modified only for CALIBRATION_HANDLE)
    TokenTyp token = parseOptPar(occOptPar);

    //End
    if (token == BlockEnd)
    {
        token = nextToken();
        if (token == Keyword && lex->getLexem() == "CALIBRATION_HANDLE")
        {
            //Sort the childNodes
            //std::sort(this->childNodes.begin(), this->childNodes.end(), nodeLessThan);

            //Sort The childItems
            //std::sort(this->optItems.begin(), this->optItems.end(), itemLessThan);
        }
        else
        {
            QString s(lex->toString(token));
            this->showError("expected token : BlockEnd CALIBRATION_HANDLE\nfind token : " + s );
        }
    }
    else
    {
        QString s1(lex->toString(token));
        QString s2(lex->getLexem());
        this->showError("expected end CALIBRATION_HANDLE\nfind : " + s1 + " " + s2 );
    }    
}

CALIBRATION_HANDLE::~CALIBRATION_HANDLE()
{
    
    {
        
    }
    delete occOptPar;
}

void CALIBRATION_HANDLE::parseFixPar(QList<TokenTyp> *typePar)
{
    //Mandatory PARAMETERS
    TokenTyp token;
    for (int i = 0; i < typePar->count(); i++)
    {
        token = this->nextToken();
        if (token == typePar->at(i))
        {
            //parameters.insert(namePar->at(i), lex->getLexem());
            
            
            parameters.append(lex->getLexem());
        }
        else
        {
            QString t(lex->toString(typePar->at(i)));
            QString s(lex->toString(token));
            this->showError("expected token : " + t +"\nfind token : " + s );
        }
    }
}

TokenTyp CALIBRATION_HANDLE::parseOptPar(QMap<QString, Occurence> *nameOptPar)
{
    // read (Handles)* !!!!! Only for CALIBRATION_HANDLE
    TokenTyp token = lex->getNextToken();
    while (token == Hex || token == Integer || token == Comment)
    {
        if (token != Comment)
        {
            // add the lexem to the list of Handle
            
            
            parameters.append(lex->getLexem());
        }

        // get next Token
        token = lex->getNextToken();
    }

    // read optional parameters
    if (nameOptPar->isEmpty())
        return nextToken();
    else
    {
        //TokenTyp token = nextToken(in);
        while (token == BlockBegin || token == Keyword)
        {
            //Nodes
            if (token == BlockBegin)
            {
                token = this->nextToken();
                if (token == Keyword)
                {
                    QString lexem = lex->getLexem();
                    if (factoryOptNode->contains(lexem))
                    {
                        if (this->occOptPar->value(lexem) == ZeroOrOne)
                        {
                            this->occOptPar->insert(lexem, Zero);
                            Node  *instance = factoryOptNode->value(lexem)->createInstance( this);
                            this->addChildNode(instance);
                            token = nextToken();
                        }
                        else if (this->occOptPar->value(lexem) == ZeroOrMore)
                        {
                            Node  *instance = factoryOptNode->value(lexem)->createInstance( this);
                            this->addChildNode(instance);
                            token = nextToken();
                        }
                        else
                        {
                            QString s(lexem);
                            this->showError(" Keyword : " + s + " can only be once declared");
                            return token;
                        }
                    }
                    else
                    {
                        QString s(lexem);
                        this->showError("unknown Keyword : " + s );
                        return token;
                    }
                }
                else
                {
                    QString s(lex->toString(token));
                    this->showError("expected token : BlockBegin or Keyword\nfind token : " + s );
                    return token;
                }
            }
            //Items
            else if (token == Keyword)
            {
                QString lexem = lex->getLexem();
                if (factoryOptItem->contains(lexem))
                {
                    if (this->occOptPar->value(lexem) == ZeroOrOne)
                    {
                        this->occOptPar->insert(lexem, Zero);
                        Item  *instance = factoryOptItem->value(lexem)->createInstance( this);
                        this->addOptItem(instance);
                        token = nextToken();
                    }
                    else if (this->occOptPar->value(lexem) == ZeroOrMore)
                    {
                        Item  *instance = factoryOptItem->value(lexem)->createInstance( this);
                        this->addOptItem(instance);
                        token = nextToken();
                    }
                    else
                    {
                        QString s(lexem);
                        this->showError(" Keyword : " + s + " can only be once declared");
                        return token;
                    }
                }
                else
                {
                    QString s(lexem);
                    this->showError("unknown Keyword : " + s );
                    return token;
                }
            }
        }
        return token;
    }
}

QMap<QString, QString> *CALIBRATION_HANDLE::getParameters()
{
    QMap<QString, QString> *par = new QMap<QString, QString>;
    for (int i = 0; i < namePar->count(); i++)
    {
        par->insert(namePar->at(i), parameters.at(i));
    }
    return par;
}

QString CALIBRATION_HANDLE::getPar(QString str)
{
    int i = namePar->indexOf(str);
    return parameters.at(i);
}
