#include "RooDataHist.h"
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "TH1.h"
#include "TSystem.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPaveText.h"

/// include pdfs
#include "RooCBExGaussShape.h"
#include "RooCMSShape.h"

#include <vector>
#include <string>
#ifdef __CINT__
#pragma link C++ class std::vector<std::string>+;
#endif

using namespace RooFit;
using namespace std;

class tnpFitter {
public:
  tnpFitter( TFile *file, std::string histname  );
  tnpFitter( TH1 *hPass, TH1 *hFail, std::string histname  );
  ~tnpFitter(void) {if( _work != 0 ) delete _work; }
  void setZLineShapes(TH1 *hZPass, TH1 *hZFail );
  void setWorkspace(std::vector<std::string>);
  void setWorkspaceJPsi(std::vector<std::string>);
  void setOutputFile(TFile *fOut ) {_fOut = fOut;}
  void fits(bool mcTruth,std::string title = "");
  // void useMinos(bool minos = true) {_useMinos = minos;}
  void useMinos(bool minos = false) {_useMinos = minos;}     // chiara
  void textParForCanvas(RooFitResult *resP, RooFitResult *resF, TPad *p);
  
  void fixSigmaFtoSigmaP(bool fix=true) { _fixSigmaFtoSigmaP= fix;}

  void setFitRange(double xMin,double xMax) { _xFitMin = xMin; _xFitMax = xMax; }
private:
  RooWorkspace *_work;
  std::string _histname_base;
  TFile *_fOut;
  double _nTotP, _nTotF;
  bool _useMinos;
  bool _fixSigmaFtoSigmaP;
  double _xFitMin,_xFitMax;
};

tnpFitter::tnpFitter(TFile *filein, std::string histname   ) : _useMinos(false),_fixSigmaFtoSigmaP(false) {
  RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
  _histname_base = histname;  

  TH1 *hPass = (TH1*) filein->Get(TString::Format("%s_Pass",histname.c_str()).Data());
  TH1 *hFail = (TH1*) filein->Get(TString::Format("%s_Fail",histname.c_str()).Data());
  _nTotP = hPass->Integral();
  _nTotF = hFail->Integral();
  for( int ib = 0; ib <= hPass->GetXaxis()->GetNbins()+1; ib++ )
    // chiara, for JPsi
    //if(  hPass->GetXaxis()->GetBinCenter(ib) <= 60 || hPass->GetXaxis()->GetBinCenter(ib) >= 120 ) {
    if(  hPass->GetXaxis()->GetBinCenter(ib) <= 2.9 || hPass->GetXaxis()->GetBinCenter(ib) >= 3.3 ) {
    //if(  hPass->GetXaxis()->GetBinCenter(ib) <= 2.6 || hPass->GetXaxis()->GetBinCenter(ib) >= 3.5 ) {
     hPass->SetBinContent(ib,0);
     hFail->SetBinContent(ib,0);
   }
  
  _work = new RooWorkspace("w") ;
  // chiara, for JPsi 
  // _work->factory("x[50,130]");
  // _work->factory("x[2.6,3.5]");
  _work->factory("x[2.9,3.3]");

  RooDataHist rooPass("hPass","hPass",*_work->var("x"),hPass);
  RooDataHist rooFail("hFail","hFail",*_work->var("x"),hFail);
  _work->import(rooPass) ;
  _work->import(rooFail) ;
  // chiara, for JPsi 
  //_xFitMin = 60;
  //_xFitMax = 120;
  _xFitMin = 2.9;
  _xFitMax = 3.3;
  //_xFitMin = 2.6;
  //_xFitMax = 3.5;
}

