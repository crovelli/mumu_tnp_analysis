#include "MakeSimplePlot.C"

// Draw options helper functions
void set_sf_pt_draw_options(TGraphErrors* gr){
	gr->GetXaxis()->SetLabelSize(.11);
	gr->GetXaxis()->SetTitleSize(.12); gr->GetXaxis()->SetTitleOffset(1.);  //1.2
	gr->GetXaxis()->SetTickSize(0.15); gr->GetXaxis()->SetMoreLogLabels();
		
	// gr->GetYaxis()->SetRangeUser(0.78, 1.4);
	gr->GetYaxis()->SetLabelSize(.099); ///.1
	gr->GetYaxis()->SetTitleSize(.12); gr->GetYaxis()->SetTitleOffset(.28); //.15, .26
	gr->GetYaxis()->SetTickSize(0.01);

	gr->SetMarkerStyle(8); 
	gr->SetMarkerSize(1.5);
}

void set_sf_eta_draw_options(TGraphErrors* gr){
	gr->GetXaxis()->SetLabelSize(.11); gr->GetXaxis()->SetLabelOffset(0.03);
	gr->GetXaxis()->SetTitleSize(.12); gr->GetXaxis()->SetTitleOffset(1.);
	gr->GetXaxis()->SetTickSize(0.14);
    
	gr->GetYaxis()->SetLabelSize(.105); 
	gr->GetYaxis()->SetTitleSize(.1); gr->GetYaxis()->SetTitleOffset(.4);
	gr->GetYaxis()->SetTickSize(0.01);

	gr->SetMarkerStyle(8);
	gr->SetMarkerSize(1.5);
}


// Used to extract bin middle value in x, y from RooFitResult object name
std::pair<Double_t, Double_t> parse_x_y(std::string name, std::string varX, std::string varY){
	Double_t xy[2] = {-999, -999};

  int posX = name.find(varX.c_str());
  int posY = name.find(varY.c_str());

  std::string y_str = name.substr(posY, posX); //Y always before X
  std::string x_str = name.substr(posX);

  std::array<string, 2> strlist = {x_str, y_str};

  for(int i = 0; i < 2; i++){
    std::string str = strlist[i];
    std::string range;
    
    do {
      range = str.substr(0, str.find("_"));
      str = str.substr(str.find("_") + 1);
    } while(range.find("To") == string::npos);
    
    while(range.find("p") != string::npos) range.replace(range.find("p"), 1, ".");
		while(range.find("m") != string::npos) range.replace(range.find("m"), 1, "-");

    std::string range_low  = range.substr(0, range.find("To"));
		std::string range_high = range.substr(range.find("To")+2);

    xy[i] = (atof(range_low.c_str()) + atof(range_high.c_str()))/2.;

  }

  return std::make_pair(xy[0], xy[1]);
}

// Find bin (starting from 0) given vector of bin centers, bin length
int find_bin(Double_t value, std::vector<Double_t> bin_centers, std::vector<Double_t> bin_errors){
  
  // NOTE: bin_centers, bin_errors must be of the same length

  // if value is below first bin or above last, no bin found
  if((value < bin_centers.front() - bin_errors.front()) || (value > bin_centers.back() + bin_errors.back())) return -1;

  int nbin = -2;

  for(int i = 0; i < bin_centers.size(); i ++){
		if(value > bin_centers[i] - bin_errors[i] && value < bin_centers[i] + bin_errors[i]) nbin = i;
  }

	return nbin;
}

std::string strip_for_print(std::string input){
	std::string str = input;
	std::array<char, 4> to_erase = {'_', '#', '{', '}'};
	for(char t : to_erase) str.erase(std::remove(str.begin(), str.end(), t), str.end());
	return str;
}

//----- MAIN LOOP ------//

