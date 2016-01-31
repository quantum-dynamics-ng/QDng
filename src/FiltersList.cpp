#include "FiltersList.h"
#include "ChainLoader.h"
#include "tools/Logger.h"

namespace QDLIB {

   FiltersList::FiltersList() : _renorm(false), _size(0), _initalized(false),
                                 _fname(DEFAULT_EXPEC_FILENAME), _writefile(false)
   {
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
//	 if (_olist[i] != NULL) {
//	    DELETE_OP(_olist[i]);
//	    _olist[i] = NULL;
//	 }
      }
      _size = 0;
      _initalized = false;
      _writefile = false;
      _ofile.close();
   }
   
   /**
    * Set the default filename for expectation value table output.
    */
   void FiltersList::SetDefaultName(const string &s)
   {
      _fname = s;
   }
   
   /**
    * Add a filter to chain.
    *
    * \param O       The Operator to use
    * \param header  Coloumn header to use. Defaults to the operator name (if string is empty)
    * \param faction One of apply, expec, expeconly, normalize. [expeconly]
    * \param val     What part of the expectation should be plotted: real, imag, complex, abs [real]
    * \param int     Integrate/sum up over time [false]
    */
   void FiltersList::Add(const Operator* O, const string& header, const string& faction, const string& val, const vector<int>& states, bool integrate)
   {
      string action, value;

      /* Set defaults */
      if (faction.length() == 0)
         action = "expeconly";
      else
         action = faction;

      if (val.length() == 0)
         value = "expeconly";
      else
         value = val;

      /* Check if full */
      if (_size >= MAX_FILTERS)
         throw EParamProblem ("Maximum numbers of filters reached.");

      /* Handle type of value */
      if (value == "real")
         _filter[_size].value = real;
      else if (value == "imag")
         _filter[_size].value = imag;
      else if (value == "complex")
         _filter[_size].value = complex;
      else if (value == "abs")
         _filter[_size].value = abs;
      else
         throw EParamProblem ("Value type not known: ", value);

      /* Accumulate/Integrate values ?*/
      _filter[_size].integrate = integrate;
      _filter[_size].sum = dcomplex(0);

      /* Check which filter action should be taken */
      if (action == "normalize")
      {
         _filter[_size].action = normalize;
      } else if (action == "expec")
      {
         _filter[_size].action = expec;
         _writefile = true;
      }
      else if (action == "apply")
         _filter[_size].action = apply;
      else if (action == "expeconly")
      {
         _filter[_size].action = expeconly;
         _writefile = true;
      }
      else if (action == "rho")
      {
         _filter[_size].action = rho;
         _writefile = true;
      }
      else
      {
         string errormsg("Unknown filter action provided: ");
         errormsg += action;
         throw EParamProblem(errormsg.c_str());
      }

      if (states.size() == 2){
	  _filter[_size].ms[0] = states[0];
	  _filter[_size].ms[1] = states[1];
      } else if (states.size() == 1 || states.size() > 2) {
	  throw(EParamProblem("Invalid state defintion. states needs exactly 2 values."));
      }
      _size++;
   }

   /**
    * If Output is redirected to file it is openend here.
    * If Operators are pushed via Add then this must be used before the filter is applied.
    *
    */
   void FiltersList::PrepareOutput()
   {
      Logger& log = Logger::InstanceRef();

      if (_writefile)
      {
         log.cout() << "Filter expectation value file: " << _fname << endl;
         log.flush();
         _ofile.open(_fname.c_str());
         _ofile.precision(10);
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
    * \li normalize  Simple normalization of wave function
    *
    * Options:
    * \li renorm     Renormalize expectation values before printing.
    */
   void FiltersList::Init(XmlNode *section)
   {

      ParamContainer params;
      XmlNode *filters;

      if (section == NULL)
         return;

      if (_size > 0)
         _destroy();

      /* Check for output file name */
      params = section->Attributes();
      if (params.isPresent("fname"))
      {
         params.GetValue("fname", _fname);
      }

      /* Check if expectation values should be renormalized */
      if (params.isPresent("renorm"))
         params.GetValue("renorm", _renorm, false);

      /* Load the filter list*/
      filters = section->NextChild();
      if (filters == NULL)
         throw EParamProblem("Empty filter list provided");

      /* Load the operator list */
      while (filters->EndNode())
      {
         string faction("expeconly"), value("real"), header;
         bool integrate = false;

         if (_size >= MAX_FILTERS)
            throw EParamProblem("Maximum numbers of filters reached.");

         faction = filters->Name();
         ParamContainer& attr = filters->Attributes();

         /* Check which kind of value we want to see */
         if (attr.isPresent("value"))
            attr.GetValue("value", value);

         /* Check if value should be integrated over time */
         if (attr.isPresent("int"))
            attr.GetValue("int", integrate);

         vector<int> states;
         if (attr.isPresent("ms"))
            attr.GetArray("ms", states);

         if (attr.isPresent("header"))
            attr.GetValue("header", _filter[_size].label);

         Operator* O=NULL;
         if (faction != "normalize" && faction != "rho")
         {
            _filter[_size].op = ChainLoader::LoadOperatorChain(filters); /* Load operator */
            ParamContainer attr = filters->Attributes(); /* Set header label */
            if (!attr.isPresent("header")) _filter[_size].label = _filter[_size].op->Name();
         } else if (faction == "rho"){
             if (!attr.isPresent("header")) _filter[_size].label = "rho";
         }

         Add(O, header, faction, value, states, integrate);

         filters->NextNode();
      }

      PrepareOutput();
   }
   
   /*
    * Write value to file for filter i, consdering the choice of value type.
    */
   void FiltersList::_write_value(int i)
   {
     switch (_filter[i].value) {
         case imag:
            _ofile << _filter[i].sum.imag() << "\t";
            break;
         case complex:
            _ofile << _filter[i].sum << "\t";
            break;
         case abs:
            _ofile << cabs(_filter[i].sum) << "\t";
            break;
         default:
            _ofile << _filter[i].sum.real() << "\t";
            break;
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
	    if (_filter[i].action != normalize && _filter[i].action != rho){
	       _filter[i].op->Clock(_clock);
	       GlobalOpList::Instance().Init(_filter[i].op, Psi);
	    }
	    if (_writefile && (_filter[i].action == expec || _filter[i].action == expeconly ||  _filter[i].action == rho) && _filter[i].action != normalize)
	       _ofile << _filter[i].label << "\t";

	    /* validity of "states" defintion */
	    if (_filter[i].ms[0] > -1 || _filter[i].ms[1] > -1){
		WFMultistate* psi  = dynamic_cast<WFMultistate*>(Psi);
		if ( psi == NULL )
		    throw(EIncompatible("Wavefunction not a multistate type, but 'states' directive in filter defined: ", Psi->Name()));

		if (psi->States() <= lint(_filter[i].ms[0]) || psi->States() <= lint(_filter[i].ms[1]))
		    throw(EParamProblem("Invalid state number in 'states' directive in filter defined."));
	    }
	 }
	 if (_writefile)
	    _ofile << endl;
      }
      	 
      /* Write time */
      if (_writefile)
         _ofile << _clock->Time() << "\t";
      
      /* Apply filters */
      for(int i=0; i < _size; i++){
	 switch (_filter[i].action ){
	   case expec:
	   case expeconly:
	   {
	    dcomplex matel = _filter[i].op->MatrixElement(Psi,Psi);

	    /* Renormalize expectation value */
	    if (_renorm)
	       matel /= *Psi * Psi;

	    if (_filter[i].integrate)
	       _filter[i].sum += matel * _clock->Dt();
	    else
	       _filter[i].sum = matel;

	    _write_value(i);

	    if (_filter[i].action == expec)
	      _filter[i].op->Apply(Psi);
	  }
	   break;
	   case apply:
	     {
	       _filter[i].op->Apply(Psi);
	     }
	     break;

	   case normalize:
	     Psi->Normalize();
	    break;
	   case rho:
	     {
	       WFMultistate* psi = dynamic_cast<WFMultistate*>(Psi);
	       dcomplex rho =  *(psi->State(_filter[i].ms[0])) * psi->State(_filter[i].ms[1]);
		if (_filter[i].integrate)
		   _filter[i].sum += rho * _clock->Dt();
		else
		   _filter[i].sum = rho;

		_write_value(i);
	     }
	     break;
	 }
      }
      
      if (_writefile) _ofile << endl;
      if (! _initalized) _initalized = true;
   }
   
}

