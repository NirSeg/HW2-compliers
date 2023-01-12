/**************************************
 *
 * University of Haifa
 *
 * Theory of compilation
 *
 * P machine compiler - code generation project
 *
 * Yosi Ben Asher
 * Email: yosibenasher@gmail.com
 *
 * Mariah Akree
 * Email: mariah.uni.haifa@gmail.com
 *
 **************************************/
 /*   PROGRAM = 258,
      BBEGIN = 259,
      END = 260,
      DECLARE = 261,
      PROCEDURE = 262,
      FUNCTION = 263,
      LABEL = 264,
      INTEGER = 265,
      REAL = 266,
      RECORD = 267,
      BOOLEAN = 268,
      ARRAY = 269,
      OF = 270,
      ASSIGN = 271,
      LC = 272,
      RC = 273,
      IF = 274,
      THEN = 275,
      ELSE = 276,
      WHILE = 277,
      REPEAT = 278,
      FI = 279,
      DO = 280,
      OD = 281,
      READ = 282,
      WRITE = 283,
      TRUE = 284,
      FALSE = 285,
      ADD = 286,
      MIN = 287,
      MUL = 288,
      DIV = 289,
      GOTO = 290,
      MOD = 291,
      LES = 292,
      LEQ = 293,
      EQU = 294,
      NEQ = 295,
      GRE = 296,
      GEQ = 297,
      AND = 298,
      OR = 299,
      NOT = 300,
      CASE = 301,
      FOR = 302,
      FIN = 303,
      IDENTICAL = 304,
      FROM = 305,
      BY = 306,
      TO = 307,
      NEW = 308,
      INTCONST = 309,
      IDE = 310,
      REALCONST = 311,
      STRING = 312,
      DUMMY = 313*/

#ifndef AST_H 
#define AST_H

#include <iostream>
#include <assert.h>
#include <string>
#include <map>
#include <iomanip>
#include <stack>
#include <vector>

using namespace std;

// the product of the array
static int arrayProduct(vector<int>& arr, int index = 0){
  int product = 1;
  int len = arr.size();
  for(int i=index; i<len; i++)
    product *= arr[len - i - 1];
  return product;
}

// {new
static int stackAddress=5;
// for array
extern vector<int> arrsizes;
// new}

// {new
// holds the type the address and the size
struct Information
{
  string type;
  int address, size;
  // for pointer
  string pointed = ""; // the variable that the pointer is pointing
  // for array
  vector<int> arrSizes;
  int subPart;
  string typeArray; // the name of the variables the array is made of
};


// The Symbol Table
class SymbolTable{
  public:
  SymbolTable(){
    stackAddress = 5;
  }
  // simpleType
  bool insertVariable(string name, string type){
    if(symbolTable[name] != NULL) return false;
    symbolTable[name] = new Information;
    symbolTable[name]->type = type;
    symbolTable[name]->size = 1;
    symbolTable[name]->address = stackAddress;
    stackAddress++;
    return true;
  }
  // pointers
  bool insertPointer(string name, string type, string pointed){
    if(symbolTable[name] != NULL) return false;
    symbolTable[name] = new Information;
    symbolTable[name]->type = type;
    symbolTable[name]->size = 1;
    symbolTable[name]->address = stackAddress;
    symbolTable[name]->pointed = pointed;
    stackAddress++;
    return true;
  }
  // records
  bool insertRecord(string name, int address, string type, int size){
    if(symbolTable[name] != NULL) return false;
    symbolTable[name] = new Information;
    symbolTable[name]->type = type;
    symbolTable[name]->size = size;
    symbolTable[name]->address = address;
    return true;
  }
  // arrays
  bool insertArray(string name, string type, int size, int subPart, string typeArray){
    if(symbolTable[name] != NULL) return false;
    symbolTable[name] = new Information;
    symbolTable[name]->type = type;
    symbolTable[name]->size = size;
    symbolTable[name]->address = stackAddress;
    for(int i=0; i<arrsizes.size(); i++)
      symbolTable[name]->arrSizes.push_back(arrsizes[i]);
    symbolTable[name]->subPart = subPart;
    symbolTable[name]->typeArray = typeArray;
    stackAddress += size;
    return true;
  }
  // insert array of pointer
  bool insertArrayOfPointer(string name, string type, int size, int subPart, string typeArray){
    if(symbolTable[name] != NULL) return false;
    symbolTable[name] = new Information;
    symbolTable[name]->type = type;
    symbolTable[name]->size = size;
    symbolTable[name]->address = -123;
    for(int i=0; i<arrsizes.size(); i++)
        symbolTable[name]->arrSizes.push_back(arrsizes[i]);
    symbolTable[name]->subPart = subPart;
    symbolTable[name]->typeArray = typeArray;
    return true;
  }
  // else
  bool insertElse(string name, string type, int size){
    if(symbolTable[name] != NULL) return false;
    symbolTable[name] = new Information;
    symbolTable[name]->type = type;
    symbolTable[name]->size = size;
    symbolTable[name]->address = stackAddress;
    stackAddress += size;
    return true;
  }
  int findAddress(const string& name){
    if(symbolTable[name] == NULL) return -1;
    return symbolTable[name]->address;
  }

  int getCurrAddress() {return stackAddress;}

  int getSize(const string& name){
    if(symbolTable[name] == NULL) return -1;
    return symbolTable[name]->size;
  }

  string getType(const string& name){
    if(symbolTable[name] == NULL) return "";
    return symbolTable[name]->type;
  }

  string getPointed(const string& name){
    if(symbolTable[name] == NULL) return "";
    return symbolTable[name]->pointed;
  }

  int getSubpart(const string& name){
    if(symbolTable[name] == NULL) return -1;
    return symbolTable[name]->subPart;
  }

  string getTypeArray(const string& name){
    if(symbolTable[name] == NULL) return "";
    return symbolTable[name]->typeArray;
  }
  vector<int>& getArrSizes(const string& name){
    return arrsizes;
  }
  void printArrSizes(const string& name, ostream& os){
    int len = symbolTable[name]->arrSizes.size();
    for(int i=0; i<len; i++)
      os << symbolTable[name]->arrSizes[i];
  }
  int getSizeArrSizes(const string& name){
    return symbolTable[name]->arrSizes.size();
  }
  int getProductArray(const string& name, int index = 0){
    return arrayProduct(symbolTable[name]->arrSizes, index);
  }
  private:
    // map = (name, (type, address, size))
    map<string, Information*> symbolTable;
    // current address
};
// new}