tnpFitter::tnpFitter(TH1 *hPass, TH1 *hFail, std::string histname  ) : _useMinos(false),_fixSigmaFtoSigmaP(false) {
  RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
  _histname_base = histname;
  
  _nTotP = hPass->Integral();
  _nTotF = hFail->Integral();

  for( int ib = 0; ib <= hPass->GetXaxis()->GetNbins()+1; ib++ )
    // chiara, for JPsi 
    //if(  hPass->GetXaxis()->GetBinCenter(ib) <= 60 || hPass->GetXaxis()->GetBinCenter(ib) >= 120 ) {
    //if(  hPass->GetXaxis()->GetBinCenter(ib) <= 2.6 || hPass->GetXaxis()->GetBinCenter(ib) >= 3.5 ) {
    if(  hPass->GetXaxis()->GetBinCenter(ib) <= 2.9 || hPass->GetXaxis()->GetBinCenter(ib) >= 3.3 ) {
      hPass->SetBinContent(ib,0);
      hFail->SetBinContent(ib,0);
    }
  
  _work = new RooWorkspace("w") ;
  // chiara, for JPsi  
  // _work->factory("x[50,130]");
  //_work->factory("x[2.6,3.5]");
  _work->factory("x[2.9,3.3]");
  
  RooDataHist rooPass("hPass","hPass",*_work->var("x"),hPass);
  RooDataHist rooFail("hFail","hFail",*_work->var("x"),hFail);
  _work->import(rooPass) ;
  _work->import(rooFail) ;
  // chiara, for JPsi   
  //_xFitMin = 60;
  //_xFitMax = 120;
  //_xFitMin = 2.6;
  //_xFitMax = 3.5;
  _xFitMin = 2.9;
  _xFitMax = 3.3;
}


void tnpFitter::setZLineShapes(TH1 *hZPass, TH1 *hZFail ) {
  RooDataHist rooPass("hGenZPass","hGenZPass",*_work->var("x"),hZPass);
  RooDataHist rooFail("hGenZFail","hGenZFail",*_work->var("x"),hZFail);
  _work->import(rooPass) ;
  _work->import(rooFail) ;  
}

void tnpFitter::setWorkspace(std::vector<std::string> workspace) {
  for( unsigned icom = 0 ; icom < workspace.size(); ++icom ) {
    _work->factory(workspace[icom].c_str());
  }

  _work->factory("HistPdf::sigPhysPass(x,hGenZPass)");
  _work->factory("HistPdf::sigPhysFail(x,hGenZFail)");
  _work->factory("FCONV::sigPass(x, sigPhysPass , sigResPass)");
  _work->factory("FCONV::sigFail(x, sigPhysFail , sigResFail)");
  _work->factory(TString::Format("nSigP[%f,0.5,%f]",_nTotP*0.9,_nTotP*1.5));
  _work->factory(TString::Format("nBkgP[%f,0.5,%f]",_nTotP*0.1,_nTotP*1.5));
  _work->factory(TString::Format("nSigF[%f,0.5,%f]",_nTotF*0.9,_nTotF*1.5));
  _work->factory(TString::Format("nBkgF[%f,0.5,%f]",_nTotF*0.1,_nTotF*1.5));
  _work->factory("SUM::pdfPass(nSigP*sigPass,nBkgP*bkgPass)");
  _work->factory("SUM::pdfFail(nSigF*sigFail,nBkgF*bkgFail)");
  _work->Print();			         
}

void tnpFitter::setWorkspaceJPsi(std::vector<std::string> workspace) {
  for( unsigned icom = 0 ; icom < workspace.size(); ++icom ) {
    _work->factory(workspace[icom].c_str());
  }
  
  // _work->factory(TString::Format("nSigP[%f,1e-8,%f]",TMath::Max(_nTotP*0.9, 2e-8), TMath::Max(_nTotP*1.5, 1.))); //min was 0.5
  // _work->factory(TString::Format("nBkgP[%f,1e-8,%f]",TMath::Max(_nTotP*0.1, 2e-8), TMath::Max(_nTotP*1.5, 1.))); //min was 0.5
  // _work->factory(TString::Format("nSigF[%f,1e-8,%f]",TMath::Max(_nTotF*0.9, 2e-8), TMath::Max(_nTotF*1.5, 1.))); //min was 0.1
  // //_work->factory(TString::Format("nSigF[%f,0.5,%f]",_nTotF*0.9,_nTotF*1.5));
  // _work->factory(TString::Format("nBkgF[%f,1e-8,%f]",TMath::Max(_nTotF*0.1, 2e-8), TMath::Max(_nTotF*1.5, 1.))); //min was 0.5

  _work->factory(TString::Format("nSigP[%f,0.5,%f]",_nTotP*0.9,_nTotP*2));//*1.5 before (max)         
  _work->factory(TString::Format("nBkgP[%f,0.01,%f]",_nTotP*0.1,_nTotP*2));                           // nBkgP = 0.5 before       
  _work->factory(TString::Format("nSigF[%f,0.01,%f]",_nTotF*0.9,_nTotF*2));//min was 0.1, 
  _work->factory(TString::Format("nBkgF[%f,0.01,%f]",_nTotF*0.1,_nTotF*2));//min was 0.5

  _work->factory("SUM::pdfPass(nSigP*sigResPass,nBkgP*bkgPass)");
  _work->factory("SUM::pdfFail(nSigF*sigResFail,nBkgF*bkgFail)");
  _work->Print();

}


