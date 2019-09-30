//
// Created by WatsonYang on 2019-09-26.
//
//  I AM A  SO FUCKING STUPID WHITEGIVER
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <fstream>
#include <vector>
#include"parse.h"
#include "grammer.h"
using namespace std;
typedef vector<mpair>* F(string *);

const map<string,vector<mpair>*(*)(string *)> callFunc = funcmap();
vector<string> returnFunNameTable;
vector<string> noReturnFunNameTable;
// mpair -> UNEND : name "UNEND" else value TYPE

void merge(vector<mpair> *p1,vector<mpair> *p2) {
    if(p2== nullptr || p1== nullptr) return ;
    p1->insert(p1->end(),p2->begin(),p2->end());
}
void backtrack(vector<mpair>* match,string* pstr) {
    string back;
    for(const auto& mp : *match) {
        if(mp.second!="UNEND") {
            back += mp.first + " ";
        }
    }
    *pstr = back + *pstr;
    match->clear();
}


vector<mpair>* addOptr(string *pstr) {
    auto p = parseNext(pstr);
    if(p== nullptr) {
        return nullptr;
    }
    if(p->second == "PLUS"|| p->second=="MINU") {
        auto matched = new vector<mpair>();
        matched->push_back(*p);
        return matched;
    }
    *pstr = p->first + " " +*pstr;
    return nullptr;
}

vector<mpair>* multOptr(string *pstr) {
    auto p = parseNext(pstr);
    if(p==nullptr) {
        return nullptr;
    }
    if(p->second == "MULT" || p->second=="DIV") {
        auto matched = new vector<mpair>();
        matched->push_back(*p);
        return matched;
    }
    *pstr = p->first + " " +*pstr;
    return nullptr;
}

vector<mpair>* releOptr(string *pstr) {
    auto p = parseNext(pstr);
    if(p== nullptr) {
        return nullptr;
    }
    auto s = p->second;
    if(s=="LSS" || s=="LEQ" || s=="GRE" || s=="GEQ" || s=="EQL" || s=="NEQ") {
        auto v = new vector<mpair>();
        v->push_back(*p);
        return v;
    }
    *pstr = p->first +" " +*pstr;
    return nullptr;
}

vector<mpair>* identifier(string *pstr) {
    auto matched = new vector<mpair>();
    auto *p = parseNext(pstr);
    if(p== nullptr) {
        return nullptr;
    }
    if(p->second=="IDENFR") {
        matched->push_back(*p);

        return matched;
    }
    *pstr = p->first + *pstr;
    return nullptr;
}

vector<mpair>* keyword(string *pstr,const string& key){
    auto matched = new vector<mpair>();
    auto *p = parseNext(pstr);
    if(p== nullptr) {
        return nullptr;
    }
    if(p->second==key) {
        matched->push_back(*p);
        return matched;
    }
    *pstr = p->first + " " +*pstr;
    return nullptr;
}

vector<mpair>* _string(string *pstr) {
    auto matched = new vector<mpair>();
    auto p = parseNext(pstr);
    if(p== nullptr) {
        return nullptr;
    }
    if(p->second == "STRCON") {
        matched->push_back(*p);
        matched->push_back(mpair("<字符串>","UNEND"));
        return matched;
    }
    *pstr = p->first + " " +*pstr;
    return nullptr;
}


vector<mpair>* character(string *pstr) {
    auto matched = new vector<mpair>();
    auto p = parseNext(pstr);
    if(p == nullptr) {
        return nullptr;
    }
    matched->push_back(*p);
    if(p->second!="CHARCON") {
        backtrack(matched,pstr);
    }
    return matched;
}

vector<mpair>* unsignedInt(string *pstr) {
    auto matched = new vector<mpair>();
    auto *p = parseNext(pstr);
    if(p== nullptr) {
        return nullptr;
    }
    matched->push_back(*p);
    if(p->second!="INTCON") {
        backtrack(matched,pstr);
        return nullptr;
    }
    matched->push_back(mpair("<无符号整数>","UNEND"));
    return matched;
}

vector<mpair>*  integer(string *pstr) {
    auto matched = new vector<mpair>();
    auto p = keyword(pstr,"PLUS");
    if(p != nullptr) {
        merge(matched, p);
    }
    p = keyword(pstr,"MINU");
    if(p != nullptr) {
        merge(matched,p);
    }
    p = unsignedInt(pstr);
    if(p != nullptr) {
        merge(matched,p);
        matched->push_back(mpair("<整数>","UNEND"));
        return matched;
    }
    backtrack(matched,pstr);
    return nullptr;
}

vector<mpair>*  typeIdentifier(string *pstr) {

    auto p = keyword(pstr,"INTTK");
    if(p!= nullptr) {
        return p;
    }
    p = keyword(pstr,"CHARTK");
    return p;
}