// {new
extern bool flagAddConst; // for inc
extern bool flagSubConst; // for dec
extern bool flagVariable; // for ind
// for types
extern string typeVariable; // says the last type that it was
extern string nameVariable; // says the last name that it was
static int sizeVariable; // the size of the variable

// for records
static stack<int> sizeOfRecord;

static stack<int> counterSwitchStack; // // to count the number of switches
static int counterIf = 1; // counter for the number of ifs
static int counterWhile = 1; // counter for the number of whiles
static int counterSwitch = 1; // counter for the number of switches
static int counterCase = 1;


// for arrays
extern int subpart;
extern int sizeArray;
extern string typearray;
// for records
extern SymbolTable symboltable; // for symbol table

// for decleration
extern bool flagDecleration;

// for RecordRef
extern bool flagRecordRefInc; // if there was a record ref, that mean ".", for inc
static int incRecord; // the number that in the inc for the record
static bool flagLdcRecord = true;
extern bool flagRecordSizeP; // for a situation in a record ref that we need the size of the pointed

// for array ref
extern string arrayName;
extern bool flagVarArray;
extern int counterDim;

extern bool flagRecordInd;

// for ind 
extern bool flagNew;
extern bool flagArray;
extern bool flagRecord;

// for pointer to array
extern bool flagPointerArray;

// counts the number of ^ (= *)
extern int addressRefCounter;
// new}

/*
all the problems:
- new statement: done
- ide type: (to handle the flagVariable)
*/

/**
 * classes 
 */
// base class for tree nodes
class Object {
public:
 virtual void print (ostream& os) = 0;
 virtual void pcodegen(ostream& os) = 0;
 virtual Object * clone () const {return NULL;}
 virtual ~Object () {}
};

// expressions
class Expr : public Object {
public :
  // Unary operations
  Expr (int op, Object * atom) : op_(op), atom_(atom), left_(NULL), right_(NULL), unary_(true) {}
  // Binary operations
  Expr (int op, Object * left, Object * right) : op_(op), left_(left), right_(right), atom_(NULL), unary_(false) {}

  Expr(const Expr & exp){
    unary_= exp.unary_; 
    op_ = exp.op_;
    left_ = exp.left_->clone();
    right_ = exp.right_->clone();
    atom_ = exp.atom_->clone();
  }

  virtual ~Expr(){
    if (left_) delete left_;
    if (right_) delete right_;
    if (atom_) delete atom_;
  }

  
  void print (ostream& os) {
    os<<"Node name : Expr"<<endl;
    assert(op_);
    if (unary_){
      os<<"Unary op is : "<<op_;
      assert(atom_);
      atom_->print(os);
    }else{
      os<<"Binary op is : "<<op_;
      assert(left_ && right_);
      left_->print(os);
      right_->print(os);
    }
  }
  
  void pcodegen(ostream& os) {
    // for add and sum
    flagAddConst = false; flagSubConst = false;
    assert(op_);
    if (unary_) {      
        assert(atom_);
        atom_->pcodegen(os);
        // {new
        switch(op_){
          case 287:
            os << "neg" << endl;
            break;
          case 300:
            os << "not" << endl;
            break;
          default:
            break;
        }
        // new}
    }
    else {         
      assert(left_ && right_);
      right_->pcodegen(os);
      // {new
      if(op_==286){
        flagAddConst = true;
      }
      if(op_==287){
        flagSubConst = true;
      }
      // new}
      left_->pcodegen(os);
      // {new
      switch (op_){
        case 286:
          if(!flagAddConst) // to handle inc
            os << "add" << endl;
          break;
        case 287:
          if(!flagSubConst)
            os << "sub" << endl; // need to take care of dec
          break;
        case 288:
          os << "mul" << endl;
          break;
        case 289:
          os << "div" << endl;
          break;
        case 292:
          os << "les" << endl;
          break;
        case 293:
          os << "leq" << endl;
          break;            
        case 294:
          os << "equ" << endl;
          break;
        case 295:
          os << "neq" << endl;
          break;
        case 296:
          os << "grt" << endl;
          break;
        case 297:
          os << "geq" << endl;
          break;
        case 298:
          os << "and" << endl;
          break;
        case 299:
          os << "or" << endl;
          break;
        default:
          break;
      }
      // new}
    }
    flagAddConst = false; flagSubConst = false;
  }
  virtual Object * clone () const { return new Expr(*this);}
  
private:
  bool unary_;
  int op_;
  Object * left_;
  Object * right_;
  Object * atom_;
};

// no need to touch
class ExprList : public Object {
public :
  ExprList (Object * expr) : expr_(expr),expr_list_(NULL){assert(expr_);}
  ExprList (Object * expr, Object * expr_list) : expr_(expr),expr_list_(expr_list) {assert(expr_ && expr_list_);}

  ExprList(const ExprList& exp){
    expr_ = exp.expr_->clone();
    expr_list_ = exp.expr_list_->clone();
  }

  virtual ~ExprList(){
    if (expr_) delete expr_;
    if (expr_list_) delete expr_list_;
  }

  void print (ostream& os) {
		os<<"Node name : ExprList";
		assert( expr_);
    expr_->print(os);
    if (expr_list_){
      expr_list_->print(os);
    }
  }
  void pcodegen(ostream& os) {
      assert(expr_);
      expr_->pcodegen(os);
      if (expr_list_) {
          expr_list_->pcodegen(os);
      }
  }
  virtual Object * clone () const { return new ExprList(*this);}
  
private:
  Object * expr_;
	Object * expr_list_;
};

// now for now, dimentions
class Dim : public Object {
public:
  Dim (Object * exp) : exp_(exp), dim_(NULL) {assert(exp_);}
  Dim (Object * exp, Object * dim) : exp_(exp),dim_(dim) {assert(exp_ && dim_);}

  Dim(const Dim& d){
    exp_ = d.exp_->clone();
    dim_ = d.dim_->clone();
  }

  virtual ~Dim(){
    if (exp_) delete exp_;
    if (dim_) delete dim_;
  }
  
