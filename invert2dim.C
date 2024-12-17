#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <iostream>

using namespace std;

void invert2dim() {

  gStyle->SetOptStat(0);

  // Files
  TFile myFile("results/vs_pt_deltaR/DoubleMu/egammaEffi.txt_EGM2D.root");

  // Taking histos
  TH2F *EGamma_EffData2D = (TH2F*)myFile.Get("EGamma_EffData2D");      
  TH2F *EGamma_EffMC2D   = (TH2F*)myFile.Get("EGamma_EffMC2D");      
  TH2F *EGamma_SF2D      = (TH2F*)myFile.Get("EGamma_SF2D");      

  int nBinY  = EGamma_EffData2D->GetNbinsY();      // EGM: this is pT
  int nBinXf = EGamma_EffData2D->GetNbinsX();      // EGM: this is dR
  int nBinX  = nBinXf/2.;                          // EGM makes this symmetric
  cout << "nBinY = " << nBinY << ", nBinXf = " << nBinXf << ", nBinX = " << nBinX << endl;

  // Reorganizing the binning
  TH1D *EGamma_EffData2D_projX = EGamma_EffData2D->ProjectionX("EGamma_EffData2D_projX");
  TH1D *EGamma_EffData2D_projY = EGamma_EffData2D->ProjectionY("EGamma_EffData2D_projY");
  TH1D *EGamma_EffMC2D_projX   = EGamma_EffMC2D->ProjectionX("EGamma_EffMC2D_projX");
  TH1D *EGamma_EffMC2D_projY   = EGamma_EffMC2D->ProjectionY("EGamma_EffMC2D_projY");
  TH1D *EGamma_SF2D_projX      = EGamma_SF2D->ProjectionX("EGamma_SF2D_projX");
  TH1D *EGamma_SF2D_projY      = EGamma_SF2D->ProjectionY("EGamma_SF2D_projY");

  float binYLowEdgeNew  = EGamma_EffData2D_projX->GetBinLowEdge(nBinX+1);
  float binYHighEdgeNew = EGamma_EffData2D_projX->GetBinLowEdge(nBinXf+1);
  int nBinXNew = nBinY;
  int nBinYNew = nBinX;

  Double_t xbinningNew[nBinXNew+1];
  for ( int i=0; i<nBinXNew; i++){ 
    xbinningNew[i] = EGamma_EffData2D_projY->GetBinLowEdge(i+1);
  }
  xbinningNew[nBinXNew] = EGamma_EffData2D_projY->GetBinLowEdge(nBinXNew+1);

  cout << "binYLowEdgeNew = " << binYLowEdgeNew << ", binYHighEdgeNew = " << binYHighEdgeNew << endl; 
  cout << "nBinXNew = " << nBinXNew << ", nBinYNew = " << nBinYNew << endl;

  TH2F *EGamma_EffData2DNew = new TH2F("EGamma_EffData2DNew", "EGamma_EffData2DNew", nBinXNew, xbinningNew, nBinYNew, binYLowEdgeNew, binYHighEdgeNew);
  TH2F *EGamma_EffMC2DNew   = new TH2F("EGamma_EffMC2DNew",   "EGamma_EffMC2DNew",   nBinXNew, xbinningNew, nBinYNew, binYLowEdgeNew, binYHighEdgeNew);
  TH2F *EGamma_SF2DNew      = new TH2F("EGamma_SF2DNew",      "EGamma_SF2DNew",      nBinXNew, xbinningNew, nBinYNew, binYLowEdgeNew, binYHighEdgeNew);
  for (int iX = 1; iX<=nBinXNew; iX++) {
    for (int iY = 1; iY<=nBinYNew; iY++) {
      int oldBinX = nBinX+iY;
      int oldBinY = iX;
      EGamma_EffData2DNew -> SetBinContent(iX, iY,  EGamma_EffData2D->GetBinContent(oldBinX, oldBinY));
      EGamma_EffMC2DNew   -> SetBinContent(iX, iY,  EGamma_EffMC2D->GetBinContent(oldBinX, oldBinY));
      EGamma_SF2DNew      -> SetBinContent(iX, iY,  EGamma_SF2D->GetBinContent(oldBinX, oldBinY));
      // cout << "Fill: " << iX << " " << iY << " (" << oldBinX << ", " << oldBinY << ") => " << EGamma_EffData2D->GetBinContent(oldBinX, oldBinY) << endl; 
    }}


  EGamma_EffData2DNew -> SetTitle("Data Efficiency");
  EGamma_EffData2DNew -> GetXaxis()->SetTitle("p_{T} [GeV]");
  EGamma_EffData2DNew -> GetYaxis()->SetTitle("#Delta R");

  EGamma_EffMC2DNew -> SetTitle("MC Efficiency");
  EGamma_EffMC2DNew -> GetXaxis()->SetTitle("p_{T} [GeV]");
  EGamma_EffMC2DNew -> GetYaxis()->SetTitle("#Delta R");

  EGamma_SF2DNew -> SetTitle("Efficiency Data/MC SF");
  EGamma_SF2DNew -> GetXaxis()->SetTitle("p_{T} [GeV]");
  EGamma_SF2DNew -> GetYaxis()->SetTitle("#Delta R");


  // Cosmetics
  Int_t nb= 256;
  Double_t val_whiteEff = 0.90;
  Double_t val_whiteSF  = 0.95;
  const Int_t Number = 3;
  Double_t Red[Number]   = { 0., 1., 1.};
  Double_t Green[Number] = { 0., 1., 0.};
  Double_t Blue[Number]  = { 1., 1., 0.};
  //
  Double_t maxEff = EGamma_EffData2DNew->GetMaximum();
  if (EGamma_EffMC2DNew->GetMaximum()>maxEff) maxEff = EGamma_EffMC2DNew->GetMaximum();
  Double_t minEff = EGamma_EffData2DNew->GetMinimum();
  if (EGamma_EffMC2DNew->GetMinimum()<minEff) minEff = EGamma_EffMC2DNew->GetMinimum();
  Double_t per_whiteEff = (val_whiteEff-minEff)/(maxEff-minEff);
  Double_t StopsEff[Number] = { 0., per_whiteEff, 1. };
  //
  Double_t maxSF = EGamma_SF2DNew->GetMaximum();
  Double_t minSF = EGamma_SF2DNew->GetMinimum();
  Double_t per_whiteSF = (val_whiteSF-minSF)/(maxSF-minSF);
  Double_t StopsSF[Number] = { 0., per_whiteSF, 1. };

  // Plots
  TColor::CreateGradientColorTable(Number,StopsEff,Red,Green,Blue,nb);
  //
  TCanvas c0("c0","",800, 600);
  EGamma_EffData2DNew->SetContour(nb);
  EGamma_EffData2DNew->Draw("colztext");
  c0.SaveAs("data.png");
  //
  TCanvas c1("c1","",800, 600);
  EGamma_EffMC2DNew->SetContour(nb);
  EGamma_EffMC2DNew->Draw("colztext");
  c1.SaveAs("mc.png");


  TColor::CreateGradientColorTable(Number,StopsSF,Red,Green,Blue,nb);
  TCanvas c2("c2","",800, 600);
  EGamma_SF2DNew->SetContour(nb);
  EGamma_SF2DNew->Draw("colztext");
  c2.SaveAs("sf.png");
}