vector<mpair>*  headDeclaration(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = keyword(pstr,"INTTK");
    if(p1!= nullptr){
        merge(matched,p1);
        auto p2 = identifier(pstr);
        if(p2!= nullptr) {
            merge(matched,p2);
            matched->push_back(mpair("<声明头部>","UNEND"));
            return matched;
        }
    }
    backtrack(matched,pstr);
    p1 = keyword(pstr,"CHARTK");
    if(p1!= nullptr){
        merge(matched,p1);
        auto p2 = identifier(pstr);
        if(p2!= nullptr) {
            merge(matched,p2);
            matched->push_back(mpair("<声明头部>","UNEND"));
            return matched;
        }
    }
    backtrack(matched,pstr);
    return nullptr;
}
vector<mpair>*  constDefinition(string *pstr) {
    auto matched = new vector<mpair>();
    auto *p = keyword(pstr,"INTTK");
    if(p!= nullptr){
        merge(matched,p);
        p = identifier(pstr);
        if(p!= nullptr) {
            merge(matched,p);
            p = keyword(pstr,"ASSIGN");
            if(p!= nullptr) {
                merge(matched,p);
                p = integer(pstr);
                if(p!= nullptr) {
                    merge(matched,p);
                    while(true) {
                        auto matched1 = new vector<mpair>();
                        p = keyword(pstr,"COMMA");
                        if(p == nullptr) {
                            backtrack(matched1,pstr);
                            break;
                        }
                        merge(matched1,p);
                        p = identifier(pstr);
                        if(p == nullptr) break;
                        merge(matched1,p);
                        p = keyword(pstr,"ASSIGN");
                        if(p == nullptr) break;
                        merge(matched1,p);
                        p = integer(pstr);
                        if(p == nullptr) break;
                        merge(matched1,p);
                        merge(matched, matched1);
                    }
                }
            }
        }
        matched->push_back(mpair("<常量定义>","UNEND"));
        return matched;
    }
    p = keyword(pstr,"CHARTK");
    if(p!= nullptr){
        merge(matched,p);
        p = identifier(pstr);
        if(p!= nullptr) {
            merge(matched,p);
            p = keyword(pstr,"ASSIGN");
            if(p!= nullptr) {
                merge(matched,p);
                p = character(pstr);
                if(p!= nullptr) {
                    merge(matched,p);
                    while(true) {
                        auto matched1 = new vector<mpair>();
                        p = keyword(pstr,"COMMA");
                        if(p == nullptr) {
                            backtrack(matched1,pstr);
                            break;
                        }
                        merge(matched1,p);
                        p = identifier(pstr);
                        if(p == nullptr) break;
                        merge(matched1,p);
                        p = keyword(pstr,"ASSIGN");
                        if(p == nullptr) break;
                        merge(matched1,p);
                        p = character(pstr);
                        if(p == nullptr) break;
                        merge(matched1,p);
                        merge(matched, matched1);
                    }
                }
            }
        }
        matched->push_back(mpair("<常量定义>","UNEND"));
        return matched;
    }
    backtrack(matched,pstr);
    return nullptr;
}

vector<mpair>*  constDeclaration(string *pstr) {
    auto matched = new vector<mpair>();
    auto p = keyword(pstr,"CONSTTK");
    merge(matched,p);
    if(p== nullptr) {
        backtrack(matched,pstr);
        return nullptr;
    }
    p = constDefinition(pstr);
    merge(matched,p);
    if(p== nullptr) {
        backtrack(matched,pstr);
        return nullptr;
    }
    p = keyword(pstr,"SEMICN");
    merge(matched,p);
    if(p== nullptr) {
        backtrack(matched,pstr);
        return nullptr;
    }
    while(true) {
        auto matched1 = new vector<mpair>();
        p = keyword(pstr,"CONSTTK");
        merge(matched1,p);
        if(p== nullptr) {
            backtrack(matched1,pstr);
            break;
        }
        p = constDefinition(pstr);
        merge(matched1,p);
        if(p== nullptr) {
            backtrack(matched1,pstr);
            break;
        }
        p = keyword(pstr,"SEMICN");
        merge(matched1,p);
        if(p== nullptr) {
            backtrack(matched1,pstr);
            break;
        }
        merge(matched,matched1);
    }
    matched->push_back(mpair("<常量说明>","UNEND"));
    return matched;
}

vector<mpair>*  arrayIdentifier(string *pstr) {
    auto arr = new vector<mpair>();
    auto p = identifier(pstr);
    if(p!= nullptr) {
        merge(arr,p);
        p = keyword(pstr, "LBRACK");
        if (p != nullptr) { // type id[
            merge(arr, p);
            p = unsignedInt(pstr);
            if (p != nullptr) { //type id[%d
                merge(arr, p);
                p = keyword(pstr, "RBRACK");
                if (p != nullptr) { //type id[%d]
                    merge(arr, p);
                    return arr;
                }
            }
        }
    }
    backtrack(arr,pstr);
    return nullptr;
}

