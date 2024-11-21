# mumu_tnp_analysis

This repo is based on 

https://github.com/crovelli/egm_tnp_analysis/tree/develTnPJpsi

with additions from 

https://github.com/noepalm/mumu_tnp_analysis/tree/develTnPJpsi

# Getting started:

  lxplus8

  cmssw-el7

  cmsrel CMSSW_11_2_0

  cd CMSSW_11_2_0/src

  cmsenv
  
  git cms-init  

# Checkout:

  git clone git@github.com:crovelli/mumu_tnp_analysis.git 

  cd mumu_tnp_analysis

# To compile the repo:

  lxplus8

  cd myWorkspace/BPhys/TnpLowMass_CMSSW_11_2_0/src/

  cmssw-el7

  cmsenv

  cd mumu_tnp_analysis/

  make clean

  make

# To run:

MY_EOS=/afs/cern.ch/work/c/crovelli/BPhys/TnpLowMass_CMSSW_11_2_0/src/mumu_tnp_analysis

python tnpEGM_fitter.py etc/config/settings.py  --flag DoubleMu --checkBins

python tnpEGM_fitter.py etc/config/settings.py  --flag DoubleMu --createBins

python tnpEGM_fitter.py etc/config/settings.py  --flag DoubleMu --createHists

python tnpEGM_fitter.py etc/config/settings.py  --flag DoubleMu --doFit

python tnpEGM_fitter.py etc/config/settings.py  --flag DoubleMu --doFit --mcSig

python tnpEGM_fitter.py etc/config/settings.py  --flag DoubleMu --doFit --altSig

python tnpEGM_fitter.py etc/config/settings.py  --flag DoubleMu --doFit --altBkg	

python tnpEGM_fitter.py etc/config/setting.py   --flag DoubleMu --sumUp