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

#include "module.h"
#include <QMessageBox>
#include "a2lgrammar.h"
#include <QStringList>
#include <QTime>
#include <qdebug.h>

//initialise static variables
Factory<Node,MODULE> MODULE::nodeFactory;

MODULE::MODULE(Node *parentNode)
    : Node(parentNode, parentNode->lex, parentNode->errorList)
{
    //get grammar
    A2lGrammar* gram = parentNode->lex->grammar;
    namePar = &gram->module.namePar;
    typePar = &gram->module.typePar;
    factoryOptNode = &gram->module.factoryOptNode;
    factoryOptItem = &gram->module.factoryOptItem;

    //opt Parameters
    occOptPar = new QMap<QString, Occurence>;
    occOptPar->insert("CHARACTERISTIC", ZeroOrMore);
    occOptPar->insert("MEASUREMENT", ZeroOrMore);
    occOptPar->insert("IF_DATA", ZeroOrMore);
    occOptPar->insert("FUNCTION", ZeroOrMore);
    occOptPar->insert("AXIS_PTS", ZeroOrMore);
    occOptPar->insert("COMPU_METHOD", ZeroOrMore);
    occOptPar->insert("COMPU_TAB", ZeroOrMore);
    occOptPar->insert("COMPU_VTAB", ZeroOrMore);
    occOptPar->insert("COMPU_VTAB_RANGE", ZeroOrMore);
    occOptPar->insert("RECORD_LAYOUT", ZeroOrMore);
    occOptPar->insert("A2ML", ZeroOrOne);
    occOptPar->insert("MOD_PAR", ZeroOrOne);
    occOptPar->insert("MOD_COMMON", ZeroOrOne);
    occOptPar->insert("GROUP", ZeroOrMore);
    occOptPar->insert("UNIT", ZeroOrMore);

    //Set the line where the Node starts in ASAP file
    a2lLine = lex->getLine();

    //check if it is a chunk for multi_threading
    TokenTyp token = lex->getNextToken();
    if (token == Identifier && lex->getLexem() == "CHUNKstart")
    {
        stopped = true;
    }
    else
    {
        QString str = lex->getLexem();
        lex->backward();
    }
    //TokenTyp token;

    //Parse Mandatory PARAMETERS
    parseFixPar(typePar);
    name = parameters.at(0);

    //Parse optional PARAMETERS    
    token = parseOptPar(occOptPar);

    //End parsing
    if (token == BlockEnd)
    {
        token = nextToken();
        if (token == Keyword && lex->getLexem() == "MODULE")
        {
            //Sort the childNodes
            foreach (Node *node, childNodes)
            {
                node->sortChildrensName();
            }
        }

        else
        {
            QString s(lex->toString(token));
            this->showError("expected token : BlockEnd MODULE\nfind token : " + s);
        }
    }
    else if (token == Identifier && lex->getLexem() == "CHUNKend")
    {
        //Sort the childNodes
        foreach (Node *node, childNodes)
            node->sortChildrensName();
        stopped = true;
        lex->backward();
    }
    else
    {
        QString s1(lex->toString(token));
        QString s2(lex->getLexem());
        this->showError("expected end MODULE\nfind : " + s1 + " " + s2);
    }

    //Sort The childItems and childNodes
    //std::sort(childNodes.begin(), childNodes.end(), nodeLessThan); remplaced by following line (same but nicer)
    std::sort(optItems.begin(), optItems.end(), itemLessThan);
    this->sortChildrensName();

    //sort the list of characteristics including (characteristics or axis_pts or...)
    this->listChar.sort();
    this->listMeas.sort();
}

MODULE::~MODULE()
{
    
    {
        
    }
    delete occOptPar;
}

void MODULE::parseFixPar(QList<TokenTyp> *typePar)
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
            this->showError("expected token : " + t +"\nfind token : " + s);
        }
    }
}

