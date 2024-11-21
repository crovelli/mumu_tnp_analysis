//Command:
//root /eos/cms/store/user/crovelli/HLT_DoubleMuv2/MuonDataset_2022Gv1__DoubleMuTriggerTnp.root/nano_
void test_kinematics(){

  gROOT->SetBatch(kTRUE);
  gStyle->SetOptStat(0);

  TChain* tree = new TChain("nano_/tree");
  tree->Add("/eos/cms/store/user/crovelli/HLT_DoubleMuv2/MuonDataset_2022Dv2__DoubleMuTriggerTnp.root");
  tree->Add("/eos/cms/store/user/crovelli/HLT_DoubleMuv2/MuonDataset_2022Ev1__DoubleMuTriggerTnp.root");
  tree->Add("/eos/cms/store/user/crovelli/HLT_DoubleMuv2/MuonDataset_2022Fv1__DoubleMuTriggerTnp___partial.root");
  tree->Add("/eos/cms/store/user/crovelli/HLT_DoubleMuv2/MuonDataset_2022Gv1__DoubleMuTriggerTnp.root");

  TFile* fmc = TFile::Open("/eos/cms/store/user/crovelli/DoubleMuMCwithTag_v2/TnP__BsToJPsiPhi__Run3Summer22EE___withTagV2.root");
  TTree* treemc = (TTree*)fmc->Get("nano_/tree");
  
  TString path = "/eos/home-n/npalmeri/www/TnP/";


  // ---- MU1 P_T ----

  TCanvas* c1 = new TCanvas("c1", "c1", 1600, 800);
  c1->Divide(2,1);
  c1->cd(1);

  // DATA
  tree->Draw("Jpsi_m1_pt >> h_Jpsi_m1_pt_data_1", "Jpsi_muonsDr > 0.4 && Jpsi_muonsDr < 0.6", "PLC");
  tree->Draw("Jpsi_m1_pt >> h_Jpsi_m1_pt_data_2", "Jpsi_muonsDr > 0.2 && Jpsi_muonsDr < 0.4", "PLC SAME");
  tree->Draw("Jpsi_m1_pt >> h_Jpsi_m1_pt_data_3", "Jpsi_muonsDr > 0.6 && Jpsi_muonsDr < 0.8", "PLC SAME");
  tree->Draw("Jpsi_m1_pt >> h_Jpsi_m1_pt_data_4", "Jpsi_muonsDr < 0.2", "PLC SAME");

  // retrieve histograms and add to legend
  TH1F* h_Jpsi_m1_pt_data_1 = (TH1F*)gDirectory->Get("h_Jpsi_m1_pt_data_1");
  TH1F* h_Jpsi_m1_pt_data_2 = (TH1F*)gDirectory->Get("h_Jpsi_m1_pt_data_2");
  TH1F* h_Jpsi_m1_pt_data_3 = (TH1F*)gDirectory->Get("h_Jpsi_m1_pt_data_3");
  TH1F* h_Jpsi_m1_pt_data_4 = (TH1F*)gDirectory->Get("h_Jpsi_m1_pt_data_4");

  h_Jpsi_m1_pt_data_1->SetTitle("[DATA] Leading #mu p_{T}");
  h_Jpsi_m1_pt_data_1->GetXaxis()->SetTitle("p_{T} [GeV]");
  h_Jpsi_m1_pt_data_1->GetYaxis()->SetTitle("Events");

  auto leg = new TLegend();
  leg->AddEntry(h_Jpsi_m1_pt_data_4, "0.0 < #DeltaR < 0.2", "l");
  leg->AddEntry(h_Jpsi_m1_pt_data_2, "0.2 < #DeltaR < 0.4", "l");
  leg->AddEntry(h_Jpsi_m1_pt_data_1, "0.4 < #DeltaR < 0.6", "l");
  leg->AddEntry(h_Jpsi_m1_pt_data_3, "0.6 < #DeltaR < 0.8", "l");
  leg->Draw();

  // MC
  c1->cd(2);
  treemc->Draw("Jpsi_m1_pt >> h_Jpsi_m1_pt_mc_1", "Jpsi_muonsDr > 0.4 && Jpsi_muonsDr < 0.6", "PLC");
  treemc->Draw("Jpsi_m1_pt >> h_Jpsi_m1_pt_mc_2", "Jpsi_muonsDr > 0.2 && Jpsi_muonsDr < 0.4", "PLC SAME");
  treemc->Draw("Jpsi_m1_pt >> h_Jpsi_m1_pt_mc_3", "Jpsi_muonsDr > 0.6 && Jpsi_muonsDr < 0.8", "PLC SAME");
  treemc->Draw("Jpsi_m1_pt >> h_Jpsi_m1_pt_mc_4", "Jpsi_muonsDr < 0.2", "PLC SAME");
  
  // retrieve histograms and add to legend
  TH1F* h_Jpsi_m1_pt_mc_1 = (TH1F*)gDirectory->Get("h_Jpsi_m1_pt_mc_1");
  TH1F* h_Jpsi_m1_pt_mc_2 = (TH1F*)gDirectory->Get("h_Jpsi_m1_pt_mc_2");
  TH1F* h_Jpsi_m1_pt_mc_3 = (TH1F*)gDirectory->Get("h_Jpsi_m1_pt_mc_3");
  TH1F* h_Jpsi_m1_pt_mc_4 = (TH1F*)gDirectory->Get("h_Jpsi_m1_pt_mc_4");

  h_Jpsi_m1_pt_mc_1->SetTitle("[MC] Leading #mu p_{T}");
  h_Jpsi_m1_pt_mc_1->GetXaxis()->SetTitle("p_{T} [GeV]");
  h_Jpsi_m1_pt_mc_1->GetYaxis()->SetTitle("Events");

  leg = new TLegend();
  leg->AddEntry(h_Jpsi_m1_pt_mc_4, "0.0 < #DeltaR < 0.2", "l");
  leg->AddEntry(h_Jpsi_m1_pt_mc_2, "0.2 < #DeltaR < 0.4", "l");
  leg->AddEntry(h_Jpsi_m1_pt_mc_1, "0.4 < #DeltaR < 0.6", "l");
  leg->AddEntry(h_Jpsi_m1_pt_mc_3, "0.6 < #DeltaR < 0.8", "l");
  leg->Draw();

  
  c1->Print(path + "kinematics_check.pdf(", "pdf");

  // ----- MU2 P_T ------
  TCanvas* c5 = new TCanvas("c5", "c5", 1600, 800);
  c5->Divide(2,1);
  
  // DATA
  c5->cd(1);
  tree->Draw("Jpsi_m2_pt >> h_Jpsi_m2_pt_data_1", "Jpsi_muonsDr > 0.4 && Jpsi_muonsDr < 0.6", "PLC");
  tree->Draw("Jpsi_m2_pt >> h_Jpsi_m2_pt_data_2", "Jpsi_muonsDr > 0.2 && Jpsi_muonsDr < 0.4", "PLC SAME");
  tree->Draw("Jpsi_m2_pt >> h_Jpsi_m2_pt_data_3", "Jpsi_muonsDr > 0.6 && Jpsi_muonsDr < 0.8", "PLC SAME");
  tree->Draw("Jpsi_m2_pt >> h_Jpsi_m2_pt_data_4", "Jpsi_muonsDr < 0.2", "PLC SAME");

  // retrieve histograms and add to legend
  TH1F* h_Jpsi_m2_pt_data_1 = (TH1F*)gDirectory->Get("h_Jpsi_m2_pt_data_1");
  TH1F* h_Jpsi_m2_pt_data_2 = (TH1F*)gDirectory->Get("h_Jpsi_m2_pt_data_2");
  TH1F* h_Jpsi_m2_pt_data_3 = (TH1F*)gDirectory->Get("h_Jpsi_m2_pt_data_3");
  TH1F* h_Jpsi_m2_pt_data_4 = (TH1F*)gDirectory->Get("h_Jpsi_m2_pt_data_4");

  h_Jpsi_m2_pt_data_1->SetTitle("[DATA] Subleading #mu p_{T}");
  h_Jpsi_m2_pt_data_1->GetXaxis()->SetTitle("p_{T} [GeV]");
  h_Jpsi_m2_pt_data_1->GetYaxis()->SetTitle("Events");

  leg = new TLegend();
  leg->AddEntry(h_Jpsi_m2_pt_data_4, "0.0 < #DeltaR < 0.2", "l");
  leg->AddEntry(h_Jpsi_m2_pt_data_2, "0.2 < #DeltaR < 0.4", "l");
  leg->AddEntry(h_Jpsi_m2_pt_data_1, "0.4 < #DeltaR < 0.6", "l");
  leg->AddEntry(h_Jpsi_m2_pt_data_3, "0.6 < #DeltaR < 0.8", "l");
  leg->Draw();


  // MC
  c5->cd(2);
  treemc->Draw("Jpsi_m2_pt >> h_Jpsi_m2_pt_mc_1", "Jpsi_muonsDr > 0.4 && Jpsi_muonsDr < 0.6", "PLC");
  treemc->Draw("Jpsi_m2_pt >> h_Jpsi_m2_pt_mc_2", "Jpsi_muonsDr > 0.2 && Jpsi_muonsDr < 0.4", "PLC SAME");
  treemc->Draw("Jpsi_m2_pt >> h_Jpsi_m2_pt_mc_3", "Jpsi_muonsDr > 0.6 && Jpsi_muonsDr < 0.8", "PLC SAME");
  treemc->Draw("Jpsi_m2_pt >> h_Jpsi_m2_pt_mc_4", "Jpsi_muonsDr < 0.2", "PLC SAME");

  // retrieve histograms and add to legend
  TH1F* h_Jpsi_m2_pt_mc_1 = (TH1F*)gDirectory->Get("h_Jpsi_m2_pt_mc_1");
  TH1F* h_Jpsi_m2_pt_mc_2 = (TH1F*)gDirectory->Get("h_Jpsi_m2_pt_mc_2");
  TH1F* h_Jpsi_m2_pt_mc_3 = (TH1F*)gDirectory->Get("h_Jpsi_m2_pt_mc_3");
  TH1F* h_Jpsi_m2_pt_mc_4 = (TH1F*)gDirectory->Get("h_Jpsi_m2_pt_mc_4");

  h_Jpsi_m2_pt_mc_1->SetTitle("[MC] Subleading #mu p_{T}");
  h_Jpsi_m2_pt_mc_1->GetXaxis()->SetTitle("p_{T} [GeV]");
  h_Jpsi_m2_pt_mc_1->GetYaxis()->SetTitle("Events");

  leg = new TLegend();
  leg->AddEntry(h_Jpsi_m2_pt_mc_4, "0.0 < #DeltaR < 0.2", "l");
  leg->AddEntry(h_Jpsi_m2_pt_mc_2, "0.2 < #DeltaR < 0.4", "l");
  leg->AddEntry(h_Jpsi_m2_pt_mc_1, "0.4 < #DeltaR < 0.6", "l");
  leg->AddEntry(h_Jpsi_m2_pt_mc_3, "0.6 < #DeltaR < 0.8", "l");
  leg->Draw();

  c5->Print(path + "kinematics_check.pdf", "pdf");

  // ----- MU1 ETA -----
  TCanvas* c7 = new TCanvas("c7", "c7", 1600, 800);
  c7->Divide(2,1);
  c7->cd(1);

  // DATA
  tree->Draw("Jpsi_m1_eta >> h_Jpsi_m1_eta_data_1", "Jpsi_muonsDr > 0.4 && Jpsi_muonsDr < 0.6", "PLC");
  tree->Draw("Jpsi_m1_eta >> h_Jpsi_m1_eta_data_2", "Jpsi_muonsDr > 0.2 && Jpsi_muonsDr < 0.4", "PLC SAME");
  tree->Draw("Jpsi_m1_eta >> h_Jpsi_m1_eta_data_3", "Jpsi_muonsDr > 0.6 && Jpsi_muonsDr < 0.8", "PLC SAME");
  tree->Draw("Jpsi_m1_eta >> h_Jpsi_m1_eta_data_4", "Jpsi_muonsDr < 0.2", "PLC SAME");

  TH1F* h_Jpsi_m1_eta_data_1 = (TH1F*)gDirectory->Get("h_Jpsi_m1_eta_data_1");
  TH1F* h_Jpsi_m1_eta_data_2 = (TH1F*)gDirectory->Get("h_Jpsi_m1_eta_data_2");
  TH1F* h_Jpsi_m1_eta_data_3 = (TH1F*)gDirectory->Get("h_Jpsi_m1_eta_data_3");
  TH1F* h_Jpsi_m1_eta_data_4 = (TH1F*)gDirectory->Get("h_Jpsi_m1_eta_data_4");

  h_Jpsi_m1_eta_data_1->SetTitle("[DATA] Leading #mu #eta");
  h_Jpsi_m1_eta_data_1->GetXaxis()->SetTitle("#eta");
  h_Jpsi_m1_eta_data_1->GetYaxis()->SetTitle("Events");

  leg = new TLegend();
  leg->AddEntry(h_Jpsi_m1_eta_data_4, "0.0 < #DeltaR < 0.2", "l");
  leg->AddEntry(h_Jpsi_m1_eta_data_2, "0.2 < #DeltaR < 0.4", "l");
  leg->AddEntry(h_Jpsi_m1_eta_data_1, "0.4 < #DeltaR < 0.6", "l");
  leg->AddEntry(h_Jpsi_m1_eta_data_3, "0.6 < #DeltaR < 0.8", "l");
  leg->Draw();

  // MC
  c7->cd(2);
  treemc->Draw("Jpsi_m1_eta >> h_Jpsi_m1_eta_mc_1", "Jpsi_muonsDr > 0.4 && Jpsi_muonsDr < 0.6", "PLC");
  treemc->Draw("Jpsi_m1_eta >> h_Jpsi_m1_eta_mc_2", "Jpsi_muonsDr > 0.2 && Jpsi_muonsDr < 0.4", "PLC SAME");
  treemc->Draw("Jpsi_m1_eta >> h_Jpsi_m1_eta_mc_3", "Jpsi_muonsDr > 0.6 && Jpsi_muonsDr < 0.8", "PLC SAME");
  treemc->Draw("Jpsi_m1_eta >> h_Jpsi_m1_eta_mc_4", "Jpsi_muonsDr < 0.2", "PLC SAME");
  
  TH1F* h_Jpsi_m1_eta_mc_1 = (TH1F*)gDirectory->Get("h_Jpsi_m1_eta_mc_1");
  TH1F* h_Jpsi_m1_eta_mc_2 = (TH1F*)gDirectory->Get("h_Jpsi_m1_eta_mc_2");
  TH1F* h_Jpsi_m1_eta_mc_3 = (TH1F*)gDirectory->Get("h_Jpsi_m1_eta_mc_3");
  TH1F* h_Jpsi_m1_eta_mc_4 = (TH1F*)gDirectory->Get("h_Jpsi_m1_eta_mc_4");

  h_Jpsi_m1_eta_mc_1->SetTitle("[MC] Leading #mu #eta");
  h_Jpsi_m1_eta_mc_1->GetXaxis()->SetTitle("#eta");
  h_Jpsi_m1_eta_mc_1->GetYaxis()->SetTitle("Events");

  leg = new TLegend();
  leg->AddEntry(h_Jpsi_m1_eta_mc_4, "0.0 < #DeltaR < 0.2", "l");
  leg->AddEntry(h_Jpsi_m1_eta_mc_2, "0.2 < #DeltaR < 0.4", "l");
  leg->AddEntry(h_Jpsi_m1_eta_mc_1, "0.4 < #DeltaR < 0.6", "l");
  leg->AddEntry(h_Jpsi_m1_eta_mc_3, "0.6 < #DeltaR < 0.8", "l");
  leg->Draw();

  
  c7->Print(path + "kinematics_check.pdf", "pdf");

  // ----- MU2 ETA ------
  TCanvas* c8 = new TCanvas("c8", "c8", 1600, 800);
  c8->Divide(2,1);

  // DATA
  c8->cd(1);
  
  tree->Draw("Jpsi_m2_eta >> h_Jpsi_m2_eta_data_1", "Jpsi_muonsDr > 0.4 && Jpsi_muonsDr < 0.6", "PLC");
  tree->Draw("Jpsi_m2_eta >> h_Jpsi_m2_eta_data_2", "Jpsi_muonsDr > 0.2 && Jpsi_muonsDr < 0.4", "PLC SAME");
  tree->Draw("Jpsi_m2_eta >> h_Jpsi_m2_eta_data_3", "Jpsi_muonsDr > 0.6 && Jpsi_muonsDr < 0.8", "PLC SAME");
  tree->Draw("Jpsi_m2_eta >> h_Jpsi_m2_eta_data_4", "Jpsi_muonsDr < 0.2", "PLC SAME");

  TH1F* h_Jpsi_m2_eta_data_1 = (TH1F*)gDirectory->Get("h_Jpsi_m2_eta_data_1");
  TH1F* h_Jpsi_m2_eta_data_2 = (TH1F*)gDirectory->Get("h_Jpsi_m2_eta_data_2");
  TH1F* h_Jpsi_m2_eta_data_3 = (TH1F*)gDirectory->Get("h_Jpsi_m2_eta_data_3");
  TH1F* h_Jpsi_m2_eta_data_4 = (TH1F*)gDirectory->Get("h_Jpsi_m2_eta_data_4");
  
  h_Jpsi_m2_eta_data_1->SetTitle("[DATA] Subleading #mu #eta");
  h_Jpsi_m2_eta_data_1->GetXaxis()->SetTitle("#eta");
  h_Jpsi_m2_eta_data_1->GetYaxis()->SetTitle("Events");

  leg = new TLegend();
  leg->AddEntry(h_Jpsi_m2_eta_data_4, "0.0 < #DeltaR < 0.2", "l");
  leg->AddEntry(h_Jpsi_m2_eta_data_2, "0.2 < #DeltaR < 0.4", "l");
  leg->AddEntry(h_Jpsi_m2_eta_data_1, "0.4 < #DeltaR < 0.6", "l");
  leg->AddEntry(h_Jpsi_m2_eta_data_3, "0.6 < #DeltaR < 0.8", "l");
  leg->Draw();

  // MC
  c8->cd(2);

  treemc->Draw("Jpsi_m2_eta >> h_Jpsi_m2_eta_mc_1", "Jpsi_muonsDr > 0.4 && Jpsi_muonsDr < 0.6", "PLC");
  treemc->Draw("Jpsi_m2_eta >> h_Jpsi_m2_eta_mc_2", "Jpsi_muonsDr > 0.2 && Jpsi_muonsDr < 0.4", "PLC SAME");
  treemc->Draw("Jpsi_m2_eta >> h_Jpsi_m2_eta_mc_3", "Jpsi_muonsDr > 0.6 && Jpsi_muonsDr < 0.8", "PLC SAME");
  treemc->Draw("Jpsi_m2_eta >> h_Jpsi_m2_eta_mc_4", "Jpsi_muonsDr < 0.2", "PLC SAME");

  TH1F* h_Jpsi_m2_eta_mc_1 = (TH1F*)gDirectory->Get("h_Jpsi_m2_eta_mc_1");
  TH1F* h_Jpsi_m2_eta_mc_2 = (TH1F*)gDirectory->Get("h_Jpsi_m2_eta_mc_2");
  TH1F* h_Jpsi_m2_eta_mc_3 = (TH1F*)gDirectory->Get("h_Jpsi_m2_eta_mc_3");
  TH1F* h_Jpsi_m2_eta_mc_4 = (TH1F*)gDirectory->Get("h_Jpsi_m2_eta_mc_4");

  h_Jpsi_m2_eta_mc_1->SetTitle("[MC] Subleading #mu #eta");
  h_Jpsi_m2_eta_mc_1->GetXaxis()->SetTitle("#eta");
  h_Jpsi_m2_eta_mc_1->GetYaxis()->SetTitle("Events");

  leg = new TLegend();
  leg->AddEntry(h_Jpsi_m2_eta_mc_4, "0.0 < #DeltaR < 0.2", "l");
  leg->AddEntry(h_Jpsi_m2_eta_mc_2, "0.2 < #DeltaR < 0.4", "l");
  leg->AddEntry(h_Jpsi_m2_eta_mc_1, "0.4 < #DeltaR < 0.6", "l");
  leg->AddEntry(h_Jpsi_m2_eta_mc_3, "0.6 < #DeltaR < 0.8", "l");
  leg->Draw();
  

  c8->Print(path + "kinematics_check.pdf", "pdf");


  // ----- Jpsi mass VS deltaR between muons -----
  TCanvas* c3 = new TCanvas("c3", "c3", 1600, 800);
  c3->Divide(2,1);

  c3->cd(1);
  tree->Draw("Jpsi_nonfit_mass:Jpsi_muonsDr >> h_mass_deltaR_data", "", "colz");
  ((TH2F*)gDirectory->Get("h_mass_deltaR_data"))->SetTitle("[DATA] m(#mu#mu) VS #DeltaR(#mu#mu)");
  ((TH2F*)gDirectory->Get("h_mass_deltaR_data"))->GetXaxis()->SetTitle("#DeltaR");
  ((TH2F*)gDirectory->Get("h_mass_deltaR_data"))->GetYaxis()->SetTitle("m(#mu#mu) [GeV]");

  c3->cd(2);
  treemc->Draw("Jpsi_nonfit_mass:Jpsi_muonsDr >> h_mass_deltaR_mc", "", "colz");
  ((TH2F*)gDirectory->Get("h_mass_deltaR_mc"))->SetTitle("[MC] m(#mu#mu) VS #DeltaR(#mu#mu)");
  ((TH2F*)gDirectory->Get("h_mass_deltaR_mc"))->GetXaxis()->SetTitle("#DeltaR");
  ((TH2F*)gDirectory->Get("h_mass_deltaR_mc"))->GetYaxis()->SetTitle("m(#mu#mu) [GeV]");

  c3->Print(path + "kinematics_check.pdf", "pdf");


  // Jpsi mass for different deltaR ranges
  TCanvas* c4 = new TCanvas("c4", "c4", 1600, 800);
  c4->Divide(2,1);
  
  c4->cd(1);
  tree->Draw("Jpsi_nonfit_mass >> h_mass_data_1", "Jpsi_muonsDr > 0.4 && Jpsi_muonsDr < 0.6", "PLC");
  tree->Draw("Jpsi_nonfit_mass >> h_mass_data_2", "Jpsi_muonsDr > 0.2 && Jpsi_muonsDr < 0.4", "PLC SAME");
  tree->Draw("Jpsi_nonfit_mass >> h_mass_data_3", "Jpsi_muonsDr > 0.6 && Jpsi_muonsDr < 0.8", "PLC SAME");
  tree->Draw("Jpsi_nonfit_mass >> h_mass_data_4", "Jpsi_muonsDr < 0.2", "PLC SAME");

  TH1F* h_mass_data_1 = (TH1F*)gDirectory->Get("h_mass_data_1");
  TH1F* h_mass_data_2 = (TH1F*)gDirectory->Get("h_mass_data_2");
  TH1F* h_mass_data_3 = (TH1F*)gDirectory->Get("h_mass_data_3");
  TH1F* h_mass_data_4 = (TH1F*)gDirectory->Get("h_mass_data_4");

  h_mass_data_1->SetTitle("[DATA] m(#mu#mu)");
  h_mass_data_1->GetXaxis()->SetTitle("m(#mu#mu) [GeV]");
  h_mass_data_1->GetYaxis()->SetTitle("Events");

  leg = new TLegend();
  leg->AddEntry(h_mass_data_4, "0.0 < #DeltaR < 0.2", "l");
  leg->AddEntry(h_mass_data_2, "0.2 < #DeltaR < 0.4", "l");
  leg->AddEntry(h_mass_data_1, "0.4 < #DeltaR < 0.6", "l");
  leg->AddEntry(h_mass_data_3, "0.6 < #DeltaR < 0.8", "l");
  leg->Draw();


  c4->cd(2);
  treemc->Draw("Jpsi_nonfit_mass >> h_mass_mc_1", "Jpsi_muonsDr > 0.4 && Jpsi_muonsDr < 0.6", "PLC");
  treemc->Draw("Jpsi_nonfit_mass >> h_mass_mc_2", "Jpsi_muonsDr > 0.2 && Jpsi_muonsDr < 0.4", "PLC SAME");
  treemc->Draw("Jpsi_nonfit_mass >> h_mass_mc_3", "Jpsi_muonsDr > 0.6 && Jpsi_muonsDr < 0.8", "PLC SAME");
  treemc->Draw("Jpsi_nonfit_mass >> h_mass_mc_4", "Jpsi_muonsDr < 0.2", "PLC SAME");

  TH1F* h_mass_mc_1 = (TH1F*)gDirectory->Get("h_mass_mc_1");
  TH1F* h_mass_mc_2 = (TH1F*)gDirectory->Get("h_mass_mc_2");
  TH1F* h_mass_mc_3 = (TH1F*)gDirectory->Get("h_mass_mc_3");
  TH1F* h_mass_mc_4 = (TH1F*)gDirectory->Get("h_mass_mc_4");

  h_mass_mc_1->SetTitle("[MC] m(#mu#mu)");
  h_mass_mc_1->GetXaxis()->SetTitle("m(#mu#mu) [GeV]");
  h_mass_mc_1->GetYaxis()->SetTitle("Events");

  leg = new TLegend();
  leg->AddEntry(h_mass_mc_4, "0.0 < #DeltaR < 0.2", "l");
  leg->AddEntry(h_mass_mc_2, "0.2 < #DeltaR < 0.4", "l");
  leg->AddEntry(h_mass_mc_1, "0.4 < #DeltaR < 0.6", "l");
  leg->AddEntry(h_mass_mc_3, "0.6 < #DeltaR < 0.8", "l");
  leg->Draw();

  c4->Print(path + "kinematics_check.pdf)", "pdf");

}