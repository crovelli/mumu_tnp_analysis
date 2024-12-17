#############################################################
# General settings

#if true, binning vs deltaR. Must also cadd -deltaR flag when launching runAnalysis.sh
is_vs_deltaR = True
is_vs_deltaR_eta = False
is_vs_ptonly = False
is_tagInEB = False
is_etaBin1 = True
is_etaBin2 = False
is_etaBin3 = False

# flag to be Tested

DoubleMu = 'probePathFired == 1'		       		       # trigger fired
DoubleMu += ' && Jpsi_m2_bestHLTdR<0.3'		       		       # trigger fired by this HLT candidate
DoubleMu += ' && Jpsi_m2_bestL1dR < 0.3'                               # L1 candidates matched to offline probe

# flag to be Tested
flags = {
    'DoubleMu' : DoubleMu
    }

if is_vs_deltaR:
    baseOutDir = 'results/vs_pt_deltaR/'
elif is_vs_deltaR_eta:
    baseOutDir = 'results/vs_deltaR_eta/'
elif is_vs_ptonly:
    baseOutDir = 'results/vs_pt_only/'
elif is_tagInEB:
    baseOutDir = 'results/tagInEB/'
else:
    baseOutDir = 'results/'


#############################################################
# Samples definition  - preparing the samples

### samples are defined in etc/inputs/tnpSampleDef.py
### not: you can setup another sampleDef File in inputs
import etc.inputs.tnpSampleDef as tnpSamples
tnpTreeDir = 'nano_'

samplesDef = {
    'data'   : tnpSamples.Trigger22['dataFv1m'].clone(),
    'mcNom'  : tnpSamples.Trigger22['MC'].clone(),
    'mcAlt'  : None,
    'tagSel' : None
  }
samplesDef['data'].add_sample( tnpSamples.Trigger22['dataCv1m'] )
samplesDef['data'].add_sample( tnpSamples.Trigger22['dataDv1m'] )
samplesDef['data'].add_sample( tnpSamples.Trigger22['dataDv2m'] )
samplesDef['data'].add_sample( tnpSamples.Trigger22['dataEv1m'] )
samplesDef['data'].add_sample( tnpSamples.Trigger22['dataGv1m'] )
samplesDef['data'].add_sample( tnpSamples.Trigger22['dataCv1dm'] )

samplesDef['data'].rename('data')

## if you need to use 2 times the same sample, then rename the second one
if not samplesDef['mcAlt'] is None:
    samplesDef['mcAlt'].rename('mcAlt')
if not samplesDef['tagSel'] is None:
    samplesDef['tagSel'].rename('tagSel')

## set MC weight
weightName = 'weight'    # 1 for data; pu_weight for MC   
# if not samplesDef['mcNom' ] is None: samplesDef['mcNom' ].set_weight(weightName)
# if not samplesDef['mcAlt' ] is None: samplesDef['mcAlt' ].set_weight(weightName)
# if not samplesDef['tagSel'] is None: samplesDef['tagSel'].set_weight(weightName)

#############################################################
# Bining definition  [can be nD bining]
deltaR_bins = [ { 'var' : 'Jpsi_muonsDr', 'type' : 'float' , 'bins' : [0.0, 0.45, 0.6, 1.2] } ] 
pt_bins     = [ { 'var' : 'Jpsi_m2_pt', 'type' : 'float' , 'bins' : [2.5, 4., 5., 6., 7., 8., 9., 10., 12., 14., 16., 18., 20., 25., 30., 40., 50.]} ]
eta_bins    = [ { 'var' : 'Jpsi_m2_eta', 'type' : 'float' , 'bins' : [-2.6, -2.05, -1.5, -0.75, 0, 0.75, 1.5, 2.05, 2.6]} ]

if is_vs_deltaR:
    biningDef = deltaR_bins + pt_bins
elif is_vs_deltaR_eta:
    biningDef = eta_bins + deltaR_bins
elif is_vs_ptonly:
    biningDef = pt_bins 
else:
    biningDef = eta_bins + pt_bins

#############################################################
# Cuts definition for all samples

cutBase = ''       

## turn-on cuts
cutBase += 'Jpsi_m1_pt > 2. && Jpsi_m2_pt > 2.'
cutBase += ' && Jpsi_m1_abseta < 2.4 && Jpsi_m2_abseta < 2.4' 
cutBase += ' && Jpsi_m1_mediumId && Jpsi_m2_mediumId' 
cutBase += ' && Jpsi_fit_vprob > 0.005'
cutBase += ' && Jpsi_muonsDz<1'
cutBase += ' && Jpsi_m1_trgobj_dR<0.3'     
cutBase += ' && Jpsi_m1_bestL1dR < 0.3'    
cutBase += ' && Jpsi_nonfit_mass > 2.9 && Jpsi_nonfit_mass < 3.3' 
#cutBase += ' && Jpsi_nonfit_mass > 2.6 && Jpsi_nonfit_mass < 3.5' 

if is_tagInEB:
    cutBase += ' && Jpsi_m1_eta < 1.4'
if is_etaBin1:
    cutBase += ' && Jpsi_m2_abseta < 0.9'
if is_etaBin2:
    cutBase += ' && Jpsi_m2_abseta < 1.2 && Jpsi_m2_abseta >= 0.8'
if is_etaBin3:
    cutBase += ' && Jpsi_m2_abseta >= 1.2'

## sanity cuts
cutBase += ' && Jpsi_m1_trgobj_pt != Jpsi_m2_trgobj_pt'

# can add addtionnal cuts for some bins (first check bin number using tnpEGM --checkBins)
additionalCuts = None

#############################################################
# Fitting params to tune fit by hand if necessary
tnpParAltSigFitJPsi = [
    "meanP[3.0969, 3.0, 3.4]","sigmaP[0.03, 0.01, 0.5]",
    "meanF[3.0969, 3.0, 3.4]","sigmaF[0.03, 0.01, 0.5]", 
    "alphaP[0., -50., .1]",
    "alphaF[0., -10., .1]",    
    ]

tnpParNomFitJPsi = [
    "meanP[3.0969, 3.07, 3.11]","sigmaP[0.03, 0.01, 0.2]", "alphaLP[0.6, 0.05, 5.]","alphaRP[1.2, 0.1, 3.]","nLP[3.6, .01, 100.]","nRP[1.85, .010, 200.]",
    "meanF[3.0969, 3.07, 3.11]","sigmaF[0.03, 0.01, 0.1]","alphaLF[1., 0.5, 3.]","alphaRF[1.2, 0.5, 5.0]","nLF[1., .1, 10.]","nRF[1., .001, 10.]",
    "expalphaP[0., -5., 5.]",
    "expalphaF[0., -20., 1.]",     # fino a 14 
#    "expalphaF[-2., -10., -1.]",    # da 14 in poi
    ]
     
tnpParAltBkgFitJPsi = [
    "meanP[3.0969, 3.07, 3.11]","sigmaP[0.03, 0.01, 0.2]", "alphaLP[0.6, 0.05, 5.]","alphaRP[1.2, 0.1, 2.]","nLP[3.6, .01, 100.]","nRP[1.85, .010, 200.]",
    "meanF[3.0969, 3.07, 3.11]","sigmaF[0.03, 0.01, 0.1]","alphaLF[1., 0.5, 2.]","alphaRF[1.2, 0.5, 5.0]","nLF[1., .1, 10.]","nRF[1., .01, 10.]",
    "cP[-0.5,-4.,4.]",
    "cF[-0.5,-4.,4.]",
    ]