TokenTyp MODULE::parseOptPar(QMap<QString, Occurence> *nameOptPar)
{
    if (nameOptPar->isEmpty())
        return nextToken();
    else
    {
        TokenTyp token = nextToken();
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
                            Node  *instance = factoryOptNode->value(lexem)->createInstance(this);
                            this->addChildNode(instance);
                            token = nextToken();
                        }
                        else if (this->occOptPar->value(lexem) == ZeroOrMore)
                        {
                            Node *instance;
                            if (lexem == "CHARACTERISTIC")
                            {
                                if (!this->isChild("CHARACTERISTIC"))
                                {
                                    Node *Char = new Node(this, this->lex, this->errorList);
                                    Char->name = (QString)"CHARACTERISTIC";
                                    this->addChildNode(Char);
                                    //Char->_pixmap = ":/icones/CHAR.bmp";
                                    Char->_pixmap = "";
                                }
                                instance = factoryOptNode->value(lexem)->createInstance(child("CHARACTERISTIC", false));
                                child("CHARACTERISTIC", false)->addChildNode(instance);
                                listChar.append(instance->name);
                            }
                            else if (lexem == "AXIS_PTS")
                            {
                                if (!this->isChild("AXIS_PTS"))
                                {
                                    Node *Axis = new Node(this, this->lex, this->errorList);
                                    Axis->name = (QString)"AXIS_PTS";
                                    this->addChildNode(Axis);
                                    //Axis->_pixmap = ":/icones/AXIS.bmp";
                                    Axis->_pixmap = "";
                                }
                                instance = factoryOptNode->value(lexem)->createInstance(child("AXIS_PTS", false));
                                child("AXIS_PTS", false)->addChildNode(instance);
                                listChar.append(instance->name);
                            }
                            else if (lexem == "MEASUREMENT")
                            {
                                if (!this->isChild("MEASUREMENT"))
                                {
                                    Node *Meas = new Node(this, this->lex, this->errorList);
                                    Meas->name = (QString)"MEASUREMENT";
                                    this->addChildNode(Meas);
                                    //Meas->_pixmap = ":/icones/MEAS.bmp";
                                    Meas->_pixmap = "";                                    
                                }
                                instance = factoryOptNode->value(lexem)->createInstance(child("MEASUREMENT", false));
                                child("MEASUREMENT", false)->addChildNode(instance);
                                listMeas.append(instance->name);
                            }
                            else if (lexem == "FUNCTION")
                            {
                                if (!this->isChild("FUNCTION"))
                                {
                                    Node *Func = new Node(this, this->lex, this->errorList);
                                    Func->name = (QString)"FUNCTION";
                                    this->addChildNode(Func);
                                    //Func->_pixmap = ":/icones/FUNCTION.bmp";
                                    Func->_pixmap = "";
                                }
                                instance = factoryOptNode->value(lexem)->createInstance(child("FUNCTION", false));
                                child("FUNCTION", false)->addChildNode(instance);
                            }
                            else if (lexem == "GROUP")
                            {
                                if (!this->isChild("GROUP"))
                                {
                                    Node *Grp = new Node(this, this->lex, this->errorList);
                                    Grp->name = (QString)"GROUP";
                                    this->addChildNode(Grp);
                                    //Func->_pixmap = ":/icones/FUNCTION.bmp";
                                    Grp->_pixmap = "";
                                }
                                instance = factoryOptNode->value(lexem)->createInstance(child("GROUP", false));
                                child("GROUP", false)->addChildNode(instance);
                            }
                            else if (lexem == "UNIT")
                            {
                                if (!this->isChild("UNIT"))
                                {
                                    Node *Grp = new Node(this, this->lex, this->errorList);
                                    Grp->name = (QString)"UNIT";
                                    this->addChildNode(Grp);
                                    //Func->_pixmap = ":/icones/FUNCTION.bmp";
                                    Grp->_pixmap = "";
                                }
                                instance = factoryOptNode->value(lexem)->createInstance(child("UNIT", false));
                                child("UNIT", false)->addChildNode(instance);
                            }
                            else if (lexem == "COMPU_METHOD")
                            {
                                if (!this->isChild("COMPU_METHOD"))
                                {
                                    Node *Comp_m = new Node(this, this->lex, this->errorList);
                                    Comp_m->name = (QString)"COMPU_METHOD";
                                    this->addChildNode(Comp_m);
                                    //Comp_m->_pixmap = ":/icones/COMPU_METHOD.bmp";
                                    Comp_m->_pixmap = "";
                                }
                                instance = factoryOptNode->value(lexem)->createInstance(child("COMPU_METHOD", false));
                                child("COMPU_METHOD", false)->addChildNode(instance);
                            }
                            else if (lexem == "COMPU_TAB")
                            {
                                if (!this->isChild("COMPU_TAB"))
                                {
                                    Node *Comp_v = new Node(this, this->lex, this->errorList);
                                    Comp_v->name = (QString)"COMPU_TAB";
                                    this->addChildNode(Comp_v);
                                    //Comp_v->_pixmap = ":/icones/COMPU_TAB.bmp";
                                    Comp_v->_pixmap = "";
                                }
                                instance = factoryOptNode->value(lexem)->createInstance(child("COMPU_TAB", false));
                                child("COMPU_TAB", false)->addChildNode(instance);
                            }
                            else if (lexem == "COMPU_VTAB")
                            {
                                if (!this->isChild("COMPU_VTAB"))
                                {
                                    Node *Comp_v = new Node(this, this->lex, this->errorList);
                                    Comp_v->name = (QString)"COMPU_VTAB";
                                    this->addChildNode(Comp_v);
                                    //Comp_v->_pixmap = ":/icones/COMPU_VTAB.bmp";
                                    Comp_v->_pixmap = "";
                                }
                                instance = factoryOptNode->value(lexem)->createInstance(child("COMPU_VTAB", false));
                                child("COMPU_VTAB", false)->addChildNode(instance);
                            }
                            else if (lexem == "COMPU_VTAB_RANGE")
                            {
                                if (!this->isChild("COMPU_VTAB_RANGE"))
                                {
                                    Node *Comp_v = new Node(this, this->lex, this->errorList);
                                    Comp_v->name = (QString)"COMPU_VTAB_RANGE";
                                    this->addChildNode(Comp_v);
                                    //Comp_v->_pixmap = ":/icones/COMPU_VTAB_RANGE.bmp";
                                    Comp_v->_pixmap = "";
                                }
                                instance = factoryOptNode->value(lexem)->createInstance(child("COMPU_VTAB_RANGE", false));
                                child("COMPU_VTAB_RANGE", false)->addChildNode(instance);
                            }
                            else if (lexem == "RECORD_LAYOUT")
                            {
                                if (!this->isChild("RECORD_LAYOUT"))
                                {
                                    Node *Rec = new Node(this, this->lex, this->errorList);
                                    Rec->name = (QString)"RECORD_LAYOUT";
                                    this->addChildNode(Rec);
                                    //Rec->_pixmap = ":/icones/CONV.bmp";
                                    Rec->_pixmap = "";
                                }
                                instance = factoryOptNode->value(lexem)->createInstance(child("RECORD_LAYOUT", false));
                                child("RECORD_LAYOUT", false)->addChildNode(instance);
                            }
                            else if (lexem == "IF_DATA")
                            {
                                if (!isChild("IF_DATA"))
                                {
                                    Node *If_data = new Node(this, this->lex, this->errorList);
                                    If_data->name = (QString)"IF_DATA";
                                    this->addChildNode(If_data);
                                    //If_data->_pixmap = ":/icones/MOD_PAR.bmp";
                                    If_data->_pixmap = "";
                                }
                                instance = factoryOptNode->value(lexem)->createInstance(child("IF_DATA", false));
                                child("IF_DATA", false)->addChildNode(instance);
                            }
                            else
                            {
                                instance = factoryOptNode->value(lexem)->createInstance(this);
                                this->addChildNode(instance);
                            }

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
                        this->showError("unknown Keyword : " + s);
                        return token;
                    }
                }
                else
                {
                    QString s(lex->toString(token));
                    this->showError("expected token : BlockBegin or Keyword\nfind token : " + s);
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
                        Item  *instance = factoryOptItem->value(lexem)->createInstance(this);
                        this->addOptItem(instance);
                        token = nextToken();
                    }
                    else if (this->occOptPar->value(lexem) == ZeroOrMore)
                    {
                        Item  *instance = factoryOptItem->value(lexem)->createInstance(this);
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
                    this->showError("unknown Keyword : " + s);
                    return token;
                }
            }
        }
        
        return token;
    }
}

QMap<QString, QString> *MODULE::getParameters()
{
    QMap<QString, QString> *par = new QMap<QString, QString>;
    for (int i = 0; i < namePar->count(); i++)
    {
        par->insert(namePar->at(i), parameters.at(i));
    }
    return par;
}

QString MODULE::getPar(QString str)
{
    int i = namePar->indexOf(str);
    return parameters.at(i);
}