void draw2Deff(string outpath, string inpath){
	TFile* f2D = TFile::Open(TString::Format("../results%s/DoubleMu/egammaEffi.txt_EGM2D.root", inpath.c_str()), "read");

	TH2F* h2 = (TH2F*)f2D->Get("EGamma_EffData2D");	

	Bool_t is_deltaR = inpath.find("/vs_pt_deltaR") != string::npos;
	Bool_t is_deltaR_eta = inpath.find("/vs_deltaR_eta") != string::npos;
	// Bool_t is_deltaR = inpath.substr(0, 13) == "/vs_pt_deltaR";
	// Bool_t is_deltaR_eta = inpath.substr(0, 14) == "/vs_deltaR_eta";
	Bool_t is_L1cuts = inpath.find("_L1cuts") != string::npos;
	int nbins_y = is_deltaR ? 4 : 8;     //in vs_pt_deltaR, pt = x, deltaR = y
	int nbins_x = is_deltaR_eta ? 4 : 7; //in vs_deltaR_eta, deltaR = x, eta = y

	Double_t deltaR_plot_max = 0.9;

	// 1D vs pT efficiency plot
	std::string line;
	ifstream fpt(("../results" + inpath + "/projPt/DoubleMu/egammaEffi.txt").c_str(), ios::in);
	std::vector<Double_t> pt_bin_center;
	std::vector<Double_t> pt_bin_center_err;
	std::vector<Double_t> pt_bin_eff;
	std::vector<Double_t> pt_bin_eff_err;
	std::vector<Double_t> pt_bin_mceff;
	std::vector<Double_t> pt_bin_mceff_err;
	std::vector<Double_t> pt_bin_alteff;
	std::vector<Double_t> pt_bin_alteff_err;

	Int_t dim1 = 0;
	for(int i = 0; i < 2; i++) std::getline(fpt, line); //read header
	
	while(std::getline(fpt, line)){
		dim1++;
		Double_t pt_min, pt_max;
		Double_t eta_min, eta_max;
		Double_t eff, eff_err;
		Double_t mceff, mceff_err;
		Double_t alteff, alteff_err;
		Double_t tmp;
		(stringstream)line >> eta_min >> eta_max >> pt_min >> pt_max >> eff >> eff_err >> mceff >> mceff_err >> tmp >> tmp >> alteff >> alteff_err >> tmp >> tmp;

		pt_bin_center.push_back((pt_min + pt_max)/2.);
		pt_bin_center_err.push_back((pt_max - pt_min)/2.);
		pt_bin_eff.push_back(eff);
		pt_bin_eff_err.push_back(eff_err);
		pt_bin_mceff.push_back(mceff);
		pt_bin_mceff_err.push_back(mceff_err);
		pt_bin_alteff.push_back(alteff);
		pt_bin_alteff_err.push_back(alteff_err);
	}

	// 1D vs eta efficiency
	ifstream feta((string("../results") + inpath + "/projEta/DoubleMu/egammaEffi.txt").c_str(), ios::in);
	std::vector<Double_t> eta_bin_center;
	std::vector<Double_t> eta_bin_center_err;
	std::vector<Double_t> eta_bin_eff;
	std::vector<Double_t> eta_bin_eff_err;
	std::vector<Double_t> eta_bin_mceff;
	std::vector<Double_t> eta_bin_mceff_err;
	std::vector<Double_t> eta_bin_alteff;
	std::vector<Double_t> eta_bin_alteff_err;

	Int_t dim2 = 0;
	for(int i = 0; i < 2; i++) std::getline(feta, line); //read header

	while(std::getline(feta, line) && eta_bin_center.size() < nbins_y){
		dim2++;
		Double_t pt_min, pt_max;
		Double_t eta_min, eta_max;
		Double_t eff, eff_err;
		Double_t mceff, mceff_err;
		Double_t alteff, alteff_err;
		Double_t tmp;
		(stringstream) line >> eta_min >> eta_max >> pt_min >> pt_max >> eff >> eff_err >> mceff >> mceff_err >> tmp >> tmp >> alteff >> alteff_err >> tmp >> tmp;

		eta_bin_center.push_back((eta_min + eta_max)/2.);
		eta_bin_center_err.push_back((eta_max - eta_min)/2);
		eta_bin_eff.push_back(eff);
		eta_bin_eff_err.push_back(eff_err);
		eta_bin_mceff.push_back(mceff);
		eta_bin_mceff_err.push_back(mceff_err);
		eta_bin_alteff.push_back(alteff);
		eta_bin_alteff_err.push_back(alteff_err);
	}


	// 2D vs pT, eta
	ifstream fpteta(("../results" + inpath + "/DoubleMu/egammaEffi.txt").c_str(), ios::in);
	//In order: x bin center, x bin width, y bin center, y bin width, efficiency, error
	std::vector<std::array<Double_t, 6>> pt_eta_point;     //to save 2D coords + value, err for 2D graph
	std::vector<std::array<Double_t, 6>> pt_eta_mc_point;  //same

	// efficiency vs pT saved separately for each y bin
	std::vector<std::vector<Double_t>> pt_ybin_bin_center;
	std::vector<std::vector<Double_t>> pt_ybin_bin_center_err;
	std::vector<std::vector<Double_t>> pt_ybin_bin_eff;
	std::vector<std::vector<Double_t>> pt_ybin_bin_eff_err;
	std::vector<std::vector<Double_t>> pt_ybin_bin_mceff;
	std::vector<std::vector<Double_t>> pt_ybin_bin_mceff_err;
	std::vector<std::vector<Double_t>> pt_ybin_bin_alteff;
	std::vector<std::vector<Double_t>> pt_ybin_bin_alteff_err;

	// prepare dynamical-size array for each y bin
	for(int i = 0; i < nbins_y; i++){
		pt_ybin_bin_center.push_back(std::vector<Double_t>());
		pt_ybin_bin_center_err.push_back(std::vector<Double_t>());
		pt_ybin_bin_eff.push_back(std::vector<Double_t>());
		pt_ybin_bin_eff_err.push_back(std::vector<Double_t>());
		pt_ybin_bin_mceff.push_back(std::vector<Double_t>());
		pt_ybin_bin_mceff_err.push_back(std::vector<Double_t>());
		pt_ybin_bin_alteff.push_back(std::vector<Double_t>());
		pt_ybin_bin_alteff_err.push_back(std::vector<Double_t>());
	}


	for(int i = 0; i < 2; i++) std::getline(fpteta, line); //read header

	int counter = 0;
	while(std::getline(fpteta, line)){
		Double_t pt_min, pt_max;
		Double_t eta_min, eta_max;
		Double_t eff, eff_err;
		Double_t mceff, mceff_err;
		Double_t alteff, alteff_err;
		Double_t tmp;
		(stringstream)line >> eta_min >> eta_max >> pt_min >> pt_max >> eff >> eff_err >> mceff >> mceff_err >> tmp >> tmp >> alteff >> alteff_err >> tmp >> tmp;

		// efficiencies are saved in file cycling through y bins first for each x bin; take nline % #y bins to get y bin's index
		pt_ybin_bin_center[counter % nbins_y].push_back((pt_min + pt_max)/2.);
		pt_ybin_bin_center_err[counter % nbins_y].push_back((pt_max - pt_min)/2.);
		pt_ybin_bin_eff[counter % nbins_y].push_back(eff);
		pt_ybin_bin_eff_err[counter % nbins_y].push_back(eff_err);
		pt_ybin_bin_mceff[counter % nbins_y].push_back(mceff);
		pt_ybin_bin_mceff_err[counter % nbins_y].push_back(mceff_err);
		pt_ybin_bin_alteff[counter % nbins_y].push_back(alteff);
		pt_ybin_bin_alteff_err[counter % nbins_y].push_back(alteff_err);

		counter++;
		
		// save (x, y, efficiency) for 2D plot
		pt_eta_point.push_back({(pt_min + pt_max)/2., (pt_max - pt_min)/2., (eta_max + eta_min)/2., (eta_max - eta_min)/2., alteff, alteff_err});
		pt_eta_mc_point.push_back({(pt_min + pt_max)/2., (pt_max - pt_min)/2., (eta_max + eta_min)/2., (eta_max - eta_min)/2., mceff, mceff_err});
	}


	/*** NOMINAL FIT ***/

	// Creating TGraphs for efficiency
	auto gpt = new TGraphErrors(dim1, &pt_bin_center[0], &pt_bin_eff[0], &pt_bin_center_err[0], &pt_bin_eff_err[0]);
	auto geta = new TGraphErrors(dim2, &eta_bin_center[0], &eta_bin_eff[0], &eta_bin_center_err[0], &eta_bin_eff_err[0]);

	// changing graph style
	TGraphErrors* graphlist[2] = {gpt, geta};
	for(TGraphErrors* graph : graphlist){
		graph->SetMarkerStyle(8);
	}

	gROOT->SetBatch(kTRUE);
	gStyle->SetOptStat(0);

	// PLOTTING
	TCanvas* c1 = new TCanvas("ceff2d", "ceff2d", 1500, 1000);

	TPad* pad_center = new TPad("pad_center", "pad_center", 0, .3, .7, 1);
	TPad* pad_bottom = new TPad("pad_bottom", "pad_bottom", 0, 0, .7, .3);
	TPad* pad_top = new TPad("pad_top", "pad_top", .7, .3, 1, 1);
	
	pad_center->Draw();
	pad_bottom->Draw();
	pad_top->Draw();

	pad_center->cd();
	h2->Draw("colz TEXT");
	pad_center->SetLogy();

	pad_bottom->cd();
	geta->SetTitle("Efficiency vs #eta;#eta;Efficiency");
	geta->GetYaxis()->SetRangeUser(.5, 1);
	geta->Draw("AP");

	pad_top->cd();
	gpt->SetTitle("Efficiency vs p_{T};p_{T} [GeV];Efficiency");
	gpt->Draw("AP");
	gpt->GetYaxis()->SetRangeUser(.5, 1);
	gpt->GetXaxis()->SetRangeUser(2., 40);

	c1->SaveAs((outpath + "/data_2D_efficiency.pdf").c_str());


	/*** ALT SIG FIT ***/

	// --- Define 1D graphs [pT, (eta OR deltaR)] --- //

	// integrated
	auto mc_gpt = new TGraphErrors(dim1, &pt_bin_center[0], &pt_bin_mceff[0], &pt_bin_center_err[0], &pt_bin_mceff_err[0]);
	auto mc_geta = new TGraphErrors(dim2, &eta_bin_center[0], &eta_bin_mceff[0], &eta_bin_center_err[0], &eta_bin_mceff_err[0]);

	auto alt_gpt = new TGraphErrors(dim1, &pt_bin_center[0], &pt_bin_alteff[0], &pt_bin_center_err[0], &pt_bin_alteff_err[0]);
	auto alt_geta = new TGraphErrors(dim2, &eta_bin_center[0], &eta_bin_alteff[0], &eta_bin_center_err[0], &eta_bin_alteff_err[0]);

	// per bin
	std::vector<TGraphErrors*> alt_gpt_ybin;
	std::vector<TGraphErrors*> mc_gpt_ybin;
	for(int i = 0; i < nbins_y; i++){
		alt_gpt_ybin.push_back(new TGraphErrors(dim1, &pt_ybin_bin_center[i][0], &pt_ybin_bin_alteff[i][0], &pt_ybin_bin_center_err[i][0], &pt_ybin_bin_alteff_err[i][0]));
		mc_gpt_ybin.push_back(new TGraphErrors(dim1, &pt_ybin_bin_center[i][0], &pt_ybin_bin_mceff[i][0], &pt_ybin_bin_center_err[i][0], &pt_ybin_bin_mceff_err[i][0]));
	}

	
	// --- Define 2D histogram --- //

	// Data
	std::vector<Double_t> pts;
	std::vector<Double_t> etas;
	std::vector<Double_t> effs;
	std::vector<Double_t> eff_errs;
	std::vector<Double_t> pt_low;
	std::vector<Double_t> eta_low;
	int idx = 0;
	for(auto arr : pt_eta_point){
		if(idx % nbins_y == 0) pt_low.push_back(arr[0] - arr[1]);
		if(idx < nbins_y) eta_low.push_back(arr[2] - arr[3]);

		pts.push_back(arr[0]);
		etas.push_back(arr[2]);
		effs.push_back(arr[4]);
		eff_errs.push_back(arr[5]);
		idx++;
	}

	pt_low.push_back(pt_eta_point.back()[0] + pt_eta_point.back()[1]); //add last bin upper bound
	eta_low.push_back(pt_eta_point.back()[2] + pt_eta_point.back()[3]); //same


	TH2D* alt_hpteta = new TH2D("alt_hpteta", "alt_hpteta", dim1, &pt_low[0], dim2, &eta_low[0]);
	alt_hpteta->FillN(effs.size(), &pts[0], &etas[0], &effs[0]);

	// NOTE: this is just to remap bin numbering (lazy solution, can be done more efficiently using eff_errs directly)
	TH2D* alt_hpteta_err = new TH2D("alt_hpteta_err", "alt_hpteta_err", dim1, &pt_low[0], dim2, &eta_low[0]);
	alt_hpteta_err->FillN(eff_errs.size(), &pts[0], &etas[0], &eff_errs[0]);

	for(int nx = 0; nx < dim1; nx++){
		for(int ny = 0; ny < dim2; ny++){
			alt_hpteta->SetBinError(nx+1, ny+1, alt_hpteta_err->GetBinContent(nx+1, ny+1));
		}
	}

	// MC
	std::vector<Double_t> pts_mc;
	std::vector<Double_t> etas_mc;
	std::vector<Double_t> effs_mc;
	std::vector<Double_t> eff_errs_mc;
	idx = 0;
	for(auto arr : pt_eta_mc_point){
		pts_mc.push_back(arr[0]);
		etas_mc.push_back(arr[2]);
		effs_mc.push_back(arr[4]);
		eff_errs_mc.push_back(arr[5]);

		idx++;
	}

	TH2D* mc_hpteta = new TH2D("mc_hpteta", "mc_hpteta", dim1, &pt_low[0], dim2, &eta_low[0]); //define binning
	mc_hpteta->FillN(effs_mc.size(), &pts_mc[0], &etas_mc[0], &effs_mc[0]); //actually fill histogram

	TH2D* mc_hpteta_err = new TH2D("mc_hpteta_err", "mc_hpteta_err", dim1, &pt_low[0], dim2, &eta_low[0]); //define binning
	mc_hpteta_err->FillN(eff_errs_mc.size(), &pts_mc[0], &etas_mc[0], &eff_errs_mc[0]); //actually fill histogram

	for(int nx = 0; nx < dim1; nx++){
		for(int ny = 0; ny < dim2; ny++){
			mc_hpteta->SetBinError(nx+1, ny+1, mc_hpteta_err->GetBinContent(nx+1, ny+1));
		}
	}


	/*** [data, MC] obscure empty bins ***/
	// For any bin (both 2D plots and 1D projection), if any of the two categories are empty, black out the corresponding efficiency
	// (the fit does not fail, but rather returns a random number, so we have to protect against it by hand)

	TFile* fFitRes_data = TFile::Open(TString::Format("../results%s/DoubleMu/data_DoubleMu.altSigFit.root", inpath.c_str()), "read");
	TFile* fFitRes_MC = TFile::Open(TString::Format("../results%s/DoubleMu/MC_DoubleMu.altSigFit.root", inpath.c_str()), "read");
	TFile* fFitRes_projPt_data = TFile::Open(TString::Format("../results%s/projPt/DoubleMu/data_DoubleMu.altSigFit.root", inpath.c_str()), "read");
	TFile* fFitRes_projPt_MC = TFile::Open(TString::Format("../results%s/projPt/DoubleMu/MC_DoubleMu.altSigFit.root", inpath.c_str()), "read");
	TFile* fFitRes_projEta_data = TFile::Open(TString::Format("../results%s/projEta/DoubleMu/data_DoubleMu.altSigFit.root", inpath.c_str()), "read");
	TFile* fFitRes_projEta_MC = TFile::Open(TString::Format("../results%s/projEta/DoubleMu/MC_DoubleMu.altSigFit.root", inpath.c_str()), "read");
	
	
	std::array<TFile*, 6> fFitRes_list = {fFitRes_data, fFitRes_MC, 
  																			fFitRes_projPt_data, fFitRes_projPt_MC,
																				fFitRes_projEta_data, fFitRes_projEta_MC};
	std::string varX, varY;
	if(is_deltaR){
		varX = "Jpsi_m2_pt";
		varY = "Jpsi_muonsDr";
	} else if(is_deltaR_eta){
		varX = "Jpsi_muonsDr";
		varY = "Jpsi_m2_eta";
	} else {
		varX = "Jpsi_m2_pt";
		varY = "Jpsi_m2_eta";
	}

	std::array<std::vector<std::pair<Double_t, Double_t>>, 6> failed_bins; //saved by bin middle values

	/* Extract empty bin coordinates */
	
	// i % 2 = 0 (data), 1 (MC)
	for(int i = 0; i < 6; i++){
    //take .root file containing fit results for each bin as RooFitResult objs
		TFile* f = fFitRes_list[i];

		// iterate over bins == res objects
		for(TObject *obj : f->GetListOfKeys()[0]){

      std::string name = obj->GetName();
      if(name.find("res") != string::npos){ //only take fit results

      	RooFitResult* res = (RooFitResult*)f->Get(name.c_str());
      	
				if(res->minNll() > 0){ //fits on non-empty bins have Negative Log Likelihood < 0
				  
					// to protect from weird bin bounds
					int max_bins = 999;
					if(i < 2) max_bins = dim1*dim2;
					else if(i < 4) max_bins = dim1;
					else max_bins = dim2;

					if(atoi(name.substr(3, 2).c_str()) < max_bins){
            // std::cout << i << ") " << name << " is an empty bin!" << std::endl;
            failed_bins[i].push_back(parse_x_y(name, varX, varY)); //extract bin coords from object name
					}
        }
 
      }
    } //end key iteration
  } //end file iteration
  

  std::array<TGraphErrors*, 6> to_obscure_graphs = {NULL, NULL, alt_gpt, mc_gpt, alt_geta, mc_geta};

	/* Obscure bin (= set to -1) in 2D histogram and 1D pT plots per y bin */
  for(int i = 0; i < 6; i++){
    for(auto pair : failed_bins[i]){
      if(i < 2){ //2D efficiency also
        TH2D* h = !i ? alt_hpteta : mc_hpteta;
			
        //2d
        int binx = h->GetXaxis()->FindBin(pair.first);
        int biny = h->GetYaxis()->FindBin(pair.second);
        h->SetBinContent(binx, biny, -1);
        h->SetBinError(binx, biny, 0);
			}
			
			//1d
			int num_1d_x = find_bin(pair.first, pt_bin_center, pt_bin_center_err);
			int num_1d_y = find_bin(pair.second, eta_bin_center, eta_bin_center_err);
			
			if(i < 2){
        auto gr_2d = !i ? alt_gpt_ybin : mc_gpt_ybin;
        gr_2d[num_1d_y]->SetPoint(num_1d_x, pair.first, -1);
			}
			else {
        auto gr = to_obscure_graphs[i];
				int n = (i < 4) ? num_1d_x : num_1d_y;
				Double_t coord = (i < 4) ? pair.first : pair.second;
				gr->SetPoint(n, coord, -1);
			}
      
    }
  }

  // -------------------------------------------------

	//--- PLOTTING ---//
	int color[9] = {62, 95, 7, 2, 9, 6, 205, 225, 1};
	if(is_deltaR){
		color[2] = 7;
		color[nbins_y] = 1; //make black last
	}
	string plotvar_y = (is_deltaR) ? "#DeltaR" : "#eta";
	string plotvar_x = (is_deltaR_eta) ? "#DeltaR" : "p_{T}";
	string x_unit = (is_deltaR_eta) ? "" : " [GeV]";

	string plotvar_x_print = strip_for_print(plotvar_x);
	string plotvar_y_print = strip_for_print(plotvar_y);


	// changing graph style
	std::vector<TGraphErrors*> alt_graphlist = {alt_gpt, alt_geta, mc_gpt, mc_geta};
	alt_graphlist.insert(alt_graphlist.end(), alt_gpt_ybin.begin(), alt_gpt_ybin.end());
	alt_graphlist.insert(alt_graphlist.end(), mc_gpt_ybin.begin(), mc_gpt_ybin.end());
	for(TGraphErrors* graph : alt_graphlist){
		graph->SetMarkerStyle(8);
		graph->SetMarkerSize(1.5);
	}


	TCanvas* c2 = new TCanvas("ceff2d_altsig", "ceff2d_altsig", 3000, 3000);
	c2->Divide(3,3);

	// data
	c2->cd(1);
	// FPCanvasStyle(gPad, "test", "test", 0, false, true);
	alt_hpteta->SetTitle(TString::Format("[DATA] Efficiency vs %s, %s;%s;%s", plotvar_x.c_str(), plotvar_y.c_str(), (plotvar_x + x_unit).c_str(), plotvar_y.c_str()));
	if(!is_deltaR_eta) {
		gPad->SetLogx();
		alt_hpteta->GetXaxis()->SetMoreLogLabels();
	}
	alt_hpteta->GetXaxis()->SetTitleOffset(1.2);
	alt_hpteta->SetMarkerSize(.6);
	alt_hpteta->Draw("colz text59 E");
	// alt_hpteta->GetXaxis()->SetRangeUser(0, 20);
	// alt_hpteta->GetYaxis()->SetRangeUser(0, 2.6);

	c2->cd(2);
	gPad->SetGrid(1,1);
	alt_geta->SetTitle(TString::Format("[DATA] Efficiency vs %s;%s;Efficiency", plotvar_y.c_str(), plotvar_y.c_str()));
	alt_geta->Draw("AP");
	alt_geta->GetYaxis()->SetRangeUser(0, 1);


	c2->cd(3);
	gPad->SetGrid(1,1);
	if(!is_deltaR_eta) gPad->SetLogx();

	alt_gpt->GetYaxis()->SetRangeUser(0, 1);
 	alt_gpt->SetTitle(TString::Format("[DATA] Efficiency vs %s;%s;Efficiency", plotvar_x.c_str(), (plotvar_x + x_unit).c_str()));
	alt_gpt->SetMarkerColor(color[nbins_y]); alt_gpt->SetLineColor(color[nbins_y]);
	alt_gpt->Draw("AP");
	// alt_gpt->GetXaxis()->SetRangeUser(2., 40);
	if(!is_deltaR_eta) alt_gpt->GetXaxis()->SetLabelOffset(-0.007);
	alt_gpt->GetXaxis()->SetMoreLogLabels();

	double leg_width = is_deltaR ? 0.3 : 0.7;
	auto leg1 = new TLegend(leg_width, 0.2);
	if(!is_deltaR) leg1->SetNColumns(3);
	
	leg1->AddEntry(alt_gpt, TString::Format("%s-integrated", plotvar_y.c_str()), "pl");

	for(int i = 0; i < nbins_y; i++){
    alt_gpt_ybin[i]->SetMarkerColor(color[i]); alt_gpt_ybin[i]->SetLineColor(color[i]);
    alt_gpt_ybin[i]->SetTitle(TString::Format("[DATA] Efficiency vs %s;%s;Efficiency", plotvar_x.c_str(), (plotvar_x + x_unit).c_str()));
    alt_gpt_ybin[i]->Draw("SAMES P");
    leg1->AddEntry(alt_gpt_ybin[i], TString::Format("%.2f < %s < %.2f", eta_low[i], plotvar_y.c_str(), eta_low[i+1]), "pl");
  }
  leg1->Draw();

	if(is_deltaR_eta && is_L1cuts){
		gPad->Update();
		leg1->SetX1(0);
		leg1->SetX2(0.65);
		leg1->SetY1(0.1);
		leg1->SetY2(0.4);
		gPad->Modified();
	}

	// MC
	c2->cd(4);
	mc_hpteta->SetTitle(TString::Format("[MC] Efficiency vs %s, %s;%s;%s", plotvar_x.c_str(), plotvar_y.c_str(), (plotvar_x + x_unit).c_str(), plotvar_y.c_str()));
	if(!is_deltaR_eta){
		gPad->SetLogx();
		mc_hpteta->GetXaxis()->SetMoreLogLabels();
	}
	mc_hpteta->GetXaxis()->SetTitleOffset(1.2);
	mc_hpteta->SetMarkerSize(.6);
	mc_hpteta->Draw("colz text59 E");


	c2->cd(5);
	gPad->SetGrid(1,1);
	mc_geta->SetTitle(TString::Format("[MC] Efficiency vs %s;%s;Efficiency", plotvar_y.c_str(), plotvar_y.c_str()));
	mc_geta->Draw("AP");
	mc_geta->GetYaxis()->SetRangeUser(0, 1);


	c2->cd(6);
	gPad->SetGrid(1,1);
	if(!is_deltaR_eta) gPad->SetLogx();

	mc_gpt->SetTitle(TString::Format("[MC] Efficiency vs %s;%s;Efficiency", plotvar_x.c_str(), (plotvar_x + x_unit).c_str()));
	mc_gpt->SetMarkerColor(color[nbins_y]);	mc_gpt->SetLineColor(color[nbins_y]);
	mc_gpt->Draw("AP");
	// mc_gpt->GetXaxis()->SetRangeUser(2., 40);
	mc_gpt->GetYaxis()->SetRangeUser(0, 1);
	if(!is_deltaR_eta) mc_gpt->GetXaxis()->SetLabelOffset(-0.007);
	mc_gpt->GetXaxis()->SetMoreLogLabels();

	auto leg2 = new TLegend(leg_width, 0.2);
	if(!is_deltaR) leg2->SetNColumns(3);
	leg2->AddEntry(mc_gpt, TString::Format("%s-integrated", plotvar_y.c_str()), "pl");

	for(int i = 0; i < nbins_y; i++){
    mc_gpt_ybin[i]->SetMarkerColor(color[i]); mc_gpt_ybin[i]->SetLineColor(color[i]);
    mc_gpt_ybin[i]->Draw("SAMES P");
    
    leg2->AddEntry(mc_gpt_ybin[i], TString::Format("%.2f < %s < %.2f", eta_low[i], plotvar_y.c_str(), eta_low[i+1]), "pl");
  }
  leg2->Draw();
  
	if(is_deltaR_eta && is_L1cuts){
		gPad->Update();
		leg2->SetX1(0);
		leg2->SetX2(0.65);
		leg2->SetY1(0.1);
		leg2->SetY2(0.4);
		gPad->Modified();
	}

	
	/*** MC vs Data on same plot + Scale factors ***/

	Bool_t integrated_only = 1; // = 0 only plots var-integrated SFs, = 1 shows per bin

	/* vs eta */
	c2->cd(8);

	// Constructing TGraph ratio
	TGraphErrors* sf_eta = new TGraphErrors(nbins_y);
	sf_eta->SetTitle(TString::Format(";%s;Data/MC", plotvar_y.c_str()));

	for(int i = 0; i < nbins_y; i++){
		double alt_x, alt_y, mc_x, mc_y;
		alt_geta->GetPoint(i, alt_x, alt_y);
		mc_geta->GetPoint(i, mc_x, mc_y);
		Double_t val = alt_y/mc_y;
		Double_t err_rel_data = alt_geta->GetErrorY(i)/alt_y, err_rel_mc = mc_geta->GetErrorY(i)/mc_y;
		Double_t err = val * sqrt( pow(err_rel_data, 2) + pow(err_rel_mc, 2) ); //in division, rel. err. is quadrature sum of relative errors

		sf_eta->SetPoint(i, alt_x, val);
		sf_eta->SetPointError(i, alt_geta->GetErrorX(i), err);
	}


	TPad* pad_top_eta = new TPad("pad_top_eta", "pad_top_eta", 0, .2, 1., 1);
	TPad* pad_bottom_eta = new TPad("pad_bottom_eta", "pad_bottom_eta", 0, 0, 1., .2);
	
	pad_top_eta->Draw();
	pad_bottom_eta->Draw();

	// sf
	pad_bottom_eta->cd();
	pad_bottom_eta->SetGrid(1,1);
	pad_bottom_eta->SetTopMargin(0.015);
	pad_bottom_eta->SetLeftMargin(0.15);
	pad_bottom_eta->SetBottomMargin(0.3);

	set_sf_eta_draw_options(sf_eta);
	sf_eta->Draw("AP"); // to get axis ready
	
	if(is_deltaR || 1){
		// Double_t xmax = is_deltaR ? 1.1 : 2.85;
		Double_t xmax = is_deltaR ? 0.86 : 3;
		Double_t xmin = is_deltaR ? 0 : -xmax;
		
		TLine* hline2 = new TLine(xmin, 1, xmax, 1);
		hline2->SetLineWidth(1);
		hline2->SetLineStyle(2);
		hline2->SetLineColor(34);

		hline2->Draw(); //plotted first, so it shows behind all other graphs
		sf_eta->Draw("SAMES P");
	}

	// efficiency
	pad_top_eta->cd();
	pad_top_eta->SetGrid(1,1);
	pad_top_eta->SetBottomMargin(0.015);
	pad_top_eta->SetLeftMargin(0.15);

	TGraphErrors* alt_geta_cloned = (TGraphErrors*)alt_geta->Clone(); //clone to prevent changing style on other plots
	TGraphErrors* mc_geta_cloned = (TGraphErrors*)mc_geta->Clone();

	alt_geta_cloned->SetTitle(TString::Format("Efficiency vs %s;%s;Efficiency", plotvar_y.c_str(), plotvar_y.c_str()));
	alt_geta_cloned->GetYaxis()->SetTitleOffset(1.1);
	alt_geta_cloned->GetXaxis()->SetLabelSize(0);
	alt_geta_cloned->Draw("AP");

	mc_geta_cloned->SetLineStyle(3);
	mc_geta_cloned->Draw("SAMES P");

	auto leg3 = new TLegend();
	leg3->AddEntry(alt_geta_cloned, TString::Format("Data, %s-integrated", plotvar_x.c_str()), "pl");
	leg3->AddEntry(mc_geta_cloned, TString::Format("MC, %s-integrated", plotvar_x.c_str()), "pl");
	leg3->Draw();
	
	/* vs pT */
	c2->cd(9);

	// Constructing TGraph ratio
	std::vector<TGraphErrors*> sf_pt_ybin;
	for(int i = 0; i < nbins_y + 1; i++) sf_pt_ybin.push_back(new TGraphErrors(dim1));

	alt_gpt_ybin.push_back(alt_gpt);
	mc_gpt_ybin.push_back(mc_gpt);

	for(int j = 0; j < nbins_y + 1; j++){

		for(int i = 0; i < dim1; i++){
			double alt_x, alt_y, mc_x, mc_y;
			alt_gpt_ybin[j]->GetPoint(i, alt_x, alt_y);
			mc_gpt_ybin[j]->GetPoint(i, mc_x, mc_y);
			Double_t val = alt_y/mc_y;
			Double_t err_rel_data = alt_gpt_ybin[j]->GetErrorY(i)/alt_y, err_rel_mc = mc_gpt_ybin[j]->GetErrorY(i)/mc_y;
			Double_t err = val * sqrt( pow(err_rel_data, 2) + pow(err_rel_mc, 2) ); //in division, rel. err. is quadrature sum of relative errors

			sf_pt_ybin[j]->SetPoint(i, alt_x, val);
			sf_pt_ybin[j]->SetPointError(i, alt_gpt_ybin[j]->GetErrorX(i), err);
		}
	}

	// Plotting
	TPad* pad_top_pt = new TPad("pad_top_pt", "pad_top_pt", 0, .2, 1., 1);
	TPad* pad_bottom_pt = new TPad("pad_bottom_pt", "pad_bottom_pt", 0, 0, 1., .2);
	
	pad_top_pt->Draw();
	pad_bottom_pt->Draw();

	// SF
	pad_bottom_pt->cd();
	pad_bottom_pt->SetGrid(1,1);
	pad_bottom_pt->SetTopMargin(0.015);
	pad_bottom_pt->SetLeftMargin(0.15);
	pad_bottom_pt->SetBottomMargin(0.3); //0.38
	if(!is_deltaR_eta) pad_bottom_pt->SetLogx();

	double sf_xmax_pt = is_deltaR_eta ? 0.86 : 43.5;
	TLine* hline = new TLine(0, 1., sf_xmax_pt, 1.);
	hline->SetLineWidth(1);
	hline->SetLineStyle(2);
	hline->SetLineColor(34);

	sf_pt_ybin[nbins_y]->SetTitle(TString::Format(";%s;Data/MC", (plotvar_x + x_unit).c_str()));
	sf_pt_ybin[nbins_y]->Draw("AP"); //just to get axis ready; will be redrawn later

	hline->Draw(); //plotted first, so it shows behind all other graphs

	for(int i = 0; i < nbins_y + 1; i++){
		set_sf_pt_draw_options(sf_pt_ybin[i]);

		if((i == nbins_y && integrated_only) || !integrated_only){		   
			sf_pt_ybin[i]->SetMarkerColor(color[i]); 
			sf_pt_ybin[i]->SetLineColor(color[i]); 
			sf_pt_ybin[i]->Draw("SAMES P");
		}
	}


	// Efficiency
	pad_top_pt->cd();
	pad_top_pt->SetGrid(1,1);
	pad_top_pt->SetBottomMargin(0.015);
	pad_top_pt->SetLeftMargin(0.15);
	if(!is_deltaR_eta) pad_top_pt->SetLogx();


	std::vector<TGraphErrors*> pt_cloned;
	std::vector<TGraphErrors*> pt_cloned_data;

	for(int i = 0; i < nbins_y + 1; i++){
    TGraphErrors* gr = (TGraphErrors*)mc_gpt_ybin[i]->Clone();
    TGraphErrors* gr2 = (TGraphErrors*)alt_gpt_ybin[i]->Clone();
		pt_cloned.push_back(gr);
		pt_cloned_data.push_back(gr2);
		gr->SetLineStyle(3); 
		gr->SetMarkerSize(1.2);
		gr2->GetXaxis()->SetLabelSize(0);
	   		
		if((i == nbins_y && integrated_only) || !integrated_only){
			gr2->SetMarkerSize(1.5);
			gr2->GetYaxis()->SetTitleOffset(1.2); 
			// gr2->GetYaxis()->SetRangeUser(0.12, 1.);
			gr2->SetTitle(TString::Format("Efficiency vs %s;%s;Efficiency", plotvar_x.c_str(), (plotvar_x + x_unit).c_str()));
			gr2->Draw((i && !integrated_only) ? "SAMES P" : "AP");
			gr->Draw("SAMES P");
		}

	}

	auto leg4 = new TLegend(); //.5, .25
	if(!integrated_only) leg4->SetNColumns(2);
	leg4->AddEntry(pt_cloned_data[nbins_y], TString::Format("Data, %s-integrated", plotvar_y.c_str()), "pl");
	leg4->AddEntry(pt_cloned[nbins_y], TString::Format("MC, %s-integrated", plotvar_y.c_str()), "pl");
	for(int i = 0; i < nbins_y; i++){
		if((i == nbins_y && integrated_only) || !integrated_only){
			leg4->AddEntry(alt_gpt_ybin[i], TString::Format("Data, %.1f < %s < %.1f", eta_low[i], plotvar_y.c_str(), eta_low[i+1]), "pl");
			leg4->AddEntry(pt_cloned[i], TString::Format("MC, %.1f < %s < %.1f", eta_low[i], plotvar_y.c_str(), eta_low[i+1]), "pl");
		}
	}

	leg4->Draw();	


	// Saving + common graphic options
	gStyle->SetGridColor(18);
	gStyle->SetLineScalePS(1.75);
	c2->SaveAs((outpath + "/altSig_data_2D_efficiency.pdf").c_str());
	// c2->SaveAs((outpath + "/altSig_data_2D_efficiency.svg").c_str());

}
