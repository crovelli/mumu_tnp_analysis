#ifndef ROO_DOUBLE_CB
#define ROO_DOUBLE_CB

#include "RooAbsPdf.h"
#include "RooAbsArg.h"
#include "RooRealProxy.h"
#include "RooRealVar.h"
#include "RooCategoryProxy.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include "TMath.h"
#include "Riostream.h"

class RooRealVar;

class RooDoubleCB : public RooAbsPdf {
 public:
  RooDoubleCB() { }
  RooDoubleCB(const char *name, const char *title, RooAbsReal& _m, RooAbsReal& _m0,
              RooAbsReal& _sigma, RooAbsReal& _alphaL, RooAbsReal& _alphaR, RooAbsReal& _nL, RooAbsReal& _nR);

  RooDoubleCB(const RooDoubleCB& other, const char* name = 0);
  virtual TObject* clone(const char* newname) const { return new RooDoubleCB(*this,newname); }

  inline virtual ~RooDoubleCB() { }

 protected:

  RooRealProxy m;
  RooRealProxy m0;
  RooRealProxy sigma;
  RooRealProxy alphaL;
  RooRealProxy alphaR;
  RooRealProxy nL;
  RooRealProxy nR;

  Double_t evaluate() const;

 private:

  ClassDef(RooDoubleCB,1) // Double Crystal Ball lineshape PDF
    };

#endif