  void print (ostream& os) {
    os<<"Node name : Dim"<<endl;
    assert(exp_);
    exp_->print(os);
    if (dim_){
      dim_->print(os);
    }
  }
  void pcodegen(ostream& os) {
    bool valueFlagLdcRecord=flagLdcRecord;
    assert(exp_);
    //{new
    counterDim++;
    flagLdcRecord = true;
    //new}
    exp_->pcodegen(os);
    //{new
    flagLdcRecord = valueFlagLdcRecord;
    // for ixa
    if(symboltable.getTypeArray(arrayName) == "Integer" || symboltable.getTypeArray(arrayName) == "Real" || symboltable.getTypeArray(arrayName) == "Boolean")
      os << "ixa " << symboltable.getProductArray(arrayName, counterDim) << endl;
    else
      os << "ixa " << symboltable.getProductArray(arrayName, counterDim) * symboltable.getSize(symboltable.getTypeArray(arrayName)) << endl;
    if(symboltable.getSizeArrSizes(arrayName) <= counterDim){
      os << "dec " << symboltable.getSubpart(arrayName) << endl;
      counterDim = 0;
      arrayName = symboltable.getTypeArray(arrayName);
    }
    //new}
    if (dim_) {
        dim_->pcodegen(os);
    }
  }
  virtual Object * clone () const { return new Dim(*this);}

private:
  Object * exp_;
  Object * dim_;
};

class Atom : public Object {
};

// give integer numbers
class IntConst : public Atom {
public:
  IntConst(const int i) : i_(i) {}
  IntConst(const IntConst& in) : i_(in.i_) {}
  
  void print (ostream& os) {
    os<<"Node name : IntConst. Value is :"<<i_<<endl;
  }
  void pcodegen(ostream& os) {
    // {new 
    if(flagAddConst)
      os << "inc " << i_ << endl;
    else if(flagSubConst)
      os << "dec " << i_ << endl;
    else
      os << "ldc " << i_ << endl;
    // new}
  }
  virtual Object * clone () const { return new IntConst(*this);}
private:
  const int i_;
};

// give real numbers
class RealConst : public Atom {
public:
  RealConst(const double r) : r_(r) {}
  RealConst(const RealConst& in) : r_(in.r_) {}  

  void print (ostream& os) {
    os<<"Node name : RealConst. Value is :"<<r_<<endl;
  }
  void pcodegen(ostream& os) {
    // {new 
    if(flagAddConst)
      os << "inc " << std::fixed << std::setprecision(1) << r_ << endl; 
    else if(flagSubConst)
      os << "dec " << std::fixed << std::setprecision(1) << r_ << endl;
    else
      os << "ldc " << std::fixed << std::setprecision(1) << r_ << endl;
    // new}
  }
  virtual Object * clone () const { return new RealConst(*this);}
private:
  const double r_;
};

// give true
class True : public Atom {
public:
  void print (ostream& os) {
    os<<"Node name : trueConst. Value is true"<<endl;
  }
  void pcodegen(ostream& os) {
    // {new 
    os << "ldc " << 1 << endl;
    // new}
  }
  virtual Object * clone () const { return new True();}

};

//give false
class False : public Atom {
public :
  void print (ostream& os) {
    os<<"Node name : trueConst. Value is false"<<endl;
  }
  void pcodegen(ostream& os) {
    // {new 
    os << "ldc " << 0 << endl;
    // new}
  }
  virtual Object * clone () const { return new False();}
};

// now for now
class Var : public Atom {
};

// now for now
class ArrayRef : public Var {
public :
  ArrayRef (Object * var, Object * dim) : var_(var),dim_(dim) {assert(var_ && dim_);}
  ArrayRef(const ArrayRef& arr){
    var_ = arr.var_->clone();
    dim_ = arr.dim_->clone();
  }

  virtual ~ArrayRef(){
    if (var_) delete var_;
    if (dim_) delete dim_;
  }

  void print (ostream& os) {
    os<<"Node name : ArrayRef"<<endl;
    assert(var_ && dim_);
    var_->print(os);
    dim_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(var_ && dim_);
      //{new
      // in array
      flagArray = true;
      flagVarArray = true;
      //new}
      var_->pcodegen(os);
      //{new
      // initilization of array dim
      thisCounterDim = counterDim;
      counterDim = 0;
      flagVarArray = false;
      //new}
      dim_->pcodegen(os);
      //{new
      counterDim = thisCounterDim;
      // for ind
      if(flagVariable && flagArray && !flagNew && !flagRecord)
        os << "ind" << endl;
      // out of the array
      flagArray = false;
      //new}
  }
  virtual Object * clone () const { return new ArrayRef(*this);}

private:
  Object * var_;
  Object * dim_;
  int thisCounterDim;
};

// now for now
class RecordRef : public Var {
public :
  RecordRef (Object * varExt, Object * varIn) : varExt_(varExt),varIn_(varIn) {assert(varExt_ && varIn_);}
  RecordRef(const RecordRef& rec){
    varExt_ = rec.varExt_->clone();
    varIn_ = rec.varIn_->clone();
  }
  
  virtual ~RecordRef(){
    if (varExt_) delete varExt_;
    if (varIn_) delete varIn_;
  }

  void print (ostream& os) {
    os<<"Node name : RecordRef"<<endl;
    assert(varExt_ && varIn_);
    varExt_->print(os);
    varIn_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(varExt_ && varIn_);
      //{new
      // in record
      flagRecord = true;
      // for ldc
      bool flagThisLdcRecord = false;
      // for a situation in a record ref that we need the size of the pointed
      flagRecordSizeP = true;
      //new}
      // name of the record
      varExt_->pcodegen(os);
      //{new
      flagRecordSizeP = false;
      os << "inc ";
      // for ldc
      if(flagLdcRecord){
        flagLdcRecord = false;
        flagThisLdcRecord = true;
      }
      // for the inc of the record
      flagRecordRefInc = true;
      // form ind
      flagRecordInd = true;
      //new}
      // variable inside the record
      varIn_->pcodegen(os);
      //{new
      flagRecordInd = false;
      if(flagThisLdcRecord)
        flagLdcRecord = true;
      // for ind
      if(flagVariable && !flagArray && !flagNew && flagRecord)
        os << "ind" << endl;
      // out of the record
      flagRecord = false;
      //new}
  }
  virtual Object * clone () const { return new RecordRef(*this);}

private:
  Object * varExt_;
  Object * varIn_;
};