vector<mpair>*  varDefinition(string *pstr) {
    auto matched = new vector<mpair>();
    auto p = typeIdentifier(pstr);
    merge(matched,p);
    if(p!= nullptr) {  // char/int
        auto arr = arrayIdentifier(pstr);
        if(arr != nullptr) {
            merge(matched, arr);
        }
        else {
            p =  identifier(pstr);
            if(p!= nullptr) {
                merge(matched,p);
            }else {
                backtrack(matched,pstr);
                return nullptr;
            }
        }
        while(true) {
            auto loop = new vector<mpair>();
            p = keyword(pstr,"COMMA");
            if(p!=nullptr) {
                merge(loop,p);
                auto arr1 = arrayIdentifier(pstr);
                if(arr1 != nullptr) {
                    merge(loop, arr1);
                    merge(matched,loop);
                    continue;
                }else {
                    p = identifier(pstr);
                    if(p!= nullptr) {
                        merge(loop,p);
                        merge(matched,loop);
                        continue;
                    }else {
                        backtrack(loop,pstr);
                        break;
                    }
                }
            }
            else break;
        }
        matched->push_back(mpair("<变量定义>","UNEND"));
        return matched;
    }
    backtrack(matched,pstr);
    return nullptr;
}

vector<mpair>*  varDeclaration(string *pstr) {
    auto matched = new vector<mpair>();
    auto p = varDefinition(pstr);
    if(p!= nullptr) {
        merge(matched,p);
        p = keyword(pstr,"SEMICN");
        if(p!= nullptr) {
            merge(matched,p);
            while(true) {
                 auto loop = varDefinition(pstr);
                 if(loop == nullptr) {
                     break;
                 }
                 p = keyword(pstr,"SEMICN");
                 if(p != nullptr) {
                     merge(loop,p);
                     merge(matched,loop);
                 }
                 else {
                     backtrack(loop,pstr);
                     break;
                 }
            }
            matched->push_back(mpair("<变量说明>","UNEND"));
            return matched;
        }
    }
    backtrack(matched,pstr);
    return nullptr;

}
vector<mpair>* valueArguList(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = expression(pstr);
    if(p1!= nullptr) {
        merge(matched,p1);
        while(true) {
            auto loop = new vector<mpair>();
            auto l1 = keyword(pstr,"COMMA");
            if(l1!= nullptr) {
                merge(loop,l1);
                auto l2 = expression(pstr);
                if(l2!= nullptr) {
                    merge(loop,l2);
                    merge(matched,loop);
                    continue;
                }
            }
            backtrack(loop,pstr);
            break;
        }
        matched->push_back(mpair("<值参数表>","UNEND"));
        return matched;
    }
    backtrack(matched,pstr);
    matched->push_back(mpair("<值参数表>","UNEND"));
    return nullptr;
}
vector<mpair>*  returnFuncCall(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = identifier(pstr);

    if(p1!= nullptr) {
        bool flag = false;
//        for(auto &it : returnFunNameTable) {
//            if(p1->at(0).first==it) {
//                flag = true;
//                break;
//            }
//        }
//        if(!flag) {
//            backtrack(p1,pstr);
//            return nullptr;
//        }

        merge(matched,p1);
        auto p2 = keyword(pstr,"LPARENT");
        if(p2!= nullptr) {
            merge(matched,p2);
            auto p3 = valueArguList(pstr);
            if(p3!= nullptr) {
                merge(matched,p3);
                auto p4 = keyword(pstr,"RPARENT");
                if(p4!= nullptr) {
                    merge(matched,p4);
                    matched->push_back(mpair("<有返回值函数调用语句>","UNEND"));
                    return matched;
                }
            }
        }
    }
    backtrack(matched,pstr);
    return nullptr;
}
vector<mpair>* noReturnFuncCall(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = identifier(pstr);

    if(p1!= nullptr) {
        merge(matched,p1);
//        bool flag = false;
//        for(auto &it : noReturnFunNameTable) {
//            if(p1->at(0).first==it) {
//                flag = true;
//                break;
//            }
//        }
//        if(!flag) {
//            backtrack(p1,pstr);
//            return nullptr;
//        }
        auto p2 = keyword(pstr,"LPARENT");
        if(p2!= nullptr) {
            merge(matched,p2);
            auto p3 = valueArguList(pstr);
            if(p3!= nullptr) {
                merge(matched,p3);
                auto p4 = keyword(pstr,"RPARENT");
                if(p4!= nullptr) {
                    merge(matched,p4);
                    matched->push_back(mpair("<无返回值函数调用语句>","UNEND"));
                    return matched;
                }
            }
        }
    }
    backtrack(matched,pstr);
    return nullptr;
}
vector<mpair>*  argulist(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = typeIdentifier(pstr);
    if(p1!= nullptr) {
        merge(matched,p1);
        auto p2 = identifier(pstr);
        if(p2!= nullptr) {
            merge(matched,p2);
            while(true) {
                auto loop = new vector<mpair>();
                auto l1 = keyword(pstr,"COMMA");
                if(l1!= nullptr) {
                    merge(loop,l1);
                    auto l2 = typeIdentifier(pstr);
                    if(l2!= nullptr) {
                        merge(loop,l2);
                        auto l3 = identifier(pstr);
                        if(l3!= nullptr) {
                            merge(loop,l3);
                            merge(matched,loop);
                            continue;
                        }
                    }
                }
                backtrack(loop,pstr);
                break;
            }
            matched->push_back(mpair("<参数表>","UNEND"));
            return matched;
        }
    }
    backtrack(matched,pstr);
    matched->push_back(mpair("<参数表>","UNEND"));
    return matched;
}
vector<mpair>* returnFunDefinition(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = headDeclaration(pstr);
    if(p1!= nullptr) {
        merge(matched,p1);
        auto p2 = keyword(pstr,"LPARENT");
        if(p2!= nullptr) {
            merge(matched,p2);
            auto p3 = argulist(pstr);
            if(p3!= nullptr) {
                merge(matched,p3);
                auto p4 = keyword(pstr,"RPARENT");
                if(p4!= nullptr) {
                    merge(matched,p4);
                    auto p5 = keyword(pstr,"LBRACE");
                    if(p5!= nullptr) {
                        merge(matched,p5);
                        auto p6 = compoundStatement(pstr);
                        if (p6 != nullptr) {
                            merge(matched, p6);
                            auto p7 = keyword(pstr,"RBRACE");
                            if (p7 != nullptr) {
                                merge(matched, p7);
                                returnFunNameTable.push_back(p1->at(1).first);
                                cout << p1->at(1).first << "return " << endl;
                                matched->push_back(mpair("<有返回值函数定义>","UNEND"));
                                return matched;
                            }
                        }

                    }
                }
            }
        }
    }
    backtrack(matched,pstr);
    return nullptr;
}
vector<mpair>* noReturnFunDefinition(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = keyword(pstr,"VOIDTK");
    if(p1!= nullptr) {
        merge(matched,p1);
        auto p2 = identifier(pstr);
        if(p2!= nullptr) {
            merge(matched,p2);
            auto p3 = keyword(pstr,"LPARENT");
            if(p3!= nullptr) {
                merge(matched,p3);
                auto p4 = argulist(pstr);
                if(p4!= nullptr) {
                    merge(matched,p4);
                    auto p5 = keyword(pstr,"RPARENT");
                    if(p5!= nullptr) {
                        merge(matched,p5);
                        auto p6 = keyword(pstr, "LBRACE");
                        if (p6 != nullptr) {
                            merge(matched, p6);
                            auto p7 = compoundStatement(pstr);
                            if (p7 != nullptr) {
                                merge(matched, p7);
                                auto p8 = keyword(pstr, "RBRACE");
                                if (p8 != nullptr) {
                                    merge(matched, p8);
                                    noReturnFunNameTable.push_back(p2->at(0).first);
                                    cout << p2->at(0).first << "   NoReturn " << endl;
                                    matched->push_back(mpair("<无返回值函数定义>", "UNEND"));
                                    return matched;
                                }
                            }

                        }
                    }
                }
            }
        }
    }
    backtrack(matched,pstr);
    return nullptr;
}
vector<mpair>*  factor(string *pstr) {
    auto fun = returnFuncCall(pstr);
    if(fun!=nullptr) {
        fun->push_back(mpair("<因子>","UNEND"));
        return fun;
    }

    auto matched = new vector<mpair>();
    auto p1 = identifier(pstr); //id
    if(p1!= nullptr) {
        merge(matched,p1);
        auto p2 = keyword(pstr,"LBRACK"); //id[
        if(p2!= nullptr) {
            merge(matched,p2);
            auto p3 = expression(pstr); //id[expr
            if(p3 != nullptr) {
                merge(matched,p3);
                auto p4 = keyword(pstr,"RBRACK"); //id[expr]
                if(p4 != nullptr) {
                    merge(matched,p4);
                    matched->push_back(mpair("<因子>","UNEND"));
                    return matched;
                }
            }
        }
    }
    backtrack(matched,pstr);

    auto lpr = keyword(pstr,"LPARENT");
    if(lpr!=nullptr) {
        merge(matched,lpr);
        auto expr = expression(pstr);
        if(expr != nullptr) {
            merge(matched,expr);
            auto rpr = keyword(pstr,"RPARENT");
            if(rpr!= nullptr) {
                merge(matched,expr);
                matched->push_back(mpair("<因子>","UNEND"));
                return matched;
            }
        }
    }
    backtrack(matched,pstr);

    auto id = identifier(pstr);
    if(id!= nullptr) {
        id->push_back(mpair("<因子>","UNEND"));
        return id;
    }

    auto p = integer(pstr);
    if(p != nullptr) {
        p->push_back(mpair("<因子>","UNEND"));
        return p;
    }

    auto c = character(pstr);
    if(c != nullptr) {
        c->push_back(mpair("<因子>","UNEND"));
        return c;
    }

    return nullptr;

}
vector<mpair>*  term(string *pstr) {
    auto matched = new vector<mpair>();
    auto fct = factor(pstr);
    if(fct!=nullptr) {
        merge(matched,fct);
        while(true) {
            auto loop = new vector<mpair>();
            auto mult = multOptr(pstr);
            if(mult!= nullptr) {
                merge(loop,mult);
                auto fct1 = factor(pstr);
                if(fct1!= nullptr) {
                    merge(loop,fct1);
                    merge(matched,loop);
                    continue;
                }
            }
            backtrack(loop,pstr);
            break;
        }
        matched->push_back(mpair("<项>","UNEND"));
        return matched;
    }
    backtrack(matched,pstr);
    return nullptr;
}
vector<mpair>*  expression(string *pstr) {
    auto matched = new vector<mpair>();
    auto add = addOptr(pstr);
    if(add != nullptr) {
        merge(matched,add);
    }
    auto t = term(pstr);
    if(t!= nullptr) {
        merge(matched,t);
        while(true) {
            add = addOptr(pstr);
            auto loop = new vector<mpair>();
            if(add!= nullptr) {
                merge(loop,add);
                auto t1 = term(pstr);
                if(t1!= nullptr) {
                    merge(loop,t1);
                    merge(matched,loop);
                    continue;
                }
            }
            backtrack(loop,pstr);
            break;
        }
        matched->push_back(mpair("<表达式>","UNEND"));
        return matched;
    }
    backtrack(matched,pstr);
    return nullptr;
}
vector<mpair>*  condition(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = expression(pstr);
    if(p1!= nullptr) {
        merge(matched,p1);
        auto p2 = releOptr(pstr);
        if(p2!= nullptr) {
            merge(matched,p2);
            auto p3 = expression(pstr);
            if(p3 != nullptr) {
                merge(matched,p3);
                matched->push_back(mpair("<条件>","UNEND"));
                return matched;
            }
        }
    }
    backtrack(matched,pstr);
    auto p4 = expression(pstr);
    if(p4!= nullptr) {
        p4->push_back(mpair("<条件>","UNEND"));
        return p4;
    }
    return nullptr;
}

