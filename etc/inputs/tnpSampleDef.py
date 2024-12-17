from libPython.tnpClassUtils import tnpSample

### samples
Trigger22 = {

    'MC' : tnpSample('MC', "/eos/cms/store/group/phys_bphys/crovelli/TriggerL1_2024Dec10/InclusiveDileptonMinBias_TuneCP5Plus_13p6TeV_pythia8/crab_InclusiveDilepton/241210_110449/TriggerStudy_InclusiveDileptonMinBias.root", isMC = True, nEvts = -1),
    #
    'dataCv1m' : tnpSample('dataCv1m', "/eos/cms/store/group/phys_bphys/crovelli/TriggerL1_2024Dec09/Muon/crab_data_Run2022Cv1m/241209_154622/TriggerStudy_2022Cv1m.root"),
    'dataDv1m' : tnpSample('dataDv1m', "/eos/cms/store/group/phys_bphys/crovelli/TriggerL1_2024Dec09/Muon/crab_data_Run2022Dv1/241209_154626/TriggerStudy_2022Dv1.root"),
    'dataDv2m' : tnpSample('dataDv2m', "/eos/cms/store/group/phys_bphys/crovelli/TriggerL1_2024Dec09/Muon/crab_data_Run2022Dv2/241209_154630/TriggerStudy_2022Dv2.root"),
    'dataEv1m' : tnpSample('dataEv1m', "/eos/cms/store/group/phys_bphys/crovelli/TriggerL1_2024Dec09/Muon/crab_data_Run2022Ev1/241209_154647/TriggerStudy_2022E.root"),
    'dataFv1m' : tnpSample('dataFv1m', "/eos/cms/store/group/phys_bphys/crovelli/TriggerL1_2024Dec09/Muon/crab_data_Run2022Fv1_0/241209_162644/TriggerStudy_2022Fv1.root"),
    'dataGv1m' : tnpSample('dataGv1m', "/eos/cms/store/group/phys_bphys/crovelli/TriggerL1_2024Dec09/Muon/crab_data_Run2022Gv1_0/241209_162648/TriggerStudy_2022Gv1.root"),
    #
    'dataCv1dm' : tnpSample('dataCv1dm', "/eos/cms/store/group/phys_bphys/crovelli/TriggerL1_2024Dec09/DoubleMuon/crab_data_Run2022Cv1dm/241209_154618/TriggerStudy_2022Cv1dm.root")
}

