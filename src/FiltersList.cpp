#include "FiltersList.h"
#include "ChainLoader.h"
#include "tools/Logger.h"

namespace QDLIB {

   FiltersList::FiltersList() : _size(0), _initalized(false),
                                 _fname(DEFAULT_EXPEC_FILENAME), _writefile(false)
   {
      for (int i=0; i < MAX_FILTERS; i++){
	 _olist[i] = NULL;
      }
      _clock = QDGlobalClock::Instance();
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
	    DELETE_OP(_olist[i]);
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
    * \li normalize  Simple normalization of wave function
    */
   void FiltersList::Init( XmlNode *section )
   {
      Logger& log = Logger::InstanceRef();
      
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
      string value;
      
      while (filters->EndNode()){
         if (_size >= MAX_FILTERS)
            throw EParamProblem ("Maximum numbers of filters reached.");
            
	 faction = filters->Name();
	 ParamContainer& attr = filters->Attributes();
	 
	 /* Check which kind of value we want to see */
	 if (attr.isPresent("value")){
	    attr.GetValue("value", value);
	    if (value == "real")
	       _value[_size] = real;
	    else if (value == "imag")
	       _value[_size] = imag;
	    else if (value == "complex")
	       _value[_size] = complex;
	    else
	       throw EParamProblem ("Value type not known: ", value);
	    
	 } else
	    _value[_size] = real;
	 
         /* Check which kind of value we want to see */
         
         attr.GetValue("int", _integrate[_size], false);
         _sum[_size] = dcomplex(0);

	 /* Check which filter action should be taken */
	 if (faction == "normalize"){
	    _action[_size] = normalize;
	 }
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
	    string errormsg("Unknown filter action provided: ");
	    errormsg += faction;
	    throw EParamProblem (errormsg.c_str());
	 }
	 if (faction != "normalize"){
	     _olist[_size] = ChainLoader::LoadOperatorChain( filters );  /* Load operator */
             ParamContainer attr = filters->Attributes();     /* Set label */
             if (attr.isPresent("label"))
                attr.GetValue("label", _labels[_size]);
              else 
                _labels[_size] = _olist[_size]->Name();
	 }
	 _size++;
	 filters->NextNode();
      }
      
      if (_writefile){
	 log.cout() << "Filter expectation value file: " << _fname << endl; log.flush();
         _ofile.open(_fname.c_str());
         _ofile.precision(10);
      }
   }
   
   /**
    * Apply the filter list and print expection values
    */
   void FiltersList::Apply( WaveFunction *Psi )
   {
      if (_size == 0) return;
      
      /* Check for initializations of the operators */
      if (! _initalized){
         _ofile << "time \t";
	 for(int i=0; i < _size; i++){
	    _olist[i]->Clock(_clock);
	    GlobalOpList::Instance().Init(_olist[i], Psi);
	    if (_writefile && (_action[i] == expec || _action[i] == expeconly))
	       _ofile << _labels[i] << "\t";
	 }
	 if (_writefile)
	    _ofile << endl;
      }
      	 
      /* Write time */
      if (_writefile)
         _ofile << _clock->Time() << "\t";
      
      /* Apply filters */
      for(int i=0; i < _size; i++){
	 if (_action[i] == expec || _action[i] == expeconly){
	    switch (_value[i]) {
	       case imag:
                  if (_integrate[i])
                     _sum[i] += _olist[i]->MatrixElement(Psi,Psi).imag() * _clock->Dt();
                  else
                     _sum[i] = _olist[i]->MatrixElement(Psi,Psi).imag();
                  
		  _ofile << _sum[i].imag() << "\t";
		  break;
	       case complex:
                  if (_integrate[i])
                     _sum[i] += _olist[i]->MatrixElement(Psi,Psi) * _clock->Dt();
                  else
                     _sum[i] = _olist[i]->MatrixElement(Psi,Psi);
                  
                  _ofile << _sum[i] << "\t";
		  break;
	       default:
                  if (_integrate[i])
                     _sum[i] += _olist[i]->Expec(Psi) * _clock->Dt();
                  else
                     _sum[i] = _olist[i]->Expec(Psi);
                  
                  _ofile << _sum[i].real() << "\t";
	    }
	 }	 
	 if (_action[i] == apply || _action[i] == expec){
	    _olist[i]->Apply(Psi);
	 }
	 if (_action[i] == normalize)
	    Psi->Normalize();
      }
      
      if (_writefile) _ofile << endl;
      if (! _initalized) _initalized = true;
   }
   
}