vector<mpair>*  conditionStatement(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = keyword(pstr,"IFTK");
    if(p1!= nullptr) {
        merge(matched,p1);
        auto p2 = keyword(pstr,"LPARENT");
        if(p2!= nullptr) {
            merge(matched,p2);
            auto p3 = condition(pstr);
            if(p3!= nullptr) {
                merge(matched,p3);
                auto p4 = keyword(pstr,"RPARENT");
                if(p4!= nullptr) {
                    merge(matched,p4);
                    auto p5 = statement(pstr);
                    if(p5!= nullptr) {
                        merge(matched,p5);
                        auto once  = new vector<mpair>();
                        auto l1 = keyword(pstr,"ELSETK");
                        if(l1!= nullptr) {
                            merge(once,l1);
                            auto l2 = statement(pstr);
                            if(l2!= nullptr) {
                                merge(once,l2);
                                merge(matched,once);

                            }
                            else {
                                backtrack(once,pstr);
                            }
                        }
                        matched->push_back(mpair("<条件语句>","UNEND"));
                        return matched;
                    }
                }
            }
        }
    }
    backtrack(matched,pstr);
    return nullptr;

}

vector<mpair>*  stride(string *pstr) {
    auto matched = unsignedInt(pstr);
    if(matched!= nullptr) {
        matched->push_back(mpair("<步长>","UNEND"));
        return matched;
    }
    return nullptr;
}
vector<mpair>*  whileStatement(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = keyword(pstr,"WHILETK");
    if(p1!= nullptr) {
        merge(matched,p1);
        auto p2 = keyword(pstr,"LPARENT");
        if(p2!= nullptr) {
            merge(matched,p2);
            auto p3 = condition(pstr);
            if(p3!= nullptr) {
                merge(matched,p3);
                auto p4 = keyword(pstr,"RPARENT");
                if(p4!= nullptr) {
                    merge(matched,p4);
                    auto p5 = statement(pstr);
                    if(p5!= nullptr) {
                        merge(matched,p5);
                        return matched;

                    }
                }
            }
        }
    }
    backtrack(matched,pstr);
    return nullptr;
}
vector<mpair>*  doWhileStatement(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = keyword(pstr,"DOTK");
    if(p1!= nullptr) {
        merge(matched,p1);
        auto p2 = statement(pstr);
        if(p2!= nullptr) {
            merge(matched,p2);
            auto p3 = keyword(pstr,"WHILETK");
            if(p3!= nullptr) {
                merge(matched,p3);
                auto p4 = keyword(pstr,"LPARENT");
                if(p4!= nullptr) {
                    merge(matched,p4);
                    auto p5 = condition(pstr);
                    if(p5!= nullptr) {
                        merge(matched,p5);
                        auto p6 = keyword(pstr,"RPARENT");
                        if(p6!= nullptr) {
                            merge(matched,p6);
                            return matched;
                        }
                    }
                }
            }
        }
    }
    backtrack(matched,pstr);
    return nullptr;
}
vector<mpair>*  forStatement(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = keyword(pstr,"FORTK");
    if(p1!= nullptr) {
        merge(matched,p1);
        auto p2 = keyword(pstr,"LPARENT");
        if(p2!= nullptr){
            merge(matched,p2);
            auto p3 = identifier(pstr);
            if(p3!= nullptr) {
                merge(matched,p3);
                auto p4 = keyword(pstr,"ASSIGN");
                if(p4!= nullptr) {
                    merge(matched,p4);
                    auto p5 = expression(pstr);
                    if(p5!= nullptr) {
                        merge(matched,p5);
                        auto p6 = keyword(pstr,"SEMICN");
                        if(p6!= nullptr) {
                            merge(matched,p6);
                            auto p7 = condition(pstr);
                            if(p7!= nullptr) {
                                merge(matched,p7);
                                auto p8 = keyword(pstr,"SEMICN");
                                if(p8!= nullptr) {
                                    merge(matched,p8);
                                    auto p9 = identifier(pstr);
                                    if(p9!= nullptr) {
                                        merge(matched,p9);
                                        auto p10 = keyword(pstr,"ASSIGN");
                                        if(p10!= nullptr) {
                                            merge(matched,p10);
                                            auto p11 = identifier(pstr);
                                            if(p11!= nullptr) {
                                                merge(matched,p11);
                                                auto p12 = addOptr(pstr);
                                                merge(matched,p12);
                                                auto p13 = stride(pstr);
                                                if(p13!= nullptr) {
                                                    merge(matched,p13);
                                                    auto p14 = keyword(pstr,"RPARENT");
                                                    if(p14!= nullptr) {
                                                        merge(matched,p14);
                                                        auto p15 = statement(pstr);
                                                        if(p15!= nullptr) {
                                                            merge(matched,p15);
                                                            return matched;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    backtrack(matched,pstr);
    return nullptr;
}
vector<mpair>*  loopStatement(string *pstr) {
    auto matched = whileStatement(pstr);
    if(matched!= nullptr) {
        matched->push_back(mpair("<循环语句>","UNEND"));
        return matched;
    }
    matched = doWhileStatement(pstr);
    if(matched!= nullptr) {
        matched->push_back(mpair("<循环语句>","UNEND"));
        return matched;
    }
    matched = forStatement(pstr);
    if(matched!= nullptr) {
        matched->push_back(mpair("<循环语句>","UNEND"));
        return matched;
    }
    return nullptr;
}

vector<mpair>*  scanStatement(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = keyword(pstr,"SCANFTK");
    if(p1!= nullptr) {
        merge(matched,p1);
        auto p2 = keyword(pstr,"LPARENT");
        if(p2!= nullptr) {
            merge(matched,p2);
            auto p3 = identifier(pstr);
            if(p3!= nullptr) {
                merge(matched,p3);

                while(true) {
                    auto loop = new vector<mpair>();
                    auto l1 = keyword(pstr,"COMMA");
                    if(l1!= nullptr) {
                        merge(loop,l1);
                        auto l2 = identifier(pstr);
                        if(l2!= nullptr) {
                            merge(loop,l2);
                            merge(matched,loop);
                            continue;
                        }
                    }
                    backtrack(loop,pstr);
                    break;
                }

                auto p4 = keyword(pstr,"RPARENT");
                if(p4!= nullptr) {
                    merge(matched,p4);
                    matched->push_back(mpair("<读语句>","UNEND"));
                    return matched;
                }

            }


        }
    }
    backtrack(matched,pstr);
    return nullptr;
}

vector<mpair>* printPatternA(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = keyword(pstr,"PRINTFTK");
    if(p1!= nullptr) {
        merge(matched,p1);
        auto p2 = keyword(pstr,"LPARENT");
        if(p2!= nullptr) {
            merge(matched,p2);
            auto p3 = _string(pstr);
            if(p3!= nullptr) {
                merge(matched,p3);
                auto p4 = keyword(pstr,"COMMA");
                if(p4!= nullptr) {
                    merge(matched,p4);
                    auto p5 = expression(pstr);
                    if(p5!= nullptr) {
                        merge(matched,p5);
                        auto p6 = keyword(pstr,"RPARENT");
                        if(p6!= nullptr) {
                            merge(matched,p6);
                            return matched;
                        }
                    }
                }
            }
        }
    }
    backtrack(matched,pstr);
    return nullptr;
}

vector<mpair>* printPatternB(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = keyword(pstr,"PRINTFTK");
    if(p1!= nullptr) {
        merge(matched,p1);
        auto p2 = keyword(pstr,"LPARENT");
        if(p2!= nullptr) {
            merge(matched,p2);
            auto p3 = _string(pstr);
            if(p3!= nullptr) {
                merge(matched,p3);
                auto p4 = keyword(pstr,"RPARENT");
                if(p4!= nullptr) {
                    merge(matched,p4);
                    return matched;
                }
            }
        }
    }
    backtrack(matched,pstr);
    return nullptr;
}

vector<mpair>* printPatternC(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = keyword(pstr,"PRINTFTK");
    if(p1!= nullptr) {
        merge(matched,p1);
        auto p2 = keyword(pstr,"LPARENT");
        if(p2!= nullptr) {
            merge(matched,p2);
            auto p3 = expression(pstr);
            if(p3!= nullptr) {
                merge(matched,p3);
                auto p4 = keyword(pstr,"RPARENT");
                if(p4!= nullptr) {
                    merge(matched,p4);
                    return matched;
                }
            }
        }
    }
    backtrack(matched,pstr);
    return nullptr;
}

vector<mpair>*  printStatement(string *pstr) {
    auto A = printPatternA(pstr);
    if(A!= nullptr) {
        A->push_back(mpair("<写语句>","UNEND"));
        return A;
    }
    auto B = printPatternB(pstr);
    if(B!= nullptr) {
        B->push_back(mpair("<写语句>","UNEND"));
        return B;
    }
    auto C = printPatternC(pstr);
    if(C!= nullptr) {
        C->push_back(mpair("<写语句>","UNEND"));
        return C;
    }
    return nullptr;
}

vector<mpair>* returnStatement(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = keyword(pstr,"RETURNTK");
    if(p1!= nullptr) {
        merge(matched,p1);
        auto once = new vector<mpair>();
        auto p2 = keyword(pstr,"LPARENT");
        if(p2!= nullptr) {
            merge(once,p2);
            auto p3 = expression(pstr);
            if(p3!= nullptr) {
                merge(once,p3);
                auto p4 = keyword(pstr,"RPARENT");
                if(p4!= nullptr) {
                    merge(once,p4);
                    merge(matched,once);
                    matched->push_back(mpair("<返回语句>","UNEND"));
                    return matched;
                }
            }
        }
        backtrack(once,pstr);
        matched->push_back(mpair("<返回语句>","UNEND"));
        return matched;
    }

    return nullptr;
}

vector<mpair>* assignStatementA(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = identifier(pstr);
    if(p1!= nullptr) {
        merge(matched,p1);
        auto p2 = keyword(pstr,"ASSIGN");
        if(p2!= nullptr) {
            merge(matched,p2);
            auto p3 = expression(pstr);
            if(p3!= nullptr) {
                merge(matched,p3);
                return matched;
            }
        }
    }
    backtrack(matched,pstr);
    return nullptr;
}
vector<mpair>* assignStatementB(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = identifier(pstr);
    if(p1!= nullptr) {
        merge(matched,p1);
        auto p2 = keyword(pstr,"LBRACK");
        if(p2!= nullptr) {
            merge(matched,p2);
            auto p3 = expression(pstr);
            if(p3!= nullptr) {
                merge(matched,p3);
                auto p4 = keyword(pstr,"RBRACK");
                if(p4!= nullptr) {
                    merge(matched,p4);
                    auto p5 = keyword(pstr,"ASSIGN");
                    if(p5!= nullptr) {
                        merge(matched,p5);
                        auto p6 = expression(pstr);
                        if(p6!= nullptr) {
                            merge(matched,p6);
                            return matched;
                        }
                    }
                }
            }
        }
    }
    backtrack(matched,pstr);
    return nullptr;
}
vector<mpair>* assignStatement(string *pstr)  {
    auto p1 = assignStatementA(pstr);
    if(p1!= nullptr) {
        p1->push_back(mpair("<赋值语句>","UNEND"));
        return p1;
    }
    auto p2 = assignStatementB(pstr);
    if(p2!= nullptr) {
        p2->push_back(mpair("<赋值语句>","UNEND"));
        return p2;
    }
    return nullptr;
}
vector<mpair>*  statement(string *pstr) {
    auto matched = conditionStatement(pstr);
    if(matched!= nullptr) {
        matched->push_back(mpair("<语句>","UNEND"));
        return matched;
    }

    matched = loopStatement(pstr);
    if(matched!= nullptr) {
        matched->push_back(mpair("<语句>","UNEND"));
        return matched;
    }

    auto p1 = keyword(pstr,"LBRACE");
    matched = new vector<mpair>();
    if(p1!= nullptr) {
        merge(matched,p1);
        auto p2 = statementlist(pstr);
        if(p2!= nullptr) {
            merge(matched,p2);
            auto p3 = keyword(pstr,"RBRACE");
            if(p3!=nullptr) {
                merge(matched,p3);
                matched->push_back(mpair("<语句>","UNEND"));
                return matched;
            }
        }
    }
    backtrack(matched,pstr);

    matched = printStatement(pstr);
    if(matched!= nullptr) {
        auto smcn = keyword(pstr,"SEMICN");
        if(smcn!= nullptr) {
            merge(matched,smcn);
            matched->push_back(mpair("<语句>","UNEND"));
            return matched;
        }
        backtrack(matched,pstr);
    }

    matched = scanStatement(pstr);
    if(matched!= nullptr) {
        auto smcn = keyword(pstr,"SEMICN");
        if(smcn!= nullptr) {
            merge(matched,smcn);
            matched->push_back(mpair("<语句>","UNEND"));
            return matched;
        }
        backtrack(matched,pstr);
    }

    matched = assignStatement(pstr);
    if(matched!= nullptr) {
        auto smcn = keyword(pstr,"SEMICN");
        if(smcn!= nullptr) {
            merge(matched,smcn);
            matched->push_back(mpair("<语句>","UNEND"));
            return matched;
        }
        backtrack(matched,pstr);
    }

    matched = returnStatement(pstr);
    if(matched!= nullptr) {
        auto smcn = keyword(pstr,"SEMICN");
        if(smcn!= nullptr) {
            merge(matched,smcn);
            matched->push_back(mpair("<语句>","UNEND"));
            return matched;
        }
        backtrack(matched,pstr);
    }
    matched = keyword(pstr,"SEMICN");
    if(matched!= nullptr) {
        matched->push_back(mpair("<语句>","UNEND"));
        return matched;
    }


    matched = noReturnFuncCall(pstr);
    if(matched!= nullptr) {
        auto smcn = keyword(pstr,"SEMICN");
        if(smcn!= nullptr) {
            merge(matched,smcn);
            matched->push_back(mpair("<语句>","UNEND"));
            return matched;
        }
        backtrack(matched,pstr);
    }

    matched = returnFuncCall(pstr);
    if(matched!= nullptr) {
        auto smcn = keyword(pstr,"SEMICN");
        if(smcn!= nullptr) {
            merge(matched,smcn);
            matched->push_back(mpair("<语句>","UNEND"));
            return matched;
        }
        backtrack(matched,pstr);
    }

    return nullptr;
}
vector<mpair>*  statementlist(string *pstr) {
    auto p = statement(pstr);
    auto matched = new vector<mpair>();
    while(p!= nullptr) {
        merge(matched,p);
        p = statement(pstr);
        ;
        ;
    }
    matched->push_back(mpair("<语句列>","UNEND"));
    return matched;
}

vector<mpair>* compoundStatement(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = constDeclaration(pstr);
    if(p1 != nullptr) {
        merge(matched,p1);
    }
    auto p2 = varDeclaration(pstr);
    if(p2 !=nullptr) {
        merge(matched,p2);
    }
    auto p3 = statementlist(pstr);
    if(p3!= nullptr) {
        merge(matched,p3);
        matched->push_back(mpair("<复合语句>","UNEND"));
        return matched;
    }
    backtrack(matched,pstr);
    return nullptr;
}



vector<mpair>* _main(string *pstr) {
    auto matched = new vector<mpair>();
    auto p1 = keyword(pstr,"VOIDTK");
    if(p1!= nullptr) {
        merge(matched,p1);
        auto p2 = keyword(pstr,"MAINTK");
        if(p2!= nullptr) {
            merge(matched,p2);
            auto p3 = keyword(pstr,"LPARENT");
            if(p3!= nullptr) {
                merge(matched,p3);
                auto p4 = keyword(pstr,"RPARENT");
                if(p4!= nullptr) {
                    merge(matched,p4);
                    auto p5 = keyword(pstr,"LBRACE");
                    if(p5!= nullptr) {
                        merge(matched,p5);
                        auto p6 = compoundStatement(pstr);
                        if (p6 != nullptr) {
                            merge(matched, p6);
                            auto p7 = keyword(pstr,"RBRACE");
                            if (p7 != nullptr) {
                                merge(matched, p7);
                                matched->push_back(mpair("<主函数>","UNEND"));
                                return matched;
                            }
                        }

                    }
                }
            }
        }
    }
    backtrack(matched,pstr);
    return nullptr;
}


vector<mpair>* program(string *pstr) {
    auto matched =new  vector<mpair>();
    auto p1 = constDeclaration(pstr);
    if(p1!=nullptr) {
        merge(matched,p1);
    }
    auto p2 = varDeclaration(pstr);
    if(p2!= nullptr) {
        merge(matched,p2);
    }
    while(true){
        auto loop = new vector<mpair>();
        auto l1 = returnFunDefinition(pstr);
        if(l1!= nullptr) {
            merge(loop,l1);
            merge(matched,loop);
            continue;
        }
        auto l2 = noReturnFunDefinition(pstr);
        if(l2!= nullptr) {
            merge(loop,l2);
            merge(matched,loop);
            continue;
        }
        break;
    }
    auto p3 = _main(pstr);
    if(p3!= nullptr) {
        merge(matched,p3);
        matched->push_back(mpair("<程序>","UNEND"));
        return matched;
    }
    backtrack(matched,pstr);
    return nullptr;
}

map<string,vector<mpair>*(*)(string *)> funcmap() {
    string s = "s";
    map<string,vector<mpair>*(*)(string *)> funcmap;
    funcmap["main"] = _main;
    funcmap["string"] =_string;
    funcmap["program"] = program;
    funcmap["string"] = _string;
    funcmap["constDeclaration"] = constDeclaration;
    funcmap["constDefinition"]  = constDefinition;
    funcmap["unsignedInt"] = unsignedInt;
    funcmap["headDeclaration"] = headDeclaration;
    funcmap["varDeclaration"]  = varDeclaration;
    funcmap["varDefinition"]  = varDefinition;
    funcmap["returnFunDefinition"] = returnFunDefinition;
    funcmap["noReturnFunDefinition"] = noReturnFunDefinition;
    funcmap["compoundStatement"] = compoundStatement;
    funcmap["argulist"] = argulist;
    funcmap["expression"] =expression;
    funcmap["term"] = term;
    funcmap["factor"] = factor;
    funcmap["statement"] = statement;
    funcmap["assignStatement"] = assignStatement;

    return funcmap;
}

vector<mpair>* matchNPattern(vector<string> &list, string* pstr) {
    auto matched = new vector<mpair>();
    for(auto &s:list) {
        auto item = callFunc.find(s);
        vector<mpair>* p = nullptr;
        if(item!= callFunc.end()) {
            p = item->second(pstr);
        }
        else {
            p = keyword(pstr,s);
        }
        if(p!= nullptr) {
            merge(matched,p);
            continue;
        }
        backtrack(matched,pstr);
        break;
    }
    return matched;
}

void grammerAnalysis(string source,ofstream* out) {
    auto funmap = funcmap();

    auto *p = funmap.find("program")->second(&source);
    for(auto & v : *p) {
        if(v.second=="UNEND") {
            *out << v.first << endl;
        }
        else {
            if(v.second == "CHARCON" || v.second== "STRCON") {
                v.first = v.first.substr(1,v.first.size()-2);
            }
            *out << v.second << " " <<v.first<<endl;
        }
    }

}




