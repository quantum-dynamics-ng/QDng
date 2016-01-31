/*
 * SyntacticTreeModel.h
 *
 *  Created on: 23.12.2010
 *      Author: Markus Kowalewski
 */

#ifndef SYNTACTICTREEMODEL_H_
#define SYNTACTICTREEMODEL_H_

#include "tools/Exception.h"
#include <math.h>

#define SIMPLEXML_BUF_SIZE 1024

map<string, string> varlist;


int Str2Int(const char* s){
   int val, ret;
   ret = sscanf(s, "%d", &val);
   if (ret <= 0)
     throw (QDLIB::EParamProblem("Not an integer number", s));
   return val;
}

int Str2Int(string& s){
   return Str2Int(s.c_str());
}

double Str2Double(const char* s){
   double val;
   int ret;
   ret = sscanf(s, "%lf", &val);
   if (ret <= 0)
     throw (QDLIB::EParamProblem("Not an floating point number", s));
   return val;
}


double Str2Double(string& s){
   return Str2Double(s.c_str());
}


/**
 * Represent a string which can contain variables.
 */
class varstring
{
   private:
      vector<string> seqlist;
      typedef enum
      {
         str, var
      } _st;
      vector<_st> seqtype;
      vector<string> fmtlist;
   public:
      varstring() {}

      varstring(const string& s)
      {
         seqlist.push_back(s);
         seqtype.push_back(str);
         fmtlist.push_back("%s");
      }

      varstring(const char* s)
      {
         seqlist.push_back(string(s));
         seqtype.push_back(str);
         fmtlist.push_back("%s");
      }

      void AppendString(const char *s)
      {
         seqlist.push_back(string(s));
         seqtype.push_back(str);
         fmtlist.push_back("%s");
      }

      void AppendVar(const char *s, const char *fmt = "%s")
      {
         seqlist.push_back(string(s));
         seqtype.push_back(var);
         fmtlist.push_back(string(fmt));
      }

      string& GetString()
      {
         string *s = new string();
         for (size_t i = 0; i < seqlist.size(); i++) {
            if (seqtype[i] == str)
               *s += seqlist[i];
            else
               *s += varlist[seqlist[i]];
         }
         return *s;
      }

      void print()
      {
	 char buf[SIMPLEXML_BUF_SIZE];
         for (size_t i = 0; i < seqlist.size(); i++) {
            if (seqtype[i] == str)
               snprintf(buf, SIMPLEXML_BUF_SIZE, fmtlist[i].c_str(), seqlist[i].c_str());
            else {
               string& s = fmtlist[i];
               char& fmt = s[s.length() - 1];
               switch (fmt) {
                  case 'd':
                     snprintf(buf, SIMPLEXML_BUF_SIZE, fmtlist[i].c_str(), Str2Int(varlist[seqlist[i]]));
                     break;
                  case 'f':
                    snprintf(buf, SIMPLEXML_BUF_SIZE, fmtlist[i].c_str(), Str2Double(varlist[seqlist[i]]));

                     break;
                  case 's':
                  default:
                    snprintf(buf, SIMPLEXML_BUF_SIZE, fmtlist[i].c_str(), varlist[seqlist[i]].c_str());
                     break;
               }
            }
            ss_yy_buf << buf;
         }

      }
};

/**
 * A basic code block.
 */
class block
{
   public:
      vector<block*> children;
      block* parent;
   protected:
      int depth;
   public:
      block() : parent(NULL), depth(-1)  {}

      /**
       * Add child block.
       */
      block* AddChild(block *b)
      {
         children.push_back(b);
         b->parent = this;
         b->depth = depth+1;
         return children.back();
      }

      /**
       *  Return the parent node
       */
      block* Parent()
      {
         return parent;
      }

      /**
       * print an indentation
       */
      void Indent()
      {
         for (int i=0; i < depth; i++)
           ss_yy_buf << "   ";
      }

      /**
       * Execute/Run the translation.
       */
      virtual void execute()
      {
         for (uint i = 0; i < children.size(); i++)
            children[i]->execute();
      }

      virtual ~block()
      {
         for (uint i = 0; i < children.size(); i++)
            delete children[i];
      }

};

/**
 * Block representing a for loop.
 */
class forloop: public block
{
   private:
      double begin;
      double end;
      double step;
      string loopvar;
   public:
      forloop() :
         begin(0), end(0), step(1)
      {
      }

      forloop(double b, double e) :
         begin(b), end(e), step(1)
      {
      }
      
      forloop(double b, double e, double s) :
         begin(b), end(e), step(s)
      {
      }

      void Begin(double d)
      {
         begin = d;
      }

      void End(double d)
      {
         end = d;
      }

      void Step(double d)
      {
         step = d;
      }

      void LoopVar(char* var)
      {
         loopvar = var;
      }

      virtual void execute()
      {
         Indent();
         ss_yy_buf << "<!-- for " << loopvar << " = " << begin << " " << end << " " << step << " -->\n";
         double d = begin;
         for (uint i=0; i <= int(fabs((begin-end)/step)); i++){
            char s[64];
            sprintf(s, "%g", d);
            varlist[loopvar] = s;
            block::execute();
            d += step;
         }
      }
};

/**
 * Block representing an xml node structure
 */
class xmlnode: public block
{
   private:
      vector<varstring> attributes;
      vector<varstring> values;
      string name;
   public:
      xmlnode(char *s) :
         name(s)
      {
      }
      void SetName(string s)
      {
         name = s;
      }
      void AddAttribute(varstring &attr, varstring &val)
      {
         attributes.push_back(attr);
         values.push_back(val);
      }

      virtual void execute()
      {
         Indent();
         ss_yy_buf << "<" << name;
         for (uint i = 0; i < attributes.size(); i++) {
            ss_yy_buf << " ";
            attributes[i].print();
            ss_yy_buf << "=\"";
            values[i].print();
            ss_yy_buf << "\"";
         }

         if (children.size() > 0) {
            ss_yy_buf << ">\n";
            block::execute();
            Indent();
            ss_yy_buf << "</" << name << ">\n";
         } else
	    ss_yy_buf << "/>\n";
      }
};

#endif /* SYNTACTICTREEMODEL_H_ */
