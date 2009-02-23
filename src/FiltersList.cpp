#include "FiltersList.h"
#include "ChainLoader.h"

namespace QDLIB {

   FiltersList::FiltersList() : _size(0), _initalized(false),  _writefile(false),
			    _fname(DEFAULT_EXPEC_FILENAME)
   {
      for (int i=0; i < MAX_FILTERS; i++){
	 _olist[i] = NULL;
      }
   }
   
   
   FiltersList::~FiltersList()
   {
      _destroy();
   }
   
   /**
    * Remove all operators.
    */
   void FiltersList::_destroy()
   {
      for (int i=0; i < MAX_FILTERS; i++){
	 if (_olist[i] != NULL) {
	    delete _olist[i];
	    _olist[i] = NULL;
	 }
      }
      _size = 0;
      _initalized = false;
      _writefile = false;
      _ofile.close();
   }
   
   /**
    * Set the default filename for expectation value table output.
    */
   void FiltersList::SetDefaultName(string &s)
   {
      _fname = s;
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
      
      if (_size > 0) _destroy();
      
      /* Check for output file name */
      params = section->Attributes();
      if (params.isPresent("fname")){
	 params.GetValue("fname", _fname);
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
	    _writefile = true;
	 }
	 else if (faction == "apply")
	    _action[_size] = apply;
	 else if (faction == "expeconly"){
	    _action[_size] = expeconly;
	    _writefile = true;
	 } else {
	    string error("Unknown filter action provided: ");
	    error += faction;
	    throw EParamProblem (error.c_str());
	 }
	 _olist[_size] = ChainLoader::LoadOperatorChain( filters );
	 _size++;
	 filters->NextNode();
      }
      
      if (_writefile){
	 cout << _fname << "<- logfile\n";
         _ofile.open(_fname.c_str());
      }
   }
   
   /**
    * Apply the filter list and print expection values
    */
   void FiltersList::Apply( WaveFunction *Psi )
   {
      if (_size == 0) return;
      
      if (! _initalized){
	 for(int i=0; i < _size; i++){
	    _olist[i]->Init(Psi);
	    if (_writefile)
	       _ofile << _olist[i]->Name() << "\t";
	 }
	 if (_writefile)
	    _ofile << endl;
      }
      	 
      for(int i=0; i < _size; i++){
	 if (_action[i] == expec || _action[i] == expeconly){
	    _ofile << _olist[i]->Expec(Psi) << "\t";
	 }	 
	 if (_action[i] == apply || _action[i] == expec){
	    _olist[i]->Apply(Psi);
	 }
      }
      
      if (_writefile) _ofile << endl;
      if (! _initalized) _initalized = true;
   }
   
}