// now for now
class AddressRef : public Var {
public :
  AddressRef (Object * var) : var_(var) {assert(var_);}
  AddressRef(const AddressRef& addre){
    var_ = addre.var_->clone();
  }

  virtual ~AddressRef(){
    if (var_) delete var_;
  }

  void print (ostream& os) {
    os<<"Node name : AddressRef"<<endl;
    assert(var_);
    var_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(var_);
      var_->pcodegen(os);
      // {new
      os << "ind" << endl;
      // referenceName = nameVariable;
      typeVariable = "Pointer";
      // new}
  }
  virtual Object * clone () { return new AddressRef(*this);}

private:
  Object * var_;
};


class Statement : public Object {
};

// now for now
class NewStatement : public Statement {
public :
  NewStatement (Object * var) : var_(var) {assert(var_);}
  NewStatement(const NewStatement& ns){
    var_ = ns.var_->clone();
  }

  virtual ~NewStatement(){
    if (var_) delete var_;
  }

  void print (ostream& os) {
		os<<"Node name : NewStatement";
		assert(var_);
    var_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(var_);
      //{new
      // in new
      flagNew = true;
      // the variable (we know it is a pointer) 
      var_->pcodegen(os);
      // the size of the new variable
      os << "ldc " << sizeVariable << endl;
      // initializing the size
      sizeVariable = 0;
      // statement new
      os << "new" << endl;
      // out of new
      flagNew = false;
      //new}
  }
  virtual Object * clone () { return new NewStatement(*this);}
  
private:
  Object * var_;
};

// not for now
class WriteStrStatement : public Statement {
public :
  WriteStrStatement (const char * str) { 
		str_ = new string(str); 
	}
  
  WriteStrStatement(const WriteStrStatement& ns){
    str_ = new string(*ns.str_); 
  }

  virtual ~WriteStrStatement () {
		if (str_) delete str_;
	}
  void print (ostream& os) {
		os<<"Node name : WriteStrStatement";
		assert(str_);
    os<<"Str statement is: "<<str_<<endl;
  }
  void pcodegen(ostream& os) {
      assert(str_);
  }
  virtual Object * clone () { return new WriteStrStatement(*this);}
  
private:
  string * str_;
};

// for now, prints
class WriteVarStatement : public Statement {
public :
  WriteVarStatement (Object * exp) : exp_(exp) {assert(exp_);}

  WriteVarStatement(const WriteVarStatement& ex){
    exp_ = ex.clone();
  }

  virtual ~WriteVarStatement(){
    if (exp_) delete exp_;
  }

  void print (ostream& os) {
		os<<"Node name : WriteVarStatement";
		assert(exp_);
    exp_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(exp_);
      exp_->pcodegen(os);
      // {new
      os << "print" << endl;
      // new}
  }
  virtual Object * clone () const { return new WriteVarStatement(*this);}
  
private:
  Object * exp_;
};

// not for now
class ProcedureStatement : public Statement {
public :
  ProcedureStatement (const char * str) { 
		str_ = new string(str); 
	}

  ProcedureStatement (Object * expr_list, const char * str) : expr_list_(expr_list) {
    assert(expr_list_);
    str_ = new string(str);
  }

  ProcedureStatement(const ProcedureStatement& ps){
    expr_list_ = ps.expr_list_->clone();
    str_ = new string(*ps.str_);
  }

   virtual ~ProcedureStatement () {
		if (str_) delete str_;
    if (expr_list_) delete expr_list_;
	} 

  void print (ostream& os) {
    os<<"Node name : ProcedureStatement. Proc name : "<<str_<<endl;
    if (expr_list_ ){
      expr_list_->print(os);
    }
  }
  void pcodegen(ostream& os) {
      if (expr_list_) {
          expr_list_->pcodegen(os);
      }
  }
  virtual Object * clone () const { return new ProcedureStatement(*this);}
  
private:
	Object * expr_list_;
  string * str_;
};

// for now, connected to switch
class Case : public Object {
public :
  Case (Object * stat_list, int val) : leafChild_ (NULL), stat_list_(stat_list) {
		// note the special treatment in miny.y for this case (makenode)
		leafChild_ = new IntConst(val);
		assert(stat_list_);
	}

  Case(const Case& c){
    stat_list_ = c.stat_list_->clone();
    leafChild_ = c.leafChild_->clone();
  }

  virtual ~Case () {
    if (stat_list_) delete stat_list_;
    if (leafChild_) delete leafChild_;
	} 
  
  void print (ostream& os) {
		os<<"Node name : Case";
		assert(stat_list_);
	  stat_list_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(stat_list_);
      stat_list_->pcodegen(os);
  }
  virtual Object * clone () const { return new Case(*this);}

private:
  Object * stat_list_;
	Object * leafChild_;
};

// for now, connected to switch
class CaseList : public Object {
public :
  CaseList (Object * ccase) : case_(ccase),case_list_(NULL) {assert(case_);}
  CaseList (Object * ccase, Object * case_list) : case_(ccase),case_list_(case_list) {assert(case_ && case_list_);}

  CaseList(const CaseList& cl){
    case_ = cl.case_->clone();
    case_list_ = cl.case_list_->clone();
  }

  virtual ~CaseList () {
    if (case_) delete case_;
    if (case_list_) delete case_list_;
	} 

  void print (ostream& os) {
		os<<"Node name : CaseList";
		assert( case_ );
    case_->print(os);
    if (case_list_){
      case_list_->print(os);
    }
  }
  void pcodegen(ostream& os) {
      assert(case_);
      // {new
      int numCase = counterCase++;
      // the case
      os << "case_" << numCase << "_" << counterSwitchStack.top() << ":" << endl;
      // new}
      case_->pcodegen(os);
      // {new
      // the ujp
      os << "ujp end_switch_" << counterSwitchStack.top() << endl;
      // new}
      if (case_list_) {
          case_list_->pcodegen(os);
      }
      // {new
      os << "ujp case_" << numCase << "_" << counterSwitchStack.top() << endl;
      // new}
  }
  virtual Object * clone () const { return new CaseList(*this);}

private:
  Object * case_;
	Object * case_list_;
};

// for now, connected to switch
class CaseStatement : public Statement {
public :
  CaseStatement (Object * exp, Object * case_list) : exp_(exp),case_list_(case_list) {assert(exp_ && case_list_);}