void tnpFitter::fits(bool mcTruth,string title) {

  RooAbsPdf *pdfPass = _work->pdf("pdfPass");
  RooAbsPdf *pdfFail = _work->pdf("pdfFail");

  if( mcTruth ) {
    _work->var("nBkgP")->setVal(0); _work->var("nBkgP")->setConstant();
    _work->var("nBkgF")->setVal(0); _work->var("nBkgF")->setConstant();
    if( _work->var("sosP")   ) { 
      _work->var("sosP")->setVal(0);
      _work->var("sosP")->setConstant(); }
    if( _work->var("sosF")   ) { 
      _work->var("sosF")->setVal(0);
      _work->var("sosF")->setConstant(); }
    if( _work->var("acmsP")  ) _work->var("acmsP")->setConstant();
    if( _work->var("acmsF")  ) _work->var("acmsF")->setConstant();
    if( _work->var("betaP")  ) _work->var("betaP")->setConstant();
    if( _work->var("betaF")  ) _work->var("betaF")->setConstant();
    if( _work->var("gammaP") ) _work->var("gammaP")->setConstant();
    if( _work->var("gammaF") ) _work->var("gammaF")->setConstant();
  }

  _work->var("x")->setRange("fitMassRange",_xFitMin,_xFitMax);
  RooFitResult* resPass = pdfPass->fitTo(*_work->data("hPass"),Minos(_useMinos),SumW2Error(kTRUE),Save(),Range("fitMassRange"));

  if( _fixSigmaFtoSigmaP ) {
    if (_work->var("sigmaF")) _work->var("sigmaF")->setVal( _work->var("sigmaP")->getVal() );
    if (_work->var("sigmaF")) _work->var("sigmaF")->setConstant();
  }

  // chiara
  // if (_work->var("sigmaF")) {
  //   _work->var("sigmaF")->setVal( _work->var("sigmaP")->getVal() );
  //   _work->var("sigmaF")->setConstant();
  // }
  // if (_work->var("meanF")) {
  //   _work->var("meanF")->setVal( _work->var("meanP")->getVal() );
  //   _work->var("meanF")->setConstant();
  // }
  // if (_work->var("alphaLF")) {
  //   _work->var("alphaLF")->setVal( _work->var("alphaLP")->getVal() );
  //   _work->var("alphaLF")->setConstant();
  // }
  // if (_work->var("nLF")) {
  //   _work->var("nLF")->setVal( _work->var("nLP")->getVal() );
  //   _work->var("nLF")->setConstant();
  // }
  // if (_work->var("alphaRF")) {
  //   _work->var("alphaRF")->setVal( _work->var("alphaRP")->getVal() );
  //   _work->var("alphaRF")->setConstant();
  // }
  // if (_work->var("nRF")) {
  //   _work->var("nRF")->setVal( _work->var("nRP")->getVal() );
  //   _work->var("nRF")->setConstant();
  // }
  // chiara

  // if (_work->var("sigmaF")) _work->var("sigmaF")->setRange(0.8* _work->var("sigmaP")->getVal(), 3.0* _work->var("sigmaP")->getVal());
  RooFitResult* resFail = pdfFail->fitTo(*_work->data("hFail"),Minos(_useMinos),SumW2Error(kTRUE),Save(),Range("fitMassRange"));

    // chiara, for JPsi
  //RooPlot *pPass = _work->var("x")->frame(60,120);
  //RooPlot *pFail = _work->var("x")->frame(60,120);
  //RooPlot *pPass = _work->var("x")->frame(2.6,3.5);
  //RooPlot *pFail = _work->var("x")->frame(2.6,3.5);
  RooPlot *pPass = _work->var("x")->frame(2.9,3.3);
  RooPlot *pFail = _work->var("x")->frame(2.9,3.3);
  pPass->SetTitle("passing probe");
  pFail->SetTitle("failing probe");
  
  _work->data("hPass") ->plotOn( pPass );
  _work->pdf("pdfPass")->plotOn( pPass, LineColor(kRed) );
  _work->pdf("pdfPass")->plotOn( pPass, Components("bkgPass"),LineColor(kBlue),LineStyle(kDashed));
  _work->data("hPass") ->plotOn( pPass );
  
  _work->data("hFail") ->plotOn( pFail );
  _work->pdf("pdfFail")->plotOn( pFail, LineColor(kRed) );
  _work->pdf("pdfFail")->plotOn( pFail, Components("bkgFail"),LineColor(kBlue),LineStyle(kDashed));
  _work->data("hFail") ->plotOn( pFail );

  cout << "chiara, jpsi2"<< endl;

  TCanvas c("c","c",1100,450);
  c.Divide(3,1);
  TPad *padText = (TPad*)c.GetPad(1);
  textParForCanvas( resPass,resFail, padText );
  c.cd(2); pPass->Draw();
  c.cd(3); pFail->Draw();

  _fOut->cd();
  c.Write(TString::Format("%s_Canv",_histname_base.c_str()),TObject::kOverwrite);
  resPass->Write(TString::Format("%s_resP",_histname_base.c_str()),TObject::kOverwrite);
  resFail->Write(TString::Format("%s_resF",_histname_base.c_str()),TObject::kOverwrite);

  
}





