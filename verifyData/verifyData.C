void verifyData() {

	// TChain* chain = new TChain();
	// for(int i = 0; i < 8; i++)chain->Add(TString::Format("/eos/cms/store/user/crovelli/HLT_DoubleMu/DoubleMuTnP_2022D_LowMass%i.root/nano_/tree", i));
	
	TFile* f = TFile::Open("../DoubleMuTnP_v2.root");
	TTree* chain = (TTree*)f->Get("nano_/tree");
	
	// 1) Verify that the first matched muon almost always coincides for the two triggers
	//    Q: What kind of events are the exceptions? Is the discriminant cut the one on the second muon? 
	TCanvas* c1 = new TCanvas("c_m1", "c_m1", 800, 800);
	// c1->SetLogy();
	chain->Draw("Jpsi_m1_Dimu_pt - Jpsi_m1_trgobj_pt"); //notice 0 peak
	c1->Print("verifyData.pdf(", "pdf");

	// 2) Verify that, most of the time, there is no matched muon for trigger 2 (and when there is, it coincides with the Dimuon one) --few exceptions--	
	TCanvas* c2 = new TCanvas("c_m2", "c_m2", 800, 800);
	// c2->SetLogy();
	chain->Draw("Jpsi_m2_Dimu_pt - Jpsi_m2_trgobj_pt", ""); //notice peak at exactly 0 and around 100 (smudged by Dimu_pt)
	c2->Print("verifyData.pdf", "pdf");
	

	// Checking for events in which the same HLT candidate is matched to both offline muons
	chain->Draw(">>elist1", "Jpsi_m1_trgobj_pt == Jpsi_m2_trgobj_pt && Jpsi_m1_Dimu_pt > 0"); //excluding cases in which no HLT candidates are found+matched
	chain->Draw(">>elist2", "Jpsi_m1_Dimu_pt == Jpsi_m2_Dimu_pt");
	chain->Draw(">>elist3", "Jpsi_m1_bestL1pt == Jpsi_m2_bestL1pt");

	// 3) Verify Jpsi mass distribution pre/post fit
	TCanvas* c3 = new TCanvas("c3", "c3", 800, 800);
	chain->Draw("Jpsi_nonfit_mass >> h(100, 2, 4)", "", "PLC");
	chain->Draw("Jpsi_fit_mass >> h2(100, 2, 4)", "", "SAMES PLC");
	c3->BuildLegend();
	c3->Print("verifyData.pdf", "pdf");

	TCanvas* c4 = new TCanvas("c4", "c4", 800, 800);
	chain->Draw("Jpsi_nonfit_pt >> h3(100, 0, 100)", "", "PLC");
	chain->Draw("Jpsi_fit_pt >> h4(100, 0, 100)", "", "SAMES PLC");
	c4->BuildLegend();
	c4->Print("verifyData.pdf", "pdf");


	// 4) Verify probe pT VS offline muons deltaR
	TCanvas* c5 = new TCanvas("c5", "c5", 800, 800);
	chain->Draw("Jpsi_m2_pt:Jpsi_muonsDr", "", "colz");
	c5->Print("verifyData.pdf", "pdf");


	TFile* fmc = TFile::Open("/eos/cms/store/user/crovelli/DoubleMuMC/TnP__JPsito2Mu_JPsiFilter_2MuFilter__Run3Summer22EE.root");
	TTree* chainmc = (TTree*)fmc->Get("nano_/tree");

	TCanvas* c6 = new TCanvas("c6", "c6", 800, 800);
	chainmc->Draw("Jpsi_m2_Dimu_dR", "DoubleMu_fired == 1", "PFC");
	chainmc->Draw("Jpsi_m2_Dimu_dR", "DoubleMu_fired == 0", "SAMES PFC");
	c6->BuildLegend();
	c6->Print("verifyData.pdf", "pdf");

	TCanvas* c7 = new TCanvas("c7", "c7", 800, 800);
	chainmc->Draw("Jpsi_m1_Dimu_dR", "DoubleMu_fired == 1", "PFC");
	chainmc->Draw("Jpsi_m1_Dimu_dR", "DoubleMu_fired == 0", "SAMES PFC");
	c7->BuildLegend();
	c7->Print("verifyData.pdf)", "pdf");
}