  CaseStatement(const CaseStatement& cs){
    exp_ = cs.exp_->clone();
    case_list_ = cs.case_list_->clone();
  }

  virtual ~CaseStatement () {
    if (exp_) delete exp_;
    if (case_list_) delete case_list_;
	} 

  void print (ostream& os) {
		os<<"Node name : CaseStatement";
		assert( exp_ && case_list_);
    exp_->print(os);
		case_list_->print(os);
  }
  void pcodegen(ostream& os) {
    // {new
    int numLastCase = counterCase;
    counterCase = 1;
    counterSwitchStack.push(counterSwitch++);
    // new}
    assert(exp_ && case_list_);
    exp_->pcodegen(os);
    // {new
    // jump to the end and backwards: ixj
    os << "neg" << endl << "ixj end_switch_" << counterSwitchStack.top() << endl;
    // new}
    case_list_->pcodegen(os);
    // {new
    // end switch
    os << "end_switch_" << counterSwitchStack.top() << ":" << endl;
    // return to the previous switch
    counterSwitchStack.pop();
    counterCase = numLastCase;
    // new}
  }
  virtual Object * clone () const { return new CaseStatement(*this);}
  
private:
  Object * exp_;
	Object * case_list_;
};

// for while
class LoopStatement : public Statement {
public :
  LoopStatement (Object * exp, Object * stat_list) : exp_(exp),stat_list_(stat_list) {assert(exp_ && stat_list_);}

  LoopStatement(const LoopStatement& ls){
    exp_ = ls.exp_->clone();
    stat_list_ = ls.stat_list_->clone();
  }

  virtual ~LoopStatement () {
    if (exp_) delete exp_;
    if (stat_list_) delete stat_list_;
	} 

  void print (ostream& os) {
		os<<"Node name : LoopStatement";
		assert( exp_ && stat_list_);
    exp_->print(os);
		stat_list_->print(os);
  }
  void pcodegen(ostream& os) {
    int numWhile = counterWhile++;
    assert(exp_ && stat_list_);
    // {new
    // label
    os << "loop_" << numWhile << ": " << endl;
    // new} 
    exp_->pcodegen(os);
    // {new
    // fjp
    os << "fjp end_loop_" << numWhile << endl;
    // new}
    stat_list_->pcodegen(os);
    // {new
    // ujp
    os << "ujp loop_" << numWhile << endl;
    // label end
    os << "end_loop_" << numWhile << ": " << endl;
  }
  virtual Object * clone () const { return new LoopStatement(*this);}

private:
  Object * exp_;
	Object * stat_list_;
};

// for if & else
class ConditionalStatement : public Statement {
public :
  ConditionalStatement (Object * exp, Object * stat_list_if) : exp_(exp),stat_list_if_(stat_list_if), stat_list_else_(NULL) {assert(exp_ && stat_list_if_);}
  ConditionalStatement (Object * exp, Object * stat_list_if, Object * stat_list_else) : exp_(exp),stat_list_if_(stat_list_if), stat_list_else_(stat_list_else) {assert(exp_ && stat_list_if_ && stat_list_else_ );}

  ConditionalStatement(const ConditionalStatement& cs){
    exp_ = cs.exp_->clone();
    stat_list_if_ = cs.stat_list_if_->clone();
    stat_list_else_ = cs.stat_list_else_->clone();
  }

  virtual ~ConditionalStatement () {
    if (exp_) delete exp_;
    if (stat_list_if_) delete stat_list_if_;
    if (stat_list_else_) delete stat_list_else_;
	} 

  void print (ostream& os) {
		os<<"Node name : ConditionalStatement";
		assert( exp_ && stat_list_if_);
    exp_->print(os);
		stat_list_if_->print(os);
		if (stat_list_else_){
			stat_list_else_->print(os);
		}
  }
  void pcodegen(ostream& os) {
    // {new
    int numIf = counterIf++;
    // new}
    assert(exp_ && stat_list_if_);
    exp_->pcodegen(os);
    // {new
    os << "fjp ";
    if(stat_list_else_)
      os << "else_if_" << numIf << endl;
    else
      os << "end_if_" << numIf << endl;
    // new}
    stat_list_if_->pcodegen(os);
    if (stat_list_else_) {
      // {new
      os << "ujp end_if_" << numIf << endl;
      os << "else_if_" << numIf << ":" << endl;
      // new}
      stat_list_else_->pcodegen(os);
    }
    // {new
    os << "end_if_" << numIf << ":" << endl;
    // new}
  }
  virtual Object * clone () const { return new ConditionalStatement(*this);}
  
private:
  Object * exp_;
	Object * stat_list_if_;
	Object * stat_list_else_;
};

// for assigntment
class Assign : public Statement {
public :
  Assign (Object * var, Object * exp) : var_(var), exp_(exp) {assert(var_ && exp_);}
  
  Assign(const Assign& as){
    var_ = as.var_->clone();
    exp_ = as.exp_->clone();
  }

  virtual ~Assign () {
    if (exp_) delete exp_;
    if (var_) delete var_;
	} 

  void print (ostream& os) {
		os<<"Node name : Assign";
		assert(var_ && exp_);
    exp_->print(os);
    var_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(var_ && exp_);
      // {new
      flagVariable = false;
      // new}
      exp_->pcodegen(os);
      // {new
      flagVariable = true;
      // new}
      var_->pcodegen(os);
      // {new
      os << "sto" << endl;
      // new}
  }
  virtual Object * clone () const { return new Assign(*this);}

private:
  Object * var_;
  Object * exp_;
};

// for now, need to understand
class StatementList : public Object {
public :
  StatementList (Object * stat) : stat_list_(NULL), stat_(stat)  { assert(stat_);}
  StatementList (Object * stat_list, Object * stat) : stat_list_(stat_list),stat_(stat)  {assert(stat_list_ && stat);}

  StatementList (const StatementList& sl){
    stat_ = sl.stat_->clone();
    stat_list_ = sl.stat_list_->clone();
  }

  virtual ~StatementList () {
    if (stat_) delete stat_;
    if (stat_list_) delete stat_list_;
	} 