/////// Stupid parameter dumper /////////
void tnpFitter::textParForCanvas(RooFitResult *resP, RooFitResult *resF,TPad *p) {

  double eff = -1;
  double e_eff = 0;

  RooRealVar *nSigP = _work->var("nSigP");
  RooRealVar *nSigF = _work->var("nSigF");
  
  double nP   = nSigP->getVal();
  double e_nP = nSigP->getError();
  double nF   = nSigF->getVal();
  double e_nF = nSigF->getError();
  double nTot = nP+nF;
  eff = nP / (nP+nF);
  e_eff = 1./(nTot*nTot) * sqrt( nP*nP* e_nF*e_nF + nF*nF * e_nP*e_nP );

  TPaveText *text1 = new TPaveText(0,0.8,1,1);
  text1->SetFillColor(0);
  text1->SetBorderSize(0);
  text1->SetTextAlign(12);

  text1->AddText(TString::Format("* fit status pass: %d, fail : %d",resP->status(),resF->status()));
  text1->AddText(TString::Format("* eff = %1.4f #pm %1.4f",eff,e_eff));

  //  text->SetTextSize(0.06);

//  text->AddText("* Passing parameters");
  TPaveText *text = new TPaveText(0,0,1,0.8);
  text->SetFillColor(0);
  text->SetBorderSize(0);
  text->SetTextAlign(12);
  text->AddText("    --- parameters " );
  RooArgList listParFinalP = resP->floatParsFinal();
  for( int ip = 0; ip < listParFinalP.getSize(); ip++ ) {
    TString vName = listParFinalP[ip].GetName();
    text->AddText(TString::Format("   - %s \t= %1.3f #pm %1.3f",
				  vName.Data(),
				  _work->var(vName)->getVal(),
				  _work->var(vName)->getError() ) );
  }


//  text->AddText("* Failing parameters");
  RooArgList listParFinalF = resF->floatParsFinal();
  for( int ip = 0; ip < listParFinalF.getSize(); ip++ ) {
    TString vName = listParFinalF[ip].GetName();
    text->AddText(TString::Format("   - %s \t= %1.3f #pm %1.3f",
				  vName.Data(),
				  _work->var(vName)->getVal(),
				  _work->var(vName)->getError() ) );
  }

  p->cd();
  text1->Draw();
  text->Draw();

}
