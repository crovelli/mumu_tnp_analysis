from libPython.tnpClassUtils import tnpSample

### samples
Trigger22 = {

    'MC' : tnpSample('MC', "/eos/cms/store/group/phys_bphys/crovelli/TriggerL1_2024Nov15/InclusiveDileptonMinBias_TuneCP5Plus_13p6TeV_pythia8/crab_InclusiveDilepton/241115_095144/TriggerStudy_DiLept.root", isMC = True, nEvts = -1),

    'dataCv1m' : tnpSample('dataCv1m', "/eos/cms/store/group/phys_bphys/crovelli/TriggerL1_2024Nov15/Muon/crab_data_Run2022Cv1m/241115_095115/TriggerStudy_2022Cv1m.root"),
    'dataDv1m' : tnpSample('dataDv1m', "/eos/cms/store/group/phys_bphys/crovelli/TriggerL1_2024Nov15/Muon/crab_data_Run2022Dv1/241115_095117/TriggerStudy_2022Dv1.root"),
    'dataDv2m' : tnpSample('dataDv2m', "/eos/cms/store/group/phys_bphys/crovelli/TriggerL1_2024Nov15/Muon/crab_data_Run2022Dv2/241115_095120/TriggerStudy_2022Dv2.root"),
    'dataEv1m' : tnpSample('dataEv1m', "/eos/cms/store/group/phys_bphys/crovelli/TriggerL1_2024Nov15/Muon/crab_data_Run2022Ev1/241115_095135/TriggerStudy_2022Ev1.root"),
    'dataFv1m' : tnpSample('dataFv1m', "/eos/cms/store/group/phys_bphys/crovelli/TriggerL1_2024Nov15/Muon/crab_data_Run2022Fv1_0/241115_095717/TriggerStudy_2022Fv1.root"),
    'dataGv1m' : tnpSample('dataGv1m', "/eos/cms/store/group/phys_bphys/crovelli/TriggerL1_2024Nov15/Muon/crab_data_Run2022Gv1_0/241115_095719/TriggerStudy_2022Gv1.root"),
    #
    'dataCv1dm' : tnpSample('dataCv1dm', "/eos/cms/store/group/phys_bphys/crovelli/TriggerL1_2024Nov15/DoubleMuon/crab_data_Run2022Cv1dm/241118_115927/TriggerStudy_2022Cv1dm.root")
}