  void print (ostream& os) {
	  os<<"Node name : StatementList"<<endl;
	  if (stat_list_){
		  stat_list_->print(os);
	  }
	  assert(stat_);
	  stat_->print(os);
  }
  void pcodegen(ostream& os) {
      if (stat_list_) {
          stat_list_->pcodegen(os);
      }
      assert(stat_);
      stat_->pcodegen(os);
  }
  virtual Object * clone () const { return new StatementList(*this);}
  
private:
  Object * stat_;
  Object * stat_list_;
};

// the list of the variables for decleration inside the record
class RecordList : public Object {
public :
  RecordList (Object * var_decl) : record_list_(NULL), var_decl_(var_decl)  { assert(var_decl_);}
  RecordList (Object * record_list, Object * var_decl) : record_list_(record_list),var_decl_(var_decl)  {assert(record_list_ && var_decl);}
  
  RecordList(const RecordList& li){
    var_decl_= li.var_decl_->clone();
    record_list_ = li.record_list_->clone();
  }

  virtual ~RecordList () {
    if (var_decl_) delete var_decl_;
    if (record_list_) delete record_list_;
	} 

  void print (ostream& os) {
	  os<<"Node name : RecordList"<<endl;
	  if (record_list_){
		  record_list_->print(os);
	  }
	  assert(var_decl_);
	  var_decl_->print(os);
  }
  void pcodegen(ostream& os) {
      if (record_list_) {
          record_list_->pcodegen(os);
      }
      assert(var_decl_);
      var_decl_->pcodegen(os);
  }
  virtual Object * clone () const { return new RecordList(*this);}
  
private:
  Object * var_decl_;
  Object * record_list_;
};

class Type : public Object {
};

// declaration for simple variables
class SimpleType : public Type {
public:
  SimpleType (const char * name) { 
		name_ = new string(name); 
	}

  virtual ~SimpleType () {
		if (name_ )delete name_;
	}

  SimpleType(const SimpleType& s){
    name_ = new string(*s.name_);
  }

  void print (ostream& os) {
		os<<"Node name : SimpleType"<<endl;;
		os<<"Type is : "<< (*name_) <<endl;
	}
  void pcodegen(ostream& os) {
    // {new
    typeVariable = *name_;
    // if in array then its the type of array
    typearray = *name_;
    sizeArray = 1;
    sizeVariable = 1;
    // os << typearray << endl;
    // new}
  }
  virtual Object * clone () const { return new SimpleType(*this);}

private:
  string * name_;
};

// for every time we go to a variable
class IdeType : public Type {
public:
  IdeType (const char * name) { 
    name_ = new string(name); 
  }

  virtual ~IdeType () {
    if (name_) delete name_;
  }

  IdeType(const IdeType& s){
    name_ = new string(*s.name_);
  }

  void print (ostream& os) {
    os<<"Node name : IdeType"<<endl;
  }
  void pcodegen(ostream& os) {
    // {new
    // the name of the current array
    if(flagVarArray)
      arrayName = *name_;
    // the size of the variable (for new statement)
    if(!flagDecleration){
      // for the inc of the record
      if(flagRecordRefInc){
        flagRecordRefInc = false;
        os << symboltable.findAddress(*name_) - incRecord << endl;
      }
      // i think for new
      if(symboltable.getType(*name_) == "Pointer")
        for(int i=0; i<addressRefCounter; i++)
        sizeVariable = symboltable.getSize(symboltable.getPointed(*name_));
      // if we need the value from the variable
      if(flagLdcRecord)
        os << "ldc " << symboltable.findAddress(*name_) << endl;
      if(flagVariable && !flagRecordInd && !flagNew && !flagRecord && !flagArray)
        os << "ind" << endl;
      // for inc and dec for Exp
      flagAddConst = false;
      flagSubConst = false;
      // for inc of the record
      if(flagRecordSizeP && symboltable.getType(*name_) == "Pointer")
        incRecord = symboltable.findAddress(symboltable.getPointed(*name_));
      else if(flagRecordSizeP && symboltable.getType(*name_) == "Array")
        incRecord = symboltable.findAddress(symboltable.getTypeArray(*name_));
      else if(flagRecordSizeP && symboltable.getType(*name_) != "Integer" && symboltable.getType(*name_) != "Real" && symboltable.getType(*name_) != "Boolean" && symboltable.getType(*name_) != "Pointer" && symboltable.getType(*name_) != "Array" && symboltable.getType(*name_) != "Record")
        incRecord = symboltable.findAddress(symboltable.getType(*name_));
      else
        incRecord = symboltable.findAddress(*name_);
    }
    else{
      nameVariable = *name_;
      sizeVariable = symboltable.getSize(*name_);
      // if in array then its the type of array
      typearray = *name_;
      sizeArray = 1;
      typeVariable = *name_;
      // os << typearray << endl;
    }
    // new}
  }
  virtual Object * clone () const { return new IdeType(*this);}

private:
  string * name_;
};

// now for now
class ArrayType : public Type {
public :
  ArrayType (int l,int u, Object * type) : low_(l),up_(u),type_(type)  { assert(type_);}
  
  ArrayType(const ArrayType& a) : low_(a.low_), up_(a.up_){
    type_ = a.type_->clone();
  }

  virtual ~ArrayType () {
    if (type_) delete type_;
  }

  void print (ostream& os) {
		os<<"Node name : ArrayType: low bound is: "<<low_<<", up bound is: "<<up_<<endl;
	  assert(type_);
	  type_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(type_);
      //{new
      subpart = 0;
      //new}
      type_->pcodegen(os);
      //{new
      // from the nost inner to out
      sizeArray *= (up_ - low_ + 1);
      subpart += low_ * sizeVariable * arrayProduct(arrsizes);
      arrsizes.push_back(up_ - low_ + 1);
      // os << subpart << endl;
      typeVariable = "Array";
      // if this is an array of a pointer
      flagPointerArray = true;
      //new}
  }
  virtual Object * clone () const { return new ArrayType(*this);}

private:
  Object * type_;
	int low_,up_;
};

// now for now
class RecordType : public Type {
public :
  RecordType (Object * record_list) : record_list_(record_list)  { assert(record_list_);}

  RecordType(const RecordType& y){
    record_list_ = y.record_list_->clone();
  }

  virtual ~RecordType () {
    if (record_list_) delete record_list_;
  }
  
