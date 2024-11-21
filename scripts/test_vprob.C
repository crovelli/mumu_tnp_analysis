void BinLogX(TH1* h){
   TAxis *axis = h->GetXaxis();
   int bins = axis->GetNbins();

   Axis_t from = axis->GetXmin();
   Axis_t to = axis->GetXmax();
   Axis_t width = (to - from) / bins;
   Axis_t *new_bins = new Axis_t[bins + 1];

   for (int i = 0; i <= bins; i++) {
     new_bins[i] = TMath::Power(10, from + (i+1) * width);
   }

   axis->Set(bins, new_bins);
}

// ------------------------------------------------------------

void test_vprob(){	
	TChain* t = new TChain("nano_/tree");
	t->AddFile("/eos/cms/store/user/crovelli/HLT_DoubleMuv2/MuonDataset_2022Ev1__DoubleMuTriggerTnp.root");
	t->AddFile("/eos/cms/store/user/crovelli/HLT_DoubleMuv2/MuonDataset_2022Fv1__DoubleMuTriggerTnp___partial.root");
	t->AddFile("/eos/cms/store/user/crovelli/HLT_DoubleMuv2/MuonDataset_2022Gv1__DoubleMuTriggerTnp.root");

  TCanvas* c1 = new TCanvas("c1", "c1", 1800, 900);
  c1->Divide(3,2);

  // GOOD BIN
  c1->cd(1);

	TH1F *h1_passed = new TH1F("h1_passed","h1_passed", 30, -4, 0);
  TH1F *h1_failed = new TH1F("h1_failed","h1_failed", 30, -4, 0);
	BinLogX(h1_passed);
  BinLogX(h1_failed);
	
  gROOT->SetBatch(kTRUE);
  t->Draw("Jpsi_fit_vprob >> h1_failed", "Jpsi_muonsDr > 0.48 && Jpsi_muonsDr < 0.80 && Jpsi_m2_eta > -0.75 && Jpsi_m2_eta < 0 && DoubleMu_fired == 0 && Jpsi_fit_mass < 3.5 && Jpsi_fit_mass > 2.6", "plc");
  t->Draw("Jpsi_fit_vprob >> h1_passed", "Jpsi_muonsDr > 0.48 && Jpsi_muonsDr < 0.80 && Jpsi_m2_eta > -0.75 && Jpsi_m2_eta < 0 && DoubleMu_fired == 1 && Jpsi_fit_mass < 3.5 && Jpsi_fit_mass > 2.6", "plc sames");
  gROOT->SetBatch(kFALSE);

  gPad->SetLogx();
  h1_passed->DrawCopy("PLC");
  h1_failed->DrawCopy("PLC SAMES");
  gPad->BuildLegend();

  // BAD BIN #1
  c1->cd(2);

	TH1F *h2_passed = new TH1F("h2_passed","h2_passed", 30, -4, 0);
  TH1F *h2_failed = new TH1F("h2_failed","h2_failed", 30, -4, 0);
	BinLogX(h2_passed);
  BinLogX(h2_failed);

  gROOT->SetBatch(kTRUE);
  t->Draw("Jpsi_fit_vprob >> h2_failed", "Jpsi_muonsDr > 0 && Jpsi_muonsDr < 0.16 && Jpsi_m2_eta > -2.05 && Jpsi_m2_eta < -1.5 && DoubleMu_fired == 0 && Jpsi_fit_mass < 3.5 && Jpsi_fit_mass > 2.6", "plc");
  t->Draw("Jpsi_fit_vprob >> h2_passed", "Jpsi_muonsDr > 0 && Jpsi_muonsDr < 0.16 && Jpsi_m2_eta > -2.05 && Jpsi_m2_eta < -1.5 && DoubleMu_fired == 1 && Jpsi_fit_mass < 3.5 && Jpsi_fit_mass > 2.6", "plc sames");
  gROOT->SetBatch(kFALSE);

  gPad->SetLogx();
  h2_passed->DrawCopy("PLC");
  h2_failed->DrawCopy("PLC SAMES");
  gPad->BuildLegend();

  // BAD BIN #2
  c1->cd(3);

	TH1F *h3_passed = new TH1F("h3_passed","h3_passed", 30, -4, 0);
  TH1F *h3_failed = new TH1F("h3_failed","h3_failed", 30, -4, 0);
	BinLogX(h3_passed);
  BinLogX(h3_failed);

  gROOT->SetBatch(kTRUE);
  t->Draw("Jpsi_fit_vprob >> h3_failed", "Jpsi_muonsDr < 0.80 && Jpsi_muonsDr > 0.48 && Jpsi_m2_pt > 5 && Jpsi_m2_pt < 7.00 && DoubleMu_fired == 0 && Jpsi_fit_mass < 3.5 && Jpsi_fit_mass > 2.6", "plc");
  t->Draw("Jpsi_fit_vprob >> h3_passed", "Jpsi_muonsDr < 0.80 && Jpsi_muonsDr > 0.48 && Jpsi_m2_pt > 5 && Jpsi_m2_pt < 7.00 && DoubleMu_fired == 1 && Jpsi_fit_mass < 3.5 && Jpsi_fit_mass > 2.6", "plc sames");
  gROOT->SetBatch(kFALSE);

  gPad->SetLogx();
  h3_passed->DrawCopy("PLC");
  h3_failed->DrawCopy("PLC SAMES");
  gPad->BuildLegend();

  // BAD BIN #3
  c1->cd(4);

	TH1F *h4_passed = new TH1F("h4_passed","h4_passed", 30, -4, 0);
  TH1F *h4_failed = new TH1F("h4_failed","h4_failed", 30, -4, 0);
	BinLogX(h4_passed);
  BinLogX(h4_failed);

  gROOT->SetBatch(kTRUE);
  t->Draw("Jpsi_fit_vprob >> h4_failed", "Jpsi_muonsDr < 0.32 && Jpsi_muonsDr > 0.16 && Jpsi_m2_pt > 3.4 && Jpsi_m2_pt < 5.00 && DoubleMu_fired == 0 && Jpsi_fit_mass < 3.5 && Jpsi_fit_mass > 2.6", "plc");
  t->Draw("Jpsi_fit_vprob >> h4_passed", "Jpsi_muonsDr < 0.32 && Jpsi_muonsDr > 0.16 && Jpsi_m2_pt > 3.4 && Jpsi_m2_pt < 5.00 && DoubleMu_fired == 1 && Jpsi_fit_mass < 3.5 && Jpsi_fit_mass > 2.6", "plc sames");
  gROOT->SetBatch(kFALSE);

  gPad->SetLogx();
  h4_passed->DrawCopy("PLC");
  h4_failed->DrawCopy("PLC SAMES");
  gPad->BuildLegend();


}
