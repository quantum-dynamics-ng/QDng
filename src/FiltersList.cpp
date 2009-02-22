#include "FiltersList.h"
#include "ChainLoader.h"

namespace QDLIB {

   FiltersList::FiltersList() : _size(0), _initalized(false),  _writefile(false)
   {
      for (int i=0; i < MAX_FILTERS; i++){
	 _olist[i] = NULL;
      }
   }
   
   
   FiltersList::~FiltersList()
   {
      for (int i=0; i < MAX_FILTERS; i++){
	 if (_olist[i] != NULL) delete _olist[i];
      }
   }
   
   
   /**
    * Initialize the filter list
    * 
    * \param section XmlNode which which contains the filter definitions
    * 
    * Rules for the node names are:
    * \li expec      Apply the operator and show the expectation value
    * \li apply      Just Apply the operator
    * \li expeconly  Only calculate and show the expectation value
    */
   void FiltersList::Init( XmlNode *section )
   {
      ParamContainer params;
      XmlNode *filters;
      
      if (section == NULL) return;
      
      /* Check for output file name */
      params = section->Attributes();
      if (params.isPresent("fname")){
	 params.GetValue("fname", _fname);
	 _writefile = true;
      }
      
      /* Load the filter list*/
      filters = section->NextChild();
      if (filters == NULL)
	 throw EParamProblem ("Empty filter list provided");
      
      /* Load the operator list */
      string faction;
      
      while (filters->EndNode()){
	 cout << filters->Name();
	 faction = filters->Name();
	 if (faction == "expec"){
	    _action[_size] = expec;
	    cout << "expec chosn\n";
	 }
	 else if (faction == "apply")
	    _action[_size] = apply;
	 else if (faction == "expeconly")
	    _action[_size] = expeconly;
	 else {
	    string error("Unknown filter action provided: ");
	    error += faction;
	    throw EParamProblem (error.c_str());
	 }
	 _olist[_size] = ChainLoader::LoadOperatorChain( filters );
	 _size++;
	 filters->NextNode();
      }
   }
   
   /**
    * Apply the filter list and print expecation values
    */
   void FiltersList::Apply( WaveFunction *Psi )
   {
      bool print = false;
      
      for(int i=0; i < _size; i++){
	 if (! _initalized){
	    _olist[i]->Init(Psi);
	 }
	 if (_action[i] == expec || _action[i] == expeconly){
	    print = true;
	    cout << _olist[i]->Expec(Psi) << "\t";
	 }	 
	 if (_action[i] == apply || _action[i] == expec){
	    _olist[i]->Apply(Psi);
	 }
      }
      if (print) cout << endl;
      if (! _initalized) _initalized = true;
   }
   
}