  void print (ostream& os) {
		os<<"Node name : RecordType"<<endl;
	  assert(record_list_);
	  record_list_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(record_list_);
      //{new
      // new record so new cell
      sizeOfRecord.push(0);
      //new}
      record_list_->pcodegen(os);
      //{new
      // name of the variable
      typeVariable = "Record";
      //new}
  }
  virtual Object * clone () const { return new RecordType(*this);}

private:
  Object * record_list_;
};

// now for now
class AddressType : public Type {
public :
  AddressType (Object * type) : type_(type)  { assert(type_);}

  AddressType(const AddressType& t){
    type_ = t.type_->clone();
  }

  virtual ~AddressType () {
    if (type_) delete type_;
  }
  
  void print (ostream& os) {
		os<<"Node name : AddressType"<<endl;
	  assert(type_);
	  type_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(type_);
      type_->pcodegen(os);
      //{new
      typeVariable = "Pointer";
      //new}
  }
  virtual Object * clone () const { return new AddressType(*this);}

private:
  Object * type_;
};


class Declaration : public Object {
};

//
class VariableDeclaration : public Declaration {
public:
  VariableDeclaration (Object * type, const char * str) : type_(type){
    assert(type_);
    name_ = new string(str);
  }

  VariableDeclaration(const VariableDeclaration& p){
    type_ = p.type_->clone();
    name_ = new string(*p.name_);
  }

  virtual ~VariableDeclaration () {
    if (type_) delete type_;
		delete name_;
	}

  void print (ostream& os) {
    os<<"Node name : VariableDeclaration. Var name is: "<<*name_<<endl;
		assert(type_);
	  type_->print(os);
  }
  void pcodegen(ostream& os) {
    //{new
    flagDecleration = true;
    int poped = 0;
    // for the address of the record
    int addressRecord = symboltable.getCurrAddress();
    //new}
    assert(type_);
    type_->pcodegen(os);
    // {new
    // for simple type
    if(typeVariable == "Integer" || typeVariable == "Real" || typeVariable  == "Boolean"){
      symboltable.insertVariable(*name_, typeVariable);
      // for size of record
      if(!sizeOfRecord.empty()){
        poped = ++sizeOfRecord.top();
        sizeOfRecord.emplace(poped);
      }
    }
    // for record type
    else if(typeVariable == "Record"){
      symboltable.insertRecord(*name_, addressRecord, typeVariable, sizeOfRecord.top());
      // handle if there is multiple records
      if(sizeOfRecord.size() > 1){
        poped = sizeOfRecord.top();
        sizeOfRecord.pop();
        poped += sizeOfRecord.top();
        sizeOfRecord.emplace(poped);
      }
      else
        sizeOfRecord.pop();
    }
    // for pointer type
    else if(typeVariable == "Pointer"){
      if(flagPointerArray){
        sizeArray *= sizeVariable;
        symboltable.insertArrayOfPointer(*name_ + "-array", "Array", sizeArray, subpart, typearray);
        symboltable.insertPointer(*name_, typeVariable, *name_ + "-array");
      }
      else
        symboltable.insertPointer(*name_, typeVariable, nameVariable);
      if(!sizeOfRecord.empty()){
        poped = ++sizeOfRecord.top();
        sizeOfRecord.emplace(poped);
      }
    }
    // for array type
    else if(typeVariable == "Array"){
      sizeArray *= sizeVariable;
      symboltable.insertArray(*name_, typeVariable, sizeArray, subpart, typearray);
      if(!sizeOfRecord.empty()){
        poped = sizeOfRecord.top() + symboltable.getSize(*name_);
        sizeOfRecord.emplace(poped);
      }
      arrsizes.clear();
    }
    else{
      symboltable.insertElse(*name_, typeVariable, symboltable.getSize(typeVariable));
      if(!sizeOfRecord.empty()){
        poped = sizeOfRecord.top() + symboltable.getSize(*name_);
        sizeOfRecord.emplace(poped);
      }
    }
    // returning the name of the variable
    // flag of the decleration
    flagDecleration = false;
    // we enter new decleration, we need to initlize this pointer
    flagPointerArray = false;
    // new}
  }
  virtual Object * clone () const { return new VariableDeclaration(*this);}

private:
  Object * type_;
  string * name_;
};

// not for now
class Parameter : public Object {
public :
  Parameter (Object * type, const char * name) : type_(type){
    assert(type_);
    name_ = new string(name);
  }
  
  Parameter(const Parameter& p){
    type_ = p.type_->clone();
    name_ = new string(*p.name_);
  }

  virtual ~Parameter () {
    if (type_) delete type_;
		delete name_;
	}
  
  void print (ostream& os) {
		printWayOfPassing(os);
    os<<"Parameter name :" <<name_<<endl;
		assert(type_);
	  type_->print(os);
  }
  void pcodegen(ostream& os) {
      printWayOfPassing(os);
      assert(type_);
      type_->pcodegen(os);
  }
protected:
	virtual void printWayOfPassing (ostream& os) const = 0;
  
private:
  Object * type_;
  string * name_;
};

// not for now
class ByReferenceParameter : public Parameter {
public :
	ByReferenceParameter (Object * type, const char * name) : Parameter (type,name) {}
  virtual Object * clone () const { return new ByReferenceParameter(*this);}
protected:
  void printWayOfPassing (ostream& os) const{
	  os<<"Node name : ByReferenceParameter ";
	}
};

// not for now
class ByValueParameter : public Parameter {
public :
	ByValueParameter (Object * type, const char * name) : Parameter(type,name) {}
  virtual Object * clone () const { return new ByValueParameter(*this);}
protected:
  void printWayOfPassing (ostream& os) const{
	  os<<"Node name : ByValueParameter ";
	}
};

// not for now
class ParameterList : public Object {
public :
  ParameterList (Object * formal) : formal_(formal),  formal_list_(NULL) { assert(formal_);}
  ParameterList (Object * formal, Object * formal_list) : formal_(formal), formal_list_(formal_list) {assert(formal_ && formal_list_);}
  
  ParameterList(const ParameterList& pl){
    formal_ = pl.formal_->clone();
    formal_list_ = pl.formal_list_->clone();
  }
  
  virtual ~ParameterList () {
    if (formal_) delete formal_;
    if (formal_list_) delete formal_list_;
	}

  void print (ostream& os) {
	  os<<"Node name : ParameterList"<<endl;
	  if (formal_list_){
		  formal_list_->print(os);
	  }
	  assert(formal_);
	  formal_->print(os);
  }
  void pcodegen(ostream& os) {
      if (formal_list_) {
          formal_list_->pcodegen(os);
      }
      assert(formal_);
      formal_->pcodegen(os);
  }
  virtual Object * clone () const { return new ParameterList(*this);}
  
private:
  Object * formal_;
  Object * formal_list_;
};

// not for now
class FunctionDeclaration : public Declaration {
public :
  FunctionDeclaration (Object * type, Object * block, const char * name) : type_(type), block_(block), formal_list_(NULL) {
    assert(type_ && block_);
    name_ = new string(name);
  }

  FunctionDeclaration (Object * type, Object * formal_list, Object * block,  const char * name) : type_(type), formal_list_(formal_list),block_(block) {
    assert(type_ && formal_list_ && block_ );
    name_ = new string(name);
  }

  virtual ~FunctionDeclaration () {
    if (type_) delete type_;
    if (block_) delete block_;
    if (formal_list_) delete formal_list_;
    if (name_) delete name_;
  }

  FunctionDeclaration(const FunctionDeclaration& fd){
    type_ = fd.type_->clone();
    block_ = fd.block_->clone();
    formal_list_ = fd.formal_list_->clone();
    name_ = new string(*fd.name_);
  }

  void print (ostream& os) {
    os<<"Node name : FunctionDeclaration. Func name is: "<<*name_<<endl;
	  assert(type_ && block_);
	  type_->print(os);
	  if (formal_list_){
		  formal_list_->print(os);
	  }
	  block_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(type_ && block_);
      type_->pcodegen(os);
      if (formal_list_) {
          formal_list_->pcodegen(os);
      }
      block_->pcodegen(os);
  }
  virtual Object * clone () const { return new FunctionDeclaration(*this);}
  
private:
  Object * type_;
  Object * block_;
  Object * formal_list_;
  string * name_;
};

// not for now
class ProcedureDeclaration : public Declaration {
public :
  ProcedureDeclaration (Object * block, const char * name) : formal_list_(NULL), block_(block) { 
    assert(block_);
    name_ = new string(name);
  }

  ProcedureDeclaration (Object * formal_list, Object * block, const char * name) : formal_list_(formal_list),block_(block)  {
    assert(formal_list_ && block_);
    name_ = new string(name);
  }

  virtual ~ProcedureDeclaration () {
    if (block_) delete block_;
    if (formal_list_) delete formal_list_;
    if (name_) delete name_;
  }

  ProcedureDeclaration(const ProcedureDeclaration& pd){
    block_ = pd.block_->clone();
    formal_list_ = pd.formal_list_->clone();
    name_ = new string(*pd.name_);
  }

  void print (ostream& os) {
	  os<<"Node name : ProcedureDeclaration. Proc name is: "<<*name_<<endl;
	  assert(block_);
	  if (formal_list_){
		  formal_list_->print(os);
	  }
	  block_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(block_);
      if (formal_list_) {
          formal_list_->pcodegen(os);
      }
      block_->pcodegen(os);
  }
  virtual Object * clone () const { return new ProcedureDeclaration(*this);}
  
private:
  Object * block_;
  Object * formal_list_;
  string * name_;
};

// not for now
class DeclarationList : public Object {
public :
  DeclarationList (Object * decl) : decl_(decl), decl_list_(NULL) { assert(decl_);}
  DeclarationList (Object * decl_list, Object * decl) : decl_list_(decl_list),decl_(decl)  {assert(decl_list_ && decl);}

  DeclarationList(const DeclarationList& dl){
    decl_ = dl.decl_->clone();
    decl_list_ = dl.decl_list_->clone();
  }

  virtual ~DeclarationList () {
    if (decl_) delete decl_;
    if (decl_list_) delete decl_list_;
  }
  
  void print (ostream& os) {
	  os<<"Node name : DeclarationList"<<endl;
	  if (decl_list_){
		  decl_list_->print(os);
	  }
	  assert(decl_);
	  decl_->print(os);
  }
  void pcodegen(ostream& os) {
      if (decl_list_) {
          decl_list_->pcodegen(os);
      }
      assert(decl_);
      decl_->pcodegen(os);
  }
  virtual Object * clone () const { return new DeclarationList(*this);}

private:
  Object * decl_;
  Object * decl_list_;
};

// for now, need to understand
class Block : public Object {
public :
  Block (Object * stat_seq) : stat_seq_(stat_seq), decl_list_(NULL)  { assert(stat_seq_);}
  Block (Object * decl_list, Object * stat_seq) : decl_list_(decl_list), stat_seq_(stat_seq)  {assert(decl_list_ && stat_seq_);}

  Block(const Block& b){
    decl_list_ = b.decl_list_->clone();
    stat_seq_ = b.stat_seq_->clone();
  }

  virtual ~Block () {
    if (stat_seq_) delete stat_seq_;
    if (decl_list_) delete decl_list_;
  }
  
  void print (ostream& os) {
	  os<<"Node name : Begin"<<endl;
	  if (decl_list_){
		  decl_list_->print(os);
	  }
	  assert(stat_seq_);
	  stat_seq_->print(os);
  }
  void pcodegen(ostream& os) {
      if (decl_list_) {
          decl_list_->pcodegen(os);
      }
      assert(stat_seq_);
      stat_seq_->pcodegen(os);
  }

  virtual Object * clone () const { return new Block(*this);}
  
private:
  Object * decl_list_;
  Object * stat_seq_;
};

// for now, need to understand
class Program : public Object {
public :
  Program (Object * block, const char * str) : block_(NULL) {
    block_ = dynamic_cast<Block *>(block);
    assert(block_);
    name_ = new string(str);
  }

  Program(const Program& prog){
    block_ = dynamic_cast<Block *>(prog.block_->clone());
    assert(block_);
    name_ = new string(*prog.name_);
  }

  virtual ~Program () {
    if (block_) delete block_;
    delete name_;
  }
  
  void print (ostream& os) {
    os<<"Node name : Root/Program. Program name is: "<<*name_<<endl;
	  assert(block_);
    block_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(block_);
      block_->pcodegen(os);
      // {new
      flagVariable = true;
      flagDecleration = true;
      flagRecordRefInc = false;
      flagLdcRecord = true;
      // new}
  }
  virtual Object * clone () const { return new Program(*this);}
  
private:
  Block * block_;
  string * name_;
};



#endif //AST_H

